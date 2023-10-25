
java -jar ..\apktool_2.9.0.jar b .\smali -o build.apk

jarsigner -storepass "adadad" -verbose -sigalg SHA1withRSA -digestalg SHA1 -keystore ..\my-release-key.keystore .\build.apk alias_name

pause