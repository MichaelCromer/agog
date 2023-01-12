#!/usr/bin/env bash

#   
#    ###     ###     ###     ###     ###   
#   ## ##   #   #   #   #   #   #   #   #  

#   #####   #  ##   #   #   #  ##   #   #  
#   #   #   #   #   #   #   #   #   #   #  
#   #   #    ## #    ###     ## #    ###   
#   

# ----------------------------------------------------------

# Agogo by Michael Cromer

# ----------------------------------------------------------

# 1. place this file somewhere, e.g., ~/scripts/remindme.sh
# 2. create a symbolic link to this file in /usr/local/bin
# sudo ln -s ~/scripts/remindme.sh /usr/local/bin/remindme
# 3. make the symlink executable
# chmod +x /usr/local/bin/remindme
# 4. launch your first reminder from anywhere in terminal
# remindme "hydration check" 7 3
# (a hydration check every 7 minutes for 3 hours)

# Bonus: if you want these commands to run in
# the background (and free up your terminal)
# add this to your ~/.bashrc or ~/.zshrc

# remindme() {
#     if ! [[ ($# -eq 3) ]]; then
#         # run in foreground if not a valid reminder (e.g., remindme -k)
#         /usr/local/bin/remindme "$@"
#     else
#         # run in background
#         nohup /usr/local/bin/remindme "$@" 2>/dev/null &
#     fi
# }

# These will catch errors and exit safely
set -o errexit
set -o pipefail
set -o nounset

# Use 'TRACE=1 agogo' at CL to track activity 
if [[ "${TRACE-0}" == "1" ]]; then set -o xtrace; fi

# Helpful if I need to change things later
agogo-current-session-file() {
    echo "userdata/.current.agogo"
}

agogo-workspaces-list-file() {
    echo "userdata/workspaces.agogo"
}

agogo-workspace-info-file() {
    echo "userdata/ws-${1}.agogo"
}

# BOOLEAN for checking workplace existence
agogo-workspace-exists() {
    grep -qE ^"${1}"$ "$(agogo-workspaces-list-file)"
}

# BOOLEAN for verifying that a project exists within a workspace
agogo-project-exists-on-workspace() {
    grep -qE ^"${1}"$ "$(agogo-workspace-info-file ${2})"
}

# BOOLEAN for checking active status (use existence of .current-ws as proxy!)
agogo-is-running() {
    [[ (-e "$(agogo-current-session-file)") ]];
}

# Return name of current ws, or empty string if none
agogo-current-workspace() {
    if (agogo-is-running); then
        cat "$(agogo-current-session-file)"
    else
        echo ""
    fi
}

# A custom read with default 'no' response. Argument $1 gives detail to the used
agogo-confirm-prompt() {
    agogo-print "${1}"
    agogo-print ""
    read -p "Are you sure? [y/N] " -n 1 -r
    [[ "${REPLY}" =~ ^[Yy]$ ]];
}

# For printing and piping errors correctly
agogo-error() {
    printf "\nagogo-error: ${*}\n\n" 1>&2
    exit 1
}

# For printing to the user properly
agogo-print() {
    printf "\n${*}"
}

# old remindme.sh function to be used later
sagen() {
    if [ -x "$(command -v say)" ]; then
        # macOS
        echo $@ | say
    else
        # other
        echo $@ | espeak -s 120 2>/dev/null
    fi
}

# Cleans up all the session data and terminates all processes
agogo-clockoff() {
    # Can't clock off if agogo is not clocked on!
    if !(agogo-is-running); then
        agogo-error "You are not clocked on!"
    fi

    # Double check with the user
    if (agogo-confirm-prompt "This will terminate all agogo processes."); then
        rm "$(agogo-current-session-file)"
        agogo-print "TODO complete clockoff logic"
    else
        agogo-print "Clockoff aborted; agogo is still running"
    fi
    agogo-print ""
}

