//
//  VungleWrapper.h
//  PowersOfTwo
//
//  Created by Freeman on 14-4-19.
//
//

#ifndef UNITYAD_WRAPPER_H_
#define UNITYAD_WRAPPER_H_

#include "AdBaseWrapper.hpp"
class UnityADWrapper : public AdBaseWrapper{
    
public:
    static UnityADWrapper * getInstance();
    
    void startAD();

    void showInterstitial();
    
    void update(float dt);
    bool init();
    bool canShowSafe();
    bool canShowUnSafe();
    
    bool isInterstitialLoaded();
public:
    
    float coldTimes;
    
};

#endif /* defined(ADMOB_WRAPPER_H_) */
