#!/bin/bash

# путь к исполняемому файлу
EXECUTABLE_PATH=$1

if [ -z "$EXECUTABLE_PATH" ]; then
    echo "Usage: $0 <path_to_executable>"
    exit 1
fi

OUTPUT=$($EXECUTABLE_PATH)

# ожидаемый по таску вывод
EXPECTED_OUTPUT="255
0.0
127.0.0.1
123.45.67.89.101.112.131.41
Hello, World!
100.200.300.400
400.300.200.100
123.456.789.0"

# нормализуем вывод 
NORMALIZED_OUTPUT=$(echo "$OUTPUT" | sed 's/[[:space:]]*$//')
NORMALIZED_EXPECTED=$(echo "$EXPECTED_OUTPUT" | sed 's/[[:space:]]*$//')

if [ "$NORMALIZED_OUTPUT" = "$NORMALIZED_EXPECTED" ]; then
    echo "Test 1: Output check passed"
    exit 0
else
    echo "Test 1: Output check failed!"
    echo "Expected output:"
    echo "$EXPECTED_OUTPUT"
    echo ""
    echo "Got output:"
    echo "$OUTPUT"
    echo ""
    echo "Diff:"
    diff <(echo "$NORMALIZED_EXPECTED") <(echo "$NORMALIZED_OUTPUT") || true
    exit 1
fi