#include "VungleWrapper.h"
#include "HLInterfaceAndroid.hpp"
#include "iOSWrapper.hpp"


#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#include <jni.h>
#include "platform/android/jni/JniHelper.h"

using namespace cocos2d;
using namespace std;

static VungleWrapper * s_Instance = NULL;

VungleWrapper * VungleWrapper::getInstance(){
	if(s_Instance == NULL){
		s_Instance = new VungleWrapper();
        s_Instance->init();
	}

	return s_Instance;
}

bool VungleWrapper::init()
{
    coldTimes = 10000;
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this,0,false);
}

void VungleWrapper::update(float dt){
    coldTimes += dt;
}

bool VungleWrapper::canShowSafe(){
    if(HLInterfaceAndroid::getInstance()->market_reviwed_status == 0){
        return false;
    }
    
    if(HLInterfaceAndroid::getInstance()->ctrl_vungle_pop_switch == 0){
        return false;
    }
    
    if(coldTimes<HLInterfaceAndroid::getInstance()->ctrl_vungle_pop_time){
        return false;
    }
    
    return isInterstitialLoaded();
}

bool VungleWrapper::canShowUnSafe(){
    if(HLInterfaceAndroid::getInstance()->market_reviwed_status == 0){
        return false;
    }
    
    if(HLInterfaceAndroid::getInstance()->ctrl_unsafe_vungle_pop_switch == 0){
        return false;
    }
    
    if(coldTimes<HLInterfaceAndroid::getInstance()->ctrl_vungle_pop_time){
        return false;
    }
    
    return isInterstitialLoaded();
}


void VungleWrapper::startAD(){
    JniMethodInfo minfo;
    
    bool isHave = JniHelper::getStaticMethodInfo(minfo,
                                                 "org.cocos2dx.cpp.VunglePlugin","startAD","(Ljava/lang/String;)V");
    
    jstring vungleID = minfo.env->NewStringUTF(UserDefault::getInstance()->getStringForKey("vungle_code").c_str());

    if(!isHave){
        //CCLog("jni:openURL 函数不存在");
    }else{
        minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID,vungleID);
    }
    
　　 minfo.env->DeleteLocalRef(vungleID);
}

void VungleWrapper::showInterstitial(){
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,
			"org.cocos2dx.cpp.VunglePlugin","showAD","()V");
	if(!isHave){
			//CCLog("jni:openURL 函数不存在");
	}else{
		minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID);
	}
    coldTimes = 0;
}


bool VungleWrapper::isInterstitialLoaded(){
    
    bool ready = false;
    
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,
				"org.cocos2dx.cpp.VunglePlugin","isReady","()Z");
	if(!isHave){
				//CCLog("jni:openURL 函数不存在");
	}else{
		ready = minfo.env->CallStaticBooleanMethod(minfo.classID,minfo.methodID);
	}
    
    return ready;
}

#endif
