java -jar ..\apktool_2.9.0.jar b .\smali -o build-native.apk

echo f | xcopy /f /Y ..\PigMod\app\build\intermediates\stripped_native_libs\release\out\lib\arm64-v8a\libpigmod.so libpigmod.so
echo f | xcopy /f /Y ..\PigMod\app\build\intermediates\dex\release\mergeDexRelease\classes.dex classes3.dex

node ../ZipCommand/index.js build-native.apk libpigmod.so lib/arm64-v8a/libpigmod.so
node ../ZipCommand/index.js build-native.apk classes3.dex classes3.dex

pause