import { Ascii } from "./ascii.ts";
import { existsSync } from "https://deno.land/std@0.224.0/fs/mod.ts";

const RWPM_FILE = ".rwpm";

function getSteamLibraryPath(): string {
    if (existsSync(RWPM_FILE)) {
        try {
            const path = Deno.readTextFileSync(RWPM_FILE).trim();

            // Make sure that there are no trailing slashes
            if (path.endsWith("\\")) {
                return path.slice(0, -1);
            }
            if (path) {
                return path;
            }
        } catch (error) {
            console.error(`${Ascii.COLOR.RED}Error reading .rwpm file: ${error}${Ascii.reset()}`);
        }
    }

    const newPath = prompt("Enter your Steam library path:");
    if (newPath && newPath.trim() !== "") {
        try {
            Deno.writeTextFileSync(RWPM_FILE, newPath.trim());
            console.log(`${Ascii.COLOR.GREEN}Steam library path saved to .rwpm file.${Ascii.reset()}`);
            // Make sure that there are no trailing slashes
            if (newPath.endsWith("\\")) {
                return newPath.slice(0, -1);
            }
            return newPath.trim();
        } catch (error) {
            console.error(`${Ascii.COLOR.RED}Error saving .rwpm file: ${error}${Ascii.reset()}`);
        }
    }

    console.log(`${Ascii.COLOR.RED}No valid Steam library path provided. Exiting.${Ascii.reset()}`);
    Deno.exit(1);
}

const SteamLibraryPath = getSteamLibraryPath();
const RimWorldExe = "RimWorldWin64.exe"; 

interface RimworldVersion {
    name: string;
    path: string;
    packs: string[];
}

async function getRimworldVersions(): Promise<RimworldVersion[]> {
    const directoryContents = Deno.readDir(SteamLibraryPath);
    const versions: RimworldVersion[] = [];

    for await (const entry of directoryContents) {
        if (entry.isDirectory && entry.name.startsWith("RimWorld")) {
            const versionPath = `${SteamLibraryPath}\\${entry.name}`;
            
            const version: RimworldVersion = {
                name: entry.name,
                path: versionPath,
                packs: []
            };
            versions.push(version);
        }
    }

    return versions;
}

async function createPacksFolder(version: RimworldVersion): Promise<void> {
    const packsFolder = version.path + "\\Packs";
    await Deno.mkdir(packsFolder, { recursive: true });
}

async function getPacks(version: RimworldVersion): Promise<void> {
    const packsPath = `${version.path}\\Packs`;
    try {
        const directoryContents = Deno.readDir(packsPath);
        for await (const entry of directoryContents) {
            if (entry.isDirectory && !version.packs.includes(entry.name)) {
                version.packs.push(entry.name);
            }
        }
    } catch (error) {
        if (error instanceof Deno.errors.NotFound) {
            // This is not an error, the folder just doesn't exist yet.
        } else {
            console.error(`${Ascii.COLOR.RED}Error reading packs for ${version.name}: ${error}${Ascii.reset()}`);
        }
    }
}

let selectedVersion: RimworldVersion | null = null;
let selectedPack: string | null = null;

function selectVersion(versions: RimworldVersion[]): void {
    console.log(`\n${Ascii.COLOR.CYAN}Select a RimWorld version:${Ascii.reset()}\n`);
    versions.forEach((version, index) => {
        console.log(`${Ascii.COLOR.YELLOW}${index + 1}. ${version.name}${Ascii.reset()}`);
    });

    const choice = prompt("Enter the number of the version you want to select:");
    const index = parseInt(choice || "", 10) - 1;

    if (index >= 0 && index < versions.length) {
        selectedVersion = versions[index];
        console.log(`\n${Ascii.COLOR.GREEN}▶ Selected version: ${selectedVersion.name}${Ascii.reset()}\n`);
    } else {
        console.log(`\n${Ascii.COLOR.RED}Invalid choice. No version selected.${Ascii.reset()}\n`);
    }
}

function createPack(version: RimworldVersion): string | null {
    const newPackName = prompt("Enter the name for the new pack:");

    if (!newPackName || newPackName.trim() === "") {
        console.log(`${Ascii.COLOR.RED}Invalid pack name. Operation cancelled.${Ascii.reset()}`);
        return null;
    }

    const newPackPath = `${version.path}\\Packs\\${newPackName}`;

    try {
        Deno.mkdirSync(newPackPath, { recursive: true });
        console.log(`${Ascii.COLOR.GREEN}Pack '${newPackName}' created successfully.${Ascii.reset()}`);
    } catch (error) {
        console.error(`${Ascii.COLOR.RED}Error creating pack '${newPackName}': ${error}${Ascii.reset()}`);
    }

    return newPackName; // Return the name of the created pack
}

