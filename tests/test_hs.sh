#!/bin/bash

BIN=./sfs
PIPE=$(mktemp -u)
mkfifo $PIPE
$BIN < $PIPE &
PID=$!
exec 3> $PIPE

send() { echo "$1" >&3; }

echo "=== Stress test: 1000 operations ==="

for i in $(seq 1 200); do
    send "CREATE test$i 512"
done

for i in $(seq 1 200); do
    send "WRITE test$i 0 \"DATA_$i\""
done

for i in $(seq 1 200); do
    send "READ test$i 0 10"
done

for i in $(seq 1 200); do
    send "DELETE test$i"
done

send "LIST"
send "EXIT"

sleep 1
kill $PID 2>/dev/null
rm -f $PIPE

echo "=== Stress Test Complete ==="
