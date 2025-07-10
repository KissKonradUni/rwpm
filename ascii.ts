const ASCII = {
    COLOR: {
        BLACK: "\x1b[30m",
        RED: "\x1b[31m",
        GREEN: "\x1b[32m",
        YELLOW: "\x1b[33m",
        BLUE: "\x1b[34m",
        MAGENTA: "\x1b[35m",
        CYAN: "\x1b[36m",
        WHITE: "\x1b[37m",
        RESET: "\x1b[0m"
    },
    BACKGROUND: {
        BLACK: "\x1b[40m",
        RED: "\x1b[41m",
        GREEN: "\x1b[42m",
        YELLOW: "\x1b[43m",
        BLUE: "\x1b[44m",
        MAGENTA: "\x1b[45m",
        CYAN: "\x1b[46m",
        WHITE: "\x1b[47m",
        RESET: "\x1b[0m"
    },
    BORDER: {
        TOP_LEFT: "\u250C",
        TOP_RIGHT: "\u2510",
        BOTTOM_LEFT: "\u2514",
        BOTTOM_RIGHT: "\u2518",
        HORIZONTAL: "\u2500",
        VERTICAL: "\u2502"
    },
};

export class Ascii {
    public static COLOR = ASCII.COLOR;
    public static BACKGROUND = ASCII.BACKGROUND;
    public static BORDER = ASCII.BORDER;

    static reset() {
        return ASCII.COLOR.RESET + ASCII.BACKGROUND.RESET;
    }

    static clear() {
        return "\x1b[2J";
    }

    static moveTo(x: number, y: number) {
        return `\x1b[${y};${x}H`;
    }
}