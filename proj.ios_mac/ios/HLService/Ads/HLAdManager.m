//
//  RateUs.m
//  zuanshimicheng
//
//  Created by zhangrui on 14-5-23.
//
//


#import "HLAdManager.h"
#import "HLInterface.h"
#import "NSString+KTAddition.h"
//#import "AdMobViewController.h"
//#import "BannerAdViewController.h"
//#import "UnityAdViewController.h"
//#import "AdMobViewController.h"
#import "AdMobImp.h"
#if !USE_OVERSEAS_AD
#import "AdMogoImp.h"
#else
#import "AdFBImp.h"
#endif
#import "AdUnityImp.h"
#import "AdVungleImp.h"
#import "AdHLImp.h"
#import "AdDummyImp.h"
#import "HLAnalyst.h"

NSString * const HLInterstitialFinishNotification = @"HLInterstitialFinishNotification";

NSString * const HLInterstitialFailureNotification = @"HLInterstitialFailureNotification";

NSString * const HLInterstitialPresentNotification = @"HLInterstitialPresentNotification";

NSString * const HLInterstitialLeaveNotification = @"HLInterstitialLeaveNotification";

NSString * const HLBannerLeaveNotification = @"HLBannerLeaveNotification";


NSString * const HLAdTypeKey = @"AdType";

HLAdType HLAdMogo = @"mogo";
HLAdType HLAdAdmob = @"admob";
HLAdType HLAdUnity = @"unity";
HLAdType HLAdVungle = @"vungle";
HLAdType HLAdHandloft = @"handloft";
HLAdType HLAdFacebook = @"facebook";

@interface HLAdManager ()

@property (nonatomic, strong) AdMobImp *adMob;
#if !USE_OVERSEAS_AD
@property (nonatomic, strong) AdMogoImp *adMogo;
#else
@property (nonatomic, strong) AdFBImp *adFB;
#endif
@property (nonatomic, strong) AdUnityImp *adUnity;

@property (nonatomic, strong) AdVungleImp *adVungle;

@property (nonatomic, strong) AdDummyImp *adDummy;

@property (nonatomic, strong) AdHLImp *adHL;

@property (nonatomic, strong) NSTimer *timer;

@property (nonatomic, assign) float interval;

@property (nonatomic, strong) NSMutableArray *interstitialList;

@end

@implementation HLAdManager


