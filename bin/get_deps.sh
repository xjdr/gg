#!/usr/bin/env bash
set -ex

if [[ ! -d $PWD/third_party ]] ; then
  mkdir third_party
fi

pushd third_party || exit

[[ ! -d "abseil-cpp" ]] && git clone https://github.com/abseil/abseil-cpp
[[ ! -d "googletest" ]] && git clone https://github.com/google/googletest
[[ ! -d "glog" ]] && git clone https://github.com/google/glog
[[ ! -d "gflags" ]] && git clone https://github.com/gflags/gflags

popd || exit


