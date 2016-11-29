package org.cocos2dx.cpp;

import android.app.Activity;
import android.util.Log;

import com.unity3d.ads.UnityAds;
import com.vungle.publisher.EventListener;
import com.vungle.publisher.VunglePub;

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.FutureTask;

public class VunglePlugin extends Activity {

    private static String MY_VUNGLE_ID = "";

    public static void startAD(String vungleID){
        MY_VUNGLE_ID = vungleID;
        VunglePlugin.getInstance().initializeAD();
    }

    public static void showAD(){
        VunglePlugin.getInstance().showVungleAD();
    }

    public static boolean isReady(){
        return VunglePlugin.getInstance().isADReady();
    }

    private static VunglePlugin instance = null;

    public static VunglePlugin getInstance() {
        if (instance == null) {
            instance = new VunglePlugin();
        }
        return instance;
    }

    private Activity activity;
    public void setActivity(Activity act){

        activity = act;

    }


    private Boolean isReady;


    final VunglePub vunglePub = VunglePub.getInstance();

    public void initializeAD(){
        isReady = false;

        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                // initialize the Publisher SDK
                vunglePub.init(activity, MY_VUNGLE_ID);
                vunglePub.setEventListeners(vungleDefaultListener);
            }
        });
    }

    public boolean isADReady(){
        FutureTask<Integer> futureResult = new FutureTask<Integer>(new Callable<Integer>() {
            @Override
            public Integer call() throws Exception {
                Integer var = vunglePub.isAdPlayable() ? 1 : 0;
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

    public void showVungleAD(){
        vunglePub.playAd();
    }

    private final EventListener vungleDefaultListener = new EventListener() {
        @Deprecated
        @Override
        public void onVideoView(boolean isCompletedView, int watchedMillis, int videoDurationMillis) {
            // This method is deprecated and will be removed. Please use onAdEnd() instead.
        }

        @Override
        public void onAdStart() {
            // Called before playing an ad.
        }

        @Override
        public void onAdUnavailable(String reason) {
            // Called when VunglePub.playAd() was called but no ad is available to show to the user.

//            initializeAD();
        }

        @Override
        public void onAdEnd(boolean wasSuccessfulView, boolean wasCallToActionClicked) {
            // Called when the user leaves the ad and control is returned to your application.

//            initializeAD();
        }

        @Override
        public void onAdPlayableChanged(boolean isAdPlayable) {
            // Called when ad playability changes.
            Log.d("DefaultListener", "This is a default eventlistener.");
            final boolean enabled = isAdPlayable;
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    // Called when ad playability changes.
                    isReady = enabled;
                }
            });
        }
    };
}
