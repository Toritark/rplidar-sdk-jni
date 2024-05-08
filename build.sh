#!/bin/bash

set -ex

original_dir=$(pwd)

return_to_original_directory() {
    cd "$original_dir"
}

trap 'return_to_original_directory' ERR

script_dir=$(dirname "$(readlink -f "$0")")

release_dir="$script_dir/release"

mkdir -p "$release_dir"
cd "$release_dir"

cmake -DCMAKE_BUILD_TYPE=Release "$script_dir"
cmake --build . --target clean
cmake --build .

echo "Built in $release_dir"

debug_dir="$script_dir/debug"

mkdir -p "$debug_dir"
cd "$debug_dir"

cmake -DCMAKE_BUILD_TYPE=Debug "$script_dir"
cmake --build . --target clean
cmake --build .

cd "$original_dir"

echo "Built in $debug_dir"