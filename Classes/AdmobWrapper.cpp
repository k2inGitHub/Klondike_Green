#include "AdmobWrapper.h"
#include "HLInterfaceAndroid.hpp"
#include "iOSWrapper.hpp"


#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#include <jni.h>
#include "platform/android/jni/JniHelper.h"

using namespace cocos2d;
using namespace std;

static AdmobWrapper * s_Instance = NULL;

AdmobWrapper * AdmobWrapper::getInstance(){
	if(s_Instance == NULL){
		s_Instance = new AdmobWrapper();
        s_Instance->init();
    }

	return s_Instance;
}


bool AdmobWrapper::init()
{
    AdBaseWrapper::init();
    coldTimes = 10000;
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this,0,false);
    return true;
}

void AdmobWrapper::update(float dt){
    coldTimes += dt;
}

bool AdmobWrapper::isInterstitialLoaded(){

    JniMethodInfo minfo;
    
    bool isHave = JniHelper::getStaticMethodInfo(minfo,
                                                "org.cocos2dx.cpp.AdmobPlugin","isInterstitialLoaded","()Z");
    if(!isHave){
        //CCLog("jni:openURL 函数不存在");
    }else{
        return minfo.env->CallStaticBooleanMethod(minfo.classID,minfo.methodID);
    }
    return false;
}

bool AdmobWrapper::canShowSafe(){
    if(HLInterfaceAndroid::getInstance()->market_reviwed_status == 0){
        return false;
    }
    
    if(HLInterfaceAndroid::getInstance()->ctrl_admob_pop_switch == 0){
        return false;
    }
    
    if(coldTimes<HLInterfaceAndroid::getInstance()->ctrl_admob_pop_time){
        return false;
    }
    
    return isInterstitialLoaded();
}

bool AdmobWrapper::canShowUnSafe(){
    if(HLInterfaceAndroid::getInstance()->market_reviwed_status == 0){
        return false;
    }
    
    if(HLInterfaceAndroid::getInstance()->ctrl_unsafe_admob_pop_switch == 0){
        return false;
    }
    
    if(coldTimes<HLInterfaceAndroid::getInstance()->ctrl_admob_pop_time){
        return false;
    }
    
    return isInterstitialLoaded();
}

void AdmobWrapper::startAD(){
    JniMethodInfo minfo;
    
    bool isHave = JniHelper::getStaticMethodInfo(minfo,
                                                 "org.cocos2dx.cpp.AdmobPlugin","startAD","(Ljava/lang/String;Ljava/lang/String;)V");
    
    jstring bannerID = minfo.env->NewStringUTF(UserDefault::getInstance()->getStringForKey("ctrl_admob_banner_id").c_str());
    jstring interstitialID = minfo.env->NewStringUTF(UserDefault::getInstance()->getStringForKey("ctrl_admob_pop_id").c_str());
    
    if(!isHave){
        //CCLog("jni:openURL 函数不存在");
    }else{
        minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID,bannerID,interstitialID);
    }
    
　　 minfo.env->DeleteLocalRef(bannerID);
　　 minfo.env->DeleteLocalRef(interstitialID);
}

void AdmobWrapper::showBanner(){
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,
			"org.cocos2dx.cpp.AdmobPlugin","showBanner","()V");
	if(!isHave){
			//CCLog("jni:openURL 函数不存在");
	}else{
		minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID);
	}
}

void AdmobWrapper::hideBanner(){
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,
				"org.cocos2dx.cpp.AdmobPlugin","hideBanner","()V");
	if(!isHave){
				//CCLog("jni:openURL 函数不存在");
	}else{
		minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID);
	}
}

void AdmobWrapper::showInterstitial(){
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,
				"org.cocos2dx.cpp.AdmobPlugin","showInterstitial","()V");
	if(!isHave){
				//CCLog("jni:openURL 函数不存在");
	}else{
		minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID);
	}
    
    coldTimes = 0;
}

#endif
