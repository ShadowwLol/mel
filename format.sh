#!/bin/bash

set -e

for hfile in $(find ./mel | grep "\.h$"); do
  indent $hfile -nut -as -br -brs -brf -sai -saf -sar -saw -cdw -ce -cli2 -nss -ts2 -pal -npcs -npsl -o $hfile
done

for cfile in $(find ./mel | grep "\.c$"); do
  indent $cfile -nut -as -br -brs -brf -sai -saf -sar -saw -cdw -ce -cli2 -nss -ts2 -pal -npcs -npsl -o $cfile
done
