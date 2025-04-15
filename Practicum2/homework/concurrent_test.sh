#!/bin/bash

echo "Starting first client to WRITE large file..."
./client <<EOF &
rfs WRITE data/test_long.txt test/long.txt
EOF

# Wait slightly so that WRITE starts before GET
sleep 1

echo "Starting second client to GET the same file (should wait until WRITE completes)..."
time ./client <<EOF
rfs GET test/long.txt data2/test_long.txt
EOF
