zipalign -p -f -v 4 build-native.apk build.apk

jarsigner -storepass "adadad" -verbose -sigalg SHA1withRSA -digestalg SHA1 -keystore ..\my-release-key.keystore build.apk alias_name

pause