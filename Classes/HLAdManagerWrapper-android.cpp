//
//  HLAdManagerWrapper-android.cpp
//  Klondike
//
//  Created by 宋扬 on 16/8/8.
//
//



#include "HLAdManagerWrapper.hpp"
#include "HLInterfaceAndroid.hpp"





class AdElement {
public:
    AdBaseWrapper * wrapper;
    int pri;
    std:: string name;
};

static std::vector<AdElement *> _interstitialList;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

static float bannerTimer = 0;
static bool isShow = false;

extern "C"
{

    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AdmobPlugin_onBannerLeaveApplication(JNIEnv *env, jclass)
    {
        __NotificationCenter::getInstance()->postNotification(kOnAdLeaveApplication);
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AdmobPlugin_onInstertialLeaveApplication(JNIEnv *env, jclass)
    {
        __NotificationCenter::getInstance()->postNotification(kOnAdLeaveApplication);
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_FaceBookPlugin_onBannerLeaveApplication(JNIEnv *env, jclass)
    {
        __NotificationCenter::getInstance()->postNotification(kOnAdLeaveApplication);
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_FaceBookPlugin_onInstertialLeaveApplication(JNIEnv *env, jclass)
    {
        __NotificationCenter::getInstance()->postNotification(kOnAdLeaveApplication);
    }
    
}

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

}

void HLAdManagerWrapper::startAD(){
    AdmobWrapper::getInstance()->startAD();
    FBWrapper::getInstance()->startAD();
    UnityADWrapper::getInstance()->startAD();
    VungleWrapper::getInstance()->startAD();

    std::vector<AdElement *> arr;
    {
        AdElement* e = new AdElement();
        e->wrapper = AdmobWrapper::getInstance();
        e->pri = HLInterfaceAndroid::getInstance()->admob_pop_level;
        e->name = "InterstitialAdmob";
        arr.push_back(e);
    }
    {
        AdElement* e = new AdElement();
        e->wrapper = UnityADWrapper::getInstance();
        e->pri = HLInterfaceAndroid::getInstance()->unityad_pop_level;
        e->name = "InterstitialUnity";
        arr.push_back(e);
    }
    {
        AdElement* e = new AdElement();
        e->wrapper = FBWrapper::getInstance();
        e->pri = HLInterfaceAndroid::getInstance()->fb_pop_level;
        e->name = "InterstitialFB";
        arr.push_back(e);
    }
    {
        AdElement* e = new AdElement();
        e->wrapper = VungleWrapper::getInstance();
        e->pri = HLInterfaceAndroid::getInstance()->vungle_pop_level;
        e->name = "InterstitialVungle";
        arr.push_back(e);
    }
    
    std::sort(arr.begin(), arr.end(), [](AdElement *a, AdElement *b){
        return a->pri < b->pri;
    });
    
    for (int i = 0; i < arr.size(); i++) {
        _interstitialList.push_back(arr[i]);
    }
}

void HLAdManagerWrapper::hideBanner(){
    AdmobWrapper::getInstance()->hideBanner();
    FBWrapper::getInstance()->hideBanner();
    isShow = false;
}

void HLAdManagerWrapper::showBanner(){
    if(HLInterfaceAndroid::getInstance()->ctrl_admob_banner_switch==1){
        AdmobWrapper::getInstance()->showBanner();
    } else if(HLInterfaceAndroid::getInstance()->ctrl_fb_banner_switch==1){
        FBWrapper::getInstance()->showBanner();
        
    }
    isShow = true;
}

void HLAdManagerWrapper::showUnsafeInterstitial(){
    
    for(auto e : _interstitialList){
        auto imp = e->wrapper;
        if(imp->canShowUnSafe() == true){
            imp->showInterstitial();
            HLAnalsytWrapper::event(e->name);
            break;
        }
    }
}

void HLAdManagerWrapper::showEncourageInterstitial(){
    if(HLInterfaceAndroid::getInstance()->encouraged_ad_strategy_unityad_switch == 1){
        UnityADWrapper::getInstance()->showInterstitial();
    }
    else if(HLInterfaceAndroid::getInstance()->encouraged_ad_strategy_vungle_switch == 1){
        VungleWrapper::getInstance()->showInterstitial();
    }
}

bool HLAdManagerWrapper::isEncourageInterstitialLoaded(){
    bool isReady = false;
    
    if(HLInterfaceAndroid::getInstance()->encouraged_ad_strategy_unityad_switch == 1){
        isReady = UnityADWrapper::getInstance()->isInterstitialLoaded();
    }
    
    if(isReady == false && HLInterfaceAndroid::getInstance()->encouraged_ad_strategy_vungle_switch == 1){
        isReady = VungleWrapper::getInstance()->isInterstitialLoaded();
    }
    
    return isReady;
}

#endif
