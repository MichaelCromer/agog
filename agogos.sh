#!/usr/bin/env bash

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

set -o errexit
set -o pipefail
set -o nounset
if [[ "${TRACE-0}" == "1" ]]; then set -o xtrace; fi

agogo-workspace-exists() {
    grep -qE ^$1$ userdata/workspaces.agogo
}

agogo-get-current-workspace() {
    if [[ -e userdata/.current-ws.agogo ]]; then
        echo < "userdata/.current-ws.agogo" 
    else
        echo ""
    fi
}

agogo-confirm-prompt() {
    read -p "Are you sure? $1 [y/N] " -n 1 -r
    [[ $REPLY =~ ^[Yy]$ ]];
}

sagen() {
    if [ -x "$(command -v say)" ]; then
        # macOS
        echo $@ | say
    else
        # other
        echo $@ | espeak -s 120 2>/dev/null
    fi
}

agogo-clockoff() {
    if [[ !(-e userdata/.current-ws.agogo) ]]; then
        echo "Error: agogo is not clocked on"
        exit 1
    fi
    if (agogo-confirm-prompt "This will terminate all agogo processes."); then
        rm userdata/.current-ws.agogo
        echo "\nTODO complete clockoff logic"
    else
        echo "Clockoff aborted; agogo is still running"
    fi
}

agogo-clockon() {
    echo "$1" > userdata/.current-ws.agogo
    echo "TODO complete clockon logic"
    echo "Clocked on to workspace '$1'"
}

agogo-create() {
    if [[ ($# -eq 0) ]]; then
        echo "Error: You must specify a workspace name"
        exit 1
    fi
    
    if (agogo-workspace-exists $1); then
        echo "Error: Workplace '$1' already exists!"
    else
        echo $1 >> userdata/workspaces.agogo
        touch "userdata/ws-$1.agogo"
        echo "Created new workspace '$1'"
    fi
}

agogo-destroy() {
    if [[ ($# -eq 0) ]]; then
        echo "Error: You must specify a workspace name"
        exit 1
    fi
    
    if (agogo-workspace-exists "$1"); then
        if (agogo-confirm-prompt "This will destroy the workspace '$1' and all its projects"); then
            sed -i "/^$1$/d" userdata/workspaces.agogo
            echo "\nDestroyed workspace '$1' and all its projects"
        fi
    else
        echo "Error: Workspace '$1' does not exist."
    fi
}

agogo-status() {
    local curr_ws=userdata/.current-ws.agogo
    if [[ !(-e $curr_ws) ]]; then
        echo "You are clocked off"
    else
        echo "Currently clocked on to workspace '$(agogo-get-current-workspace)'"
    fi
}

agogo-list-workspaces() {
    cat userdata/workspaces.agogo
}

agogo-list-projects() {
    if (agogo-workspace-exists "$1"); then
        cat "userdata/ws-$1.agogo"
    else
        echo "Error: No workspace with name '$1' exists."
        exit 1
    fi
}

remindme() {
    ((frequency = $2 * 60))
    ((duration = $3 * 3600))
    if [[ $frequency -ge $duration ]]; then
        echo "Duration needs to be greater than frequency!"
        echo "you gave a frequency of $2 min ($frequency sec), and a duration of $3 hours ($duration sec)"
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

agogo-help() {
    if [[ ($# -eq 0) ]]; then
        cat "helpfiles/basic.agogo"
        exit 0
    fi

    local helpfile="helpfiles/$1.agogo"
    
    if [[ !(-e "$helpfile") ]]; then
        echo "Error: $1 is not a recognised agogo command or help topic"
        exit 1
    fi
    
    cat "$helpfile"
    exit 0
}

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
            echo "Clockoff aborted; agogo is still running"
            exit 0
        fi
    else
        echo "No reminders to kill"
    fi
}

list-all() {
    echo "This will terminate agogo running on the following workspace"
    ps -ef | grep $0 | grep -vE "(grep|$0 -l|$0 --list|$0 -k|$0 --kill)" | tr -s ' ' | awk -F' ' '{$1=$2=$3=$4=$5=$6=$7=$8=""; print $0}'
}

help-requested() {
    [[ ($# -eq 0) || ( "${1-}" =~ ^-+h(elp)?$ ) ]];
}
info-requested() {
    [[ ( "${1-}" =~ ^-+info$ ) ]];
}

agogo-verify-setup() {
    if [[ !(-e "userdata/workspaces.agogo") ]]; then
        touch userdata/workspaces.agogo
    fi
    if !(agogo-workspace-exists "default");  then
        agogo-create "default"
    fi
}

main() {
    # This checks that all the required files are in place
    agogo-verify-setup
    
    # We catch some floundering around and certain flags
    if help-requested "$@"; then
        agogo-help
    elif info-requested "$@"; then
        agogo-help "info"
    fi
    
    # Otherwise we check for a proper use case
    local main_command=$1
   
    if  [[ ($main_command == "help") ]]; then
        shift
        agogo-help "$@"
    elif [[ ($main_command == "clockon") ]]; then
        local workspace=${2:-"default"}
        agogo-clockon "$workspace"
    elif [[ ($main_command == "clockoff") ]]; then
        agogo-clockoff
    elif [[ ($main_command == "create") ]]; then
        shift
        agogo-create "$@"
    elif [[ ($main_command == "destroy") ]]; then
        shift
        agogo-destroy "$@"
    elif [[ ($main_command == "list") ]]; then
        shift
        if [[ ($# -eq 0) ]]; then
            agogo-list-workspaces
        else
            agogo-list-projects "$@"
        fi
    elif [[ ($main_command == "status") ]]; then
        agogo-status
    else
        echo "Not a recognised agogo command. Try "agogo help", "agogo -h", or "agogo --help" for a list of commands."
    fi
    exit 0
}

main "$@"

