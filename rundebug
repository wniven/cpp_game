#!/bin/bash
#
# Runs the given program and pipes its STDERR messages to the "err" file.
# Opens a new terminal and listens to the end of the "err" output file.
#

# Make sure a program to run was given.
if [ -z "$1" ]
then
    echo "./rundebug <program>"
    exit
fi

# Setup debug terminal
rm err
touch err
mate-terminal -- bash -c 'echo "Listening for debugging...";tail -f err'

# Run it.
eval ./$1 2>> err
