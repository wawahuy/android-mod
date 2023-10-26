package com.wawahuy.hotreload;

import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class GLViewWrapper extends GLSurfaceView implements GLSurfaceView.Renderer {

    public GLViewWrapper(Context context) {
        super(context);
        setEGLContextClientVersion(3);
        setEGLConfigChooser(8, 8, 8, 8, 16, 0);
        getHolder().setFormat(PixelFormat.TRANSPARENT);
        setZOrderOnTop(true);
        setRenderer(this);
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        try {
            NativeMethods.onSurfaceCreated();
        } catch (Exception ex) {
        }
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        try {
            NativeMethods.onSurfaceChanged(width, height);
        } catch (Exception ex) {
        }
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        try {
            NativeMethods.onDrawFrame();
        } catch (Exception ex) {
        }
    }
}
