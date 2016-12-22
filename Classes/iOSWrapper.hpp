//
//  iOSWrapper.hpp
//  Klondike
//
//  Created by 宋扬 on 16/7/7.
//
//

#ifndef iOSWrapper_hpp
#define iOSWrapper_hpp

#include <stdio.h>
#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

static const char* kBatteryChanged = "BatteryChanged";

class iOSWrapper {
    
public:
    
    static void showToSetupThemeAppPage(bool directly = false);
    
    //0-1.0
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    static char* jstringTostring(JNIEnv* env, jstring jstr);
    
    static std::string getAppName();
    
#endif
    
    static void setStart(bool value);
    
    static std::string getVersionName();
    
    static float getBattery();
    
    static std::string getRateURL();
    
    static std::string getLocalTimeZone();
    
    static void toSettingController();
    
    static void showNewGameSheet();
    
    static void showHUD(std::string title);
    
    static bool isPad();
    
    static std::string getCurrentLanguage();
    
    static std::string getUserID();
    
    static void showShareUI();
    
    static bool isScreenPortrait();
    
    //更新相关
    static bool isForceUpdate();
    
    static std::string updateVersion();
    
    static bool isUpdateEnable();

    static std::string updateContent();
    
    static std::string updateUrl();
    
    static void showImagePicker();
    
    static void showThemePicker();
    
    static void showMoreTheme();
};



#endif /* iOSWrapper_hpp */
