package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.util.Log;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.json.JSONException;
import org.json.JSONObject;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

public class Notifications extends Cocos2dxActivity {
	private static Activity activity;
	public static void setActivity(Activity act){
		activity = act;
	}

	public static void addNoticfy(String title,String content,int delalt,int key,int repeatTime)
	{
		JSONObject j = new JSONObject();
		try {
			j.put("ticker", content);
			j.put("title", title);
			j.put("text", content);
			if(repeatTime<=0)
			{
				j.put("tag", "once");
			}
			else
			{
				j.put("intervalAtMillis", repeatTime);
			}
			j.put("triggerOffset", delalt);
			j.put("id", key);
			j.put("packageName", DeviceInfo.getAppName());//包名注意填
			Cocos2dxAlarmManager.alarmNotify(Cocos2dxActivity.getContext(), j.toString(), false);
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public static void addTimeNoticfy(String content,String time,int weekDay,int key)
	{

		Log.v("v","+++++++++++++++++++++++++++++++++++++++content"+content+"time"+time);

		for(int i=0; i<10; i++){

			weekDay = weekDay+i*7;
			key = key+i*7;

			JSONObject j = new JSONObject();
			try {
				String title = getAppName(Cocos2dxActivity.getContext());
				long repeatTime = 86400L*24L;
				repeatTime = 0;
				j.put("ticker", content);
				j.put("title", title);
				j.put("text", content);
				if(repeatTime<=0)
				{
					j.put("tag", "once");
				}
				else
				{
					j.put("intervalAtMillis", repeatTime);
				}
				j.put("triggerOffset", -100);
				long triggerAtMillis = 1472461440;
				triggerAtMillis = getTime(time,weekDay);

				j.put("triggerAtMillis", triggerAtMillis);
				j.put("id", key);
				j.put("packageName", DeviceInfo.getAppName());//包名注意填

				Cocos2dxAlarmManager.cancelNotify(Cocos2dxActivity.getContext(),key);
				Cocos2dxAlarmManager.alarmNotify(Cocos2dxActivity.getContext(), j.toString(), true);
			} catch (JSONException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}

	public static long getTime(String notiTime, int days){

		Calendar CD = Calendar.getInstance();
		int YY = CD.get(Calendar.YEAR) ;
		int MM = CD.get(Calendar.MONTH)+1;
		int DD = CD.get(Calendar.DATE);
		int HH = CD.get(Calendar.HOUR_OF_DAY);
		int NN = CD.get(Calendar.MINUTE);
		int SS = CD.get(Calendar.SECOND);


		String nowTime = ""+HH+":"+NN+":"+SS;
		if(ComperTime(nowTime,notiTime)==true){
			CD.add(CD.DATE,1);
			YY = CD.get(Calendar.YEAR) ;
			MM = CD.get(Calendar.MONTH)+1;
			DD = CD.get(Calendar.DATE);
		}


		String time = ""+YY+"年"+MM+"月"+DD+"日"+notiTime;
		long re_str = getTime(time)+ days*86400L;
		Log.v("Test","TestTime+++++++++++++++++++++++++++++++++++++++ "+re_str);
		return re_str;
	}

	public static boolean ComperTime(String big, String small){

		String tBig = "2010年1月1日"+big;
		String tSmall = "2010年1月1日"+small;

		if(getTime(tBig)>getTime(tSmall)){
			return true;
		}

		return false;
	}


	// 将字符串转为时间戳
	public static Long getTime(String user_time) {
		String re_time = null;
		SimpleDateFormat sdf = new SimpleDateFormat("yyyy年MM月dd日HH:mm:ss");
		Date d;
		try {
			d = sdf.parse(user_time);
			long l = d.getTime();
			String str = String.valueOf(l);
			re_time =str.substring(0, 10);
		} catch(ParseException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return Long.parseLong(re_time);
	}

	public static String getAppName(Context context)
	{
		try
		{
			PackageManager packageManager = context.getPackageManager();
			PackageInfo packageInfo = packageManager.getPackageInfo(
					context.getPackageName(), 0);
			int labelRes = packageInfo.applicationInfo.labelRes;
			return context.getResources().getString(labelRes);
		} catch (PackageManager.NameNotFoundException e)
		{
			e.printStackTrace();
		}
		return null;
	}

}
