#!/bin/bash

# Constants
INVALID_USER_ID_MSG="Please provide a user ID as argument."
INVALID_DATE_FORMAT_MSG="Invalid date format: %s. Please provide a date in the format yyyy-mm-dd."

# Functions
check_user_id() {
    if [[ -z "$1" ]]; then
        echo "$INVALID_USER_ID_MSG"
        exit 1
    fi
}

check_date_format() {
    if [[ ! $1 =~ ^[0-9]{4}-[0-9]{2}-[0-9]{2}$ || $(date -j -f "%Y-%m-%d" "$1" >/dev/null 2>&1 && echo "valid" || echo "invalid") =~ "invalid" ]]; then
        printf "$INVALID_DATE_FORMAT_MSG\n" "$1"
        exit 1
    else
        echo "$1"
    fi
}

get_user_location() {
    location=$(echo "$1" | awk -F'"location":' '{print $2}' | awk -F'"' '{print $2}')
    echo "User location: $location"
}

get_user_coalition() {
    coalition=$(echo "$1" | awk -F'"name":' '{print $2}' | awk -F'"' '{print $2}')
    score=$(echo "$1" | awk -F'"score":' '{print $2}' | awk -F',' '{print $1}')
    echo "User coalition: $coalition, score: $score"
}

get_user_logtime() {
    command=$(echo "$1" | grep -o '[0-9]\{2\}:[0-9]\{2\}' | awk -F: '{sum += ($1 * 60) + $2} END {printf "%d:%02d\n", sum/60, sum%60}')
    echo "User logtime: $command"
}

# Main script
check_user_id "$2"
case "$1" in
"location")
    output=$(curl -sH "Authorization: Bearer $API42_TOKEN" "https://api.intra.42.fr/v2/users/${2}/locations?filter\[end\]=false")
    get_user_location "$output"
    ;;
"coalition")
    output=$(curl -sH "Authorization: Bearer $API42_TOKEN" "https://api.intra.42.fr/v2/users/${2}/coalitions")
    get_user_coalition "$output"
    ;;
"logtime")
    start_date=$(check_date_format "${3:-$(date +'%Y-%m-%01')}")
    end_date=$(check_date_format "${3:-$(date +'%Y-%m-%d')}")
    # echo "$start_date $end_date"
    output=$(curl -sH "Authorization: Bearer $API42_TOKEN" "https://api.intra.42.fr/v2/users/${2}/locations_stats?begin_at=\[$start_date\]&end_at=\[$end_date\]")
    get_user_logtime "$output"
    ;;
esac