+(id)sharedInstance{
    static HLAdManager* instance = nil;
    if(instance == nil){
        @synchronized(self){
            if(instance == nil){
                instance = [[HLAdManager alloc] init];
            }
        }
    }
    return  instance;
}
-(id)init{
    self = [super init];
    if(self){
        
        HLInterface *hl = [HLInterface sharedInstance];
        
        _adMob = [[AdMobImp alloc] initWithAdManager:self
                                             andInfo:@{@"bannerID" : hl.ctrl_admob_banner_id,
                                                       @"initerialID" : hl.ctrl_admob_pop_id}];
        [_adMob getAd];
        
#if !USE_OVERSEAS_AD

        _adMogo = [[AdMogoImp alloc] initWithAdManager:self
                                               andInfo:@{@"phoneID" : hl.ctrl_banner_iphone_id,
                                                         @"padID" : hl.ctrl_banner_ipad_id}];
        [_adMogo getAd];
#else
        _adFB = [[AdFBImp alloc] initWithAdManager:self
                                               andInfo:@{@"bannerID" : hl.ctrl_fb_banner_id,
                                                         @"initerialID" : hl.ctrl_fb_pop_id,
                                                         @"repeatRequestTime" : @(hl.ctrl_fb_repeat_time)}];
        [_adFB getAd];
#endif
        _adUnity = [[AdUnityImp alloc] initWithAdManager:self andInfo:@{@"appID" : hl.unityad_code}];
        [_adUnity getAd];
        
        _adVungle = [[AdVungleImp alloc] initWithAdManager:self andInfo:@{@"appID" : hl.vungle_code}];
        [_adVungle getAd];
        
        _adHL = [[AdHLImp alloc] initWithAdManager:self andInfo:@{@"bannerID" : hl.ctrl_left_banner_id,
                                                                  @"initerialID" : hl.ctrl_left_pop_id,
                                                                  @"fixIniterialID" : hl.ctrl_fixed_pop_id}];
        [_adHL getAd];
        
        _adDummy = [[AdDummyImp alloc] initWithAdManager:self andInfo:nil];
        [_adDummy getAd];
        
        fbColdTime = mangoColdTime=unityColdTime=vungleColdTime=admobColdTime=-1;
        haveTick = false;
        
        _interval = 0.1f;
        _timer = [NSTimer scheduledTimerWithTimeInterval:_interval target:self selector:@selector(tick) userInfo:nil repeats:YES];
        
        
        //根据优先级排序
        NSMutableArray *arr = [NSMutableArray array];
        NSMutableDictionary *dic = [NSMutableDictionary dictionary];
        [dic setObject:_adMob forKey:@"imp"];
        [dic setObject:@(hl.admob_pop_level) forKey:@"pri"];
        [arr addObject:dic];
        
        dic = [NSMutableDictionary dictionary];
        [dic setObject:_adUnity forKey:@"imp"];
        [dic setObject:@(hl.unityad_pop_level) forKey:@"pri"];
        [arr addObject:dic];
        
        dic = [NSMutableDictionary dictionary];
        [dic setObject:_adFB forKey:@"imp"];
        [dic setObject:@(hl.fb_pop_level) forKey:@"pri"];
        [arr addObject:dic];
        
        dic = [NSMutableDictionary dictionary];
        [dic setObject:_adVungle forKey:@"imp"];
        [dic setObject:@(hl.vungle_pop_level) forKey:@"pri"];
        [arr addObject:dic];
        
        dic = [NSMutableDictionary dictionary];
        [dic setObject:_adHL forKey:@"imp"];
        [dic setObject:@(hl.left_pop_level) forKey:@"pri"];
        [arr addObject:dic];
        
        [arr sortUsingComparator:^NSComparisonResult(id  _Nonnull obj1, id  _Nonnull obj2) {
            return [obj1[@"pri"] compare:obj2[@"pri"]];
        }];
        
        
        
        _interstitialList = [NSMutableArray arrayWithCapacity:5];
        for (int i = 0; i < arr.count; i++) {
            [_interstitialList addObject:arr[i][@"imp"]];
        }
    }
    
    return self;
}

-(void)showBanner{
    
    if ([HLInterface sharedInstance].market_reviwed_status == 0) {
        [_adDummy showBanner];
    }else if([[HLInterface sharedInstance] ctrl_admob_banner_switch]==1){
        [_adMob showBanner];
    }
#if !USE_OVERSEAS_AD
    else if([[HLInterface sharedInstance] ctrl_banner_switch]==1){
        [_adMogo showBanner];
    }
#else
    else if([[HLInterface sharedInstance] ctrl_fb_banner_switch]==1){
        [_adFB showBanner];
    }
#endif
    else if ([HLInterface sharedInstance].ctrl_left_banner_switch == 1) {
        [_adHL showBanner];
    }
}

- (void)repositionBanner{
    
    AdBaseImp *ad = [self targetBannerImp];
    UIView *bannerView = [self bannerView];
    UIView *unityView = [ad viewControllerForPresentAd].view;
    
    if (bannerView != nil) {
        CGPoint center = CGPointMake(CGRectGetMidX(unityView.bounds),
                                     CGRectGetMaxY(unityView.bounds) - CGRectGetMidY(bannerView.bounds));
        bannerView.center = center;
    }
}

- (AdBaseImp *)targetBannerImp{
    AdBaseImp *ad = nil;
    if ([HLInterface sharedInstance].market_reviwed_status == 0) {
        ad = _adDummy;
    }else if([[HLInterface sharedInstance] ctrl_admob_banner_switch]==1){
        ad = _adMob;
    }
#if !USE_OVERSEAS_AD
    else if([[HLInterface sharedInstance] ctrl_banner_switch]==1){
        ad = _adMogo;
    }
#else
    else if([[HLInterface sharedInstance] ctrl_fb_banner_switch]==1){
        ad = _adFB;
    }
#endif
    else if ([HLInterface sharedInstance].ctrl_left_banner_switch == 1) {
        ad = _adHL;
    }
    return ad;
}

