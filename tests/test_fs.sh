#!/bin/bash

BIN=./sfs

echo "===== File System Tests ====="

# Check if sfs exists
if [ ! -f "$BIN" ]; then
    echo "Error: the sfs executable does not exist. Run 'make' first."
    exit 1
fi

# Create a temporary FIFO to interact with the process
PIPE=$(mktemp -u)
mkfifo $PIPE

# Run the FS in the background
$BIN < $PIPE & 
PID=$!

exec 3> $PIPE

# Helper to send commands
send() {
    echo "$1" >&3
}

echo "=== Starting automated tests ==="

### TEST 1: File Creation ###
echo "[1] Creating files..."
for i in $(seq 1 10); do
    send "CREATE file$i.txt 1024"
done

### TEST 2: Writing ###
echo "[2] Writing content..."
send 'WRITE file1.txt 0 "Hello world"'
send 'WRITE file2.txt 100 "ABCDEF"'

### TEST 3: Reading ###
echo "[3] Reading content..."
send "READ file1.txt 0 11"
send "READ file2.txt 100 6"

### TEST 4: Listing ###
echo "[4] Listing..."
send "LIST"

### TEST 5: Invalid operations ###
echo "[5] Error tests..."
send "READ file1.txt 99999 10"
send "WRITE file3.txt 5000 \"Out of bounds\""
send "DELETE no_existe.txt"

### TEST 6: Deletion ###
echo "[6] Deleting files..."
for i in $(seq 1 10); do
    send "DELETE file$i.txt"
done

### FINAL TEST: Listing ###
echo "[7] Final listing..."
send "LIST"

# Exit
send "EXIT"

sleep 1

kill $PID 2>/dev/null
rm -f $PIPE

echo "===== TESTS COMPLETED ====="

