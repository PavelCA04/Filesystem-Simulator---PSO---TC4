#!/bin/bash

BIN=./sfs
OPS=2000                    # Total operations to perform
MAX_FILES=200               # Range of possible files
SEED=$RANDOM                # Seed for reproducibility
RATE_LIMIT=0.002            # Pause between operations (2 ms)

echo "========== FS FUZZ TESTER =========="
echo "Binary: $BIN"
echo "Operations: $OPS"
echo "Seed: $SEED"
echo "====================================="

# Create temporary FIFO
PIPE=$(mktemp -u)
mkfifo $PIPE

# Run the FS
$BIN < $PIPE &
PID=$!
exec 3> $PIPE

rand() {
    awk -v seed=$SEED 'BEGIN { srand(seed); print int(rand()*32767) }'
    SEED=$((SEED + 1))
}

send() {
    echo "$1" >&3
}

random_string() {
    LEN=$((RANDOM % 20 + 5))
    tr -cd 'A-Za-z0-9' < /dev/urandom | head -c $LEN
}

echo "=== Fuzzing started ==="

for ((i=1; i<=OPS; i++)); do

    r=$((RANDOM % 100))
    FILE="f$((RANDOM % MAX_FILES)).txt"

    case $r in

        # 0–20%: CREATE
        [0-1]*)
            SIZE=$((RANDOM % 2048 + 1))
            send "CREATE $FILE $SIZE"
        ;;

        # 20–40%: WRITE
        [2-3]*)
            OFFSET=$((RANDOM % 2048))
            DATA=$(random_string)
            send "WRITE $FILE $OFFSET \"$DATA\""
        ;;

        # 40–60%: READ
        [4-5]*)
            OFFSET=$((RANDOM % 2048))
            SIZE=$((RANDOM % 150 + 1))
            send "READ $FILE $OFFSET $SIZE"
        ;;

        # 60–80%: DELETE
        [6-7]*)
            send "DELETE $FILE"
        ;;

        # 80–100%: LIST
        *)
            send "LIST"
        ;;

    esac

    sleep $RATE_LIMIT
done

echo "=== Final LIST ==="
send "LIST"

send "EXIT"

sleep 1
kill $PID 2>/dev/null
rm -f $PIPE

echo "======= FUZZ TEST FINALIZADO ======="
