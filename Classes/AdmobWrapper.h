//
//  VungleWrapper.h
//  PowersOfTwo
//
//  Created by Freeman on 14-4-19.
//
//

#ifndef ADMOB_WRAPPER_H_
#define ADMOB_WRAPPER_H_

#include "cocos2d.h"
#include "AdBaseWrapper.hpp"
using namespace cocos2d;


class AdmobWrapper: public AdBaseWrapper{
    
public:
    static AdmobWrapper * getInstance();
    
    void startAD();
    
    // show banner ad
    void showBanner();
    // hide banner ad
    void hideBanner();
    // show interstitial
    virtual void showInterstitial();
    
    
    void update(float dt);
    bool init();
    virtual bool canShowSafe();
    virtual bool canShowUnSafe();
    bool isInterstitialLoaded();
public:
    
    
    float coldTimes;
    
};

#endif /* defined(ADMOB_WRAPPER_H_) */
