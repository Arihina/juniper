#!/usr/bin/env bash

EXAMPLES_DIR="examples"
LOG_DIR="valgrind_test"

mkdir -p "$LOG_DIR"

for file in "$EXAMPLES_DIR"/*.py; do
    filename=$(basename "$file" .py)
    logfile="$LOG_DIR/valgrind_${filename}.log"

    echo "▶ Running $file"

    valgrind \
        --leak-check=full \
        --show-leak-kinds=all \
        --track-origins=yes \
        --log-file="$logfile" \
        python3.11 "$file"

    echo "✔ Done → $logfile"
    echo ""
done
