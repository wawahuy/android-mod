adb root
adb shell mkdir /data/test
adb push ./ceserver_arm64 /data/test
adb push ./libceserver-extension_arm64.so /data/test
adb shell "chmod 755 /data/test/ceserver_arm64"
adb shell "chmod 755 /data/test/libceserver-extension_arm64.so"
pause