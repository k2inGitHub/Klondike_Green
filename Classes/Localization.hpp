//
//  Localization.hpp
//  Klondike
//
//  Created by 宋扬 on 16/7/11.
//
//

#ifndef Localization_hpp
#define Localization_hpp

#include "cocos2d.h"
using namespace cocos2d;

#define LocalizedString(key) Localization::get(key)

static const char *kOnLanguageChangedNotification = "OnLanguageChanged";

class Localization {
        
    static void loadData();
public:
    
    static const std::map<std::string,std::string>& getLanguageDict();
    
    static void setSystemCurrentLanguage();
    
    //获取当前语言 未设置为空
    static std::string getLanguage();
    
    static void setLanguage(std::string language);
    
    static std::string get(std::string key);
};

#endif /* Localization_hpp */
