echo f | xcopy /f /Y ..\PigMod\app\build\intermediates\stripped_native_libs\release\out\lib\arm64-v8a\libpigmod.so libpigmod.so
echo f | xcopy /f /Y ..\PigMod\app\build\intermediates\dex\release\mergeDexRelease\classes.dex classes3.dex

pause