adb shell am force-stop com.wzlr.sea

adb push ./smali/assets/main/index.b42dd.js /storage/emulated/0/index.b42dd.js
adb shell "su -c cp /storage/emulated/0/index.b42dd.js /data/data/com.wzlr.sea/files/hotUpdateDir/assets/main/index.b42dd.js"
adb shell "su -c chmod 755 /data/data/com.wzlr.sea/files/hotUpdateDir/assets/main/index.b42dd.js"

adb shell monkey -p com.wzlr.sea -c android.intent.category.LAUNCHER 1