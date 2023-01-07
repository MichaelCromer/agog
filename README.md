# AGOGOS 

Help you to partition your time between your various projects in a way that tries to be fair to all of them.

agogo clock-on {workspace}          : start the timer, and switch to the given workspace (default if none specified)
agogo clock-off                     : end the timer

agogo create {workspace}            : create a new empty workspace
agogo destroy {workspace}           : destroy the workspace and all projects in it

agogo add {workspace} {project}     : add the named project to the given workspace
agogo remove {workspace} {project}  : remove the named project from the given workspace

agogo modify {project} {-flags}     : set the details for the given project
agogo toggle {project}              : turn the specified project on or off

agogo list {workspace}              : list all current projects in the workspace, current active if none specified
