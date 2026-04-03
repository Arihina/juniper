#!/usr/bin/env bash

set +e

EXAMPLES_DIR="examples"
LOG_DIR="valgrind_test"

mkdir -p "$LOG_DIR"

FAIL=0

echo "===== VALGRIND TESTS START ====="
echo ""

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

    status=$?

    echo "----- Summary for $filename -----"

    grep "definitely lost" "$logfile"
    grep "indirectly lost" "$logfile"
    grep "possibly lost" "$logfile"

    if grep -q "definitely lost: 0 bytes" "$logfile"; then
        echo "✔ No definite leaks"
    else
        echo "❌ Memory leaks detected!"
        FAIL=1
    fi

    if [ $status -ne 0 ]; then
        echo "❌ Script error (exit code $status)"
        FAIL=1
    fi

    echo ""
    echo "✔ Log saved to $logfile"
    echo "------------------------------"
    echo ""
done

echo "===== VALGRIND TESTS END ====="

if [ $FAIL -ne 0 ]; then
    echo "❌ FAIL: There were errors or memory leaks"
    exit 1
else
    echo "✅ SUCCESS: No leaks detected"
    exit 0
fi