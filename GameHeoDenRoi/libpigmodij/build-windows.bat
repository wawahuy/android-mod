set ANDROID_HOME=C:/Users/nguye/AppData/Local/Android/Sdk
set CMAKE_VERSION=3.22.1
set ANDROID_PLATFORM=android-16
set ANDROID_ABI=arm64-v8a

set CMAKE="C:/Users/nguye/AppData/Local/Android/Sdk/cmake/%CMAKE_VERSION%/bin/cmake.exe"
%CMAKE% -H. -B .build -G "Unix Makefiles" -DANDROID_PLATFORM=%ANDROID_PLATFORM% -D ANDROID_ABI=%ANDROID_ABI% -DCMAKE_BUILD_TYPE=Release

set CMAKE_MAKE_PROGRAM="%ANDROID_HOME%/ndk-bundle/prebuilt/windows-x86_64/bin/make.exe"
cd .build

%CMAKE_MAKE_PROGRAM%

echo f | xcopy /f /Y libpigmodij.so libpigmodij-striped.so
set STRIP=%ANDROID_HOME%\ndk-bundle\toolchains\llvm\prebuilt\windows-x86_64\bin\llvm-strip.exe
%STRIP% libpigmodij-striped.so --strip-debug 

cd ..