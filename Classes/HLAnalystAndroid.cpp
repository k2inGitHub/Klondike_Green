#include "HLInterfaceAndroid.hpp"
#include "HLAnalystAndroid.hpp"
#include "iOSWrapper.hpp"
#include "HLOnlineConfigAndroid.hpp"

using namespace cocos2d;
using namespace std;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#include <jni.h>
#include "platform/android/jni/JniHelper.h"

void HLAnalystAndroid::StartAnalyst(){
    
    if(HLInterfaceAndroid::getInstance()->firebase_Switch==1){
        HLAnalystAndroid::StartFirebase();
    }
    
    HLAnalystAndroid::StartAppsflyer();
    
    if(HLInterfaceAndroid::getInstance()->market_reviwed_status==1){
        HLOnlineConfigAndroid::getInstance()->startGet();
    }
}

void HLAnalystAndroid::StartFirebase(){
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo,
                                                 "org.cocos2dx.cpp.AnalystPlugin","startFirebase","()V");
    if(!isHave){
        //CCLog("jni:openURL 函数不存在");
    }else{
        minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID);
    }
}

void HLAnalystAndroid::StartAppsflyer(){
    JniMethodInfo minfo;
    
    bool isHave = JniHelper::getStaticMethodInfo(minfo,
                                                 "org.cocos2dx.cpp.AnalystPlugin","startAppsFlyer","(Ljava/lang/String;Ljava/lang/String;)V");
    
    jstring keyID = minfo.env->NewStringUTF(UserDefault::getInstance()->getStringForKey("flyerDevKey").c_str());
    jstring gcmID = minfo.env->NewStringUTF(UserDefault::getInstance()->getStringForKey("flyerAppID").c_str());
    
    if(!isHave){
        //CCLog("jni:openURL 函数不存在");
    }else{
        minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID,keyID,gcmID);
    }
    
    minfo.env->DeleteLocalRef(keyID);
    minfo.env->DeleteLocalRef(gcmID);
}


#endif