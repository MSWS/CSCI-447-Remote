#!/bin/bash

failed=0

test() {
  echo "Running suite for $1..."
  for file in $(ls $1); do
    f=$(basename $file)
    dir=$(basename $1)
    if [ $f == $dir.txt ]; then
      continue
    fi
    test_unit $1 $f
  done
}

test_unit() {
  echo -n "Running unit $1/$2... "
  A=${2:0:1}
  if [ $A == "X" ]; then
    A=0
  fi
  B=${2:1:1}
  P=${2:2:1}
  base=$(basename $1)
  ./Simulation $1/$base.txt $A $B $P > unit.txt
  diff unit.txt $1/$2
  if [ $? -eq 0 ]; then
    echo "Passed"
  else
    echo "Failed"
    failed=1
  fi
}

if [ $# -eq 0 ]; then
  for dir in $(ls tests); do
    if [ -d tests/$dir ]; then
      test tests/$dir
    fi
  done
  exit $failed
fi

test $1
exit $failed
