#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m'

separator() {
   echo -e "\n${BLUE}==================== $1 ====================${NC}\n"
   sleep 1
}

test_result() {
   local cmd="$1"
   local test_name="$2"
   
   echo -e "${YELLOW}Command:${NC} $cmd"
   echo -e "${YELLOW}Output:${NC}"
   eval "$cmd"
   local result=$?
   echo -e "${YELLOW}Exit code:${NC} $result"
   
   if [ $result -eq 0 ]; then
       echo -e "${GREEN}✓ Test passed: $test_name${NC}"
   else
       echo -e "${RED}✗ Test failed: $test_name${NC}"
   fi
   echo -e "${YELLOW}Expected shell behavior:${NC}"
   if [[ "$cmd" == *"nonexistent"* ]]; then
       eval "< nonexistent ls | wc > outfile" 2>&1
   elif [[ "$cmd" == *"noperm"* ]]; then
       eval "< noperm ls | wc > outfile" 2>&1
   elif [[ "$cmd" == *"'invalidcmd'"* ]]; then
       eval "< infile invalidcmd | wc > outfile" 2>&1
   else
       eval "$cmd" 2>&1
   fi
   echo -e "Shell exit code: $?"
   echo -e "----------------------------------------\n"
   sleep 0.5
}

clear
echo -e "${YELLOW}Starting Pipex Tests...${NC}"
sleep 1

separator "BASIC FUNCTIONALITY TESTS"

echo "hello world\ntest line\nhello test\nrandom line" > infile
test_result "echo 'hello world' > infile" "Create test file"
test_result "./pipex infile 'grep hello' 'wc -l' outfile" "Basic pipe functionality"
test_result "diff outfile <(< infile grep hello | wc -l)" "Compare with shell command"

separator "ERROR HANDLING TESTS"

test_result "./pipex nonexistent 'ls' 'wc' outfile" "Non-existent input file"
test_result "./pipex infile 'invalidcmd' 'wc' outfile" "Invalid command"
touch noperm && chmod 000 noperm
test_result "./pipex noperm 'ls' 'wc' outfile" "Permission denied (input)"
test_result "./pipex infile 'ls' 'wc' noperm" "Permission denied (output)"
test_result "./pipex infile" "Too few arguments"
test_result "./pipex infile 'ls' 'wc' outfile extra" "Too many arguments"

separator "COMPLEX COMMAND TESTS"

test_result "./pipex infile 'sort' 'uniq -c' outfile" "Sort and unique"
test_result "diff outfile <(< infile sort | uniq -c)" "Compare sort|uniq"
test_result "./pipex infile 'cat -e' 'grep test' outfile" "Cat and grep"
test_result "diff outfile <(< infile cat -e | grep test)" "Compare cat|grep"

separator "BONUS TESTS"

test_result "./pipex infile 'cat' 'grep test' 'wc -l' 'tr a b' outfile" "Multiple pipes"
test_result "diff outfile <(< infile cat | grep test | wc -l | tr a b)" "Compare multiple pipes"

echo -e "${YELLOW}Testing heredoc (manual input required):${NC}"
test_result "./pipex here_doc EOF 'grep test' 'wc -l' outfile" "Heredoc functionality"

separator "MEMORY TESTS"

test_result "valgrind --leak-check=full --show-leak-kinds=all ./pipex infile 'ls' 'wc' outfile" "Memory leaks check"

separator "CLEANUP"

test_result "rm -f infile outfile outfile_expected noperm" "Cleanup"

separator "TEST SUMMARY"