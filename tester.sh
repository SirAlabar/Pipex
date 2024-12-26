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
   
   # Mudando condições de sucesso/falha para erros específicos
   if [[ "$test_name" == *"Permission denied"* && $result -eq 126 ]]; then
       echo -e "${GREEN}✓ Test passed: $test_name${NC}"
   elif [[ "$test_name" == *"Invalid command"* && $result -eq 127 ]]; then
       echo -e "${GREEN}✓ Test passed: $test_name${NC}"
   elif [[ "$test_name" == *"Non-existent"* && $result -eq 1 ]]; then
       echo -e "${GREEN}✓ Test passed: $test_name${NC}"
   elif [[ "$test_name" == *"arguments"* && $result -eq 1 ]]; then
       echo -e "${GREEN}✓ Test passed: $test_name${NC}"
   elif [ $result -eq 0 ]; then
       echo -e "${GREEN}✓ Test passed: $test_name${NC}"
   else
       echo -e "${RED}✗ Test failed: $test_name${NC}"
   fi
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

echo -e "${YELLOW}Testing heredoc:${NC}"
test_result "./pipex here_doc EOF 'cat' 'grep test' outfile" "Heredoc with simple commands"
test_result "./pipex here_doc LIMITER 'grep a' 'wc -l' outfile" "Heredoc with different limiter"
test_result "./pipex here_doc END 'cat' 'sort' 'uniq' outfile" "Heredoc with multiple pipes"

separator "MEMORY TESTS"

echo -e "${YELLOW}Testing memory with basic commands:${NC}"
test_result "valgrind --leak-check=full --show-leak-kinds=all ./pipex infile 'ls' 'wc' outfile" "Basic memory test"

echo -e "${YELLOW}Testing memory with invalid commands:${NC}"
test_result "valgrind --leak-check=full --show-leak-kinds=all ./pipex infile 'invalid' 'wc' outfile" "Memory with invalid command"

echo -e "${YELLOW}Testing memory with heredoc:${NC}"
test_result "valgrind --leak-check=full --show-leak-kinds=all ./pipex here_doc EOF 'cat' 'wc -l' outfile" "Memory with heredoc"

echo -e "${YELLOW}Testing memory with multiple pipes:${NC}"
test_result "valgrind --leak-check=full --show-leak-kinds=all ./pipex infile 'cat' 'grep a' 'wc -l' 'sort' outfile" "Memory with multiple pipes"

separator "ADDITIONAL MEMORY TESTS"

echo -e "${YELLOW}Testing memory with empty file:${NC}"
touch empty_file
test_result "valgrind --leak-check=full --show-leak-kinds=all ./pipex empty_file 'cat' 'wc -l' outfile" "Memory with empty file"

echo -e "${YELLOW}Testing memory with large command:${NC}"
test_result "valgrind --leak-check=full --show-leak-kinds=all ./pipex infile 'cat' 'grep test | sort | uniq | wc -l' outfile" "Memory with large command"

echo -e "${YELLOW}Testing memory with special characters:${NC}"
test_result "valgrind --leak-check=full --show-leak-kinds=all ./pipex infile 'grep \"\\$\"' 'tr -d \"\\$\"' outfile" "Memory with special chars"

echo -e "${YELLOW}Testing memory with non-existent path:${NC}"
test_result "valgrind --leak-check=full --show-leak-kinds=all ./pipex infile '/non/existent/path/ls' 'wc' outfile" "Memory with invalid path"

echo -e "${YELLOW}Testing memory with heredoc and special delimiter:${NC}"
test_result "valgrind --leak-check=full --show-leak-kinds=all ./pipex here_doc 'END$$##' 'sort -r' 'uniq' outfile" "Memory with special delimiter"

# Cleanup the extra test file
rm -f empty_file

separator "CLEANUP"

test_result "rm -f infile outfile outfile_expected noperm" "Cleanup"

separator "TEST SUMMARY"