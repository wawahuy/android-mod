#!/bin/bash

export ANDROID_HOME=/home/yuh/Android/Sdk
export CMAKE_VERSION="3.22.1"
export ANDROID_PLATFORM=android-16
export ANDROID_ABI=arm64-v8a

CMAKE="/home/yuh/Android/Sdk/cmake/$CMAKE_VERSION/bin/cmake"
eval "$CMAKE -H. -B .build -G \"Unix Makefiles\" -DANDROID_PLATFORM=$ANDROID_PLATFORM -D ANDROID_ABI=$ANDROID_ABI -DCMAKE_BUILD_TYPE=Release"

CMAKE_MAKE_PROGRAM="$ANDROID_HOME/ndk-bundle/prebuilt/linux-x86_64/bin/make"
cd .build

eval "$CMAKE_MAKE_PROGRAM"

cp libpigmodij.so libpigmodij-striped.so
STRIP="$ANDROID_HOME/ndk-bundle/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-strip"
eval "$STRIP libpigmodij-striped.so --strip-debug"

cd ..