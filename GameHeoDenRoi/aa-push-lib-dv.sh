#!/bin/bash

adb shell am force-stop com.aladinfun.clashofsky_th_pig

cp ../PigMod/app/build/intermediates/stripped_native_libs/release/out/lib/arm64-v8a/libpigmod.so libpigmod.so
cp ../PigMod/app/build/intermediates/dex/release/mergeDexRelease/classes.dex classes3.dex

adb push ../PigMod/app/build/intermediates/stripped_native_libs/release/out/lib/arm64-v8a/libpigmod.so /storage/emulated/0/libpigmod.so
adb shell "su -c cp /storage/emulated/0/libpigmod.so /data/app/com.aladinfun.clashofsky_th_pig-*/lib/arm64/libpigmod.so"
adb shell "su -c chmod 755 /data/app/com.aladinfun.clashofsky_th_pig-*/lib/arm64/libpigmod.so"

cp ./libpigmodij/.build/libpigmodij-striped.so libpigmodij.so

cp libpigmodij.so ../PigServer/data/libso/com.aladinfun.clashofsky_th_pig
cp libpigmod.so ../PigServer/data/libso/libpigmod.so

adb shell monkey -p com.aladinfun.clashofsky_th_pig -c android.intent.category.LAUNCHER 1