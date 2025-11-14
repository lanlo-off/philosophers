#!/bin/bash
PHILO="./philo"
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Vérifier que philo est compilé
if [ ! -f "$PHILO" ]; then
    echo -e "${RED}Error: $PHILO not found. Run 'make' first.${NC}"
    exit 1
fi

declare -a TESTS=(
    "1 800 200 200:death"
    "5 800 200 200:no_death"
    "5 800 200 200 7:no_death_count"
    "4 410 200 200:no_death"
    "4 310 200 100:death"
)

REPORT="philo_test_report.txt"
LOGDIR="logs"
mkdir -p "$LOGDIR"
echo "PHILO TEST SUITE REPORT - $(date)" > "$REPORT"
echo "" >> "$REPORT"

echo -e "${YELLOW}=== PHILO TEST SUITE ===${NC}\n"

for test in "${TESTS[@]}"; do
    IFS=':' read -r args expected <<< "$test"
    label=$(echo "$args" | tr ' ' '_')
    valgrind_log="$LOGDIR/valgrind_${label}.log"
    helgrind_log="$LOGDIR/helgrind_${label}.log"
    run_log="$LOGDIR/run_${label}.log"

    echo -e "${YELLOW}Testing: philo $args (expected: $expected)${NC}"
    echo "─────────────────────────────────────────────"
    echo "Test: philo $args (expected: $expected)" >> "$REPORT"

    # for runs expected no_death, enforce 8s timeout
    if [[ $expected == no_death* ]]; then
        RUN_TIMEOUT=8
    else
        RUN_TIMEOUT=0
    fi

    # Test 1: Normal run (with optional timeout)
    echo -e "${YELLOW}[1/3] Normal run:${NC}"
    if [ $RUN_TIMEOUT -gt 0 ]; then
        timeout $RUN_TIMEOUT $PHILO $args > "$run_log" 2>&1
        run_status=$?
        # collect output (may be truncated by timeout)
        output=$(cat "$run_log")
    else
        $PHILO $args > "$run_log" 2>&1
        run_status=$?
        output=$(cat "$run_log")
    fi

    if echo "$output" | grep -q "died"; then
        echo -e "${RED}Result: DEATH detected${NC}"
        echo "Result: DEATH detected" >> "$REPORT"
    else
        echo -e "${GREEN}Result: NO DEATH${NC}"
        echo "Result: NO DEATH" >> "$REPORT"
    fi

    # Count meals per philosopher if any "is eating" lines present
    if echo "$output" | grep -q "is eating"; then
        echo -e "\n${YELLOW}Meal count per philosopher:${NC}"
        echo "Meal counts:" >> "$REPORT"
        echo "$output" | grep "is eating" | awk '{print $2}' | sort -n | uniq -c | \
            while read -r cnt idx; do
                printf "  Philo %s: %s meals\n" "$idx" "$cnt"
                echo "  Philo $idx: $cnt" >> "$REPORT"
            done
    else
        echo "No 'is eating' lines in output." >> "$REPORT"
    fi

    echo "" >> "$REPORT"

    # Test 2: Valgrind leak check (use timeout for no_death runs)
    echo -e "${YELLOW}[2/3] Valgrind leak check:${NC}"
    if [ $RUN_TIMEOUT -gt 0 ]; then
        timeout $RUN_TIMEOUT valgrind --leak-check=full --errors-for-leak-kinds=all \
            --error-exitcode=1 --log-file="$valgrind_log" $PHILO $args
        leak_status=$?
    else
        valgrind --leak-check=full --errors-for-leak-kinds=all \
            --error-exitcode=1 --log-file="$valgrind_log" $PHILO $args
        leak_status=$?
    fi

    # Inspect log to decide if Valgrind actually reported errors
    if grep -q "ERROR SUMMARY: 0 errors" "$valgrind_log" 2>/dev/null; then
        echo -e "${GREEN}✓ Valgrind: no errors reported in log${NC}"
        echo "Valgrind: OK (no errors in log)" >> "$REPORT"
    else
        # If no summary line present, treat as error / timeout
        if [ $leak_status -eq 124 ]; then
            echo -e "${YELLOW}Valgrind: timed out${NC}"
            echo "Valgrind: timed out (log: $valgrind_log)" >> "$REPORT"
        else
            echo -e "${RED}✗ Valgrind reported errors or didn't finish (see $valgrind_log)${NC}"
            echo "Valgrind: ERR (see $valgrind_log)" >> "$REPORT"
            echo "---- Valgrind tail for $args ----" >> "$REPORT"
            tail -n 100 "$valgrind_log" >> "$REPORT"
            echo "---------------------------------" >> "$REPORT"
        fi
    fi

    echo "" >> "$REPORT"

    # Test 3: Helgrind (thread safety)
    echo -e "${YELLOW}[3/3] Helgrind thread check:${NC}"
    if [ $RUN_TIMEOUT -gt 0 ]; then
        timeout $RUN_TIMEOUT valgrind --tool=helgrind --error-exitcode=1 \
            --log-file="$helgrind_log" $PHILO $args
        thread_status=$?
    else
        timeout 5 valgrind --tool=helgrind --error-exitcode=1 \
            --log-file="$helgrind_log" $PHILO $args
        thread_status=$?
    fi

    # Inspect helgrind log for actual error summary
    if grep -q "ERROR SUMMARY: 0 errors" "$helgrind_log" 2>/dev/null; then
        echo -e "${GREEN}✓ Helgrind: no errors reported in log${NC}"
        echo "Helgrind: OK (no errors in log)" >> "$REPORT"
    else
        if [ $thread_status -eq 124 ]; then
            echo -e "${YELLOW}Helgrind: timed out${NC}"
            echo "Helgrind: timed out (log: $helgrind_log)" >> "$REPORT"
        else
            echo -e "${RED}✗ Helgrind reported issues (see $helgrind_log)${NC}"
            echo "Helgrind: ERR (see $helgrind_log)" >> "$REPORT"
            echo "---- Helgrind tail for $args ----" >> "$REPORT"
            tail -n 100 "$helgrind_log" >> "$REPORT"
            echo "---------------------------------" >> "$REPORT"
        fi
    fi

    echo "" >> "$REPORT"
    echo "========================================" >> "$REPORT"
    echo "" >> "$REPORT"

    echo ""
    echo -e "${YELLOW}═══════════════════════════════════════════${NC}\n"
done

echo -e "${GREEN}All tests completed! Report: $REPORT${NC}"