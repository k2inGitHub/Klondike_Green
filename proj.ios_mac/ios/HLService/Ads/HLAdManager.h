#import <Foundation/Foundation.h>

//广告被点击
extern NSString * const HLInterstitialFinishNotification;
//广告点击失败
extern NSString * const HLInterstitialFailureNotification;
//广告展示
extern NSString * const HLInterstitialPresentNotification;
//点击跳转
extern NSString * const HLInterstitialLeaveNotification;

extern NSString * const HLBannerLeaveNotification;


extern NSString * const HLAdTypeKey;

typedef NSString* const HLAdType;

extern HLAdType HLAdMogo;
extern HLAdType HLAdAdmob;
extern HLAdType HLAdUnity;
extern HLAdType HLAdVungle;
extern HLAdType HLAdHandloft;
extern HLAdType HLAdFacebook;

@interface HLAdManager : NSObject {
    float admobColdTime;
    float unityColdTime;
    float mangoColdTime;
    float vungleColdTime;
    float fbColdTime;
    bool haveTick;
}

+ (id)sharedInstance;
- (void)showBanner;
- (void)hideBanner;
- (void)showSafeInterstitial;
- (void)showUnsafeInterstitial;
- (void)showEncourageInterstitial;
- (BOOL)isEncourageInterstitialLoaded;

- (BOOL)isSplashInterstitialLoaded;
- (void)showSplashInterstitial;
- (void)showButtonInterstitial:(NSString *)positionTag;

- (UIView *)bannerView;
- (void)repositionBanner;

@end
