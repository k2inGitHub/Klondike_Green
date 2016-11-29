//
//  VungleWrapper.h
//  PowersOfTwo
//
//  Created by Freeman on 14-4-19.
//
//

#ifndef FB_WRAPPER_H_
#define FB_WRAPPER_H_

#include "AdBaseWrapper.hpp"
class FBWrapper : public AdBaseWrapper{
    
public:
    static FBWrapper * getInstance();
    
    void startAD();
    
    // show banner ad
    void showBanner();
    // hide banner ad
    void hideBanner();
    // show interstitial
    virtual void showInterstitial();
    virtual bool canShowSafe();
    virtual bool canShowUnSafe();
    
    bool isInterstitialLoaded();
    float coldTimes;
    bool init();
    void update(float dt);
};

#endif /* defined(ADMOB_WRAPPER_H_) */
