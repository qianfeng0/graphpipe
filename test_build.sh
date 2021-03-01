#!/bin/bash

function compile() {

set -e
if [ ! -d "build" ];then
    mkdir build
else
    echo "./build exist"
fi

cd build
cmake ../ \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_C_COMPILER=/usr/local/opt/llvm/bin/clang \
    -DCMAKE_CXX_COMPILER=/usr/local/opt/llvm/bin/clang++ \
    -DCMAKE_CXX_COMPILER_AR=/usr/bin/ar \
    -DCMAKE_CXX_COMPILER_RANLIB=/usr/bin/ranlib \
    -DCMAKE_RANLIB=/usr/bin/ranlib \
    -DCMAKE_AR=/usr/bin/ar \
    

# cmake ../../

make -j8

# cmake ../../ \
#     -DCMAKE_TOOLCHAIN_FILE=$CMAKE_TOOLCHAIN_FILE \
#     -DANDROID_ABI=${SINGLE_ABI} \
#     -DANDROID_NATIVE_API_LEVEL=$MINSDKVERSION \
#     -DANDROID_NDK=${ANDROID_NDK_HOME} \
#     -DANDROID_TOOLCHAIN=gcc \
#     -DCMAKE_BUILD_TYPE=Release \
#     -DANDROID_STL=c++_static

}

function clean() {
    rm -rf build
}

function run() {
    cd ./build/test
    ASAN_OPTIONS=detect_leaks=1:detect_stack_use_after_return=1 ./Qctest
    # ./dna2_test


}

set -e

if [[ -z "$1" ]]
then
    compile
else
    case "$1" in
        clean) 
            clean
            shift
            ;;
        compile)
            compile
            ;;
        run)
            run
            ;;
        *) 
            __error "${CBOLD}Invalid Option!${CNORM}\n"
            exit 1
            ;;
    esac
fi