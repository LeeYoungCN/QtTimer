#!/usr/local/bin/bash
SCRIPT_DIR="$(
    cd "$(dirname "$0")" || exit 1
    pwd
)"
ROOT_DIR="$(
    cd "${SCRIPT_DIR}/.." || exit 1
    pwd
)"
# shellcheck disable=SC1091
source "${SCRIPT_DIR}/common_func.sh"

cd "${ROOT_DIR}" || exit 1

# 定义重试间隔时间(秒)
RETRY_DELAY=5

function git_push() {
    print_log "开始尝试执行git push -f, 将持续重试直到成功..."
    print_log "重试间隔: $RETRY_DELAY 秒"
    print_log "按Ctrl+C可终止脚本"

    # 循环执行循环执行git push -f
    while true; do
        # 执行git push -f并捕获退出码
        git push -f
        exit_code=$?

        # 检查命令是否成功执行
        if [ $exit_code -eq 0 ]; then
            print_log "✅ git push -f 执行成功!"
            exit 0
        else
            print_log "❌ git push -f 执行失败，退出码: $exit_code"
            print_log "将在 $RETRY_DELAY 秒后重试..."
            sleep $RETRY_DELAY
        fi
    done
}

function git_pull() {
    print_log "开始尝试执行git pull --rebase, 将持续重试直到成功..."
    print_log "重试间隔: $RETRY_DELAY 秒"
    print_log "按Ctrl+C可终止脚本"

    # 循环执行循环执行git push -f
    while true; do
        # 执行git push -f并捕获退出码
        git pull --rebase
        exit_code=$?

        # 检查命令是否成功执行
        if [ $exit_code -eq 0 ]; then
            print_log "✅ git pull --rebase 执行成功!"
            exit 0
        else
            print_log "❌ git pull --rebase 执行失败，退出码: $exit_code"
            print_log "将在 $RETRY_DELAY 秒后重试..."
            sleep $RETRY_DELAY
        fi
    done
}

if [ "$1" == "pull" ]; then
    git_pull
elif [ "$1" == "push" ]; then
    git_push
else
    print_log "Input invalid!"
fi
