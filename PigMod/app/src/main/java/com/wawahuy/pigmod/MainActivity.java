package com.wawahuy.pigmod;

import static android.content.ClipDescription.MIMETYPE_TEXT_PLAIN;

import android.app.Activity;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.graphics.PixelFormat;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.Gravity;
import android.view.InputEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.Toast;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLClassLoader;
import java.security.NoSuchAlgorithmException;
import java.util.Objects;

public class MainActivity extends Activity {
    static final String endpoint = "http://192.168.1.21:3000";
    static final String libraryHashUrl = endpoint + "/libpigmod/hash";
    static final String libraryUrl = endpoint + "/libpigmod/down";
    static final String libraryName = "pigmod";
    static final String packageGame = "com.aladinfun.clashofsky_th_pig";

    public static Context contextCurrent;

    public static GLViewWrapper glViewWrapper;

    private static final int REQUEST_CODE = 1;

    boolean canExit = false;

    public Toast toastCurrent;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (contextCurrent == null) {
            dynamicLoadLibrary();
        }
    }

    public void dynamicLoadLibrary() {
        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                try {
                    final String keyHash = "pigmodHash";
                    SharedPreferences settings = getApplicationContext().getSharedPreferences("PIGMOD", 0);
                    String md5Old = settings.getString(keyHash, "");
                    String md5Remote = readURL(libraryHashUrl);
                    Log.e("YUH", "library hash old: " + md5Old);
                    Log.e("YUH", "library hash new: " + md5Remote);

                    String folderData = "/data/data/" + packageGame + "/";
                    String fileName = "lib" + libraryName + ".so";
                    if (!md5Old.equals(md5Remote)) {
                        // download library
                        postToast("Download menu...", Toast.LENGTH_LONG);
                        downloadLibrary(libraryUrl, folderData, fileName);

                        // save hash
                        SharedPreferences.Editor editor = settings.edit();
                        editor.putString(keyHash, md5Remote);
                        editor.apply();
                    }

                    // load library
                    loadLibrary(folderData + fileName, libraryName);

                    // run app
                    Handler handler = new Handler(Looper.getMainLooper());
                    handler.post(() -> {
                        MainActivity.startMain(MainActivity.this);
                    });
                } catch (IOException e) {
                    Log.e("YUH", "run: ", e);
                    postToast("Error Server", Toast.LENGTH_LONG);
                    try {
                        Thread.sleep(3000);
                    } catch (InterruptedException ex) {
                        throw new RuntimeException(ex);
                    }
                    System.exit(0);
                }
            }
        };
        new Thread(runnable).start();
    }

    @Override
    public void onBackPressed() {
        if (!canExit) {
            canExit = true;
            Toast.makeText(this, "Press exit", Toast.LENGTH_LONG).show();
            return;
        }
        super.onBackPressed();
        System.exit(0);
    }

    public void postToast(String msg, int dur) {
        Handler handler = new Handler(Looper.getMainLooper());
        handler.post(() -> {
            if (toastCurrent != null) {
                toastCurrent.cancel();
            }
            toastCurrent = Toast.makeText(MainActivity.this, msg, dur);
            toastCurrent.show();
        });
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == REQUEST_CODE) {
            if (resultCode == RESULT_OK || resultCode == RESULT_CANCELED) {
                finish();
                System.exit(0);
            }
        }
    }

    public static void startGame(String packageName, String className) {
        Activity activity = (Activity)contextCurrent;
        WindowManager windowManager = activity.getWindowManager();
        windowManager.removeView(glViewWrapper);
        Intent intent = new Intent();
        if (Objects.equals(packageName, "com.wawahuy.pigmod")) {
            intent.setType("image/*");
            intent.setAction(Intent.ACTION_GET_CONTENT);
        } else {
            intent.setClassName(packageName, className);
        }
        intent.addFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
        activity.startActivityForResult(intent, REQUEST_CODE);
        activity.overridePendingTransition(0, 0);
    }

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

    public static void startMain(Context context) {
        contextCurrent = context;
        getClipboard();

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

        glViewWrapper = new GLViewWrapper(context);
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

    public static void downloadLibrary(String libraryUrl, String localLibraryPath, String libraryName) throws IOException {
        URL url = new URL(libraryUrl);
        try (InputStream in = new BufferedInputStream(url.openStream());
             FileOutputStream fileOutputStream = new FileOutputStream(localLibraryPath + libraryName)) {

            byte[] dataBuffer = new byte[1024];
            int bytesRead;

            while ((bytesRead = in.read(dataBuffer, 0, 1024)) != -1) {
                fileOutputStream.write(dataBuffer, 0, bytesRead);
            }
        }
    }

    public static void loadLibrary(String localLibraryPath, String libraryName) throws IOException {
        System.load(localLibraryPath);
    }


    public static String readURL(String urlString) throws IOException {
        URL url = new URL(urlString);
        HttpURLConnection connection = (HttpURLConnection) url.openConnection();
        try {
            connection.setRequestMethod("GET");
            int responseCode = connection.getResponseCode();
            if (responseCode == HttpURLConnection.HTTP_OK) {
                try (BufferedReader reader = new BufferedReader(new InputStreamReader(connection.getInputStream()))) {
                    StringBuilder content = new StringBuilder();
                    String line;
                    while ((line = reader.readLine()) != null) {
                        content.append(line).append("\n");
                    }
                    return content.substring(0, content.length() - 1).toString();
                }
            } else {
                throw new IOException("Failed to retrieve content from URL. Response Code: " + responseCode);
            }
        } finally {
            connection.disconnect();
        }
    }
}