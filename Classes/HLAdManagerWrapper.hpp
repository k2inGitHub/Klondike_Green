//
//  HLAdManagerWrapper.hpp
//  MaLi2
//
//  Created by 宋扬 on 16/4/7.
//
//

#ifndef HLAdManagerWrapper_hpp
#define HLAdManagerWrapper_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "AdmobWrapper.h"
#include "FBWrapper.h"
#include "UnityADWrapper.h"
#include "VungleWrapper.h"
#include "HLAnalsytWrapper.hpp"

static const char *kOnAdLeaveApplication = "AdLeaveApplication";

class HLAdManagerWrapper {
    
public:
    
    static void setBannerShowTime(float v);
    
    static int getBannerShowTime();
    
    static void update(float dt);
    
    static void repositionBanner();
    
    static void startAD();
    
    static void hideBanner();
    
    static void showBanner();
    
    static void showUnsafeInterstitial();
    
    static void showEncourageInterstitial();
    
    static bool isEncourageInterstitialLoaded();
};

#endif /* HLAdManagerWrapper_hpp */
