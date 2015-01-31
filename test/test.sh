#!/bin/bash

################################################################
# Test framework.

function error_exit() {
  echo -n -e "\033[1;31m[ERROR]\033[0;39m "
  echo "$1"
  exit 1
}

function run() {
  echo -n "Testing $1 ... "
  result=$(./$1)
  code=$?
  if [ $code -ne 0 ]; then
    error_exit "exit status is not 0 [$code]"
  fi
  if [ "$result" != "$2" ]; then
    error_exit "$2 expected, but got '$result'"
  fi
  echo ok
}

function fail() {
  echo -n "Testing $1 ... "
  result=$(./$1)
  code=$?
  if [ $code -eq 0 ]; then
    error_exit "Failure expected, but succeeded!"
  fi
  if [ "$result" != "$2" ]; then
    error_exit "$2 expected, but got '$result'"
  fi
  echo ok
}

################################################################
# Test cases.

run void 'dummy called'
run int '1234321'
run float '408.0'
run string '* Hello, mruby! *'
run cptr 'cptr test'
run class 'Foo::ctor(123)
690
99980001
Foo::dtor()'
run module 'modfunc called: 1234
Mod::ModClass::CNST 12345
c.get_a 4'
run class_value 'start ClassPointerTest
ClassValue construct.
cv -> 7
cv -> 8
cv -> 7
cv.call_block -> test7
wk->7
GC
ClassValue destruct.
wk->0'
run many_bind 'arena_index = 2
start ClassPointerTest
ClassValue construct.
cv -> 7
cv -> 8
cv -> 7
cv.call_block -> test7
wk->7
GC
ClassValue destruct.
wk->0'
run mruby_ref 'mruby_ref = 3test
mruby_ref = 3
mruby_ref = 3toast
:a == :a = 1
arena_index = 0'
run reference 'start ReferenceTest
v_a = 1
v_a = 2'
run avoid_gc 'start avoid_gc
called!
start avoid_gc 2
called!
block called!'


# Failure cases
fail wrong_type "TypeError: can't convert String into Fixnum, argument 1(1111)"
fail wrong_arg_num "ArgumentError: 'square': wrong number of arguments (2 for 1)"

################################################################
# All tests succeeded.

echo -n -e "\033[1;32mTEST ALL SUCCEEDED!\033[0;39m\n"
