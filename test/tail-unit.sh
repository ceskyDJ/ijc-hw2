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

# Verify if program exited with the error (for bad input tests)
# Params:
# - command Tested command
# - exit_code Program exit code
function verify_error() {
  if [ "$2" != 1 ]; then
    error=true
    echo -e "\e[91m[$test_id] Failed: $1\e[39m"

    printf "\t\e[1mProgram exit code:\e[0m %s\n" "$2"
    printf "\t\e[1mProgram output:\e[0m\n"
    cat $DIR/program-output | awk '{printf "\t\t%s\n", $0}'
    printf "\t\e[1mReference output:\e[0m\n"
    cat $DIR/reference-output | awk '{printf "\t\t%s\n", $0}'
  else
    echo -e "\e[92m[$test_id] Successful\e[39m"
  fi

  test_id=$((test_id + 1))
}

# Basic run
tail $DIR/test-file >$DIR/reference-output
./src/tail $DIR/test-file >$DIR/program-output ; exit_code=$?
verify_output "tail test-file" $exit_code

# Input from stdin
cat $DIR/test-file | tail >$DIR/program-output
cat $DIR/test-file | ./src/tail >$DIR/program-output ; exit_code=$?
verify_output "cat test-file | tail" $exit_code

# Explicit offset from the end of the file 1
tail -n 5 $DIR/test-file >$DIR/reference-output
./src/tail -n 5 $DIR/test-file >$DIR/program-output ; exit_code=$?
verify_output "tail -n 5 test-file" $exit_code

# Explicit offset from the end of the file 2
cat $DIR/test-file | tail -n 15 >$DIR/reference-output
cat $DIR/test-file | ./src/tail -n 15 >$DIR/program-output ; exit_code=$?
verify_output "cat test-file | tail -n 15" $exit_code

# Explicit offset from the beginning of the file 1
tail -n +20 $DIR/test-file >$DIR/reference-output
./src/tail -n +20 $DIR/test-file >$DIR/program-output ; exit_code=$?
verify_output "tail -n +20 test-file" $exit_code

# Explicit offset from the beginning of the file 2
cat $DIR/test-file | tail -n +8 >$DIR/reference-output
cat $DIR/test-file | ./src/tail -n +8 >$DIR/program-output ; exit_code=$?
verify_output "cat test-file | tail -n +8" $exit_code

# Empty file 1
tail $DIR/empty-file >$DIR/reference-output
./src/tail $DIR/empty-file >$DIR/program-output ; exit_code=$?
verify_output "tail empty-file" $exit_code

# Empty file 2
tail -n 3 $DIR/empty-file >$DIR/reference-output
./src/tail -n 3 $DIR/empty-file >$DIR/program-output ; exit_code=$?
verify_output "tail -n 3 empty-file" $exit_code

# Empty file 3
tail -n +11 $DIR/empty-file >$DIR/reference-output
./src/tail -n +11 $DIR/empty-file >$DIR/program-output ; exit_code=$?
verify_output "tail -n +11 empty-file" $exit_code

# Long lines file 1
tail -n +3 $DIR/long-lines-file | awk '{ printf "%.511s\n", $0 }' >$DIR/reference-output
./src/tail -n +3 $DIR/long-lines-file >$DIR/program-output ; exit_code=$?
verify_output "tail -n +3 long-lines-file" $exit_code

# Long lines file 2
tail -n 2 $DIR/long-lines-file | awk '{ printf "%.511s\n", $0 }' >$DIR/reference-output
./src/tail -n 2 $DIR/long-lines-file >$DIR/program-output ; exit_code=$?
verify_output "tail -n 2 long-lines-file" $exit_code

# Error message for long lines file
echo "Soubor obsahuje delší řádek než je maximum (511 znaků bez \n)" >$DIR/reference-output
./src/tail -n +9 $DIR/long-lines-file > /dev/null 2>$DIR/program-output ; exit_code=$?
verify_output "tail -n +9 long-lines-file" $exit_code

# Bad filename 1
echo "Argument bez přepínače musí být platná cesta k souboru
Pokud byla zadána platná cesta, soubor se nepodařilo otevřít" >$DIR/reference-output
./src/tail $DIR/bad-file > /dev/null 2>$DIR/program-output ; exit_code=$?
verify_error "tail bad-file" $exit_code

# Bad filename 2
echo "Argument bez přepínače musí být platná cesta k souboru
Pokud byla zadána platná cesta, soubor se nepodařilo otevřít" >$DIR/reference-output
./src/tail -n 1 $DIR/bad-file > /dev/null 2>$DIR/program-output ; exit_code=$?
verify_error "tail -n 1 bad-file" $exit_code

# Bad filename 3
echo "Argument bez přepínače musí být platná cesta k souboru
Pokud byla zadána platná cesta, soubor se nepodařilo otevřít" >$DIR/reference-output
./src/tail -n +4 $DIR/bad-file > /dev/null 2>$DIR/program-output ; exit_code=$?
verify_error "tail -n +4 bad-file" $exit_code

# Input arguments switches special variants 1
tail -n22 $DIR/test-file >$DIR/reference-output
./src/tail -n22 $DIR/test-file >$DIR/program-output ; exit_code=$?
verify_output "tail -n22 test-file" $exit_code

# Input arguments switches special variants 2
tail -n+13 $DIR/test-file >$DIR/reference-output
./src/tail -n+13 $DIR/test-file >$DIR/program-output ; exit_code=$?
verify_output "tail -n+13 test-file" $exit_code

# Exit code by results
if [ $error == true ]; then
  exit 1
else
  exit 0
fi
