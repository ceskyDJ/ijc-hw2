#!/bin/bash

export DIR="./test"
export error=false
export test_id=1

# Verify test outputs
# Params:
# - command Tested command
# - exit_code Program exit code
function verify_output() {
  inner_error=false
  # Verify program exit code
  if [ "$2" != 0 ]; then
      inner_error=true
  fi

  # Verify program output
  diff $DIR/program-output $DIR/reference-output >/dev/null 2>&1
  if [ $? == 1 ]; then
    inner_error=true
  fi

  # Publish results
  if [ $inner_error == true ]; then
    error=true
    printf "\e[91m[%.2d] Failed: $1\e[39m\n" $test_id

    printf "\t\e[1mProgram exit code:\e[0m %s\n" "$2"
    printf "\t\e[1mProgram output:\e[0m\n"
    cat $DIR/program-output | awk '{printf "\t\t%s\n", $0}'
    printf "\t\e[1mReference output:\e[0m\n"
    cat $DIR/reference-output | awk '{printf "\t\t%s\n", $0}'
  else
    printf "\e[92m[%.2d] Successful\e[39m\n" $test_id
  fi

  test_id=$((test_id + 1))
}

# Set source for dynamic libraries
export LD_LIBRARY_PATH=./src

# Basic run - static
cat $DIR/words | ./test/wordcount-cc | sort >$DIR/reference-output
cat $DIR/words | ./src/wordcount | sort >$DIR/program-output ; exit_code=$?
verify_output "Basic run (static)" $exit_code

# Basic run - dynamic
cat $DIR/words | ./test/wordcount-cc | sort >$DIR/reference-output
cat $DIR/words | ./src/wordcount-dynamic | sort >$DIR/program-output ; exit_code=$?
verify_output "Basic run (dynamic)" $exit_code

# /usr/share/dict/words - static
cat /usr/share/dict/words | ./test/wordcount-cc | sort >$DIR/reference-output
cat /usr/share/dict/words | ./src/wordcount | sort >$DIR/program-output ; exit_code=$?
verify_output "/usr/share/dict/words (static)" $exit_code

# /usr/share/dict/words - dynamic
cat /usr/share/dict/words | ./test/wordcount-cc | sort >$DIR/reference-output
cat /usr/share/dict/words | ./src/wordcount-dynamic | sort >$DIR/program-output ; exit_code=$?
verify_output "/usr/share/dict/words (dynamic)" $exit_code

# Generated numbers - static
seq 1000000 2000000 | shuf | tail -n 10000 > $DIR/input
cat $DIR/input | ./test/wordcount-cc | sort >$DIR/reference-output
cat $DIR/input | ./src/wordcount | sort >$DIR/program-output ; exit_code=$?
verify_output "/usr/share/dict/words (static)" $exit_code

# Generated numbers - dynamic
seq 1000000 2000000 | shuf | tail -n 10000 > $DIR/input
cat $DIR/input | ./test/wordcount-cc | sort >$DIR/reference-output
cat $DIR/input | ./src/wordcount-dynamic | sort >$DIR/program-output ; exit_code=$?
verify_output "/usr/share/dict/words (dynamic)" $exit_code

# Exit code by results
if [ $error == true ]; then
  exit 1
else
  exit 0
fi
