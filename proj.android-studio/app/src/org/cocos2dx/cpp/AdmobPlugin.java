package org.cocos2dx.cpp;

import android.app.Activity;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.widget.FrameLayout;

import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.InterstitialAd;

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.FutureTask;

public class AdmobPlugin {
	private static Activity activity;
	
	private static InterstitialAd interstitial;
	private static AdView adView;
	
	private static String MY_INTERSTITIAL_ID = "";
	private static String MY_BANNER_ID = "";
	
	public static void setActivity(Activity act){
		activity = act;
	}

	public static void startAD(String bannerID, String interstitialID){
		MY_BANNER_ID = bannerID;
		MY_INTERSTITIAL_ID = interstitialID;
		requesInterstitial();
	}

	public  static void requesInterstitial(){

		activity.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				interstitial = new InterstitialAd(activity);
				interstitial.setAdUnitId(MY_INTERSTITIAL_ID);
				AdRequest adRequest = new AdRequest.Builder()
						.addTestDevice("593B342D0D4AAA965C36D76B97736478")
						.addTestDevice("65FBDD0F7A529C9DE5E7BB425207238A")
						.addTestDevice("A56C839CB76E1F03C2CA749531994D60")
						.addTestDevice("6DE7BE63808B81D85D3E247F89F06BDB")
						.addTestDevice("2DCBF6BC35E390077AEACE2F24C93A56")
						.build();
				interstitial.loadAd(adRequest);
				interstitial.setAdListener(new AdListener() {
					@Override
					public void onAdClosed() {
						requesInterstitial();
					}

					@Override
					public void onAdLeftApplication() {
						onInstertialLeaveApplication();
					}
				});

			}
		});
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
						// 实例化LayoutParams(重要)
						FrameLayout.LayoutParams layoutParams = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT,
								FrameLayout.LayoutParams.WRAP_CONTENT);
						// 设置广告条的悬浮位置
						layoutParams.gravity = Gravity.BOTTOM | Gravity.CENTER;

						adView = new AdView(activity);
			            adView.setAdSize(AdSize.BANNER);
			            adView.setAdUnitId(MY_BANNER_ID);
			            // Adding full screen container
			            activity.addContentView(adView, layoutParams);

						AdRequest request = new AdRequest.Builder()
								.addTestDevice("593B342D0D4AAA965C36D76B97736478")
								.addTestDevice("65FBDD0F7A529C9DE5E7BB425207238A")
								.addTestDevice("A56C839CB76E1F03C2CA749531994D60")
								.addTestDevice("6DE7BE63808B81D85D3E247F89F06BDB")
								.addTestDevice("2DCBF6BC35E390077AEACE2F24C93A56")
								.build();
						adView.loadAd(request);

						adView.setAdListener(new AdListener() {
							@Override
							public void onAdLeftApplication() {
								onBannerLeaveApplication();
							}
						});

			        }catch (Exception e) {
			            Log.e("AdmobPlugin", e.toString());
			        }
				}
				

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

	public static  boolean isInterstitialLoaded(){
		FutureTask<Integer> futureResult = new FutureTask<Integer>(new Callable<Integer>() {
			@Override
			public Integer call() throws Exception {
				Integer var = interstitial.isLoaded() ? 1 : 0;
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
	
	public static void showInterstitial(){

//		if(MY_INTERSTITIAL_ID.equals("")){
//			return;
//		}
		activity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				if (interstitial.isLoaded()){
					interstitial.show();
				} else {
					requesInterstitial();
				}
			}
		});
	}

	public static native void onBannerLeaveApplication();

	public static native void onInstertialLeaveApplication();
}
