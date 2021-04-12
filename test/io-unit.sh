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

# Basic run
printf "first-word\n10\nY\n" >$DIR/reference-output
printf "first-word second-word ..." | ./src/io-test >$DIR/program-output ; exit_code=$?
verify_output "'first-word second-word ...' --> read_line()" $exit_code

# New line
printf "foo\n3\nY\n" >$DIR/reference-output
printf "foo\n ..." | ./src/io-test >$DIR/program-output ; exit_code=$?
verify_output "'foo\n ...' --> read_line()" $exit_code

# Tabulator
printf "bar\n3\nY\n" >$DIR/reference-output
printf "bar\t ..." | ./src/io-test >$DIR/program-output ; exit_code=$?
verify_output "'bar\t ...' --> read_line()" $exit_code

# Tabulator
printf "baz\n3\nY\n" >$DIR/reference-output
printf "baz   ..." | ./src/io-test >$DIR/program-output ; exit_code=$?
verify_output "'baz   ...' --> read_line()" $exit_code

# Spaces around
printf "baz\n3\nY\n" >$DIR/reference-output
printf "   baz   ..." | ./src/io-test >$DIR/program-output ; exit_code=$?
verify_output "'   baz   ...' --> read_line()" $exit_code

# Tabulators around
printf "baz\n3\nY\n" >$DIR/reference-output
printf "\tbaz\t ..." | ./src/io-test >$DIR/program-output ; exit_code=$?
verify_output "'\tbaz\t ...' --> read_line()" $exit_code

# New lines around
printf "baz\n3\nY\n" >$DIR/reference-output
printf "\nbaz\n ..." | ./src/io-test >$DIR/program-output ; exit_code=$?
verify_output "'\nbaz\n ...' --> read_line()" $exit_code

# Bigger word
printf "this-is-super-long-word-for-testing-read_line-function-in-io-module-used-for-unit-tests-of-this-module-this-is-still-valid-part\n127\nY\n" >$DIR/reference-output
printf "this-is-super-long-word-for-testing-read_line-function-in-io-module-used-for-unit-tests-of-this-module-this-is-still-valid-part-but-this-is-not ..." | ./src/io-test >$DIR/program-output ; exit_code=$?
verify_output "'this-is-super-long-word-for-testing-read_line-function-in-io-module-used-for-unit-tests-of-this-module-this-is-still-valid-part-but-this-is-not ...' --> read_line()" $exit_code

# Empty input
echo "EOF" >$DIR/reference-output
printf "" | ./src/io-test >$DIR/program-output ; exit_code=$?
verify_output "'' --> read_line()" $exit_code

# End of the file
echo "EOF" >$DIR/reference-output
printf "   small-word-at-the-end" | ./src/io-test >$DIR/program-output ; exit_code=$?
verify_output "'   small-word-at-the-end' --> read_line()" $exit_code

# Exit code by results
if [ $error == true ]; then
  exit 1
else
  exit 0
fi
