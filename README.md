# caelestia-cursor

A tiny command-line tool to view and change the cursor theme and size on a
[Caelestia](https://github.com/caelestia-dots) (Hyprland) setup, without editing
config files by hand.

## Features

- Print the current cursor theme
- Change the cursor theme (updates `variables.lua`, Hyprland and GNOME/GTK settings)
- Change the cursor size on the fly
- Automatic one-time backup of your `variables.lua`

## Requirements

- [Hyprland](https://hyprland.org/) (`hyprctl` must be in your `PATH`)
- A Caelestia config with a `variables.lua` at `~/.config/hypr/variables.lua`
  containing `cursorTheme` and `cursorSize` entries
- Cursor themes installed in `~/.icons`
- `gsettings` (from `glib2`) — optional, used to sync the GTK cursor theme
- `g++` with C++17 support (build only)

## Installation

### Arch Linux (PKGBUILD)

Place `PKGBUILD` and `caelestia-cursor.cpp` in the same folder, then:

```sh
makepkg -si
```

### Install script

```sh
chmod +x install.sh
./install.sh            # system-wide, installs to /usr/local/bin (uses sudo)
./install.sh --user     # installs to ~/.local/bin, no root needed
./install.sh --uninstall [--user]
```

### Manual build

```sh
g++ -std=c++17 -O2 -o caelestia-cursor caelestia-cursor.cpp
sudo install -Dm755 caelestia-cursor /usr/local/bin/caelestia-cursor
```

## Usage

```
caelestia-cursor [OPTION] [VALUE]
```

| Option                          | Description                                   |
|---------------------------------|-----------------------------------------------|
| `--help`, `-H`                  | Show the help message                         |
| `--current-theme`, `-C`         | Print the current cursor theme                |
| `--set-theme`, `-T` `THEME`     | Set the cursor theme                          |
| `--set-size`, `-S` `SIZE`       | Set the cursor size in pixels (integer)       |

### Examples

```sh
# Show the current theme
caelestia-cursor --current-theme

# Change the theme (use the folder name inside ~/.icons)
caelestia-cursor --set-theme Bibata-Modern-Ice

# Change the size to 32px
caelestia-cursor --set-size 32
```

If you pass a theme that is not installed, the tool lists the available ones.

## What it does

**`--set-theme`**

1. Checks that the theme exists in `~/.icons`
2. Creates `~/.config/hypr/variables_BAK.lua` if no backup exists yet
3. Rewrites the `cursorTheme` line in `~/.config/hypr/variables.lua`
4. Runs `hyprctl setcursor <theme> <current size>`
5. Runs `gsettings set org.gnome.desktop.interface cursor-theme '<theme>'`

**`--set-size`**

Runs `hyprctl setcursor <current theme> <size>`.

> **Note:** `--set-size` only applies the size for the current session. It does
> not modify `variables.lua`, so the size resets after restarting Hyprland.

## Troubleshooting

- **"Cursor theme config line not found"** — the tool expects this exact line
  (including indentation) in `variables.lua`:
  ```lua
      cursorTheme                = "Your-Theme",
  ```
  Make sure you are on a recent Caelestia version (`caelestia update`).
- **Theme not listed** — the theme folder must be in `~/.icons`, not only in
  `~/.local/share/icons` or `/usr/share/icons`.

## License

MIT.