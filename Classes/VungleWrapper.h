//
//  VungleWrapper.h
//  PowersOfTwo
//
//  Created by Freeman on 14-4-19.
//
//

#ifndef VUNGLE_WRAPPER_H_
#define VUNGLE_WRAPPER_H_
#include "AdBaseWrapper.hpp"

class VungleWrapper : public AdBaseWrapper{
    
public:
    static VungleWrapper * getInstance();
    
    void startAD();

    void showInterstitial();
    
    bool isInterstitialLoaded();
    
    
    void update(float dt);
    bool init();
    bool canShowSafe();
    bool canShowUnSafe();
    
public:
    
    float coldTimes;
    
};

#endif /* defined(ADMOB_WRAPPER_H_) */
