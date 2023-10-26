package com.wawahuy.hotreload;

import android.app.Activity;
import android.content.Context;
import android.graphics.PixelFormat;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.view.Gravity;
import android.view.InputEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;

public class MainActivity extends Activity {
    static {
        System.loadLibrary("hotreload");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        startMain(this);
    }

    public static void startMain(Context context) {
        Handler handler = new Handler(Looper.getMainLooper());
        handler.postDelayed(() -> startMenu(context), 500);
    }

    public static void startMenu(Context context) {
        WindowManager windowManager = ((Activity)context).getWindowManager();
        WindowManager.LayoutParams layoutParams = new WindowManager.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT, 0, 0,
                WindowManager.LayoutParams.TYPE_APPLICATION,
                WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE | WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE |
                        WindowManager.LayoutParams.FLAG_FULLSCREEN,
                PixelFormat.TRANSPARENT);
        layoutParams.gravity = Gravity.TOP | Gravity.CENTER_HORIZONTAL | Gravity.CENTER_VERTICAL;

        GLViewWrapper glViewWrapper = new GLViewWrapper(context);
        glViewWrapper.setSystemUiVisibility(
            View.SYSTEM_UI_FLAG_IMMERSIVE
            | View.SYSTEM_UI_FLAG_LAYOUT_STABLE
            | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
            | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
        );
        windowManager.addView(glViewWrapper, layoutParams);
    }

    public static boolean hookEvent(InputEvent event) {
        if (event instanceof MotionEvent) {
            try {
                return NativeMethods.hookEvent(((MotionEvent) event).getX(), ((MotionEvent) event).getY(), ((MotionEvent) event).getAction());
            } catch (Exception ex) {
                return true;
            }
        }
        return true;
    }
}