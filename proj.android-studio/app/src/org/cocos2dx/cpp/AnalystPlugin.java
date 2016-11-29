package org.cocos2dx.cpp;

import android.app.Activity;
import android.util.Log;
import android.os.Bundle;

import com.appsflyer.*;
import com.google.firebase.analytics.FirebaseAnalytics;

import org.cocos2dx.lib.Cocos2dxActivity;
import java.util.*;


public class AnalystPlugin {
	private static Activity activity;

	private static String MY_KEY_ID = "";
	private static String MY_GCM_ID = "";

	private static String _event = "";
	private static String _value = "";


	public static void setActivity(Activity act){
		activity = act;
	}

	public static void startAppsFlyer(String keyID, String GCMID){
		//"ufz2i59YyTU6tii4MCj6G5"

		Log.v("kl", "++++++++++keyID = " + keyID);
		Log.v("kl", "++++++++++GCMID = " + GCMID);

		MY_KEY_ID = keyID;
		MY_GCM_ID = GCMID;

		if(keyID.equals("")){
			return;
		}
		activity.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				AppsFlyerLib.getInstance().startTracking(activity.getApplication(),MY_KEY_ID);
				AppsFlyerLib.getInstance().setGCMProjectNumber(activity, MY_GCM_ID);
			}
		});
	}

	private static FirebaseAnalytics mFirebaseAnalytics;

	public static void startFirebase(){

		Log.i("kl", "++++++++++FireBaseStart = ");

		mFirebaseAnalytics = FirebaseAnalytics.getInstance(activity);

		Bundle bundle = new Bundle();
		bundle.putString(FirebaseAnalytics.Param.ITEM_ID, "AAA");
		bundle.putString(FirebaseAnalytics.Param.ITEM_NAME, "AAA");
		bundle.putString(FirebaseAnalytics.Param.CONTENT_TYPE, "image");
		mFirebaseAnalytics.logEvent(FirebaseAnalytics.Event.SELECT_CONTENT, bundle);
	}

	public static  void event(String event){
		_event = event;
		activity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				Log.i("kl", "event = " + _event);
				Map<String, Object> eventValue = new HashMap<String, Object>();
				eventValue.put("value","1");
				AppsFlyerLib.getInstance().trackEvent(Cocos2dxActivity.getContext(), _event, eventValue);
			}
		});
	}

	public static  void event(String event, String value){
		_event = event;
		_value = value;
		activity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				Log.i("kl", "event = " + _event);
				Map<String, Object> eventValue = new HashMap<String, Object>();
				eventValue.put("value",_value);
				AppsFlyerLib.getInstance().trackEvent(Cocos2dxActivity.getContext(), _event, eventValue);
			}
		});
	}

}
