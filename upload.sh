#!/usr/bin/bash

# Get absolute path to directory of script
readonly BASE_DIR="$( readlink -f "$(dirname "$0")" )"

arduino-cli upload -b arduino:megaavr:uno2018 -p /dev/ttyACM0 "$BASE_DIR"/arduino -v
