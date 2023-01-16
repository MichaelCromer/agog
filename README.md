# AGOGO

Help you to partition your time between your various projects in a way that tries to be fair to all of them.

## What?
Create a workspace and add projects to it. While clocked on to a workspace, agogo will randomly select projects for
you to work on. Agogo will prioritise projects you mark as important as well as those you have not touched for a while.

## Commands

### General
- `agogo clockon <workspace>`           : starts agogo if not running, and switches to the specified workspace
- `agogo clockoff`                      : stops agogo
- `agogo status`                        : report information about the current running state
- `agogo config [-OPTS]`                : change user configuration settings
- `agogo help <topic>`                  : print help about the requested topic

### Workspace
- `agogo create <workspace>`            : create a new empty workspace
- `agogo destroy <workspace>`           : destroy the workspace and all projects in it
- `agogo list`                          : list all the workspaces, indicating active 

### Project
- `agogo add <workspace> <project>`     : add the named project to the given workspace
- `agogo remove <workspace> <project>`  : remove the named project from the given workspace
- `agogo list <workspace>`              : list all current projects in the workspace, current active if none specified
- `agogo modify <project> [-OPTS]`      : set the details for the given project
- `agogo toggle <project>`              : turn the specified project on or off

