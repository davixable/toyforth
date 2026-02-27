
EXECUTABLE="./toyforth"
TEST_DIR="tests"
PASSED=0
FAILED=0


GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo "Running ToyForth Test Suite..."
echo "------------------------------"

if [ ! -f "$EXECUTABLE" ]; then
    echo -e "${RED}Error: Executable '$EXECUTABLE' not found. Run 'make' first.${NC}"
    exit 1
fi

for test_file in "$TEST_DIR"/*.tf; do
    base_name=$(basename "$test_file" .tf)
    expected_file="$TEST_DIR/$base_name.expected"
    out_file="$TEST_DIR/$base_name.out"

    if [ ! -f "$expected_file" ]; then
        echo -e "${RED}[SKIP]${NC} $base_name (Missing .expected file)"
        continue
    fi

    $EXECUTABLE "$test_file" > "$out_file" 2>&1

    if diff -q -w "$expected_file" "$out_file" > /dev/null; then
        echo -e "${GREEN}[PASS]${NC} $base_name"
        ((PASSED++))
    else
        echo -e "${RED}[FAIL]${NC} $base_name"
        echo "  Expected:"
        cat "$expected_file"
        echo ""
        echo "  Got:"
        cat "$out_file"
        echo ""
        ((FAILED++))
    fi
done

echo "------------------------------"
echo "Tests completed: $((PASSED + FAILED)) | Passed: $PASSED | Failed: $FAILED"

if [ "$FAILED" -ne 0 ]; then
    exit 1
else
    exit 0
fi