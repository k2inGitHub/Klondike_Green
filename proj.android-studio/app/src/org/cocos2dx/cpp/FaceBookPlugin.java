package org.cocos2dx.cpp;

import android.app.Activity;
import android.os.Handler;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.widget.FrameLayout;

import java.util.concurrent.ExecutionException;
import java.util.concurrent.FutureTask;
import java.util.concurrent.Callable;

import com.facebook.ads.AbstractAdListener;
import com.facebook.ads.Ad;
import com.facebook.ads.AdError;
import com.facebook.ads.AdListener;
import com.facebook.ads.AdSettings;
import com.facebook.ads.AdSize;
import com.facebook.ads.AdView;
import com.facebook.ads.InterstitialAd;
import com.facebook.ads.InterstitialAdListener;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxHelper;

public class FaceBookPlugin{
	private static Activity activity;

	private static InterstitialAd interstitial;
	private static AdView adView;
	
	private static String MY_INTERSTITIAL_ID = "";
	private static String MY_BANNER_ID = "";

	static int repeatRequestTime = 0;

	static boolean canRequest = true;
	
	public static void setActivity(Activity act){
		activity = act;
	}

	public static void startAD(String bannerID, String interstitialID){
		MY_BANNER_ID = bannerID;
		MY_INTERSTITIAL_ID = interstitialID;

        repeatRequestTime = Cocos2dxHelper.getIntegerForKey("ctrl_fb_repeat_time", 8);

		activity.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				AdSettings.addTestDevice("496798ad939908dba8ea836179c7506c");
				AdSettings.addTestDevice("9de9ceb49d4cfe0d9e52e91d8c9367b6");
				AdSettings.addTestDevice("03c72ade07bfc41e0502f0f602f66eb5");
				AdSettings.addTestDevice("a3dcaae8774c5f1ecd4139937fba5d78");
				AdSettings.addTestDevice("1f855e5bb29ead3844a34dbb0a70e84b");
				AdSettings.addTestDevice("585992dab903fe266cdd65d1701624e1");




			}
		});

        requesInterstitial();
	}
	
	public static void showBanner(){

		if(MY_BANNER_ID.equals("")){
			return;
		}

		activity.runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				if(adView == null){
					try{
						FrameLayout.LayoutParams layoutParams = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT,
								FrameLayout.LayoutParams.WRAP_CONTENT);
						layoutParams.gravity = Gravity.BOTTOM | Gravity.CENTER;

						adView = new AdView(activity,MY_BANNER_ID,AdSize.BANNER_HEIGHT_50);
			            activity.addContentView(adView, layoutParams);

						adView.loadAd();

						adView.setAdListener(new AdListener() {
							@Override
							public void onError(Ad ad, AdError adError) {

							}

							@Override
							public void onAdLoaded(Ad ad) {

							}

							@Override
							public void onAdClicked(Ad ad) {
								onBannerLeaveApplication();

							}
						});
			        }catch (Exception e) {
			            Log.e("AdmobPlugin", e.toString());
			        }
				}
//				adView.setAdListener(activity);

				adView.setVisibility(View.VISIBLE);
			}
		});
	}
	
	public static void hideBanner(){
		activity.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				if(adView != null){
					adView.setVisibility(View.GONE);
				}
			}
		});
	}

	public static void showInterstitial(){
		activity.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				if (interstitial != null && interstitial.isAdLoaded()) {
					interstitial.show();
				}
			}
		});
	}

	public  static void requesInterstitial(){
        Log.i("fuck", "requesInterstitial");

		activity.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				if (interstitial != null) {
					interstitial.destroy();
					interstitial = null;
				}

				interstitial = new InterstitialAd(activity, MY_INTERSTITIAL_ID);

				interstitial.setAdListener(new AbstractAdListener() {
					@Override
					public void onInterstitialDismissed(Ad ad) {
						requesInterstitial();
					}

					@Override
					public void onAdClicked(Ad ad) {
						onInstertialLeaveApplication();
					}

					@Override
					public void onAdLoaded(Ad ad) {

                        Log.i("fuck", "onAdLoaded");
						FaceBookPlugin.canRequest = false;
					}

					@Override
					public void onError(Ad ad, AdError adError) {

                        Log.i("fuck", "onError = " + adError);
						FaceBookPlugin.canRequest = true;
						final Handler handler = new Handler();
						handler.postDelayed(new Runnable() {
							@Override
							public void run() {
								if (FaceBookPlugin.canRequest){
									FaceBookPlugin.requesInterstitial();
								}
							}
						}, 1000 * FaceBookPlugin.repeatRequestTime);
					}
				});
				interstitial.loadAd();
			}
		});
	}

	public static  boolean isInterstitialLoaded() {
		FutureTask<Integer> futureResult = new FutureTask<Integer>(new Callable<Integer>() {
			@Override
			public Integer call() throws Exception {
				Integer var = interstitial.isAdLoaded() ? 1 : 0;
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

	public static native void onBannerLeaveApplication();

	public static native void onInstertialLeaveApplication();


}
