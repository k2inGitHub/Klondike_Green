//
//  Item_Setting.hpp
//  Klondike
//
//  Created by 宋扬 on 16/11/30.
//
//

#ifndef Item_Setting_hpp
#define Item_Setting_hpp

#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

class NameItem : public Ref {
public:
    CC_SYNTHESIZE_PASS_BY_REF(string, _name, Name);
};

class PreviewItem : public NameItem{
public:
    //预览文件
    CC_SYNTHESIZE_PASS_BY_REF(string, _previewFile, PreviewFile);
    //显示提示徽章
    CC_SYNTHESIZE(bool, _isNew, IsNew);
};

//tag1
class FaceItem : public PreviewItem {
public:
    CC_SYNTHESIZE_PASS_BY_REF(string, _filePrefix, FilePrefix);
    string getFile(int suit, int num){
        return StringUtils::format("%s/%d/%d.png", _filePrefix.c_str(), suit, num);
    };
};

class CardbackItem : public PreviewItem {
public:
    CC_SYNTHESIZE_PASS_BY_REF(string, _file, File);
};

class ThemeItem : public PreviewItem{
    
public:
    CC_SYNTHESIZE_PASS_BY_REF(string, _bgFile, BgFile);
};

//tag1
class ThemeSetItem : public PreviewItem {
private:
    string _faceName;
    string _cardbackName;
    string _themeName;
public:
    void setFaceName(string var){
        _faceName = var;
    }
    string getFaceName();
    
    void setCardbackName(string var){
        _cardbackName = var;
    }
    string getCardbackName();
    
    void setThemeName(string var){
        _themeName = var;
    }
    string getThemeName();
};


#endif /* Item_Setting_hpp */
