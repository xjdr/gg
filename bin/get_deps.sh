#!/usr/bin/env bash
set -ex

sudo apt-get install -y wget software-properties-common
sudo bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)"

sudo apt-get update
sudo apt-get upgrade -y

sudo apt-get install -y git-core cmake libjemalloc-dev 
# LLVM
sudo apt-get install -y libllvm-12-ocaml-dev libllvm12 llvm-12 llvm-12-dev llvm-12-doc llvm-12-examples llvm-12-runtime
# Clang and co
sudo apt-get install -y clang-12 clang-tools-12 clang-12-doc libclang-common-12-dev libclang-12-dev libclang1-12 clang-format-12 clangd-12
# libfuzzer
sudo apt-get install -y libfuzzer-12-dev
# lldb
sudo apt-get install -y lldb-12
# lld (linker)
sudo apt-get install -y lld-12
# libc++
sudo apt-get install -y libc++-12-dev libc++abi-12-dev
# OpenMP
sudo apt-get install -y libomp-12-dev

if [[ ! -d $PWD/third_party ]] ; then
  mkdir third_party
fi

pushd third_party || exit

[[ ! -d "abseil-cpp" ]] && git clone https://github.com/abseil/abseil-cpp
[[ ! -d "googletest" ]] && git clone https://github.com/google/googletest
[[ ! -d "glog" ]] && git clone https://github.com/google/glog
[[ ! -d "gflags" ]] && git clone https://github.com/gflags/gflags

popd || exit


