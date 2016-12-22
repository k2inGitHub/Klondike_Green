package org.cocos2dx.cpp;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.support.v4.app.NotificationCompat;
import android.util.Log;

import com.skyeye.solitaire1.R;

public class Cocos2dxNotification {
	public static void doNotify(Context paramContext, String packageName, String ticker, String title, String text, int id)
	{
		int icon = paramContext.getResources().getIdentifier("notification_icon", "drawable", paramContext.getPackageName());

		NotificationManager localNotificationManager = (NotificationManager)paramContext.getSystemService(Context.NOTIFICATION_SERVICE);
		NotificationCompat.Builder localBuilder = new NotificationCompat.Builder(paramContext);
		localBuilder.setSmallIcon(R.mipmap.ic_launcher);
		localBuilder.setTicker(ticker);
		localBuilder.setContentTitle(title);
		localBuilder.setContentText(text);
		localBuilder.setAutoCancel(true);
//		try
//		{
			Log.v("222222222","-22222222222222222222");
			Log.v("MyService",packageName);
//			Log.v("MyService",Class.forName(packageName).toString());
			Intent localIntent = new Intent(paramContext, AppActivity.class);
			localIntent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
			localIntent.addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);
			localBuilder.setContentIntent(PendingIntent.getActivity(paramContext, 0, localIntent, PendingIntent.FLAG_ONE_SHOT));
			Notification notfi =localBuilder.build();
			notfi.defaults=Notification.DEFAULT_SOUND;
			notfi.defaults |= Notification.DEFAULT_VIBRATE;
			notfi.defaults|=Notification.DEFAULT_LIGHTS;
			localNotificationManager.notify(id, notfi);

//			return;
//		}
//		catch (ClassNotFoundException localClassNotFoundException)
//		{
//			localClassNotFoundException.printStackTrace();
//		}
	}
}