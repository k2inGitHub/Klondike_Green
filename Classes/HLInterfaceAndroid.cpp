#include "HLInterfaceAndroid.hpp"
#include "HttpManager.hpp"
#include "KTUtils.h"
#include "iOSWrapper.hpp"
#include "HLAdManagerWrapper.hpp"
#include "HLInterfaceAndroid.hpp"
#include "HLAnalystAndroid.hpp"
#include "HLNotificationAndroid.hpp"

using namespace cocos2d;
using namespace std;

#define MainUrl "http://45.113.69.21/ao2/u3d.php"

static HLInterfaceAndroid * hlinterface_Instance = NULL;

HLInterfaceAndroid * HLInterfaceAndroid::getInstance(){
	if(hlinterface_Instance == NULL){
		hlinterface_Instance = new HLInterfaceAndroid();
	}
	return hlinterface_Instance;
}


void HLInterfaceAndroid::startGet(){
    
    std::unordered_map<std::string, std::string> content;
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

    content["app_bundle_id"] = iOSWrapper::getAppName();
    content["app_version"] = iOSWrapper::getVersionName();
    content["app_os"] = "2";
    
#else
    content["app_bundle_id"] = "com.skyeyemobi.solitaire";
    content["app_version"] = "1.1";
    content["app_os"] = "1";
    
#endif
    
    CCLOG("AppName:%s    AppVersion:%s", content["app_bundle_id"].c_str(),content["app_version"].c_str());
    
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
        LoadData();
        
    },[=](string error){
        LoadData();
    });
}

