#include "HLNotificationAndroid.hpp"
#include "HttpManager.hpp"
#include "KTUtils.h"
#include "iOSWrapper.hpp"
#include "HLAdManagerWrapper.hpp"
#include "HLInterfaceAndroid.hpp"
#include "HLAnalystAndroid.hpp"
#include "KTLayer.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#include <jni.h>
#include "platform/android/jni/JniHelper.h"

using namespace cocos2d;
using namespace std;

#define MainUrl "http://45.113.69.21/ao2/u3d_pc.php"


static HLNotificationAndroid * hlnoti_Instance = NULL;

HLNotificationAndroid * HLNotificationAndroid::getInstance(){
	if(hlnoti_Instance == NULL){
		hlnoti_Instance = new HLNotificationAndroid();
	}
	return hlnoti_Instance;
}


void HLNotificationAndroid::startGet(){
    
    std::unordered_map<std::string, std::string> content;
    content["app_bundle_id"] = iOSWrapper::getAppName();
    content["app_version"] = iOSWrapper::getVersionName();
    content["language"] = Localization::getLanguage();
    content["app_os"] = "2";
    
    
    std::unordered_map<std::string, std::string> param;
    param["content"] = KTUtils::jsonString(content);
    
    HttpManager::create()->sendGET(MainUrl, param, [=](const rapidjson::Document &json){
        
        int status = json["status"].GetInt();
        
        int weekID = 0;
        if (status == 0) {
            const rapidjson::Value& data = json["data"]["list"];
            for (rapidjson::Value::ConstValueIterator itr = data.Begin(); itr != data.End(); ++itr) {
                string msg_content = Localization::get(itr->FindMember("msg_content")->value.GetString());
                int msg_id = atoi(itr->FindMember("msg_id")->value.GetString());
                string msg_sending_time = Localization::get(itr->FindMember("msg_sending_time")->value.GetString());
    
                addNotification(msg_content.c_str(),msg_sending_time.c_str(),weekID,msg_id);
                
                weekID++;
            }
        }
    },[=](string error){

    });
}

void HLNotificationAndroid::addNotification(const char* msg_content, const char* msg_sending_time, int weekID, int msg_id){
    JniMethodInfo minfo;
    
    bool isHave = JniHelper::getStaticMethodInfo(minfo,
                                                 "org.cocos2dx.cpp.Notifications","addTimeNoticfy","(Ljava/lang/String;Ljava/lang/String;II)V");
    
    jstring content = minfo.env->NewStringUTF(msg_content);
    jstring sendingTime = minfo.env->NewStringUTF(msg_sending_time);
    jint jweekID = weekID;
    jint jmsg_id = msg_id;
    
    if(!isHave){
        //CCLog("jni:openURL 函数不存在");
    }else{
        minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID,content,sendingTime,jweekID,jmsg_id);
    }
    
    　　 minfo.env->DeleteLocalRef(content);
    　　 minfo.env->DeleteLocalRef(sendingTime);
}

#endif
