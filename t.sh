#!/bin/sh

RED='\033[0;31m'
GRN='\033[0;32m'
CLR='\033[0m'
CALC='cl'

check_input() {
    expected="$(printf "$1")"
    recieved="$(printf "$2")"
    if [ "$expected" = "$recieved" ] ; then
        printf "${GRN}PASSED${CLR}\n"
    else
        printf "${RED}FAILED${CLR}\n"
        printf "expected: %s\n" "$expected"
        printf "recieved: %s\n" "$recieved"
    fi
}

printf "%-40s" "TEST 1: stack prints top"
recieved=$(printf "77438291047 7329847 7428391748 4789231748 2398 p\n" | "$CALC")
check_input "2398" "$recieved"

printf "%-40s" "TEST 2: stack clears"
recieved=$(printf "3489 3948 932 84984 2\nc\ns\n" | "$CALC")
check_input "" "$recieved"

printf "%-40s" "TEST 3: clear stack add one prints"
recieved=$(printf "3489 3948 932 84984 2\nc\n23894 s\n" | "$CALC")
check_input "23894" "$recieved"

printf "%-40s" "TEST 4: stack prints all"
recieved=$(printf "3489 3948 932 84984 2\ns\n" | "$CALC")
check_input "3489\n3948\n932\n84984\n2" "$recieved"

printf "%-40s" "TEST 5: two small numbers add"
recieved=$(printf "3489 1392 + p\n" | "$CALC")
check_input "4881" "$recieved"

printf "%-40s" "TEST 6: two large numbers add"
recieved=$(printf "47647892536598465938746598765923876453869754295 37819456915670831256934185610651084756378580315 + p\n" | "$CALC")
check_input "85467349452269297195680784376574961210248334610" "$recieved"

printf "%-40s" "TEST 7: small and large numbers add"
recieved=$(printf "13 37819456915670831256934185610651084756378580315 + p\n" | "$CALC")
check_input "37819456915670831256934185610651084756378580328" "$recieved"

printf "%-40s" "TEST 8: large and small numbers add"
recieved=$(printf "37819456915670831256934185610651084756378580315 13 + p\n" | "$CALC")
check_input "37819456915670831256934185610651084756378580328" "$recieved"

printf "%-40s" "TEST 9: small numbers carry"
recieved=$(printf "99999999999999999 99999999999999999 + p\n" | "$CALC")
check_input "199999999999999998" "$recieved"

printf "%-40s" "TEST 10: numbers carry over word"
recieved=$(printf "999999999999999999 999999999999999999 + p\n" | "$CALC")
check_input "1999999999999999998" "$recieved"

printf "%-40s" "TEST 11: large numbers carry"
recieved=$(printf "99999999999999999999 99999999999999999999 + p\n" | "$CALC")
check_input "199999999999999999998" "$recieved"

printf "%-40s" "TEST 12: 1 plus carry over word"
recieved=$(printf "1 999999999999999999 + p\n" | "$CALC")
check_input "1000000000000000000" "$recieved"

