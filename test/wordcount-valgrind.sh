#!/bin/bash

export DIR="./test"
export error=false
export test_id=1

# Set source for dynamic libraries
export LD_LIBRARY_PATH=./src

# Verify test outputs
# Params:
# - command Tested command
# - name Tested command in friendly format
function verify() {
  # Run Valgrind test on the command
  # Exit code 3 is reserved for C++ unhandled exceptions, so in C normally can't occur - this is a workaround
  # Eval propagates exit code if program or valgrind set exit code non-equal to zero
  eval "valgrind --error-exitcode=3 --log-file=$DIR/valgrind-output --quiet --leak-check=full --show-leak-kinds=all -s $1 >/dev/null 2>&1"

  # Publish results
  if [ $? == 3 ]; then
    error=true
    printf "\e[91m[%.2d] Failed: $2\e[39m\n" $test_id

    printf "\t\e[1mValgrind output:\e[0m\n"
    cat $DIR/valgrind-output | awk '{printf "\t\t%s\n", $0}'
  else
    printf "\e[92m[%.2d] Successful\e[39m\n" $test_id
  fi

  test_id=$((test_id + 1))
}

# Basic run
verify "cat $DIR/words | ./src/wordcount-dynamic" "Basic run (dynamic)"

# /usr/share/dict/words
verify "cat /usr/share/dict/words | ./src/wordcount-dynamic" "/usr/share/dict/words (dynamic)"

# Generated numbers
seq 1000000 2000000 | shuf | tail -n 10000 > $DIR/input
verify "cat $DIR/input | ./src/wordcount-dynamic" "/usr/share/dict/words (dynamic)"

# Exit code by results
if [ $error == true ]; then
  exit 1
else
  exit 0
fi