void HLInterfaceAndroid::LoadData(){
    
    ctrl_admob_banner_switch = atoi(UserDefault::getInstance()->getStringForKey("ctrl_admob_banner_switch").c_str());
    
    ctrl_admob_banner_id = (char*)UserDefault::getInstance()->getStringForKey("ctrl_admob_banner_id").c_str();
    
    ctrl_admob_pop_id = (char*)UserDefault::getInstance()->getStringForKey("ctrl_admob_pop_id").c_str();
    
    ctrl_fb_banner_switch = atoi(UserDefault::getInstance()->getStringForKey("ctrl_fb_banner_switch").c_str());
    
    ctrl_fb_banner_id = (char*)UserDefault::getInstance()->getStringForKey("ctrl_fb_banner_id").c_str();
    
    ctrl_fb_pop_id = (char*)UserDefault::getInstance()->getStringForKey("ctrl_fb_pop_id").c_str();
    
    ctrl_banner_switch = atoi(UserDefault::getInstance()->getStringForKey("ctrl_banner_switch").c_str());
    
    ctrl_banner_iphone_id = (char*)UserDefault::getInstance()->getStringForKey("ctrl_banner_iphone_id").c_str();
    
    ctrl_banner_ipad_id = (char*)UserDefault::getInstance()->getStringForKey("ctrl_banner_ipad_id").c_str();
    
    unityad_code = (char*)UserDefault::getInstance()->getStringForKey("unityad_code").c_str();
    
    vungle_code = (char*)UserDefault::getInstance()->getStringForKey("vungle_code").c_str();
    
    ctrl_pop_switch = atoi(UserDefault::getInstance()->getStringForKey("ctrl_pop_switch").c_str());
    
    ctrl_admob_pop_switch = atoi(UserDefault::getInstance()->getStringForKey("ctrl_admob_pop_switch").c_str());
    
    ctrl_fb_pop_switch = atoi(UserDefault::getInstance()->getStringForKey("ctrl_fb_pop_switch").c_str());
    
    ctrl_unityad_pop_switch = atoi(UserDefault::getInstance()->getStringForKey("ctrl_unityad_pop_switch").c_str());
    
    ctrl_mango_pop_switch = atoi(UserDefault::getInstance()->getStringForKey("ctrl_mango_pop_switch").c_str());
    
    ctrl_vungle_pop_switch = atoi(UserDefault::getInstance()->getStringForKey("ctrl_vungle_pop_switch").c_str());
    
    ctrl_unsafe_admob_pop_switch = atoi(UserDefault::getInstance()->getStringForKey("ctrl_unsafe_admob_pop_switch").c_str());
    
    ctrl_unsafe_fb_pop_switch = atoi(UserDefault::getInstance()->getStringForKey("ctrl_unsafe_fb_pop_switch").c_str());
    
    
    ctrl_unsafe_unityad_pop_switch = atoi(UserDefault::getInstance()->getStringForKey("ctrl_unsafe_unityad_pop_switch").c_str());
    
    ctrl_unsafe_mango_pop_switch = atoi(UserDefault::getInstance()->getStringForKey("ctrl_unsafe_mango_pop_switch").c_str());
    
    ctrl_unsafe_vungle_pop_switch = atoi(UserDefault::getInstance()->getStringForKey("ctrl_unsafe_vungle_pop_switch").c_str());
    
    ctrl_admob_pop_time = atoi(UserDefault::getInstance()->getStringForKey("ctrl_admob_pop_time").c_str());
    
    ctrl_fb_pop_time = atoi(UserDefault::getInstance()->getStringForKey("ctrl_fb_pop_time").c_str());
    
    ctrl_unityad_pop_time = atoi(UserDefault::getInstance()->getStringForKey("ctrl_unityad_pop_time").c_str());
    
    ctrl_mango_pop_time = atoi(UserDefault::getInstance()->getStringForKey("ctrl_mango_pop_time").c_str());
    
    ctrl_vungle_pop_time = atoi(UserDefault::getInstance()->getStringForKey("ctrl_vungle_pop_time").c_str());
    
    encouraged_ad_strategy = atoi(UserDefault::getInstance()->getStringForKey("encouraged_ad_strategy").c_str());
    
    encouraged_ad_strategy_unityad_switch = atoi(UserDefault::getInstance()->getStringForKey("encouraged_ad_strategy_unityad_switch").c_str());
    
    encouraged_ad_strategy_vungle_switch = atoi(UserDefault::getInstance()->getStringForKey("encouraged_ad_strategy_vungle_switch").c_str());
    
    umeng_code = (char*)UserDefault::getInstance()->getStringForKey("umeng_code").c_str();
    
    umeng_Channel = (char*)UserDefault::getInstance()->getStringForKey("umeng_Channel").c_str();
    
    flurry_key = (char*)UserDefault::getInstance()->getStringForKey("flurry_key").c_str();
    
    flyerDevKey = (char*)UserDefault::getInstance()->getStringForKey("flyerDevKey").c_str();
    
    flyerAppID = (char*)UserDefault::getInstance()->getStringForKey("flyerAppID").c_str();
    
    firebase_Switch = atoi(UserDefault::getInstance()->getStringForKey("firebase_Switch").c_str());
    
    
    
    market_reviwed_status = atoi(UserDefault::getInstance()->getStringForKey("market_reviwed_status").c_str());
    
    comment_ctrl_switch = atoi(UserDefault::getInstance()->getStringForKey("comment_ctrl_switch").c_str());
    
    comment_content = (char*)UserDefault::getInstance()->getStringForKey("comment_content").c_str();
    
    comment_btnok = (char*)UserDefault::getInstance()->getStringForKey("comment_btnok").c_str();
    
    comment_btncancel = (char*)UserDefault::getInstance()->getStringForKey("comment_btncancel").c_str();

    comment_download_link = (char*)UserDefault::getInstance()->getStringForKey("comment_download_link").c_str();
    
    itunes_update_ctrl_switch = atoi(UserDefault::getInstance()->getStringForKey("itunes_update_ctrl_switch").c_str());
    
    itunes_update_content = (char*)UserDefault::getInstance()->getStringForKey("itunes_update_content").c_str();
    
    itunes_update_btnok = (char*)UserDefault::getInstance()->getStringForKey("itunes_update_btnok").c_str();

    itunes_update_btncancel = (char*)UserDefault::getInstance()->getStringForKey("itunes_update_btncancel").c_str();
    
    itunes_updated_url = (char*)UserDefault::getInstance()->getStringForKey("itunes_updated_url").c_str();
    
    ctrl_internal_01 = atoi(UserDefault::getInstance()->getStringForKey("ctrl_internal_01").c_str());
    
    ctrl_internal_02 = atoi(UserDefault::getInstance()->getStringForKey("ctrl_internal_02").c_str());
    
    ctrl_internal_03 = atoi(UserDefault::getInstance()->getStringForKey("ctrl_internal_03").c_str());
    
    ctrl_internal_04 = atoi(UserDefault::getInstance()->getStringForKey("ctrl_internal_04").c_str());
    
    ctrl_internal_05 = atoi(UserDefault::getInstance()->getStringForKey("ctrl_internal_05").c_str());
    
    girl_start = atoi(UserDefault::getInstance()->getStringForKey("girl_start").c_str());
    
    ctrl_unlock_img_switch = atoi(UserDefault::getInstance()->getStringForKey("ctrl_unlock_img_switch").c_str());
    
    ctrl_left_banner_switch = atoi(UserDefault::getInstance()->getStringForKey("ctrl_left_banner_switch").c_str());
    
    ctrl_left_pop_switch = atoi(UserDefault::getInstance()->getStringForKey("ctrl_left_pop_switch").c_str());
    
    ctrl_unsafe_left_pop_switch = atoi(UserDefault::getInstance()->getStringForKey("ctrl_unsafe_left_pop_switch").c_str());
    
    girl_img_url = (char*)UserDefault::getInstance()->getStringForKey("girl_img_url").c_str();
    
    ctrl_left_banner_id = (char*)UserDefault::getInstance()->getStringForKey("ctrl_left_banner_id").c_str();
    
    ctrl_left_pop_id = (char*)UserDefault::getInstance()->getStringForKey("ctrl_left_pop_id").c_str();
    
    ctrl_fixed_pop_id = (char*)UserDefault::getInstance()->getStringForKey("ctrl_fixed_pop_id").c_str();
    
    ctrl_btn_img_url_01 = (char*)UserDefault::getInstance()->getStringForKey("ctrl_btn_img_url_01").c_str();
    
    ctrl_btn_img_url_02 = (char*)UserDefault::getInstance()->getStringForKey("ctrl_btn_img_url_02").c_str();
    
    ctrl_btn_img_url_03 = (char*)UserDefault::getInstance()->getStringForKey("ctrl_btn_img_url_03").c_str();
    
    ctrl_btn_img_url_04 = (char*)UserDefault::getInstance()->getStringForKey("ctrl_btn_img_url_04").c_str();

    loading_left_pop_switch = atoi(UserDefault::getInstance()->getStringForKey("loading_left_pop_switch").c_str());
    
    loading_admob_pop_switch = atoi(UserDefault::getInstance()->getStringForKey("loading_admob_pop_switch").c_str());
    
    loading_fb_pop_switch = atoi(UserDefault::getInstance()->getStringForKey("loading_fb_pop_switch").c_str());
    
    loading_mango_pop_switch = atoi(UserDefault::getInstance()->getStringForKey("loading_mango_pop_switch").c_str());
    
    button_left_pop_switch = atoi(UserDefault::getInstance()->getStringForKey("button_left_pop_switch").c_str());
    
    button_unityad_pop_switch = atoi(UserDefault::getInstance()->getStringForKey("button_unityad_pop_switch").c_str());
    
    button_vungle_pop_switch = atoi(UserDefault::getInstance()->getStringForKey("button_vungle_pop_switch").c_str());
    
    ctrl_unlock_video_switch = atoi(UserDefault::getInstance()->getStringForKey("ctrl_unlock_video_switch").c_str());
    
    ctrl_unlock_default_video_switch = atoi(UserDefault::getInstance()->getStringForKey("ctrl_unlock_default_video_switch").c_str());
    
    girl_video_url = (char*)UserDefault::getInstance()->getStringForKey("girl_video_url").c_str();
    
    admob_pop_level = atoi(UserDefault::getInstance()->getStringForKey("admob_pop_level").c_str());;
    unityad_pop_level = atoi(UserDefault::getInstance()->getStringForKey("unityad_pop_level").c_str());;
    vungle_pop_level = atoi(UserDefault::getInstance()->getStringForKey("vungle_pop_level").c_str());;
    fb_pop_level = atoi(UserDefault::getInstance()->getStringForKey("fb_pop_level").c_str());;
    left_pop_level = atoi(UserDefault::getInstance()->getStringForKey("left_pop_level").c_str());;
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

    HLAdManagerWrapper::startAD();
    HLAnalystAndroid::StartAnalyst();
    HLNotificationAndroid::getInstance()->startGet();
    
#endif
}
