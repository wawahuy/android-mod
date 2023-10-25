package com.wawahuy.pigmod;

public class NativeMethods {
    public static native void onSurfaceCreated();
    public static native void onSurfaceChanged(int width, int height);
    public static native void onDrawFrame();
    public static native boolean hookEvent(float x, float y, int action);
}
