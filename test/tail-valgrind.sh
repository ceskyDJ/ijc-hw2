#!/bin/bash

export DIR="./test"
export error=false
export test_id=1

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
verify "./src/tail $DIR/test-file" "tail test-file"

# Input from stdin
verify "cat $DIR/test-file | ./src/tail" "cat test-file | tail"

# Explicit offset from the end of the file 1
verify "./src/tail -n 5 $DIR/test-file" "tail -n 5 test-file"

# Explicit offset from the end of the file 2
verify "cat $DIR/test-file | ./src/tail -n 15" "cat test-file | tail -n 15"

# Explicit offset from the beginning of the file 1
verify "./src/tail -n +20 $DIR/test-file" "tail -n +20 test-file"

# Explicit offset from the beginning of the file 2
verify "cat $DIR/test-file | ./src/tail -n +8" "cat test-file | tail -n +8"

# Empty file 1
verify "./src/tail $DIR/empty-file" "tail empty-file"

# Empty file 2
verify "./src/tail -n 3 $DIR/empty-file" "tail -n 3 empty-file"

# Empty file 3
verify "./src/tail -n +11 $DIR/empty-file" "tail -n +11 empty-file"

# Long lines file 1
verify "./src/tail -n +3 $DIR/long-lines-file" "tail -n +3 long-lines-file"

# Long lines file 2
verify "./src/tail -n 2 $DIR/long-lines-file" "tail -n 2 long-lines-file"

# Error message for long lines file
verify "./src/tail -n +9 $DIR/long-lines-file" "tail -n +9 long-lines-file"

# Bad filename 1
verify "./src/tail $DIR/bad-file" "tail bad-file"

# Bad filename 2
verify "./src/tail -n 1 $DIR/bad-file" "tail -n 1 bad-file"

# Bad filename 3
verify "./src/tail -n +4 $DIR/bad-file" "tail -n +4 bad-file"

# Exit code by results
if [ $error == true ]; then
  exit 1
else
  exit 0
fi
