//
//  HLAdManagerWrapper.cpp
//  MaLi2
//
//  Created by 宋扬 on 16/4/7.
//
//

#include "HLAdManagerWrapper.hpp"
#import "HLAdManager.h"


static float bannerTimer = 0;
static bool isShow = false;

void HLAdManagerWrapper::setBannerShowTime(float v){
    bannerTimer = v;
}

int HLAdManagerWrapper::getBannerShowTime(){
    return bannerTimer;
}

void HLAdManagerWrapper::update(float dt){
    if(isShow){
        bannerTimer+= dt;
    }
}

void HLAdManagerWrapper::repositionBanner(){
    
    
    
    [[HLAdManager sharedInstance] repositionBanner];
}

void HLAdManagerWrapper::hideBanner(){
    [[HLAdManager sharedInstance] hideBanner];
    isShow = false;
}

void HLAdManagerWrapper::showBanner(){
    [[HLAdManager sharedInstance] showBanner];
    isShow = true;
}

void HLAdManagerWrapper::showUnsafeInterstitial(){
    [[HLAdManager sharedInstance] showUnsafeInterstitial];
}

void HLAdManagerWrapper::showEncourageInterstitial(){
    
    [[HLAdManager sharedInstance] showEncourageInterstitial];
}

bool HLAdManagerWrapper::isEncourageInterstitialLoaded(){
    return [[HLAdManager sharedInstance] isEncourageInterstitialLoaded];
}
