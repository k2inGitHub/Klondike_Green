//
//  HLAnalsytWrapper-android.cpp
//  Klondike
//
//  Created by 宋扬 on 16/8/8.
//
//

#include "HLAnalsytWrapper.hpp"


using namespace cocos2d;
using namespace std;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

std::string HLAnalsytWrapper::stringValue(const std::string& key, const std::string& defaultValue){
    
    return UserDefault::getInstance()->getStringForKey(key.c_str(),defaultValue);
    
}


bool HLAnalsytWrapper::boolValue(const std::string& key, const bool& defaultValue){
    
    std::string s = UserDefault::getInstance()->getStringForKey(key.c_str());
    if(s.empty()){
        return defaultValue;
    }
    
    int value = atoi(UserDefault::getInstance()->getStringForKey(key.c_str()).c_str());
    
    return defaultValue;
}

float HLAnalsytWrapper::floatValue(const std::string& key, const float& defaultValue){
    std::string s = UserDefault::getInstance()->getStringForKey(key.c_str());
    if(s.empty()){
        return defaultValue;
    }
    
    float value = atof(UserDefault::getInstance()->getStringForKey(key.c_str()).c_str());
    return value;
}
int HLAnalsytWrapper::intValue(const std::string& key, const int& defaultValue){
    
    std::string s = UserDefault::getInstance()->getStringForKey(key.c_str());
    if(s.empty()){
        return defaultValue;
    }
    
    int value = atoi(UserDefault::getInstance()->getStringForKey(key.c_str()).c_str());
    return value;
}
void HLAnalsytWrapper::event(const std::string& event){

    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo,
                                                 "org.cocos2dx.cpp.AnalystPlugin","event","(Ljava/lang/String;)V");
    if(!isHave){
        //CCLog("jni:openURL 函数不存在");
    }else{
        jstring p1 = minfo.env->NewStringUTF(event.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID, p1);
    }
}

void HLAnalsytWrapper::event(const std::string& event, const std::string& value){
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo,
                                                 "org.cocos2dx.cpp.AnalystPlugin","event","(Ljava/lang/String;Ljava/lang/String;)V");
    if(!isHave){
        //CCLog("jni:openURL 函数不存在");
    }else{
        jstring p1 = minfo.env->NewStringUTF(event.c_str());
        jstring p2 = minfo.env->NewStringUTF(value.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID, p1, p2);
    }
}

#endif
