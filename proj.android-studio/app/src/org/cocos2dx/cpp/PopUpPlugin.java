package org.cocos2dx.cpp;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;

public class PopUpPlugin {
	private static Activity activity;
	public static void setActivity(Activity act){
		activity = act;
	}

	public static void showUpdata(String unityID){
		AlertDialog ad = new AlertDialog.Builder(activity).setTitle("")
				.setMessage("Do you really want to exit?")
				.setPositiveButton("YES", new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int which) {

					}
				})
				.setNegativeButton("NO", new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int which) {

					}
				}).create();
		ad.show();
	}
}