- (UIView *)bannerView{
    AdBaseImp *ad = [self targetBannerImp];
    if (ad != nil) {
        return [ad bannerView];
    }
    return nil;
}

-(void)hideBanner{
    [_adMob hideBanner];
#if !USE_OVERSEAS_AD
    [_adMogo hideBanner];
#else
    [_adFB hideBanner];
#endif
    [_adHL hideBanner];
    [_adDummy hideBanner];
}

-(void)showSafeInterstitial{
    if([[HLInterface sharedInstance] ctrl_pop_switch]!=1){
        return;
    }
    
    for(int i = 0; i < _interstitialList.count; i++){
        AdBaseImp *imp = _interstitialList[i];
        if (imp == _adMob) {
            if([[HLInterface sharedInstance] ctrl_admob_pop_switch]==1&&admobColdTime<=0){
                if(haveTick==true){
                    admobColdTime=[[HLInterface sharedInstance] ctrl_admob_pop_time];
                }
                [_adMob showInterstitial];
                break;
            }
        } else if (imp == _adUnity){
            if([[HLInterface sharedInstance] ctrl_unityad_pop_switch]==1&&[_adUnity isInterstitialLoaded]==true&&unityColdTime<=0){
                if(haveTick==true){
                    unityColdTime=[[HLInterface sharedInstance] ctrl_unityad_pop_time];
                }
                [_adUnity showInterstitial];
                break;
            }
        } else if (imp == _adFB){
            if([[HLInterface sharedInstance] ctrl_fb_pop_switch]==1&&fbColdTime<=0){
                if(haveTick==true){
                    fbColdTime=[[HLInterface sharedInstance] ctrl_fb_pop_time];
                }
                [_adFB showInterstitial];
                break;
            }
        } else if (imp == _adVungle){
            if ([HLInterface sharedInstance].ctrl_vungle_pop_switch == 1 && vungleColdTime <= 0){
                if(haveTick==true){
                    vungleColdTime=[[HLInterface sharedInstance] ctrl_vungle_pop_time];
                }
                [_adVungle showInterstitial];
                break;
            }
        } else if (imp == _adHL){
            if ([HLInterface sharedInstance].ctrl_left_pop_switch == 1) {
                [_adHL showInterstitial];
                break;
            }
        }
    }
}

-(void)showUnsafeInterstitial{
    if([[HLInterface sharedInstance] ctrl_pop_switch]!=1){
        return;
    }
    
    for(int i = 0; i < _interstitialList.count; i++){
        AdBaseImp *imp = _interstitialList[i];
        
        if (imp == _adMob) {
            if([[HLInterface sharedInstance] ctrl_unsafe_admob_pop_switch]==1&&admobColdTime<=0 && [_adMob isInterstitialLoaded]){
                if(haveTick==true){
                    admobColdTime=[[HLInterface sharedInstance] ctrl_admob_pop_time];
                }
                [_adMob showInterstitial];
                [HLAnalyst event:@"InterstitialAdmob"];
                break;
            }
        } else if (imp == _adUnity){
            if([[HLInterface sharedInstance] ctrl_unsafe_unityad_pop_switch]==1&&[_adUnity isInterstitialLoaded]==true&&unityColdTime<=0){
                if(haveTick==true){
                    unityColdTime=[[HLInterface sharedInstance] ctrl_unityad_pop_time];
                }
                [_adUnity showInterstitial];
                [HLAnalyst event:@"InterstitialUnity"];
                break;
            }
        } else if (imp == _adFB) {
            if([[HLInterface sharedInstance] ctrl_unsafe_fb_pop_switch]==1&&fbColdTime<=0 && [_adFB isInterstitialLoaded]){
                if(haveTick==true){
                    fbColdTime=[[HLInterface sharedInstance] ctrl_fb_pop_time];
                }
                [_adFB showInterstitial];
                [HLAnalyst event:@"InterstitialFB"];
                break;
            }
        } else if (imp == _adVungle){
            if ([HLInterface sharedInstance].ctrl_unsafe_vungle_pop_switch == 1 && vungleColdTime <= 0 && [_adVungle isInterstitialLoaded]){
                if(haveTick==true){
                    vungleColdTime=[[HLInterface sharedInstance] ctrl_vungle_pop_time];
                }
                [_adVungle showInterstitial];
                [HLAnalyst event:@"InterstitialVungle"];
                break;
            }
        } else if (imp == _adHL){
        
            if ([HLInterface sharedInstance].ctrl_unsafe_left_pop_switch == 1) {
                [_adHL showInterstitial];
                break;
            }
        }
    }
    
    //#if !USE_OVERSEAS_AD
    //            else if([[HLInterface sharedInstance] ctrl_unsafe_mango_pop_switch]==1&&mangoColdTime<=0){
    //                if(haveTick==true){
    //                    mangoColdTime=[[HLInterface sharedInstance] ctrl_mango_pop_time];
    //                }
    //                [_adMogo showInterstitial];
    //            }
    //#else
}

