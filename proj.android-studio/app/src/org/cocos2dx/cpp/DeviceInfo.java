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

import android.content.Context;
import android.content.SharedPreferences;
import android.provider.Settings.Secure;
import android.telephony.TelephonyManager;
import android.util.Log;

import org.cocos2dx.lib.Cocos2dxActivity;

import java.io.UnsupportedEncodingException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Locale;
import java.util.TimeZone;
import java.util.UUID;

import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;

public class DeviceInfo {

    private static DeviceInfo instance = null;

    public static DeviceInfo getInstance() {
        if (instance == null) {
            instance = new DeviceInfo();
        }
        return instance;
    }

    private static native void getLanguage(String value);
    public static void __getLanguage(String value){
        getLanguage(value);
    }

    private static native void getTimeZone(String value);
    public static void __getTimeZone(String value){
        getTimeZone(value);
    }

    private static native void getUserID(String value);
    public static void __getUserID(String value){
        getUserID(value);
    }

    public static Object getAppName(){
        Cocos2dxActivity context = (Cocos2dxActivity) Cocos2dxActivity.getContext();
        String name = context.getPackageName();
        return name;
    }

    public static Object getVersionName(){
        Cocos2dxActivity context = (Cocos2dxActivity) Cocos2dxActivity.getContext();
        PackageManager pm = context.getPackageManager();
        PackageInfo pi = null;
        try {
            pi = pm.getPackageInfo(context.getPackageName(), 0);
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
        String version = pi.versionName;
        return  version;
    }

    public static void configSet(){
        String lan = Locale.getDefault().getLanguage();
        String country = Locale.getDefault().getCountry();
        if (country.equals("CN")) {
            lan = "zh-CN";
        }
        Log.v("kl", "string = " + country);
        DeviceInfo.__getLanguage(lan);

        TimeZone tz = TimeZone.getDefault();
        String timeZone = tz.getID().toString();
        DeviceInfo.__getTimeZone(timeZone);

        String deviceID =  DeviceInfo.getInstance().getDeviceID();
        DeviceInfo.__getUserID(deviceID);
    }

    public static String getMD5(String info)
    {
        try
        {
            MessageDigest md5 = MessageDigest.getInstance("MD5");
            md5.update(info.getBytes("UTF-8"));
            byte[] encryption = md5.digest();

            StringBuffer strBuf = new StringBuffer();
            for (int i = 0; i < encryption.length; i++)
            {
                if (Integer.toHexString(0xff & encryption[i]).length() == 1)
                {
                    strBuf.append("0").append(Integer.toHexString(0xff & encryption[i]));
                }
                else
                {
                    strBuf.append(Integer.toHexString(0xff & encryption[i]));
                }
            }

            return strBuf.toString();
        }
        catch (NoSuchAlgorithmException e)
        {
            return "";
        }
        catch (UnsupportedEncodingException e)
        {
            return "";
        }
    }

    protected static final String PREFS_FILE = "device_id.xml";
    protected static final String PREFS_DEVICE_ID = "device_id";

    protected static UUID uuid;


    public DeviceInfo(){
        getDiviceInfo((Cocos2dxActivity) Cocos2dxActivity.getContext());
    }

    public String getDeviceID(){
        String text = uuid+"C4veqM0bQpXWmADE";
        return DeviceInfo.getMD5(text);
    }

    public void getDiviceInfo(Context context) {

        if( uuid ==null ) {
            synchronized (DeviceInfo.class) {
                if( uuid == null) {
                    final SharedPreferences prefs = context.getSharedPreferences( PREFS_FILE, 0);
                    final String id = prefs.getString(PREFS_DEVICE_ID, null );

                    if (id != null) {
                        // Use the ids previously computed and stored in the prefs file
                        uuid = UUID.fromString(id);

                    } else {

                        final String androidId = Secure.getString(context.getContentResolver(), Secure.ANDROID_ID);

                        // Use the Android ID unless it's broken, in which case fallback on deviceId,
                        // unless it's not available, then fallback on a random number which we store
                        // to a prefs file
                        try {
                            if (!"9774d56d682e549c".equals(androidId)) {
                                uuid = UUID.nameUUIDFromBytes(androidId.getBytes("utf8"));
                            } else {
                                final String deviceId = ((TelephonyManager) context.getSystemService( Context.TELEPHONY_SERVICE )).getDeviceId();
                                uuid = deviceId!=null ? UUID.nameUUIDFromBytes(deviceId.getBytes("utf8")) : UUID.randomUUID();
                            }
                        } catch (UnsupportedEncodingException e) {
                            throw new RuntimeException(e);
                        }

                        // Write the value out to the prefs file
                        prefs.edit().putString(PREFS_DEVICE_ID, uuid.toString() ).commit();

                    }

                }
            }
        }

    }

    public UUID getDeviceUuid() {
        return uuid;
    }


}