# inits a new running session to a specified workspace or changes workspaces
agogo-clockon() {
    # Can't clock on twice, but might clock on to a different workspace
    if [[ ($# -eq 0) ]]; then
        if (agogo-is-running); then
            agogo-error "You are already clocked on! To switch workspace, specify the target name."
        fi
    fi

    # Use the default workspace if none specififed
    local ws="${1:-"default"}"
    
    # Don't need to restart timers etc if already here
    if [[ ("${ws}" == "$(agogo-current-workspace)") ]]; then
        agogo-error "Already clocked on to workspace '${ws}'"
    fi

    # Check if the specified thing is actually a workspace, then clock onto it
    if (agogo-workspace-exists "${ws}") then
        echo "${ws}" > "$(agogo-current-session-file)"
        agogo-print "TODO complete clockon logic"
        agogo-print "Clocked on to workspace '${ws}'"
        agogo-mainloop &
    else
        agogo-error "'${ws}' is not a workspace"
    fi
    agogo-print ""
}

# Here will be implemented the interesting mathematics
agogo-mainloop() {
    zenity --info "test message" & 
    sleep 1000
}

# creates a new workspace
agogo-create() {
    if [[ ($# -eq 0) ]]; then
        agogo-error "You must specify a workspace name"
    fi

    local ws="${1}"
    
    if (agogo-workspace-exists "${ws}"); then
        agogo-error "Workplace '"${ws}"' already exists!"
    else
        echo "${ws}" >> "$(agogo-workspaces-list-file)"
        touch "$(agogo-workspace-info-file "${ws}")"
        agogo-print "Created new workspace '${ws}'"
    fi
    agogo-print ""
}

# destroys the specified workspace
agogo-destroy() {

    # I like to be dramatic
    local RED="\033[0;31m"
    local PLN="\033[0m"

    # Needs a ws name
    if [[ ($# -eq 0) ]]; then
        agogo-error "You must specify a workspace name"
    fi
    
    # Catch that ~~pokemon~~ name!
    local ws="${1}"
    
    # Check it exists, then confirm prompt
    if (agogo-workspace-exists "${ws}"); then
        if (agogo-confirm-prompt "This will destroy the workspace '${ws}' and all its projects. 
            ${RED}This action cannot be undone.${PLN}"); then

            # Remove it from the list file, and delete its project data
            sed -i "/^${ws}$/d" "$(agogo-workspaces-list-file)"
            rm "$(agogo-workspace-info-file "${ws}")"
            agogo-print "\nDestroyed workspace '${ws}' and all its projects"
        fi
    else
        agogo-error "Workspace '${ws}' does not exist."
    fi
    agogo-print ""
}

# Prints whether there is an active session right now
agogo-status() {
    if !(agogo-is-running); then
        agogo-print "You are clocked off"
    else
        agogo-print "Currently clocked on to workspace '$(agogo-current-workspace)'"
    fi
    agogo-print ""
}

# prints a list of all workspaces
agogo-list-workspaces() {
    agogo-print "Currently tracked workspaces :"
    agogo-print ""

    local GRN="\033[0;32m"
    local BLD="\033[1m"
    local PLN="\033[0m"
    
    while read -r line
    do
        if [[ "${line}" == "$(agogo-current-workspace)" ]]; then
            agogo-print "${GRN}CURR${PLN}\t${BLD}${line}${PLN}"
        else
            agogo-print "\t$line"
        fi
    done < "$(agogo-workspaces-list-file)"
    
    agogo-print ""
    agogo-print "To see detail about a workspace's projects, include its name as an argument."
    agogo-print ""
}

# given a workspace name, prints a list of all its projects
agogo-list-projects() {
    if [[ ($# -eq 0) ]]; then
        if !(agogo-is-running); then
            agogo-error "agogo is not clocked on. Please specify a workspace to list projects from."
        fi
        local ws="$(agogo-current-workspace)"
    else
        local ws="${1}"
        if !(agogo-workspace-exists "${ws}"); then
            agogo-error "No workspace with name '${ws}' exists."
        fi       
    fi   
    cat "$(agogo-workspace-info-file "${ws}")"
    agogo-print ""
}

# Add a new project to a given workspace, or the current one if none specified
agogo-add() {
    # Requres a name for the new project
    if [[ ($# -eq 0) ]]; then
        agogo-error "You must specify a project name"
    fi

    # Capture the name locally
    local pj="${1}"
    
    # Can add to current ws without specifying its name, but agogo must be running
    if [[ ($# -eq 1) ]]; then
        if !(agogo-is-running); then
            agogo-error "agogo is not clocked on. Please clock on first, or specify a workspace to add project '${1}' to."
        fi
        local ws="$(agogo-current-workspace)"
    else
        local ws="${2}"
    fi
    
    # Check it's actually a workspace
    if !(agogo-workspace-exists ${ws}); then
        agogo-error "Workspace '${ws}' does not exist"
    fi

    # Check the project doesn't already exist
    if (agogo-project-exists-on-workspace "${pj}" "${ws}"); then
        agogo-error "Project '${pj}' already exists on workspace '${ws}'."
    fi

    # We're all good! Set up the thing!
    echo "${pj}"        >> "$(agogo-workspace-info-file "${ws}")"
    echo "AGE:0"        >> "$(agogo-workspace-info-file "${ws}")" 
    echo "PRIORITY:1"   >> "$(agogo-workspace-info-file "${ws}")"
    echo "SCORE:"       >> "$(agogo-workspace-info-file "${ws}")"
    echo "\n"           >> "$(agogo-workspace-info-file "${ws}")" 
    agogos-print "Successfully created project '${pj}' on workspace '${ws}'."
    agogo-print ""
}
    
# Remove a project from a given workspace, or the current one if none specified
agogo-remove() {
    # Must specify a project
    if [[ ($# -eq 0) ]]; then
        agogo-error "You must specify a project name"
    fi

    local pj="${1}"

    # Use current ws if none specified, but must be currently running
    if [[ ($# -eq 1) ]]; then
        if !(agogo-is-running); then
            agogo-error "agogo is not clocked on. Please specify a workspace to remove project '${1}' from."
        fi
        local ws="$(agogo-current-workspace)"
    else
        local ws="${2}"
    fi

    # Check it's actually a workspace
    if !(agogo-workspace-exists ${ws}); then
        agogo-error "Workspace '${ws}' does not exist"
    fi
    
    # Check the project actually exists
    if !(agogo-project-exists-on-workspace "${pj}" "${ws}"); then
        agogo-error "Project '${pj}' does not exist on workspace '${ws}'."
    fi

    # Now we can proceed
    if (agogo-confirm-prompt "This will permanently remove the project '${pj}' from the workspace '${ws}'.
        ${RED}This action cannot be undone.${PLN}"); then
        sed -i "/^${pj}$/d" "$(agogo-workspace-info-file "${ws}")"
    else
        agogo-print "Remove command aborted. No action taken."
    fi
    agogo-print ""
}

# old remindme.sh function to use later
remindme() {
    ((frequency = $2 * 60))
    ((duration = $3 * 3600))
    if [[ $frequency -ge $duration ]]; then
        agogo-print "Duration needs to be greater than frequency!"
        agogo-print "you gave a frequency of $2 min ($frequency sec), and a duration of $3 hours ($duration sec)"
        exit 0
    fi
    ((occurrences = $duration / $frequency))
    sagen "$1"
    for ((i = 1; i <= $occurrences; i++)); do
        sleep $frequency
        sagen "$1"
    done
    sagen "finished $1"
}

# searches for and then fetches the appropriate help file
agogo-help() {
    if [[ ($# -eq 0) ]]; then
        cat "helpfiles/basic.agogo"
        exit 0
    fi

    local helpfile="helpfiles/${1}.agogo"
    
    if [[ !(-e "${helpfile}") ]]; then
        agogo-error "'$1' is not a recognised agogo command or help topic"
    fi
    
    cat "${helpfile}"
    exit 0
}

# old remindme.sh function to use later
purge-all() {
    procs=$(ps -ef | grep $0 | grep -vE "(grep|$0 -k|$0 --kill)")
    if ! [ -z "$procs" ]; then # check if ps results are null
        list-all
        read -p "Are you sure? [y/N] " -n 1 -r
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            echo
            ps -ef | grep $0 | grep -vE "(grep|$0 -k|$0 --kill)" | tr -s ' ' | awk -F' ' '{print $2}' | xargs kill
        else
            if [[ $REPLY =~ ^[Nn]$ ]]; then echo; fi
            agogo-print "Clockoff aborted; agogo is still running"
            exit 0
        fi
    else
        agogo-print "No reminders to kill"
    fi
    agogo-print ""
}

# old remindme.sh function to use later
list-all() {
    ps -ef | grep $0 | grep -vE "(grep|$0 -l|$0 --list|$0 -k|$0 --kill)" | tr -s ' ' | awk -F' ' '{$1=$2=$3=$4=$5=$6=$7=$8=""; print $0}'
}

# BOOLEAN to yield basic help 
help-requested() {
    [[ ($# -eq 0) || ( "${1-}" =~ ^-+h(elp)?$ ) ]];
}
# BOOLEAN to yield basic info
info-requested() {
    [[ ( "${1-}" =~ ^-+info$ ) ]];
}

# checks the file structure is correct
agogo-verify-setup() {
    if [[ !(-d userdata) ]]; then
        mkdir "userdata"
    fi

    if [[ !(-e "$(agogo-workspaces-list-file)") ]]; then
        touch "$(agogo-workspaces-list-file)"
    fi
    if !(agogo-workspace-exists "default");  then
        agogo-create "default"
    fi
    agogo-print ""
}

main() {
    # Check that all the required files are in place
    agogo-verify-setup
    
    # We catch some floundering around with args, and certain flags
    if help-requested "$@"; then
        agogo-help
    elif info-requested "$@"; then
        agogo-help "info"
    fi
    
    # Otherwise we check for a proper use case
    local main_command=$1
    shift
    if  [[ ($main_command == "help") ]]; then
        agogo-help "$@"
    elif [[ ($main_command == "clockon") ]]; then
        agogo-clockon "$@"
    elif [[ ($main_command == "clockoff") ]]; then
        agogo-clockoff
    elif [[ ($main_command == "create") ]]; then
        agogo-create "$@"
    elif [[ ($main_command == "destroy") ]]; then
        agogo-destroy "$@"
    elif [[ ($main_command == "list") ]]; then
        if [[ ($# -eq 0) ]]; then
            agogo-list-workspaces
        else
            agogo-list-projects "$@"
        fi
    elif [[ ($main_command == "status") ]]; then
        agogo-status
    elif [[ ($main_command == "add") ]]; then
        agogo-add "$@"
    elif [[ ($main_command == "remove") ]]; then
        agogo-remove "$@"
    else
        # if all else fails, we complain
        agogo-print "Not a recognised agogo command. Try 'agogo -h', or 'agogo --help' for a list of commands."
    fi
    agogo-print ""
    exit 0
}

main "$@"