- (BOOL)isEncourageInterstitialLoaded{
    BOOL ret = NO;
    if ([HLInterface sharedInstance].encouraged_ad_strategy_unityad_switch == 1 && [_adUnity isInterstitialLoaded]) {
        ret = YES;
    } else if ([HLInterface sharedInstance].encouraged_ad_strategy_vungle_switch == 1 && [_adVungle isInterstitialLoaded]) {
        ret = YES;
    }
    return ret;
}

- (void)showEncourageInterstitial{

    AdBaseImp *_targetAdEncouragedInterstitial = nil;
    if ([HLInterface sharedInstance].encouraged_ad_strategy_unityad_switch == 1 && [_adUnity isInterstitialLoaded]) {
        _targetAdEncouragedInterstitial = _adUnity;
    } else if ([HLInterface sharedInstance].encouraged_ad_strategy_vungle_switch == 1 && [_adVungle isInterstitialLoaded]) {
        _targetAdEncouragedInterstitial = _adVungle;
    }
    if (_targetAdEncouragedInterstitial != nil) {
        [_targetAdEncouragedInterstitial showInterstitial];
    }
}

- (BOOL)isSplashInterstitialLoaded{
    BOOL ret = NO;
    if ([HLInterface sharedInstance].loading_left_pop_switch && [_adHL isInterstitialSplashLoaded]) {
        ret = YES;
    } else if ([HLInterface sharedInstance].loading_admob_pop_switch && [_adMob isInterstitialLoaded]) {
        ret = YES;
    }
    #if !USE_OVERSEAS_AD
    else if ([HLInterface sharedInstance].loading_mango_pop_switch && [_adMogo isInterstitialLoaded]){
        ret = YES;
    }
    #else
    else if ([HLInterface sharedInstance].loading_fb_pop_switch && [_adFB isInterstitialLoaded]) {
        ret = YES;
    }
    #endif
    return ret;
}

- (void)showSplashInterstitial{

    if ([HLInterface sharedInstance].loading_left_pop_switch && [_adHL isInterstitialSplashLoaded]) {
        [_adHL showInterstitialSplash];
    } else if ([HLInterface sharedInstance].loading_admob_pop_switch && [_adMob isInterstitialLoaded]) {
        [_adMob showInterstitial];
    }
    #if !USE_OVERSEAS_AD
    else if ([HLInterface sharedInstance].loading_mango_pop_switch && [_adMogo isInterstitialLoaded]){
        [_adMogo showInterstitial];
    }
    #else
    else if ([HLInterface sharedInstance].loading_fb_pop_switch && [_adFB isInterstitialLoaded]) {
        [_adFB showInterstitial];
    }
    #endif
}

- (void)showButtonInterstitial:(NSString *)positionTag{
    if ([HLInterface sharedInstance].button_left_pop_switch && [_adHL isInterstitialButtonLoaded]) {
        [_adHL showInterstitialButton:positionTag];
    } else if ([HLInterface sharedInstance].button_unityad_pop_switch && [_adUnity isInterstitialLoaded]){
        [_adUnity showInterstitial];
    } else if ([HLInterface sharedInstance].button_vungle_pop_switch && [_adVungle isInterstitialLoaded]) {
        [_adVungle showInterstitial];
    }
}

- (void)tick{
    haveTick = true;
    admobColdTime -= _interval;
    unityColdTime -= _interval;
    mangoColdTime -= _interval;
    vungleColdTime -= _interval;
    fbColdTime -= _interval;
}

@end
