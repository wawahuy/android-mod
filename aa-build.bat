

java -jar D:\apktool_2.9.0.jar b D:\apk\heo\smali -o build.apk

jarsigner -storepass "adadad" -verbose -sigalg SHA1withRSA -digestalg SHA1 -keystore D:\keys\my-release-key.keystore D:\apk\heo\build.apk alias_name

pause