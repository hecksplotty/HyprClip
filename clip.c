/**
 * HyprClip Slots
 * A lightweight multi-slot clipboard manager for Hyprland.
 *
 * Dependencies: wl-clipboard, wtype
 * Compile: gcc clip.c -o clip
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

// Function to detect if the active window is a terminal
// This is necessary because terminals use Ctrl+Shift+V, while GUI apps use Ctrl+V.
int is_terminal() {
    FILE *fp;
    char buffer[1024];
    int is_term = 0;

    // Query Hyprland for the active window
    fp = popen("hyprctl activewindow", "r");
    if (fp == NULL) return 0;

    // Parse the output
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // Convert to lowercase for easier matching
        for(int i = 0; buffer[i]; i++){
            buffer[i] = tolower(buffer[i]);
        }

        // Check the "class:" line for known terminal emulators
        if (strstr(buffer, "class:")) {
            if (strstr(buffer, "kitty") || 
                strstr(buffer, "alacritty") || 
                strstr(buffer, "foot") || 
                strstr(buffer, "wezterm") ||
                strstr(buffer, "konsole") || 
                strstr(buffer, "gnome-terminal") ||
                strstr(buffer, "xterm")) {
                is_term = 1;
                break; 
            }
        }
    }
    pclose(fp);
    return is_term;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: clip [c|p] [0-9]\n");
        return 1;
    }

    char mode = argv[1][0];
    int slot = atoi(argv[2]);
    char cmd[512];

    if (mode == 'c') {
        // --- COPY MODE ---
        // Uses wl-paste --primary to grab the highlighted text (not Ctrl+C clipboard)
        printf("[DEBUG] Saving primary selection to slot %d...\n", slot);
        snprintf(cmd, sizeof(cmd), "wl-paste --primary > /tmp/clip_%d.txt", slot);
        system(cmd);
    } 
    else if (mode == 'p') {
        // --- PASTE MODE ---
        printf("[DEBUG] Loading slot %d and pasting...\n", slot);
        
        // 1. Load file content into system clipboard
        // --trim-newline prevents unwanted Enter key presses in terminals
        snprintf(cmd, sizeof(cmd), "wl-copy --trim-newline < /tmp/clip_%d.txt", slot);
        system(cmd);

        // 2. Small delay to ensure clipboard is ready and focus is correct
        usleep(100000); // 100ms

        // 3. Smart Paste: Simulate keystrokes based on window type
        if (is_terminal()) {
            printf("[DEBUG] Terminal detected -> Sending Ctrl+Shift+V\n");
            system("wtype -M ctrl -M shift -k v -m shift -m ctrl");
        } else {
            printf("[DEBUG] GUI App detected -> Sending Ctrl+V\n");
            system("wtype -M ctrl -k v -m ctrl");
        }
    }

    return 0;
}
