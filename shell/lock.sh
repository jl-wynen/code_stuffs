#!/bin/bash

#
# Locking bash scripts to build critical sections
#
# From http://www.kfirlavi.com/blog/2012/11/06/elegant-locking-of-bash-program/
#

set -e

readonly PROGNAME=$(basename "$0")
readonly LOCKFILE_DIR=/tmp
readonly LOCK_FD=200

do_A() {
    echo "A"
}

do_B() {
    echo "B"
}

lock() {
    local prefix=$1
    local fd=${2:-$LOCK_FD}
    local lock_file=$LOCKFILE_DIR/$prefix.lock
    
    # create lock file
    eval "exec $fd>$lock_file"

    # -n makes this fail if the lock is already held
    flock -n $fd \
        && return 0 \
            || return 1
}

eexit() {
    local error_str="$@"
    echo $error_str
    exit 1
}

main() {
    lock $PROGNAME || eexit "only one instance of $PROGNAME can run at one time"

    do_A
    do_B
}
main
