#ifndef  _BOTTOMMENU_H_
#define  _BOTTOMMENU_H_

#include "KTPauseLayer.h"
#include "cocos2d.h"

// 尺寸
static const int SIZE_TEXTURE_X = 20.f;
static const int SIZE_TEXTURE_Y = 122.0f;


typedef std::function<void(int tag)> OnItemClick;

class BottomMenu : public cocos2d::LayerColor, public KTPauseLayerDelegate {
    
    
    Sprite *_bg;
    
    bool _isHide;
public:
    
    void hide(bool isAnimate);
    
    void show(bool isAnimate);
    
    void onScreenSizeChanged(Ref *sender);
    
    void onLanguageChangedNotification(Ref *sender);
    
    void decode(ValueMap root){
    
        _detailLabel->setString(root["detailString"].asString());
        _detailLabel->setVisible(root["detailVisible"].asBool());
    }
    
    ValueMap encode(){
        ValueMap root;
        root["detailString"] = Value(_detailLabel->getString());
        root["detailVisible"] = Value(_detailLabel->isVisible());
        return root;
    }
    
    CC_SYNTHESIZE(Sprite*, _indicator, Indicator);
    
    CC_SYNTHESIZE(Node*, _detailContainer, DetailContainer);
    
    CC_SYNTHESIZE(Label*, _detailLabel, DetailLabel);
    
    OnItemClick onItemClick;
    
    void updateMenu();
    
    Menu *_menu;
    
    void setItemVisible(int idx, bool visible);
    
    void setItemBadge(int idx, bool value);
    
    void setConerItemBadge(int idx, bool value);
    
    void setConerItemVisible(int idx, bool value);
    
    Vector<MenuItem *>items;
    
    Vector<Button *>cornerBtns;
    
    BottomMenu();
    
    ~BottomMenu();

    virtual bool init();
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);    
    
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
    
    // 设置回调
    void menuSettingCallback(cocos2d::Ref* pSender);
    // 音效回调
    void menuMuteCallback(cocos2d::Ref* pSender);
    // gamecenter 回调
    void menuGameCenterCallback(cocos2d::Ref* pSender);
    
    void addTitle(Node *node, std::string title);
    
    CREATE_FUNC(BottomMenu);
private:
//    cocos2d::LabelTTF*  undo_label;
   // cocos2d::LabelTTF*  new_game_label;
    //cocos2d::MenuItemToggle*     mute_toogle;
    
    // 设置layer
//    SettingLayer*  m_setting_layer;
};

#endif
