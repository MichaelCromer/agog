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
# add this function to your ~/.bashrc or ~/.zshrc

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


function sagen() {
    if [ -x "$(command -v say)" ]; then
        # macOS
        echo $@ | say
    else
        # other
        echo $@ | espeak -s 120 2>/dev/null
    fi
}

function agogo-clockoff() {
    echo "in clockoff"
}

function agogo-clockon() {
    agogo-clockoff
    echo "in clockon with $1"
}

function remindme() {
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

function usage() {
    cat <<HELPMSG
usage: agogo [--version] [--help] <command> <args>

Help to partition time between various projects in a way that is fair to all of them.

Available commands:

basic flags
    -h, --help      Show this help and exit
    -v, --version   Show the version number
    -?, --info      Explain agogo.sh

start and end the program
    clockon         Initialise the program
    clockoff        End the program

operate on workspaces
    create          Create a new workspace
    destroy         Destroy a workspace
    list            List workspaces or their contents

operate on projects
    add             Create a new project
    remove          Remove a project
    set             Set the properties of a project
    toggle          Toggle the project as active/inactive

HELPMSG
    exit 0
}

function information() {
    cat <<INFOMSG
################################################################################
--------------------------------------------------------------------------------
ἀγωγός - agōgós - /a.ɡɔː.ɡós/
    adjective, m or f; second declension (Attic Greek)
    1. leading, guiding
    2. (masc. substantive) guide, escort
    3. (with πρός or ἐπί) leading to
    4. drawing, attracting
    5. eliciting, evoking
--------------------------------------------------------------------------------
agogô
    noun, m (Portuguese < Yoruba)
    1. percussion instrument consisting of two or three small conical bells
--------------------------------------------------------------------------------
à gogo 
    adverb (French)
    1. in abundance, galore
--------------------------------------------------------------------------------
################################################################################

# What and why?

A *pedagogue* is a teacher, especially a strict or pedantic one. It derives in
part from the Greek ἀγωγός, with the sense of "leading" or "guiding".

Do you sometimes reflect upon your time in high school and consider that
(regardless of any *other* evils of that place) the timetable system was highly
effective at getting nformation into your head? If only you could recapture
that structure for yourself - rigid blocks of time with clear topics, scattered
almost uniformly at random through your week.

Even if you have the discipline to commit to one particular topic at a time, do
you have the arbitrary disregard for human feelings of an automated timetabling
system? (Don't answer that). How do you sprinkle your workdays evenly with all
the things you need to do?  

Agogos is here to help. Create a workspace and add projects to it. Agogos will
ring two or three small bells and prompt you with the change of topic at a
frequency you choose. Agogos will select your next topic at random, but will
weight projects more heavily if it has been a long time since you looked at
them, or if you flagged them as important (or both).

Hopefully your guilt and feeling of hopelessness over your abundance of tasks
galore will soon abate.

################################################################################
INFOMSG
    exit 0
}

function purge-all() {
    procs=$(ps -ef | grep $0 | grep -vE "(grep|$0 -k|$0 --kill)")
    if ! [ -z "$procs" ]; then # check if ps results are null
        list-all
        read -p "Kill these processes? [y/N] " -n 1 -r
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            echo
            ps -ef | grep $0 | grep -vE "(grep|$0 -k|$0 --kill)" | tr -s ' ' | awk -F' ' '{print $2}' | xargs kill
        else
            if [[ $REPLY =~ ^[Nn]$ ]]; then echo; fi
            echo "kill command aborted"
            exit 0
        fi
    else
        echo "No reminders to kill"
    fi
}

function list-all() {
    echo "running reminders"
    ps -ef | grep $0 | grep -vE "(grep|$0 -l|$0 --list|$0 -k|$0 --kill)" | tr -s ' ' | awk -F' ' '{$1=$2=$3=$4=$5=$6=$7=$8=""; print $0}'
}

function help-requested() {
    [[ ($# -eq 0) || ( "${1-}" =~ ^-*h(elp)?$ ) ]];
}
function info-requested() {
    [[ ( "${1-}" =~ ^-*i(nformation)?$ ) ]];
}

function main() {
    if help-requested "$@"; then
        usage
        
    fi
    if info-requested "$@"; then
        information
    fi
    local main_command=$1
    
    if [[ ($main_command == "clockon") ]]; then
        local workspace=${2:-"default"}
        agogo-clockon "$workspace"
    elif [[ ($main_command == "clockoff") ]]; then
        agogo-clockoff
    fi
    usage
}

#if [[ "${1-}" =~ ^-*h(elp)?$ ]]; then
    #usage
#fi

main "$@"

#   
#   function main() {
#       if [[ ($@ == "--kill") || ($@ == "-k") ]]; then
#           purge-all
#       elif [[ ($@ == "--list") || ($@ == "-l") ]]; then
#           list-all
#       elif [[ ($@ == "--help") || ($@ == "-h") || ! ($# -eq 3) ]]; then
#           usage
#       else
#           remindme "$@"
#       fi
#   }
#   
#   main "$@"
