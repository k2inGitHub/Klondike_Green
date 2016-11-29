#include "FBWrapper.h"
#include "HLInterfaceAndroid.hpp"
#include "iOSWrapper.hpp"


#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#include <jni.h>
#include "platform/android/jni/JniHelper.h"

using namespace cocos2d;
using namespace std;

static FBWrapper * s_Instance = NULL;

bool FBWrapper::canShowSafe(){
    if(HLInterfaceAndroid::getInstance()->market_reviwed_status == 0){
        return false;
    }
    
    if(HLInterfaceAndroid::getInstance()->ctrl_fb_pop_switch == 0){
        return false;
    }
    
    if(coldTimes<HLInterfaceAndroid::getInstance()->ctrl_admob_pop_time){
        return false;
    }
    
    return isInterstitialLoaded();
}

bool FBWrapper::canShowUnSafe(){
    if(HLInterfaceAndroid::getInstance()->market_reviwed_status == 0){
        return false;
    }
    
    if(HLInterfaceAndroid::getInstance()->ctrl_unsafe_fb_pop_switch == 0){
        return false;
    }
    
    if(coldTimes<HLInterfaceAndroid::getInstance()->ctrl_admob_pop_time){
        return false;
    }
    
    return isInterstitialLoaded();
}

bool FBWrapper::isInterstitialLoaded(){
    
    JniMethodInfo minfo;
    
    bool isHave = JniHelper::getStaticMethodInfo(minfo,
                                                 "org.cocos2dx.cpp.FaceBookPlugin","isInterstitialLoaded","()Z");
    if(!isHave){
        //CCLog("jni:openURL 函数不存在");
    }else{
        return minfo.env->CallStaticBooleanMethod(minfo.classID,minfo.methodID);
    }
    return false;
}

FBWrapper * FBWrapper::getInstance(){
	if(s_Instance == NULL){
		s_Instance = new FBWrapper();
	}
	return s_Instance;
}

bool FBWrapper::init(){
    AdBaseWrapper::init();
    coldTimes = 10000;
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this,0,false);
}

void FBWrapper::update(float dt){
    coldTimes += dt;
}

void FBWrapper::startAD(){
    JniMethodInfo minfo;
    
    bool isHave = JniHelper::getStaticMethodInfo(minfo,
                                                 "org.cocos2dx.cpp.FaceBookPlugin","startAD","(Ljava/lang/String;Ljava/lang/String;)V");
    
    jstring bannerID = minfo.env->NewStringUTF(UserDefault::getInstance()->getStringForKey("ctrl_fb_banner_id").c_str());
    jstring interstitialID = minfo.env->NewStringUTF(UserDefault::getInstance()->getStringForKey("ctrl_fb_pop_id").c_str());
    
//    std::string repeatNum = "";
//    repeatNum += UserDefault::getInstance()->getIntegerForKey("ctrl_fb_banner_id", 8);
//    jstring p3 = minfo.env->NewStringUTF(repeatNum.c_str());

    
    if(!isHave){
        //CCLog("jni:openURL 函数不存在");
    }else{
        minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID,bannerID,interstitialID);
    }
    
　　 minfo.env->DeleteLocalRef(bannerID);
　　 minfo.env->DeleteLocalRef(interstitialID);
}

void FBWrapper::showBanner(){
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,
			"org.cocos2dx.cpp.FaceBookPlugin","showBanner","()V");
	if(!isHave){
			//CCLog("jni:openURL 函数不存在");
	}else{
		minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID);
	}
}

void FBWrapper::hideBanner(){
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,
				"org.cocos2dx.cpp.FaceBookPlugin","hideBanner","()V");
	if(!isHave){
				//CCLog("jni:openURL 函数不存在");
	}else{
		minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID);
	}
}

void FBWrapper::showInterstitial(){
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,
				"org.cocos2dx.cpp.FaceBookPlugin","showInterstitial","()V");
	if(!isHave){
				//CCLog("jni:openURL 函数不存在");
	}else{
		minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID);
	}
    
    coldTimes = 0;
}

#endif
