#!/usr/bin/bash

function print_log() {
    local RED='\033[0;31m'
    local GREEN='\033[32m'
    local NC='\033[0m' # 重置颜色

    local log_str="${1}"
    local log_level="${2}"

    case ${log_level} in
    error)
        printf "%s ${RED}%s${NC}\n" "[$(date "+%Y-%m-%d %H:%M:%S.%3N")]" "${log_str}"
        ;;
    info)
        printf "%s ${GREEN}%s${NC}\n" "[$(date "+%Y-%m-%d %H:%M:%S.%3N")]" "${log_str}"
        ;;
    debug | *)
        printf "%s %s\n" "[$(date "+%Y-%m-%d %H:%M:%S.%3N")]" "${log_str}"
        ;;
    esac
}

function replace_text() {
    local old_str="$1"
    local new_str="$2"
    local file_path="$3"
    if [ "$(uname -s)" == "Darwin" ]; then
        if ! sed -i '' "s|${old_str}|${new_str}|g" "${file_path}"; then
            print_log "Replace failed. ${old_str} -> ${new_str}  ${file_path}." error
            return 1
        fi
    else
        if ! sed -i "s|${old_str}|${new_str}|g" "${file_path}"; then
            print_log "Replace failed. ${old_str} -> ${new_str}  ${file_path}." error
            return 1
        fi
    fi
    return 0
}

function rm_dir() {
    local dir="${1}"
    if [ ! -d "${dir}" ]; then
        print_log "Remove [${dir}] success, not exist." info
        return 0
    fi

    if ! rm -rf "${dir}"; then
        print_log "Remove [${dir}] failed." error
        return 1
    else
        print_log "Remove [${dir}] success." info
        return 0
    fi
}

function wright_kv_to_file() {
    local key="$1"
    local val="${2}"
    local file="${3}"

    echo "${key}=\"${val}\"" >>"${file}"
}
