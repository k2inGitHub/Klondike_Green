#include "UnityADWrapper.h"
#include "HLInterfaceAndroid.hpp"
#include "iOSWrapper.hpp"


#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#include <jni.h>
#include "platform/android/jni/JniHelper.h"

using namespace cocos2d;
using namespace std;

static UnityADWrapper * s_Instance = NULL;

UnityADWrapper * UnityADWrapper::getInstance(){
	if(s_Instance == NULL){
		s_Instance = new UnityADWrapper();
        s_Instance->init();
	}

	return s_Instance;
}

bool UnityADWrapper::init()
{
    AdBaseWrapper::init();
    coldTimes = 10000;
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this,0,false);
    return true;
}

void UnityADWrapper::update(float dt){
    coldTimes += dt;
}

bool UnityADWrapper::canShowSafe(){
    if(HLInterfaceAndroid::getInstance()->market_reviwed_status == 0){
        return false;
    }
    
    if(HLInterfaceAndroid::getInstance()->ctrl_unityad_pop_switch == 0){
        return false;
    }
    
    if(coldTimes<HLInterfaceAndroid::getInstance()->ctrl_unityad_pop_time){
        return false;
    }
    
    return isInterstitialLoaded();
}

bool UnityADWrapper::canShowUnSafe(){
    if(HLInterfaceAndroid::getInstance()->market_reviwed_status == 0){
        return false;
    }
    
    if(HLInterfaceAndroid::getInstance()->ctrl_unsafe_unityad_pop_switch == 0){
        return false;
    }
    
    if(coldTimes<HLInterfaceAndroid::getInstance()->ctrl_unityad_pop_time){
        return false;
    }
    
    return isInterstitialLoaded();
}


void UnityADWrapper::startAD(){
    JniMethodInfo minfo;
    
    bool isHave = JniHelper::getStaticMethodInfo(minfo,
                                                 "org.cocos2dx.cpp.UnityAdsPlugin","startAD","(Ljava/lang/String;)V");
    
    jstring unityID = minfo.env->NewStringUTF(UserDefault::getInstance()->getStringForKey("unityad_code").c_str());

    if(!isHave){
        //CCLog("jni:openURL 函数不存在");
    }else{
        minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID,unityID);
    }
    
　　 minfo.env->DeleteLocalRef(unityID);
}

void UnityADWrapper::showInterstitial(){
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,
			"org.cocos2dx.cpp.UnityAdsPlugin","showAD","()V");
	if(!isHave){
			//CCLog("jni:openURL 函数不存在");
	}else{
		minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID);
	}
    coldTimes = 0;
}


bool UnityADWrapper::isInterstitialLoaded(){
    
    bool ready = false;
    
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,
				"org.cocos2dx.cpp.UnityAdsPlugin","isReady","()Z");
	if(!isHave){
				//CCLog("jni:openURL 函数不存在");
	}else{
		ready = minfo.env->CallStaticBooleanMethod(minfo.classID,minfo.methodID);
	}
    
    return ready;
}

#endif
