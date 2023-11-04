adb push ./ceserver_arm64 /storage/emulated/0
adb push ./libceserver-extension_arm64.so /storage/emulated/0

adb shell "su -c mkdir /data/test"
adb shell "su -c mv /storage/emulated/0/ceserver_arm64 /data/test/ceserver_arm64"
adb shell "su -c mv /storage/emulated/0/libceserver-extension_arm64.so /data/test/libceserver-extension_arm64.so"
adb shell "su -c chmod 755 /data/test/ceserver_arm64"
adb shell "su -c chmod 755 /data/test/libceserver-extension_arm64.so"
pause