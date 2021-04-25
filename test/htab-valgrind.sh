#!/bin/bash

export DIR="./test"

# C script with tests
test_script="./test/htab-test"

# Run Valgrind test on the command
# Exit code 3 is reserved for C++ unhandled exceptions, so in C normally can't occur - this is a workaround
# Eval propagates exit code if program or valgrind set exit code non-equal to zero
export LD_LIBRARY_PATH=./src
eval "valgrind --error-exitcode=3 --log-file=$DIR/valgrind-output --quiet --leak-check=full --show-leak-kinds=all -s $test_script >/dev/null 2>&1"

# Publish results
if [ $? == 3 ]; then
  printf "\e[91mValgrind test failed\e[39m\n"

  printf "\t\e[1mValgrind output:\e[0m\n"
  cat $DIR/valgrind-output | awk '{printf "\t\t%s\n", $0}'
else
  printf "\e[92mValgrind test was successful\e[39m\n"
fi
