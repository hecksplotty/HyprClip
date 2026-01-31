# HyprClip Slots

A minimalist, C-based multi-slot clipboard manager designed for **Hyprland**.

It allows you to have 10 distinct clipboard slots (0-9). You can save text to a slot and paste it immediately using "Chord" keybindings (e.g., `Super+C` then `1`).

## Features

* **10 Slots:** Manage slots 0 through 9 independently.
* **Primary Selection Workflow:** Captures highlighted text (no need to press Ctrl+C first).
* **Auto-Paste:** Automatically simulates keystrokes to paste the text.
* **Smart Context Detection:** Automatically detects if you are in a Terminal or a GUI app:
    * Sends `Ctrl+Shift+V` for Terminals (Kitty, Alacritty, Gnome-terminal, etc.).
    * Sends `Ctrl+V` for Browsers/Editors.
* **Trim Newlines:** Prevents accidental command execution in terminals by trimming trailing newlines.

## Prerequisites

You need these tools installed on your system (Arch Linux example):

sudo pacman -S wl-clipboard wtype gcc

wl-clipboard: To read/write to the system clipboard.
wtype: To simulate keystrokes (requires Wayland support).
gcc: To compile the tool.

Installation

Compile the tool:
gcc clip.c -o clip

Move it to your binary path:
sudo mv clip /usr/bin/
sudo chmod +x /usr/bin/clip

Configuration (Hyprland)
Add the following configuration to your ~/.config/hypr/hyprland.conf. This uses Hyprland Submaps to create clean key chords.

Copy Mode (Highlight -> Save)
Usage: Highlight text, press Super + C, release, then press a number 0-9.


bind = SUPER, C, submap, copymode
submap = copymode
bind = , 1, exec, clip c 1
bind = , 1, submap, reset
# ... repeat for 2-9 and 0 ...
bind = , escape, submap, reset
submap = reset
Paste Mode (Load -> Paste)
Usage: Place cursor, press Super + V, release, then press a number 0-9.


bind = SUPER, V, submap, pastemode
submap = pastemode
bind = , 1, exec, clip p 1
bind = , 1, submap, reset
# ... repeat for 2-9 and 0 ...
bind = , escape, submap, reset
submap = reset
(See the hyprland.conf example file for the full list of bindings).

Usage Example
Open a browser. Highlight a URL (don't press Ctrl+C).

Press Super + C, release, press 1. (Saved to slot 1).

Highlight another sentence.

Press Super + C, release, press 2. (Saved to slot 2).

Go to a terminal.

Press Super + V, release, press 1. (Pastes URL).

Press Super + V, release, press 2. (Pastes sentence).

Notes
Persistence: Currently, clips are saved in /tmp/, meaning they are cleared on reboot.

Terminals: Supports Kitty, Alacritty, Foot, Wezterm, Konsole, Gnome-terminal, Xterm. To add more, edit the is_terminal() function in clip.c .