function selectPack(version: RimworldVersion): void {
    console.log(`\n${Ascii.COLOR.CYAN}Select a pack from ${version.name} (or type '+' to create a new pack):${Ascii.reset()}\n`);

    if (version.packs.length > 0) {
        version.packs.forEach((pack, index) => {
            console.log(`${Ascii.COLOR.YELLOW}${index + 1}. ${pack}${Ascii.reset()}`);
        });
    } else {
        console.log(`${Ascii.COLOR.RED}No packs available for ${version.name}.${Ascii.reset()}\n`);
    }

    const choice = prompt("Enter the number of the pack you want to select, or '+' to create a new pack:");

    if (choice === "+") {
        const newPackName = createPack(version);
        if (newPackName) {
            selectedPack = newPackName;
            console.log(`\n${Ascii.COLOR.GREEN}▶ Created and selected pack: ${selectedPack}${Ascii.reset()}\n`);
        }
        return;
    }

    const index = parseInt(choice || "", 10) - 1;

    if (index >= 0 && index < version.packs.length) {
        selectedPack = version.packs[index];
        console.log(`\n${Ascii.COLOR.GREEN}▶ Selected pack: ${selectedPack}${Ascii.reset()}\n`);
    } else {
        console.log(`\n${Ascii.COLOR.RED}Invalid choice. No pack selected.${Ascii.reset()}\n`);
    }
}

async function startWithPack(version: RimworldVersion, pack: string): Promise<void> {
    // Confirm if the user wants to start with the selected pack
    const confirmation = prompt(`Do you want to start RimWorld with pack '${pack}'? (y/n):`);
    if (confirmation?.toLowerCase() !== "y") {
        console.log(`${Ascii.COLOR.RED}Operation cancelled by user.${Ascii.reset()}\n`);
        return;
    }
    
    const packPath = `${version.path}\\Packs\\${pack}`;
    const exePath = `${SteamLibraryPath}\\${version.name}\\${RimWorldExe}`;

    try {
        console.log(`${Ascii.COLOR.CYAN}Starting game...${Ascii.reset()}`);
        console.log(`${Ascii.COLOR.YELLOW}Launching: ${exePath} with data folder: ${packPath}${Ascii.reset()}`);

        const process = new Deno.Command(exePath, {
            args: [
                "-savedatafolder=" + packPath
            ],
            cwd: version.path,
            stdout: "inherit",
            stderr: "inherit"
        }).spawn();

        const status = await process.status;

        if (status.success) {
            console.log(`${Ascii.COLOR.GREEN}RimWorld started with pack ${pack}${Ascii.reset()}`);
        } else {
            console.error(`${Ascii.COLOR.RED}Failed to start RimWorld with pack ${pack}${Ascii.reset()}`);
        }
    } catch (error) {
        console.error(`${Ascii.COLOR.RED}Error starting RimWorld with pack ${pack}: ${error}${Ascii.reset()}`);
    }
}

async function main() {
    console.log(Ascii.clear());
    const title = " RimWorld Packs Manager ";
    const padding = "=".repeat(Math.floor((50 - title.length) / 2));
    console.log(`\n${Ascii.COLOR.YELLOW}${padding}${title}${padding}${Ascii.reset()}\n`);

    const versions = await getRimworldVersions();

    if (versions.length === 0) {
        console.log(`\n${Ascii.COLOR.RED}No RimWorld installations found in:${Ascii.reset()} ${SteamLibraryPath}`);
        console.log(`${Ascii.COLOR.YELLOW}Please update the 'SteamLibraryPath' variable in main.ts${Ascii.reset()}\n`);
        return;
    }

    console.log(`\n${Ascii.COLOR.CYAN}Found ${versions.length} RimWorld installation(s):${Ascii.reset()}\n`);

    for (const version of versions) {
        await getPacks(version); // Get existing packs first
        await createPacksFolder(version); // Ensure folder exists

        console.log(`\n${Ascii.COLOR.GREEN}▶ ${version.name}${Ascii.reset()}`);
        if (version.packs.length > 0) {
            console.log(`  ${Ascii.COLOR.WHITE}Packs:${Ascii.reset()}`);
            version.packs.forEach(pack => {
                console.log(`    - ${Ascii.COLOR.BLUE}${pack}${Ascii.reset()}`);
            });
        } else {
            console.log(`  ${Ascii.COLOR.WHITE}Packs: ${Ascii.COLOR.RED}<None found>${Ascii.reset()}`);
        }
    }

    console.log(`\n${Ascii.COLOR.YELLOW}${"=".repeat(50)}${Ascii.reset()}\n`);

    selectVersion(versions);
    if (selectedVersion) {
        await getPacks(selectedVersion); // Refresh packs for the selected version
        selectPack(selectedVersion);
    } else {
        console.log(`\n${Ascii.COLOR.RED}No version selected.${Ascii.reset()}\n`);
    }

    console.log(`\n${Ascii.COLOR.YELLOW}${"=".repeat(50)}${Ascii.reset()}\n`);

    if (selectedPack) {
        console.log(`\n${Ascii.COLOR.GREEN}Selected Pack: ${selectedPack}${Ascii.reset()}\n`);
    } else {
        console.log(`\n${Ascii.COLOR.RED}No pack selected.${Ascii.reset()}\n`);
    }

    if (selectedVersion && selectedPack) {
        await startWithPack(selectedVersion, selectedPack);
    } else {
        console.log(`\n${Ascii.COLOR.RED}Cannot start RimWorld without a version and pack selected.${Ascii.reset()}\n`);
    }

    console.log(`\n${Ascii.COLOR.YELLOW}${"=".repeat(50)}${Ascii.reset()}\n`);
}

main().catch((error) => {
    console.error(`${Ascii.COLOR.RED}An unexpected error occurred: ${error}${Ascii.reset()}`);
});
