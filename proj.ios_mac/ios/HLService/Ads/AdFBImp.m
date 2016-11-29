//
//  ADMobImp.m
//  VPN-iOS
//
//  Created by 宋扬 on 16/2/26.
//  Copyright © 2016年 handloft. All rights reserved.
//

#import "AdFBImp.h"
#import <FBAudienceNetwork/FBAudienceNetwork.h>

@interface AdFBImp () <FBAdViewDelegate,FBInterstitialAdDelegate>

@property (nonatomic, assign) BOOL isPad;

@property (nonatomic, copy) NSString *bannerID;

@property (nonatomic, copy) NSString *initerialID;

@property (nonatomic, strong) FBAdView *bannerView;

@property (nonatomic, strong) FBInterstitialAd *gadinterstitial;

@property (nonatomic, assign) BOOL canRequest;

@property (nonatomic, assign) int repeatRequestTime;

@end

@implementation AdFBImp

- (UIView *)bannerView{
    return _bannerView;
}

- (void)getAd{
    self.isPad = [[UIDevice currentDevice].model rangeOfString:@"iPad"].location != NSNotFound;
    
    _bannerID = self.infoDic[@"bannerID"];
    
    _initerialID = self.infoDic[@"initerialID"];
    
    self.repeatRequestTime = [self.infoDic[@"repeatRequestTime"] intValue];
    
    [FBAdSettings addTestDevices:@[
                                   @"5e13b5ee1d2bdfffbde657e339ad9b76f2248218",
                                   @"4f670ef1182484ca0c2b1d3c2c683c11e1183e02",
                                   @"1fdfa4bc5ef732be5e7890f70276e9cb2593b06e",
                                   ]];
    //debug
    [FBAdSettings setLogLevel:FBAdLogLevelNone];
    
    [self requestBanner];
    [self hideBanner];
    [self requesInterstitial];
}

- (void)requestBanner {
    if (_bannerView == nil) {
        FBAdSize adSize = _isPad ? kFBAdSizeHeight90Banner : kFBAdSizeHeight50Banner;

        _bannerView = [[FBAdView alloc] initWithPlacementID:self.bannerID adSize:adSize rootViewController:[self viewControllerForPresentAd]];
        _bannerView.delegate = self;
        UIView *unityView = [self viewControllerForPresentAd].view;
        
        CGPoint center = CGPointMake(CGRectGetMidX(unityView.bounds),
                                     CGRectGetMaxY(unityView.bounds) - CGRectGetMidY(_bannerView.bounds));
        self.bannerView.center = center;
        
        [[self viewControllerForPresentAd].view addSubview:_bannerView];
        
    }
    [_bannerView loadAd];
}

- (void)requesInterstitial{
    if (_gadinterstitial != nil) {
        _gadinterstitial.delegate = nil;
        _gadinterstitial = nil;
    }
    
    _gadinterstitial = [[FBInterstitialAd alloc] initWithPlacementID:self.initerialID];
    _gadinterstitial.delegate = self;
    //    _gadinterstitial.adUnitID = self.initerialID;
    
    [_gadinterstitial loadAd];
}


- (void)showBanner {
    if (_bannerView) {
        _bannerView.hidden = NO;
    }
}

- (void)hideBanner
{
    if (_bannerView) {
        _bannerView.hidden = YES;
    }
}

- (void)showInterstitial{
    
    if ([self isInterstitialLoaded]) {
        [_gadinterstitial showAdFromRootViewController:[self viewControllerForPresentAd]];
    } else {
        [self requesInterstitial];
    }
}

- (BOOL)isInterstitialLoaded{
    if (_gadinterstitial) {
        return _gadinterstitial.isAdValid;
    }
    return NO;
}

#pragma mark - FBAdViewDelegate implementation

- (void)adViewDidClick:(FBAdView *)adView
{
    NSLog(@"Ad was clicked.");
    [[NSNotificationCenter defaultCenter] postNotificationName:HLBannerLeaveNotification object:nil userInfo:@{HLAdTypeKey : HLAdFacebook}];

}

- (void)adViewDidFinishHandlingClick:(FBAdView *)adView
{
    NSLog(@"Ad did finish click handling.");
}

- (void)adViewDidLoad:(FBAdView *)adView
{
    NSLog(@"adViewDidLoad");
}

- (void)adView:(FBAdView *)adView didFailWithError:(NSError *)error
{
    NSLog(@"didFailWithError %@", error);
}

- (void)adViewWillLogImpression:(FBAdView *)adView
{
    NSLog(@"Ad impression is being captured.");
}

#pragma mark - FBInterstitialAdDelegate implementation

- (void)interstitialAdDidLoad:(FBInterstitialAd *)interstitialAd
{
    NSLog(@"Interstitial ad was loaded. Can present now.");
    [[NSNotificationCenter defaultCenter] postNotificationName:HLInterstitialPresentNotification object:nil userInfo:@{HLAdTypeKey : HLAdAdmob}];
    self.canRequest = NO;
}

- (void)interstitialAd:(FBInterstitialAd *)interstitialAd didFailWithError:(NSError *)error
{
    NSLog(@"Interstitial failed to load with error: %@", error.description);
    [[NSNotificationCenter defaultCenter] postNotificationName:HLInterstitialFailureNotification object:nil userInfo:@{HLAdTypeKey : HLAdAdmob}];
    self.canRequest = YES;
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(self.repeatRequestTime * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        if(self.canRequest){
            [self requesInterstitial];
        }
    });
}

- (void)interstitialAdDidClick:(FBInterstitialAd *)interstitialAd
{
    NSLog(@"Interstitial was clicked.");
    [[NSNotificationCenter defaultCenter] postNotificationName:HLInterstitialLeaveNotification object:nil userInfo:@{HLAdTypeKey : HLAdFacebook}];
}

- (void)interstitialAdDidClose:(FBInterstitialAd *)interstitialAd
{
    NSLog(@"Interstitial closed.");
    
    [[NSNotificationCenter defaultCenter] postNotificationName:HLInterstitialFinishNotification object:nil userInfo:@{HLAdTypeKey : HLAdAdmob}];
    [self requesInterstitial];
}

- (void)interstitialAdWillClose:(FBInterstitialAd *)interstitialAd
{
    NSLog(@"Interstitial will close.");
}

- (void)interstitialAdWillLogImpression:(FBInterstitialAd *)interstitialAd
{
    NSLog(@"Interstitial impression is being captured.");
}


@end
