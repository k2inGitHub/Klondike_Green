//
//  Item_Setting.cpp
//  Klondike
//
//  Created by 宋扬 on 16/11/30.
//
//

#include "Item_Setting.hpp"
#include "DataManager.h"
using namespace NSDataManage;

string ThemeSetItem::getFaceName(){
    
    if(_name == "theme_set_1") {
        return DataManager::getInstance()->getCustomSelectFaceName();
    }
    return _faceName;
}
//debug3
string ThemeSetItem::getCardbackName(){
    
    if(_name == "theme_set_1") {
        string cardbackName = DataManager::getInstance()->getCustomSelectCardbackName();
        return DataManager::getInstance()->getCustomSelectCardbackName();
    }
    return _cardbackName;
}

string ThemeSetItem::getThemeName(){
    
    if(_name == "theme_set_1") {
        return DataManager::getInstance()->getCustomSelectThemeName();
    }
    return _themeName;
}
