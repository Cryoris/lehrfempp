#!/bin/bash

# This script is a utility script for linux/osx users to run clang-tidy over all relevant files
# The continuous integration build environment (travis) runs exactly the same command.
#
# HOW TO USE:
# - change into your CMake build directory and call this script from there.
# - If you want to run it in parallel, append e.g. `-j4` as an argument.


# Check that compile_commands.json exists.
if [[ ! -e ./compile_commands.json ]]; then
  echo "compile_commands.json not found in current directory"
  echo "run 'cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=On . ' to create it."
fi

# find clang-tidy
if [ -x "$(command -v clang-tidy)" ]; then
  ct=$(command -v clang-tidy)
fi
if [ -x "$(command -v clang-tidy-5.0)" ]; then
  ct=$(command -v clang-tidy-5.0)
fi
if [ -x "$(command -v clang-tidy-6.0)" ]; then
  ct=$(command -v clang-tidy-6.0)
fi

if [ -z "$ct" ]; then
  echo "clang-tidy, clang-tidy-6.0 or clang-tidy-5.0 not found in path"
fi

$(dirname $0)/travis/run-clang-tidy.py -p . -header-filter=lib/ '^((?!snippets).)*(?<!_tests\.cc)$' $1