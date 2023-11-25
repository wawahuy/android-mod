adb shell am force-stop com.g4m.daitaydu

echo f | xcopy /f /Y ..\PigMod\app\build\intermediates\stripped_native_libs\release\out\lib\arm64-v8a\libpigmod.so libpigmod.so
echo f | xcopy /f /Y ..\PigMod\app\build\intermediates\dex\release\mergeDexRelease\classes.dex classes3.dex

adb push ..\PigMod\app\build\intermediates\stripped_native_libs\release\out\lib\arm64-v8a\libpigmod.so /data/app/oke/lib/arm64/libpigmod.so
adb shell "mv /data/app/oke/lib/arm64/libpigmod.so /data/app/com.g4m.daitaydu-*/lib/arm64/libpigmod.so"
adb shell "su -c chmod 755 /data/app/com.g4m.daitaydu-*/lib/arm64/libpigmod.so"

adb shell monkey -p com.g4m.daitaydu -c android.intent.category.LAUNCHER 1