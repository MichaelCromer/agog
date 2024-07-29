# TODO

## Major features

Ability to modify a project's importance.

Currently, clocking on creates a dotfile .current.agog. Clocking off removes this file.  The main loop of clockon
checks for the presence of the file before deciding to proceed with project selection. This will become a problem if
users clock on and then clock on again. This should be fixed by adding a unique identifier to each clockon session and
including it in the .current filename. Each instance of the main loop will be looking for its own .current file?

Desired functionality is to print a daily timetable for the workspace. It would make sense for this timetable to
actually be the source of the "randomly generated projects" that the user recieves alerts about. So when the user clocks
on, if a timetable for that day/session does not already exist for that workspace, then one is generated. Then, it
doesn't matter if the user changes workspace with another call to clockon. 
   
   clockon ${ws}    --->    (-e tt-${ws}-${date})   --[y]-->    put ${ws} in .current.agog

                                        |                           Λ           |
                                       [n]                          |           |
                                        |                           |           |
                                        V                           |           |
                                                                    |           |
                            create tt file for ${ws}    ------------+           |
                                                                                |
                                                                                V
                                                                                
                                                                             run main loop 
                                                                                        
                                                                             |           Λ
                                                                             |           |
                                                                             V           |
                                                                                         |
    exit 0      <-----------------------------------------[n]--    (-e .current.agog)   |
                                                                                         |  
                                                                             |           |
                                                                            [y]          |
                                                                             |           |
                                                                             V           |
                                                                                                                        
                                                                 draw next item from tt-${current-ws}
                                                                                

### Future goals

Symbolic links to other workspaces within the project listing of a workspace. This should allow
tasks to be chosen from the other workspace 

Ability for user to config global settings (such as relative values of differen project priortities)

Ability to config mandatory break and/or work times. For example set lunch break duration and placement.

### Help files

Topics:
- clocking on and off
- creating and destroying workspaces
- adding and removing projects
- listing workspaces and projects
- modifying project properties and how agog calculates score
- configuration options

## Bindings

- Use of `agog list .` as a synonym for current workspace.
