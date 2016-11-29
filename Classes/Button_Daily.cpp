//
//  Button_Daily.cpp
//  Klondike
//
//  Created by 宋扬 on 16/7/14.
//
//

#include "Button_Daily.hpp"

void Button_Daily::setTitleText(const std::string &text){

    Button::setTitleText(text);
    
    getTitleRenderer()->setPosition(Vec2(getContentSize()/2) + Vec2(0, 4));
}

void Button_Daily::setSelected(bool value){
    _selected = value;
    if (_selected) {
        onPressStateChangedToPressed();
    } else {
        onPressStateChangedToNormal();
    }
}

bool Button_Daily::isSelected(){
    return _selected;
}

void Button_Daily::onPressStateChangedToNormal(){
    if (_selected) {
        return ;
    }
    Button::onPressStateChangedToNormal();
}

Button_Daily::Button_Daily():_date(nullptr){
    
};

Button_Daily::~Button_Daily(){
    CC_SAFE_RELEASE_NULL(_date);
};

void Button_Daily::setEmpty(bool value){
    _empty = value;
    updateUI();
}

bool Button_Daily::isEmpty(){
    return _empty;
}

bool Button_Daily::isFuture(){
    return _future;
}

void Button_Daily::setFuture(bool value){
    _future = value;
    updateUI();
}


bool Button_Daily::isUnlock(){
    return _unlock;
}

void Button_Daily::setUnlock(bool value){
    _unlock = value;
    updateUI();
}

int Button_Daily::getDay(){
    return _day;
}

void Button_Daily::setDay(int value){
    _day = value;
    updateUI();
}

void Button_Daily::setGetToday(bool value){
    _isGetToday = value;
    updateUI();
}

bool Button_Daily::isGetToday(){
    return _isGetToday;
}

bool Button_Daily::init(){
    if (!Button::init("bg_green_daily.png")) {
        return false;
    }
    
    setScale9Enabled(true);
    setContentSize(Size(55, 55));
    setPressedActionEnabled(false);
    setTitleAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    
    setTitleFontName("Arial");
    setTitleFontSize(40);
    
    _day = 0;
    _unlock = false;
    _future = false;
    _empty = false;
    _selected = false;
    
    _crown = ImageView::create("king0.png");
    _crown->setScale(55.f/131.f);
    addChild(_crown);
    _crown->setPosition(Vec2(getContentSize()/2) + Vec2(0, 4));
    
    updateUI();
    
    return true;
}

void Button_Daily::reset(){
    
    _isGetToday = false;
    _day = 0;
    _unlock = false;
    _future = false;
    _empty = false;
    
    setSelected(false);
    updateUI();
}

void Button_Daily::updateUI(){
    setEnabled(true);
    if (_empty) {
        loadTextures("null.png", "null.png", "null.png");
        setTitleText("");
        _crown->setVisible(false);
        setEnabled(false);
    } else {
        
        if (_unlock) {
            //解锁
            loadTextures("bg_green_daily.png", "daily_orange.png", "daily_orange.png");
            setTitleText("");
            _crown->setVisible(true);
            
            if(_isGetToday){
                _crown->loadTexture("king0.png");
            } else {
                _crown->loadTexture("king.png");
            }
            
        } else {
            
            if (_future) {
                
                setTitleColor(Color3B::GRAY);
                loadTextures("null.png", "null.png", "null.png");
                setEnabled(false);
            } else {
            
                setTitleColor(Color3B::WHITE);
                loadTextures("bg_green_daily.png", "daily_green.png", "daily_green.png");
            }
            
            char str[64];
            sprintf(str, "%d", _day);
            setTitleText(str);
            _crown->setVisible(false);
        }
    }
}