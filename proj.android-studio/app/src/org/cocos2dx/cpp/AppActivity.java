/****************************************************************************
 Copyright (c) 2015 Chukong Technologies Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
package org.cocos2dx.cpp;

import android.Manifest;
import android.app.Activity;
import android.content.ComponentName;
import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.content.BroadcastReceiver;
import android.content.IntentFilter;
import android.content.pm.ActivityInfo;
import android.content.pm.LabeledIntent;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Parcelable;
import android.provider.ContactsContract;
import android.provider.MediaStore;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.ActivityCompat;
import android.util.Log;

import org.cocos2dx.lib.*;
import org.cocos2dx.lib.Cocos2dxActivity;

import com.google.android.gms.appindexing.Action;
import com.google.android.gms.appindexing.AppIndex;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.firebase.analytics.FirebaseAnalytics;

import com.soundcloud.android.crop.*;

import android.view.KeyEvent;
import android.widget.Toast;
import android.app.AlertDialog;
import android.content.DialogInterface;

import java.io.ByteArrayOutputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.URI;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Locale;
import java.io.File;
import java.util.Timer;
import java.util.TimerTask;
import java.util.logging.Logger;

public class AppActivity extends Cocos2dxActivity {

    protected static final int REQUEST_STORAGE_READ_ACCESS_PERMISSION = 101;
    protected static final int REQUEST_STORAGE_WRITE_ACCESS_PERMISSION = 102;

    protected static final int PickerTypeCardback = 0;
    protected static final int PickerTypeTheme = 1;

//    private android.support.v7.app.AlertDialog mAlertDialog;

    private static boolean isPortrait = false;
    /**
     * ATTENTION: This was auto-generated to implement the App Indexing API.
     * See https://g.co/AppIndexing/AndroidStudio for more information.
     */
    private GoogleApiClient client;
    public static Context con;
    public static AppActivity instance;

    public static int curPickerType;

    DBOpenHelper mHelper;

    BatteryReceiver batteryReceiver;

    @Override
    public void onStart() {
        super.onStart();

//        // ATTENTION: This was auto-generated to implement the App Indexing API.
//        // See https://g.co/AppIndexing/AndroidStudio for more information.
//        client.connect();
//        Action viewAction = Action.newAction(
//                Action.TYPE_VIEW, // TODO: choose an action type.
//                "App Page", // TODO: Define a title for the content shown.
//                // TODO: If you have web page content that matches this app activity's content,
//                // make sure this auto-generated web page URL is correct.
//                // Otherwise, set the URL to null.
//                Uri.parse("http://host/path"),
//                // TODO: Make sure this auto-generated app URL is correct.
//                Uri.parse("android-app://org.cocos2dx.cpp/http/host/path")
//        );
//        AppIndex.AppIndexApi.start(client, viewAction);
    }

    @Override
    public void onStop() {
        super.onStop();

//        // ATTENTION: This was auto-generated to implement the App Indexing API.
//        // See https://g.co/AppIndexing/AndroidStudio for more information.
//        Action viewAction = Action.newAction(
//                Action.TYPE_VIEW, // TODO: choose an action type.
//                "App Page", // TODO: Define a title for the content shown.
//                // TODO: If you have web page content that matches this app activity's content,
//                // make sure this auto-generated web page URL is correct.
//                // Otherwise, set the URL to null.
//                Uri.parse("http://host/path"),
//                // TODO: Make sure this auto-generated app URL is correct.
//                Uri.parse("android-app://org.cocos2dx.cpp/http/host/path")
//        );
//        AppIndex.AppIndexApi.end(client, viewAction);
//        client.disconnect();

//        if (mAlertDialog != null && mAlertDialog.isShowing()) {
//            mAlertDialog.dismiss();
//        }
    }

    /**
     * 广播接受者
     */
    class BatteryReceiver extends BroadcastReceiver {

//        private static  mCocos2dxActivity;

        @Override
        public void onReceive(Context context, Intent intent) {
            // TODO Auto-generated method stub
            //判断它是否是为电量变化的Broadcast Action
            if (Intent.ACTION_BATTERY_CHANGED.equals(intent.getAction())) {
                //获取当前电量
                final int level = intent.getIntExtra("level", 0);
                //电量的总刻度
                final int scale = intent.getIntExtra("scale", 100);
                //把它转成百分比
//                tv.setText("电池电量为"+((level*100)/scale)+"%");

                Cocos2dxActivity mCocos2dxActivity = (Cocos2dxActivity) Cocos2dxActivity.getContext();
                mCocos2dxActivity.runOnGLThread(new Runnable() {
                    @Override
                    public void run() {
                        AppActivity.__onBatteryChanged((float) level / (float) scale);
                    }
                });
            }
        }
    }

    public static void showToSetupThemeAppPage(String devName){
        Log.v("fuck", devName);
        try {
            instance.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("market://search?q=pub:"+devName)));
        } catch (android.content.ActivityNotFoundException anfe) {
            instance.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("http://play.google.com/store/search?q=pub:"+devName)));
        }
    }

    void checkThemeSetup(final boolean isStart){

        Log.v("fuck", "checkThemeSetup");
        Intent i = getIntent();
        Log.v("fuck", "i = " + i);
        if(null != i){
            Log.v("fuck", "intentName = " + i.getStringExtra("name"));
            if ("themeSetup".equals(i.getStringExtra("name"))){
                String themeName = i.getStringExtra("themeName");
                Log.v("fuck", "extra themeName = " + themeName);

                SQLiteDatabase database = mHelper.getWritableDatabase();
                Cursor cursor = database.query("theme", new String[]{"id", "name", "isNew"}, "name=?", new String[]{themeName}, null, null, null);
                if (cursor.moveToFirst()){

                    String isNew = cursor.getString(cursor.getColumnIndex("isNew"));
                    Log.v("fuck", "isNew = " + isNew);
                    if (isNew.equals("1")){

                        Log.v("fuck", "themeName = " + themeName);
                        query(themeName, isStart);

                        ContentValues values = new ContentValues();
                        values.put("name", themeName);
                        values.put("isNew", "0");
                        database.update("theme", values, "name=?", new String[]{themeName});
                    }
                }
                cursor.close();
            }
        }
    }

    Bitmap dataToBmp(byte[] data) {

        try {
            return BitmapFactory.decodeByteArray(data, 0, data.length);
        } catch (Exception e) {
            return null;
        }
    }

    public byte[] bmpToByteArray(Bitmap bmp) {
        // Default size is 32 bytes
        ByteArrayOutputStream bos = new ByteArrayOutputStream();
        try {
            bmp.compress(Bitmap.CompressFormat.PNG, 100, bos);
            bos.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return bos.toByteArray();
    }

    public static native  void onThemeSetup(String themeName);

    public static native void copyThemeFolder(String themeName);

    //name : theme_**** theme_galaxy
    void query(String name, final boolean isStart){

//        SQLiteDatabase database = mHelper.getWritableDatabase();
//        Cursor cursor = database.query("theme", new String[]{"id", "name", "bg"}, "name=?", new String[]{themeName}, null, null, null);
////        while (cursor.moveToNext()){
//            int id = cursor.getInt(cursor.getColumnIndex("id"));
//            final String name = cursor.getString(cursor.getColumnIndex("name"));
//            Log.i("fuck", "id = " + id + ", name = " + name);
//            byte[] blob = cursor.getBlob(cursor.getColumnIndex("bg"));
//
//            Bitmap bitmap = dataToBmp(blob);
//            KTUtils.saveBitmap(name+"_bg.jpg", bitmap,Bitmap.CompressFormat.JPEG);
//
//            AppActivity mCocos2dxActivity = (AppActivity) Cocos2dxActivity.getContext();
//            mCocos2dxActivity.runOnGLThread(new Runnable() {
//                @Override
//                public void run() {
//                    onThemeSetup(name);
//                }
//            });
//
//        }
//        cursor.close();

        final String themeName = name;

        AppActivity mCocos2dxActivity = (AppActivity) Cocos2dxActivity.getContext();
        mCocos2dxActivity.runOnGLThread(new Runnable() {
            @Override
            public void run() {
                copyThemeFolder(themeName);
            }
        });
        if (isStart){
            Log.v("fuck", "setString");
            Cocos2dxHelper.setStringForKey("KL_NeedSetupTheme", themeName);
        } else {
            Log.v("fuck", "onThemeSetup");
            mCocos2dxActivity.runOnGLThread(new Runnable() {
                @Override
                public void run() {
                    onThemeSetup(themeName);
                }
            });
        };

    }

    public static void showThemePicker(){

        curPickerType = PickerTypeTheme;
        {

            Intent intentFromGallery = new Intent();
            // 设置文件类型
            intentFromGallery.setType("image/*");
            intentFromGallery.setAction(Intent.ACTION_GET_CONTENT);
            intentFromGallery.addCategory(Intent.CATEGORY_OPENABLE);
            instance.startActivityForResult(intentFromGallery, 258);
        }

    }

    public static void showImagePicker(){
        AppActivity mCocos2dxActivity = (AppActivity) Cocos2dxActivity.getContext();
        curPickerType = PickerTypeCardback;
        {

            Intent intentFromGallery = new Intent();
            // 设置文件类型
            intentFromGallery.setType("image/*");
            intentFromGallery.setAction(Intent.ACTION_GET_CONTENT);
            intentFromGallery.addCategory(Intent.CATEGORY_OPENABLE);
            instance.startActivityForResult(intentFromGallery, 258);
        }
    }

    public static native void onBatteryChanged(float value);

    public static void __onBatteryChanged(float value) {
        onBatteryChanged(value);
    }


    //====分享 begin====
    public static Bitmap getImageFromAssetsFile(String fileName){

        Bitmap image = null;
        AssetManager am = con.getResources().getAssets();
        try {
            InputStream is = am.open(fileName);
            image = BitmapFactory.decodeStream(is);
            is.close();
        }catch (IOException e){
            e.printStackTrace();
        }
        return image;
    }

    public static void saveBitmap(Bitmap bm) {
        Log.e("shareUI", "保存图片");
        File file = new File("/sdcard/myFolder");
        if (!file.exists()){
            file.delete();
        }
        file.mkdir();
        file = new File("/sdcard/share_pic.jpg".trim());
        String fileName = file.getName();
        String mName = fileName.substring(0, fileName.lastIndexOf("."));
        String sName = fileName.substring(fileName.lastIndexOf("."));

        // /sdcard/myFolder/temp_cropped.jpg
        String newFilePath = "/sdcard/myFolder" + "/" + mName + "_cropped" + sName;
        file = new File(newFilePath);
        try {
            file.createNewFile();
            FileOutputStream fos = new FileOutputStream(file);
            bm.compress(Bitmap.CompressFormat.JPEG, 100, fos);
            fos.flush();
            fos.close();
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }

    public static void shareUI(String msgTitle, String msgText, String fbUrl) {

        Cocos2dxActivity mCocos2dxActivity = (Cocos2dxActivity) Cocos2dxActivity.getContext();

        Intent it = new Intent(Intent.ACTION_SEND);
        it.setType("text/plain");
        List<ResolveInfo> resInfo = mCocos2dxActivity.getPackageManager().queryIntentActivities(it, PackageManager.MATCH_DEFAULT_ONLY);
        PackageManager pm = mCocos2dxActivity.getPackageManager();
//        Collections.sort(resInfo,new ResolveInfo.DisplayNameComparator(pm));
        if (!resInfo.isEmpty()) {
            List<LabeledIntent> targetedShareIntents = new ArrayList<LabeledIntent>();
            for (ResolveInfo info : resInfo) {
                Intent targeted = new Intent(Intent.ACTION_SEND);
                targeted.setType("text/plain");
                ActivityInfo activityInfo = info.activityInfo;

                String packageName = info.activityInfo.packageName;

                if(activityInfo.packageName.contains("facebook")){
                    targeted.putExtra(Intent.EXTRA_TEXT, fbUrl);
                }
                else if (activityInfo.packageName.contains("twitter"))
                {

                    Bitmap image = getImageFromAssetsFile("share_pic.jpg");
                    saveBitmap(image);
                    File f = new File("/sdcard/myFolder/share_pic_cropped.jpg");
                    if (f != null && f.exists()) {
//            intent.setType("image/jpg");
                        targeted.putExtra(Intent.EXTRA_TEXT, msgText);

                        Uri u = Uri.fromFile(f);
                        targeted.putExtra(Intent.EXTRA_STREAM, u);
                        targeted.setType("image/jpeg");
                        targeted.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
                    }
                    targeted.setType("text/plain");
                }
                else
                {
                    targeted.putExtra(Intent.EXTRA_TEXT, msgText);
                }
                targeted.setPackage(activityInfo.packageName);
                targeted.setClassName(packageName,info.activityInfo.name);
                targeted.setComponent(new ComponentName(packageName,info.activityInfo.name));

                targetedShareIntents.add(new LabeledIntent(targeted, packageName, info.loadLabel(pm), info.icon));
                Log.i("shareUI","Can share activityname:"+info.activityInfo.name+"  package:"+info.activityInfo.packageName);
            }

//            Intent chooserIntent = Intent.createChooser(targetedShareIntents.get(0), "Select app to share");
            Intent chooserIntent = Intent.createChooser(targetedShareIntents.get(0), "Share");
            if (chooserIntent == null) {
                return;
            }
//            // A Parcelable[] of Intent or LabeledIntent objects as set with
//            // putExtra(String, Parcelable[]) of additional activities to place
//            // a the front of the list of choices, when shown to the user with a
//            // ACTION_CHOOSER.
            LabeledIntent[] extraIntents = targetedShareIntents.toArray( new LabeledIntent[ targetedShareIntents.size() ]);

            chooserIntent.putExtra(Intent.EXTRA_INITIAL_INTENTS, extraIntents);//new Parcelable[targetedShareIntents.size()]
            mCocos2dxActivity.startActivityForResult(chooserIntent, 10000);

        }
    }

    //====分享 end====

    public static boolean isScreenPortrait() {
        return isPortrait;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);


        if(requestCode == 10000){
//            Log.i("shareUI","share success---------");
//
            Log.i("shareUI", "result = " + resultCode);
            onSharedResult(resultCode);
        } else if (requestCode == 258){
            Log.i("shareUI", "result = " + resultCode);
            if (resultCode == RESULT_CANCELED){

                if (curPickerType == PickerTypeCardback) {
                    final Handler handler = new Handler();
                    handler.postDelayed(new Runnable() {
                        @Override
                        public void run() {
                            //Do something after 100ms
                            Cocos2dxActivity mCocos2dxActivity = (Cocos2dxActivity) Cocos2dxActivity.getContext();

                            mCocos2dxActivity.runOnGLThread(new Runnable() {
                                @Override
                                public void run() {

                                    onCardbackFileCanceled();

                                }
                            });
                        }
                    }, 1000 / 24);
                } else {
                    final Handler handler = new Handler();
                    handler.postDelayed(new Runnable() {
                        @Override
                        public void run() {
                            //Do something after 100ms
                            Cocos2dxActivity mCocos2dxActivity = (Cocos2dxActivity) Cocos2dxActivity.getContext();

                            mCocos2dxActivity.runOnGLThread(new Runnable() {
                                @Override
                                public void run() {

                                    onThemeFileCanceled();

                                }
                            });
                        }
                    }, 1000 / 24);

                }

            } else if (resultCode == RESULT_OK){

                Uri origin = data.getData();

                if (curPickerType == PickerTypeCardback){

                    Log.i("fuck", "path = " + Cocos2dxHelper.getCocos2dxWritablePath());
                    Uri des = Uri.fromFile(new File(getCacheDir(), "cropped"));
                    Log.i("fuck", "dir = " + getCacheDir());
                    Uri destination = Uri.fromFile(new File(Cocos2dxHelper.getCocos2dxWritablePath(), "cardback.png"));
                    Log.i("fuck", "destination = " + destination);
                    Crop.of(origin, destination).withAspect(78,124).withMaxSize(78,124).start(instance);
                } else if (curPickerType == PickerTypeTheme) {


                    AppActivity mCocos2dxActivity = (AppActivity) Cocos2dxActivity.getContext();
                    Intent intent = new Intent(mCocos2dxActivity,EditThemeActivity.class);
                    intent.putExtra("origin", origin);
                    mCocos2dxActivity.startActivity(intent);
                }
            }
        } else if (requestCode == Crop.REQUEST_CROP){
            if (curPickerType == PickerTypeCardback) {
                if (resultCode == RESULT_OK){

                    final Handler handler = new Handler();
                    handler.postDelayed(new Runnable() {
                        @Override
                        public void run() {
                            //Do something after 100ms
                            Cocos2dxActivity mCocos2dxActivity = (Cocos2dxActivity) Cocos2dxActivity.getContext();

                            mCocos2dxActivity.runOnGLThread(new Runnable() {
                                @Override
                                public void run() {
                                    onCardbackFileChanged();
                                }
                            });
                        }
                    }, 1000 / 24);


                }
                else {
                    final Handler handler = new Handler();
                    handler.postDelayed(new Runnable() {
                        @Override
                        public void run() {
                            //Do something after 100ms
                            Cocos2dxActivity mCocos2dxActivity = (Cocos2dxActivity) Cocos2dxActivity.getContext();

                            mCocos2dxActivity.runOnGLThread(new Runnable() {
                                @Override
                                public void run() {
                                    onCardbackFileCanceled();
                                }
                            });
                        }
                    }, 1000 / 24);
                }
            }
        }
    }

    public static native String getLocalization(String key);

    public static native  void onThemeFileCanceled();

    public static native  void onThemeFileChanged();

    public static native  void onCardbackFileCanceled();

    public static native  void onCardbackFileChanged();

    public static native void onScreenSizeChanged(boolean value);

    public static native  void onSharedResult(int code);

    int lastOrientation;

    private static final int REQUESTCODE_STORAGE_PERMISSION = 860;
    private static boolean storagePermitted(Activity activity) {

        if (ActivityCompat.checkSelfPermission(activity, Manifest.permission.READ_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED &&

                ActivityCompat.checkSelfPermission(activity, Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED)

            return true;

        ActivityCompat.requestPermissions(activity, new String[]{Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.WRITE_EXTERNAL_STORAGE}, REQUESTCODE_STORAGE_PERMISSION);

        return false;

    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);

        if (lastOrientation == newConfig.orientation) return;
        if (newConfig.orientation == this.getResources().getConfiguration().ORIENTATION_PORTRAIT) {

            isPortrait = true;
            //Toast.makeText(this,"por",Toast.LENGTH_LONG).show();
        }
//切换为横屏
        else if (newConfig.orientation == this.getResources().getConfiguration().ORIENTATION_LANDSCAPE) {
            isPortrait = false;
            //Toast.makeText(this,"landscape",Toast.LENGTH_LONG).show();
        }
        Cocos2dxActivity mCocos2dxActivity = (Cocos2dxActivity) Cocos2dxActivity.getContext();
        mCocos2dxActivity.runOnGLThread(new Runnable() {
            @Override
            public void run() {
                onScreenSizeChanged(isPortrait);
            }
        });
        lastOrientation = newConfig.orientation;
    }

    static boolean isStarted = false;

    @Override
    protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        Log.v("fuck2", "onNewIntent");
        setIntent(intent);


    }

    @Override
    protected void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        AdmobPlugin.setActivity(this);
        FaceBookPlugin.setActivity(this);
        AnalystPlugin.setActivity(this);
        UnityAdsPlugin.getInstance().setActivity(this);
        VunglePlugin.getInstance().setActivity(this);
        Notifications.setActivity(this);
        DeviceInfo.configSet();

        instance = this;
        con = this;
        //注册广播接受者java代码
        IntentFilter intentFilter = new IntentFilter(Intent.ACTION_BATTERY_CHANGED);
        //创建广播接受者对象
        batteryReceiver = new BatteryReceiver();

        //注册receiver
        registerReceiver(batteryReceiver, intentFilter);


        Configuration newConfig = this.getResources().getConfiguration();
        if (newConfig.orientation == this.getResources().getConfiguration().ORIENTATION_PORTRAIT) {

            isPortrait = true;
            //Toast.makeText(this,"por",Toast.LENGTH_LONG).show();
        }
//切换为横屏
        else if (newConfig.orientation == this.getResources().getConfiguration().ORIENTATION_LANDSCAPE) {
            isPortrait = false;
            //Toast.makeText(this,"landscape",Toast.LENGTH_LONG).show();
        }
        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        client = new GoogleApiClient.Builder(this).addApi(AppIndex.API).build();

        Log.i("fuck", "onCreate");

        mHelper = new DBOpenHelper(this);
        if (isStarted){

            checkThemeSetup(false);
        } else {
            checkThemeSetup(true);
            isStarted = true;
        }

        storagePermitted(this);
    }

    public static native void onActivityDestroy();


    @Override
    protected void onDestroy() {
        super.onDestroy();

        Log.i("fuck", "onDestroy");
        unregisterReceiver(batteryReceiver);
        onActivityDestroy();

    }

    //KT SYN2
    @Override
    protected void onResume() {
        super.onResume();
        Log.i("fuck", "onResume");

        Handler handler = new Handler();
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                checkThemeSetup(false);
            }
        }, 1000 / 24);


    }

    @Override
    protected void onPause() {
        super.onPause();
        Log.i("fuck", "onPause");
    }

    private long mkeyTime = 0;

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        //二次返回退出
        if (keyCode == KeyEvent.KEYCODE_BACK) {
            if ((System.currentTimeMillis() - mkeyTime) > 2000) {
                mkeyTime = System.currentTimeMillis();

                String country = Locale.getDefault().getCountry();
                if (country.equals("CN")) {
                    Toast.makeText(this, "再次点击退出游戏", Toast.LENGTH_LONG).show();
                } else {
                    Toast.makeText(this, "Click Back Again To Exit", Toast.LENGTH_LONG).show();
                }

//                Notifications.addTimeNoticfy("lll11","12:25:00",0,2);
//                Notifications.addTimeNoticfy("lll12","12:26:00",0,3);
//                Notifications.addTimeNoticfy("lll13","12:27:00",0,4);
            } else {
                finish();
//                System.exit(0);
            }
            return false;
        }
        return super.onKeyDown(keyCode, event);
    }
}
