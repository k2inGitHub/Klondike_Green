//
//  Localization.cpp
//  Klondike
//
//  Created by 宋扬 on 16/7/11.
//
//

#include "Localization.hpp"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
using namespace  rapidjson;
#include "iOSWrapper.hpp"


static std::string _language;

static Document doc;

const std::map<std::string,std::string>& Localization::getLanguageDict(){
    static std::map<std::string, std::string> lanDic;
    if (lanDic.empty()) {
        lanDic["en"] = "en";
        
        lanDic["zh-CN"] = "chs";
        lanDic["zh-CHS"] = "chs";
        lanDic["zh-Hans"] = "chs";
        
        lanDic["zh"] = "cht";
        lanDic["fr"] = "fr";
        
        lanDic["de"] = "de";
        lanDic["it"] = "it";
        lanDic["ko"] = "kr";
        lanDic["nl"] = "nl";
        
        lanDic["pt"] = "pt";
        lanDic["ru"] = "ru";
        lanDic["es"] = "es";
        lanDic["tr"] = "tr";
        
        lanDic["ja"] = "jp";
        lanDic["id"] = "in";
        lanDic["ar"] = "ar";
        lanDic["hi"] = "hi";
    }
    return lanDic;
}

void Localization::setSystemCurrentLanguage(){
    auto lanDic = getLanguageDict();
    
    auto lan = iOSWrapper::getCurrentLanguage();
    std::string target;
    for (auto it : lanDic) {
        if (lan.find(it.first) != std::string::npos) {
            target = it.second;
        }
    }
    if (target.empty()) {
        target = "en";
    }
    setLanguage(target);
    CCLOG("target = %s", target.c_str());
}

std::string Localization::getLanguage(){
    _language = UserDefault::getInstance()->getStringForKey("Language", "");
    return _language;
}
void Localization::setLanguage(std::string language){

    if (_language != language) {
        _language = language;
        UserDefault::getInstance()->setStringForKey("Language", _language);
        __NotificationCenter::getInstance()->postNotification(kOnLanguageChangedNotification);
    }
}

void Localization::loadData(){
    std::string localPath = FileUtils::getInstance()->fullPathForFilename("localization.json");
    std::string jsonStr = FileUtils::getInstance()->getStringFromFile(localPath);
    doc.Parse(jsonStr.c_str());
    if (doc.IsArray()) {
    }
}

//KT SYN2
std::string Localization::get(std::string key){
    
    if (key.empty()) {
        return "";
    }
    if (doc.IsNull()) {
        loadData();
        CCASSERT(!doc.IsNull(), "Localization.json不存在或者格式错误");
    }
    
    for (rapidjson::Value::ConstValueIterator itr = doc.Begin(); itr != doc.End(); ++itr)
    {
        rapidjson::Value::ConstMemberIterator memitr = itr->FindMember("id");
        if (memitr != itr->MemberEnd()) {
            std::string id = memitr->value.GetString();
            if (id == key) {
                std::string lan = getLanguage();
                rapidjson::Value::ConstMemberIterator findItr = itr->FindMember(lan.c_str());
                if (findItr != itr->MemberEnd()) {
                    std::string ret = findItr->value.GetString();
                    return ret;
                } else {
                    rapidjson::Value::ConstMemberIterator findItr = itr->FindMember("en");
                    std::string ret = findItr->value.GetString();
                    if (ret.empty()) {
                        CCLOG("key = %s language = %s 未配置", id.c_str(), lan.c_str());
                    } else {
                        return ret;
                    }

                }
            }
        }
    }

    CCLOG("key = %s 未配置", key.c_str());
    
    return key;
}
