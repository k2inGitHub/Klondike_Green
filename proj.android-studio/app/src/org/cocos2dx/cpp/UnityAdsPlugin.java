package org.cocos2dx.cpp;

import android.app.Activity;
import android.util.Log;

import com.unity3d.ads.IUnityAdsListener;
import com.unity3d.ads.UnityAds;
import com.unity3d.ads.log.DeviceLog;
import com.unity3d.ads.misc.Utilities;

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.FutureTask;


public class UnityAdsPlugin extends Activity {

    private static String MY_UNITY_ID = "";

    public static void startAD(String unityID){
        Log.v("kl", "unityID = " + unityID);

        MY_UNITY_ID = unityID;
        UnityAdsPlugin.getInstance().initializeAD();
    }

    public static void showAD(){
        Log.v("kl", "+++++++++++++++showAD ");


        UnityAdsPlugin.getInstance().showUnityAD();
    }

    public static boolean isReady(){
        return UnityAdsPlugin.getInstance().isUnityADReady();
    }

    private static UnityAdsPlugin instance = null;

    public static UnityAdsPlugin getInstance() {
        if (instance == null) {
            instance = new UnityAdsPlugin();
        }
        return instance;
    }

    private Activity activity;
    public void setActivity(Activity act){
        activity = act;
    }


    private String incentivizedPlacementId;
    private Boolean isReady;


    public void initializeAD(){
        isReady = false;

        final UnityAdsListener unityAdsListener = new UnityAdsListener();
        UnityAds.setListener(unityAdsListener);
        UnityAds.initialize(activity, MY_UNITY_ID, unityAdsListener, false);
    }

    public boolean isUnityADReady(){
        FutureTask<Integer> futureResult = new FutureTask<Integer>(new Callable<Integer>() {
            @Override
            public Integer call() throws Exception {
                Integer var = UnityAds.isReady() ? 1 : 0;
                return var;
            }
        });
        activity.runOnUiThread(futureResult);
        int v = 0;
        try {
            v = futureResult.get();
        } catch (InterruptedException e) {
            e.printStackTrace();
        } catch (ExecutionException e) {
            e.printStackTrace();
        }
        return v == 1;
    }

    public void showUnityAD(){
        UnityAds.show(activity, incentivizedPlacementId);
    }


    	/* LISTENER */

    private class UnityAdsListener implements IUnityAdsListener {

        @Override
        public void onUnityAdsReady(final String zoneId) {

            DeviceLog.debug("onUnityAdsReady: " + zoneId);
            Utilities.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    // look for various default placement ids over time
                    switch (zoneId) {
                        case "video":
                        case "defaultZone":
                        case "defaultVideoAndPictureZone":
                            break;
                        case "rewardedVideo":
                        case "rewardedVideoZone":
                        case "incentivizedZone":
                            break;
                    }

                    incentivizedPlacementId = zoneId;
                    isReady = true;
                }
            });

            toast("Ready", zoneId);
        }

        @Override
        public void onUnityAdsStart(String zoneId) {
            DeviceLog.debug("onUnityAdsStart: " + zoneId);
            toast("Start", zoneId);
        }

        @Override
        public void onUnityAdsFinish(String zoneId, UnityAds.FinishState result) {
            DeviceLog.debug("onUnityAdsFinish: " + zoneId + " - " + result);
            toast("Finish", zoneId + " " + result);


            initializeAD();
        }

        @Override
        public void onUnityAdsError(UnityAds.UnityAdsError error, String message) {
            DeviceLog.debug("onUnityAdsError: " + error + " - " + message);
            toast("Error", error + " " + message);

            initializeAD();
        }

        private void toast(String callback, String msg) {
//            Toast.makeText(getApplicationContext(), callback + ": " + msg, Toast.LENGTH_SHORT).show();
        }
    }

}
