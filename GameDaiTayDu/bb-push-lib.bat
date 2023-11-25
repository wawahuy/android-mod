adb shell am force-stop com.g4m.daitaydu

adb push ..\HotReload\app\build\intermediates\stripped_native_libs\release\out\lib\arm64-v8a\libhotreload.so /data/app/oke/lib/arm64/libhotreload.so
adb shell "mv /data/app/oke/lib/arm64/libhotreload.so /data/app/com.g4m.daitaydu-*/lib/arm64/libhotreload.so"

adb shell monkey -p com.g4m.daitaydu -c android.intent.category.LAUNCHER 1