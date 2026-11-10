#!/usr/bin/env bash
set -euo pipefail

NAME="caelestia-cursor"
SRC="${NAME}.cpp"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

MODE="system"
ACTION="install"

for arg in "$@"; do
    case "$arg" in
        --user)      MODE="user" ;;
        --uninstall) ACTION="uninstall" ;;
        -h|--help)
            echo "Usage: $0 [--user] [--uninstall]"
            echo "  --user       install to ~/.local/bin (no root needed)"
            echo "  --uninstall  remove the installed binary"
            exit 0
            ;;
        *)
            echo "Unknown option: $arg" >&2
            exit 1
            ;;
    esac
done

if [[ "$MODE" == "user" ]]; then
    DEST_DIR="$HOME/.local/bin"
    SUDO=""
else
    DEST_DIR="/usr/local/bin"
    SUDO="sudo"
fi

DEST="$DEST_DIR/$NAME"

if [[ "$ACTION" == "uninstall" ]]; then
    if [[ -f "$DEST" ]]; then
        $SUDO rm -v "$DEST"
        echo "Uninstalled $NAME."
    else
        echo "$NAME is not installed at $DEST."
    fi
    exit 0
fi

# --- Checks ---------------------------------------------------------------
if ! command -v g++ >/dev/null 2>&1; then
    echo "Error: g++ is required to build $NAME." >&2
    echo "On Arch: sudo pacman -S gcc" >&2
    exit 1
fi

if [[ ! -f "$SCRIPT_DIR/$SRC" ]]; then
    echo "Error: $SRC not found in $SCRIPT_DIR." >&2
    exit 1
fi

command -v hyprctl >/dev/null 2>&1 || echo "Warning: hyprctl not found in PATH."
command -v gsettings >/dev/null 2>&1 || echo "Warning: gsettings not found (optional, install glib2)."

# --- Build ----------------------------------------------------------------
BUILD_DIR="$(mktemp -d)"
trap 'rm -rf "$BUILD_DIR"' EXIT

echo "Building $NAME..."
g++ -std=c++17 -O2 -Wall -o "$BUILD_DIR/$NAME" "$SCRIPT_DIR/$SRC"

# --- Install --------------------------------------------------------------
echo "Installing to $DEST..."
$SUDO install -Dm755 "$BUILD_DIR/$NAME" "$DEST"

echo "Done. Run '$NAME --help' to get started."

if [[ "$MODE" == "user" && ":$PATH:" != *":$DEST_DIR:"* ]]; then
    echo "Note: $DEST_DIR is not in your PATH. Add it to your shell config."
fi