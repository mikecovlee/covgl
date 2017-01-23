#!/bin/sh
clear
echo "using $1 compile file $2...."
$1 -std=c++11 -lpthread -o $HOME/a.out $2 2>compiler_output.log
cat compiler_output.log