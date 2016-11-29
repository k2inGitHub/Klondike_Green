#include "HLOnlineConfigAndroid.hpp"
#include "HttpManager.hpp"
#include "KTUtils.h"
#include "iOSWrapper.hpp"
#include "HLAdManagerWrapper.hpp"
#include "HLInterfaceAndroid.hpp"
#include "HLAnalystAndroid.hpp"

using namespace cocos2d;
using namespace std;

#define MainUrl "http://45.113.69.21/ao2/online_config.php"

static HLOnlineConfigAndroid * hlonline_Instance = NULL;

HLOnlineConfigAndroid * HLOnlineConfigAndroid::getInstance(){
	if(hlonline_Instance == NULL){
		hlonline_Instance = new HLOnlineConfigAndroid();
	}
	return hlonline_Instance;
}


void HLOnlineConfigAndroid::startGet(){
    
    std::unordered_map<std::string, std::string> content;
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

    content["app_bundle_id"] = iOSWrapper::getAppName();
    content["app_version"] = iOSWrapper::getVersionName();
    content["app_os"] = "2";
    
#endif
    
    std::unordered_map<std::string, std::string> param;
    param["content"] = KTUtils::jsonString(content);
    
    HttpManager::create()->sendGET(MainUrl, param, [=](const rapidjson::Document &json){
        
        const rapidjson::Value& data = json;
        
        for(auto iter = data.MemberBegin(); iter != data.MemberEnd(); ++iter)
        {
            auto key = (iter->name).GetString();
            if(json[key].IsString()){
                UserDefault::getInstance()->setStringForKey(key, json[key].GetString());
                CCLOG("%s:%s",key,json[key].GetString());
            }
        }
        UserDefault::getInstance()->flush();
    },[=](string error){

    });
}
