#!/bin/bash

apktool b ./smali -o build-native.apk

mkdir -p ./lib/arm64-v8a
cp ../PigMod/app/build/intermediates/stripped_native_libs/release/out/lib/arm64-v8a/libpigmod.so lib/arm64-v8a/libpigmod.so
cp ../PigMod/app/build/intermediates/dex/release/mergeDexRelease/classes.dex classes3.dex

zip -ur build-native.apk ./classes3.dex
zip -ur build-native.apk ./lib/arm64-v8a/libpigmod.so