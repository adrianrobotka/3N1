#!/usr/bin/env bash

PIDFILE="log/collatz.pid"

if [ ! -f "$PIDFILE" ] || ! kill -0 $(cat "$PIDFILE"); then
        echo "Not running" >&2
        exit 1
fi

kill -15 $(cat "$PIDFILE") && rm -f "$PIDFILE"