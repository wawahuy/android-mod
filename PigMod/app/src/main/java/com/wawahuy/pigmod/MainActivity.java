package com.wawahuy.pigmod;

import static android.content.ClipDescription.MIMETYPE_TEXT_PLAIN;

import android.app.Activity;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
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
        System.loadLibrary("pigmod");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        startMain(this);
    }

    public static Context contextCurrent;

    public static String getClipboard() {
        String dataClipboard = "";
        ClipboardManager clipboard = (ClipboardManager)contextCurrent.getSystemService(Context.CLIPBOARD_SERVICE);
        if (clipboard.hasPrimaryClip()) {
            ClipData clip = clipboard.getPrimaryClip();
            if (clip.getDescription().hasMimeType(MIMETYPE_TEXT_PLAIN))
                dataClipboard = clip.getItemAt(0).getText().toString();
            dataClipboard = clip.getItemAt(0).coerceToText(contextCurrent).toString();
        }
        return dataClipboard;
    }

    public static String getPackageVersion() {
        String version = "";
        try {
            PackageInfo pInfo = contextCurrent.getPackageManager().getPackageInfo(contextCurrent.getPackageName(), 0);
            version = pInfo.versionName;
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
        return version;
    }

    public static boolean getSaveContains(String name) {
        SharedPreferences settings = contextCurrent.getApplicationContext().getSharedPreferences("pigmod", 0);
        return settings.contains(name);
    }

    public static boolean getSaveBool(String name) {
        SharedPreferences settings = contextCurrent.getApplicationContext().getSharedPreferences("pigmod", 0);
        return settings.getBoolean(name, false);
    }

    public static String getSaveString(String name) {
        SharedPreferences settings = contextCurrent.getApplicationContext().getSharedPreferences("pigmod", 0);
        return settings.getString(name, "");
    }

    public static void setSaveString(String name, String value) {
        SharedPreferences settings = contextCurrent.getApplicationContext().getSharedPreferences("pigmod", 0);
        SharedPreferences.Editor editor = settings.edit();
        editor.putString(name, value);
        editor.apply();
    }

    public static void setSaveBool(String name, boolean value) {
        SharedPreferences settings = contextCurrent.getApplicationContext().getSharedPreferences("pigmod", 0);
        SharedPreferences.Editor editor = settings.edit();
        editor.putBoolean(name, value);
        editor.apply();
    }

    public static void startMain(Context context) {
        contextCurrent = context;
        NativeMethods.initEnv();
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
            return NativeMethods.hookEvent(((MotionEvent) event).getX(), ((MotionEvent) event).getY(), ((MotionEvent) event).getAction());
        }
        return true;
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        return hookEvent(event);
    }
}