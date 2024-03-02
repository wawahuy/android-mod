adb shell am force-stop com.aladinfun.clashofsky_th_pig

echo f | xcopy /f /Y ..\PigMod\app\build\intermediates\stripped_native_libs\release\out\lib\arm64-v8a\libpigmod.so libpigmod.so
echo f | xcopy /f /Y ..\PigMod\app\build\intermediates\dex\release\mergeDexRelease\classes.dex classes3.dex

adb push ..\PigMod\app\build\intermediates\stripped_native_libs\release\out\lib\arm64-v8a\libpigmod.so /data/app/oke/lib/arm64/libpigmod.so
adb shell "mv /data/app/oke/lib/arm64/libpigmod.so /data/app/com.aladinfun.clashofsky_th_pig-*/lib/arm64/libpigmod.so"
adb shell "su -c chmod 755 /data/app/com.aladinfun.clashofsky_th_pig-*/lib/arm64/libpigmod.so"

adb shell monkey -p com.aladinfun.clashofsky_th_pig -c android.intent.category.LAUNCHER 1