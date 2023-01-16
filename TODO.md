# TODO

## Major features

Currently, clocking on creates a dotfile .current-session.agogo. Clocking off removes this file.  The main loop of
clockon checks for the presence of the file before deciding to proceed with project selection. This will become a
problem if users clock off and then clock on again. This should be fixed by adding a unique identifier to each clockon
session and including it in the .current-session filename. Each instance of the main loop will be looking for its own
.current file.

### Help files

Topics:
- clocking on and off
- creating and destroying workspaces
- adding and removing projects
- listing workspaces and projects
- modifying project properties and how agogo calculates score
- configuration options

## Keybindings

- Use of `agogo list .` as a synonym for current workspace.
