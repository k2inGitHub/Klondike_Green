//
//  VungleWrapper.h
//  PowersOfTwo
//
//  Created by Freeman on 14-4-19.
//
//

#ifndef HLANALYSTANDROID_H_
#define HLANALYSTANDROID_H_


class HLAnalystAndroid{
    
public:
    
    static void StartAnalyst();
    
    
    #if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    static void StartFirebase();
    static void StartAppsflyer();
    #endif
};

#endif
