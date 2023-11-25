zipalign -p -f -v 4 build-native.apk build.apk

apksigner sign --v1-signing-enabled true --v2-signing-enabled -ks ..\my-release-key-v2.keystore --ks-pass pass:adadad  -v build.apk

pause