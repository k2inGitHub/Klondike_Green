//
//  Button_Highlight.cpp
//  Klondike
//
//  Created by 宋扬 on 16/7/26.
//
//

#include "Button_Highlight.hpp"

Button_Highlight::Button_Highlight():
_customButton(nullptr),
_badge(nullptr){

}

void Button_Highlight::lazyInitBadge(){
    if (_badge == nullptr) {
        _badge = ImageView::create("redpoint.png");
        _badge->setPosition(Vec2(getContentSize().width - 20, getContentSize().height - 20));
        this->addChild(_badge, 64);
    }
}

void Button_Highlight::setBadgeType(BadgeType badgeType){
    lazyInitBadge();
    if (badgeType == BadgeType::Simple) {
        _badge->loadTexture("redpoint.png");
    } else if (badgeType == BadgeType::New) {
        _badge->loadTexture("new.png");
    }
}

void Button_Highlight::setBadgeEnabled(bool value){
    if(value){
        lazyInitBadge();
        _badge->setVisible(true);
    } else {
        if(_badge != nullptr){
            _badge->setVisible(false);
        }
    }
}

void Button_Highlight::setBackgroundOutlineWidth(float value){

    _bgOutlineWidth = value;
    _icon->setContentSize(getContentSize() - Size(20 + _bgOutlineWidth * 2, 20 + _bgOutlineWidth * 2));

}

void Button_Highlight::setSelected(bool value){
    
    if (value) {
        _selectIcon->setVisible(true);
        _bgIcon->setColor(Color3B::BLACK);
        _bgIcon->setOpacity(225);
    } else {
        _selectIcon->setVisible(false);
        _bgIcon->setColor(Color3B::BLACK);
        _bgIcon->setOpacity(100);
    }
}

bool Button_Highlight::init(const Size& contentSize){
    if (!Layout::init()) {
        return false;
    }

    _bgOutlineWidth = 8;
    
    setTouchEnabled(true);
    addTouchEventListener([=](Ref*,Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            if (onClick) {
                onClick(this);
            }
        }
    });
    setContentSize(contentSize);
    
    _selectIcon = ImageView::create("select_icon.png");
    _selectIcon->setScale9Enabled(true);
    _selectIcon->setContentSize(contentSize);
    _selectIcon->setPosition(contentSize/2);
    addChild(_selectIcon, 1);
    
    _bgIcon = ImageView::create("9_white_s.png");
    _bgIcon->setScale9Enabled(true);
    _bgIcon->setContentSize(contentSize - Size(20, 20));
    _bgIcon->setPosition(contentSize/2);
    addChild(_bgIcon, 5);
    
    _icon = ImageView::create();
    _icon->setScale9Enabled(true);
    _icon->setContentSize(contentSize - Size(20 + _bgOutlineWidth * 2, 20 + _bgOutlineWidth * 2));
    _icon->setPosition(contentSize/2);
    addChild(_icon, 10);
    
    
    _addButton = ImageView::create("B_high_add.png");
    _addButton->setScale9Enabled(true);
//    _addButton->setContentSize(Size(20, 20));
    _addButton->setPosition(Size(contentSize.width-30, 30));
    addChild(_addButton, 15);
    _addButton->setVisible(false);
    
    setSelected(false);
    
    return true;
}
