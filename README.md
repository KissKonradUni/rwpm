# 🎮 RimWorld Packs Manager (RWPM)

A ~~beautifully polished~~ somewhat janky but effective utility to manage different packs (and save data folders) for RimWorld installations. It just works! 🛠️

[![Download Latest Release](https://img.shields.io/github/v/release/kisskonraduni/rwpm)](https://github.com/kisskonraduni/rwpm/releases) [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT) [![Made with Deno](https://img.shields.io/badge/Made%20with-Deno-black?logo=deno)](https://deno.land) [![Works on my machine](https://img.shields.io/badge/Works-on%20my%20machine-success)](https://github.com/kisskonraduni/rwpm/issues)

> **⚠️ IMPORTANT WARNING ⚠️**  
> If your RimWorld is installed in the default `C:\Program Files\Steam\steamapps\common` folder, you'll need to run this app as administrator!
>
> But most likely you would not want that. I recommend setting a Steam library folder outside of the Program Files folder. (Why is that not a default?)

## 📋 Overview

RimWorld Packs Manager (or as I like to call it, "That Mod Pack Switcher Thingy") allows you to create and manage different "packs" for your RimWorld installations. Each pack represents a separate save data folder and distinct config folder, allowing you to:

- Juggle multiple modlists without your game having an existential crisis
- Keep different save games isolated so your "peaceful farming colony" doesn't accidentally load your "war crimes expanded" mods
- Switch between game configurations faster than you can say "Randy Random"
- Support multiple RimWorld installations/versions simultaneously (because who doesn't need both 1.5 AND 1.6 installed?)

## 🔄 How to Install Multiple RimWorld Versions

Want to enjoy both the stable release and beta versions? Here's the not-so-secret sauce:

1. In Steam, right-click RimWorld → Properties → Betas
2. Select a version (e.g., "unstable - unstable public testing")
3. Wait for Steam to download/update the game
4. **IMPORTANT:** Exit Steam completely (yes, fully exit, not just minimize)
5. Go to your Steam library folder and rename the RimWorld folder to something descriptive (e.g., "RimWorld1.5" or "RimWorldUnstable")
6. Restart Steam, and repeat with a different beta channel

Now you can have RimWorld 1.4, 1.5, 1.6, and whatever else all installed at the same time! This tool will automatically detect all folders starting with "RimWorld" and let you manage them like the organizational wizard you are.

## 🚀 Usage

1. Download the latest release from [GitHub Releases](https://github.com/kisskonraduni/rwpm/releases)
2. Run `RimWorldPacksManager.exe` (If Windows Defender gets suspicious, you can verify the source code yourself - this app steals fewer organs than Randy Random)
3. On first launch, enter your Steam library path (typically `C:\Program Files\Steam\steamapps\common` or your custom library location - just point to the parent folder where your RimWorld folders live)
4. Select a RimWorld version from the detected list
5. Create a new pack or select an existing one (Get creative with names! "VanillaVentures", "ModMadness", "WhyDidIInstall500Mods", etc.)
6. Launch the game with your selected pack and enjoy your perfectly isolated modlist!

The application will automatically detect RimWorld installations in your Steam library path and manage pack folders for each installation. It's not rocket science, just some clever file system operations that somehow work reliably.

## ✨ Features

- 🔍 **Smart Installation Detection** - Automatically finds all your RimWorld installations (using the sophisticated technique of "looking for folders that start with RimWorld")
- 📁 **Pack Management** - Creates and manages separate mod/save environments with zero conflicts
- 🚀 **One-Click Launch** - Starts RimWorld with your selected pack (no more manual `-savedatafolder` command line wizardry)
- 🎮 **Multi-Version Support** - Play 1.4, 1.5, 1.6, or any combination without mod conflicts (but keep backups because computers are fickle beasts)
- 💾 **Configuration Persistence** - Remembers your Steam library path between sessions (your memory might fail, but ours won't)
- 🛠️ **Retro Terminal UI** - Delightfully old-school command-line interface with ASCII art and colors (it's not primitive, it's "vintage")

## 👨‍💻 Development

### Requirements

- [Deno](https://deno.land/) v1.35 or higher
- [rcedit-x64](https://github.com/electron/rcedit) 2.0.0 or higher - For that fancy icon goodness

### 🏗️ Building from Source

1. Clone the repository (or just copy-paste the code if you're feeling rebellious)
2. Make sure Deno is installed (yes, I chose the hipster runtime)
3. Compile the project using these surprisingly straightforward commands:

```bash
# Install dependencies (it's just two .ts files, don't get too excited)
deno cache --reload main.ts

# Build executable (transforms coffee-fueled code into an actual app)
deno task build
```

Is this overengineered for a simple tool? Perhaps. But it's TypeScript! That automatically makes it enterprise-grade software, right? Right?

### 📂 Project Structure

- `main.ts` - Where the magic happens (all 250+ lines of it)
- `ascii.ts` - Makes your terminal pretty with colors and formatting
- `icon.ico` - The face of our application (it's beautiful, don't judge)
- `deno.json` - Tells Deno how to behave
- `.rwpm` - Configuration file (created on first run, contains your Steam path)

## 🐛 Known Issues

- Sometimes the terminal colors go wild. It's not a bug, it's a feature! 🌈
- The UI is... well, there is no UI. Just embrace the terminal life. It builds character!
- Zero automated tests. We test in production like the professionals!
- Exactly as many error checks as I had patience to implement (which is more than none, but less than "comprehensive")

## 📜 License

MIT License (Feel free to take this code and make it less janky)

## 🤝 Contributing

Contributions are welcome! Feel free to open issues or submit pull requests. The code is simple enough that you could probably rewrite it better in a weekend.

---

Did you actually read this far? Wow, you must really want to manage your RimWorld packs! Here's a cookie: 🍪

[![Star this project](https://img.shields.io/github/stars/kisskonraduni/rwpm?style=social)](https://github.com/kisskonraduni/rwpm) [![Made with ❤️ and minimal sleep](https://img.shields.io/badge/Made%20with-%E2%9D%A4%EF%B8%8F%20and%20minimal%20sleep-red)](https://github.com/kisskonraduni/rwpm)
