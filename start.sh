#!/usr/bin/env bash

DATE=$(date +%Y-%m-%d_%H-%M-%S)

mkdir -p log

LOGFILE="log/$DATE.log"
PIDFILE="log/collatz.pid"

if [ -f $PIDFILE ] && kill -0 $(cat $PIDFILE); then
        echo "Already running" >&2
        exit 1
fi

touch $LOGFILE

echo "[$(date +%Y-%m-%d) $(date +%H:%M:%S)] Starting program" >> $LOGFILE
nohup ./collatz_tester > $LOGFILE & echo $! > $PIDFILE
