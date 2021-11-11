#!/usr/bin/bash

# Get absolute path to directory of script
readonly BASE_DIR="$( readlink -f "$(dirname "$0")" )"

arduino-cli compile "$BASE_DIR"/arduino -b arduino:megaavr:uno2018
