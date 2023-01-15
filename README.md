# AGOGO

Help you to partition your time between your various projects in a way that tries to be fair to all of them.

## What?
Create a workspace and add projects to it. When you clock on to the workspace, agogo will randomly select a project for
you to work on. Agogo will prioritise projects you mark as important but will also bias towards projects you have not
touched for a while.

## Commands

### General
agogo clockon {workspace}           : start the timer, and switch to the given workspace (default if none specified)
agogo clockoff                      : end the timer
agogo status                        : give the current clockon/off and workspace information

### Workspace
agogo create {workspace}            : create a new empty workspace
agogo destroy {workspace}           : destroy the workspace and all projects in it
agogo list                          : list all the workspaces, indicating active 

### Project
agogo add {workspace} {project}     : add the named project to the given workspace
agogo remove {workspace} {project}  : remove the named project from the given workspace
agogo modify {project} [-OPTS]      : set the details for the given project
agogo toggle {project}              : turn the specified project on or off
agogo list {workspace}              : list all current projects in the workspace, current active if none specified

