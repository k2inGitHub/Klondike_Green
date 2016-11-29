//
//  Cell_Setting.cpp
//  Klondike
//
//  Created by 宋扬 on 16/7/25.
//
//

#include "Cell_Setting.hpp"
#include "KTLayer.h"

#pragma mark - Cell_Base -

void Cell_Base::onEnter(){
    Layout::onEnter();
    onScreenSizeChanged(nullptr);
}

Widget* Cell_Base::createCloneInstance()
{
    return Cell_Base::create();
}

void Cell_Base::copyClonedWidgetChildren(Widget* model)
{
}

bool Cell_Base::init(){
    if (!Layout::init()) {
        return false;
    }
    
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(Cell_Base::onScreenSizeChanged), kScreenSizeChangedNotification, nullptr);
    
    
    return true;
}

void Cell_Base::onScreenSizeChanged(Ref *sender){
    updateLayout();
}

void Cell_Base::updateLayout(){
    
}

Cell_Base::~Cell_Base(){
    __NotificationCenter::getInstance()->removeObserver(this, kScreenSizeChangedNotification);
}

#pragma mark - Cell_CardBack -

Widget* Cell_CardBack::createCloneInstance()
{
    return Cell_CardBack::create();
}

void Cell_CardBack::updateLayout()
{
    auto winSize = Director::getInstance()->getVisibleSize();
    auto cellSize = Size(winSize.width, 160);
    setContentSize(cellSize);
    _box->setPosition((cellSize)/2);
}


bool Cell_CardBack::init(){
    
    if (!Cell_Base::init()) {
        return false;
    }
    
    auto cellSize = Size(640, 160);
    setContentSize(cellSize);
    
    auto s = Size(88 + 36, 131 + 36);
    
    auto box = HBox::create(Size(s.width * 4, s.height));
    box->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    box->setPosition((cellSize)/2);
    addChild(box);
    _box = box;
    
    for (int i = 0; i < 4; i++) {
        auto _iconView = Button_Highlight::create(s);
        box->addChild(_iconView);
        _iconView->setBackgroundOutlineWidth(4);
        _iconViews.pushBack(_iconView);
    }
    
    return true;
}

#pragma mark - Cell_Faces -

Widget* Cell_Faces::createCloneInstance()
{
    return Cell_Faces::create();
}

void Cell_Faces::updateLayout()
{
    auto winSize = Director::getInstance()->getVisibleSize();
    auto cellSize = Size(winSize.width, 160);
    setContentSize(cellSize);
    auto s = Size(476, 168);
    _iconView->setPosition((cellSize - s)/2);
}

bool Cell_Faces::init(){
    
    if (!Cell_Base::init()) {
        return false;
    }
    
    auto cellSize = Size(640, 160);
    setContentSize(cellSize);
    
    auto s = Size(476, 168);
    _iconView = Button_Highlight::create(s);
    _iconView->setPosition((cellSize - s)/2);
    addChild(_iconView);
    
    
    return true;
}

#pragma mark - Cell_Setting -

Cell_Setting::Cell_Setting():
_badge(nullptr){

}

void Cell_Setting::setBadgeEnabled(bool value){

    if(value){
        if (_badge == nullptr) {
            _badge = Sprite::create("redpoint.png");
            _badge->setPosition(Vec2(40, 35));
            this->addChild(_badge);
        }
        _badge->setVisible(true);
    } else {
        if(_badge != nullptr){
            _badge->setVisible(false);
        }
    }
}

void Cell_Setting::setForceSwitchOff(bool value){
    _forceSwitchOff = value;
    if (_forceSwitchOff) {
        _lock->setVisible(true);
        _sw->setEnabled(false);
        _sw->getRendererBackgroundDisabled()->setColor(Color3B::GRAY);
        _sw->getRendererFrontCrossDisabled()->setColor(Color3B::GRAY);
    } else {
        _lock->setVisible(false);
        _sw->setEnabled(true);
        _sw->getRendererBackgroundDisabled()->setColor(Color3B::WHITE);
        _sw->getRendererFrontCrossDisabled()->setColor(Color3B::WHITE);
    }
}

void Cell_Setting::setSwitchSelected(bool value){
    if (!_forceSwitchOff) {
        _sw->setSelected(value);
    }
}
bool Cell_Setting::isSwitchSelected(){
    return _sw->isSelected();
}

Widget* Cell_Setting::createCloneInstance()
{
    return Cell_Setting::create();
}

void Cell_Setting::updateLayout()
{
    
}

bool Cell_Setting::init()
{
    if (!Cell_Base::init()) {
        return false;
    }
    Size size = Size(576, 70);
    setContentSize(size);
    {
        setTouchEnabled(true);
        _bg = ImageView::create("9_line_white.png");
        _bg->setContentSize(size);
        _bg->setScale9Enabled(true);
        _bg->setColor(Color3B::BLACK);
        _bg->setOpacity(100);
        _bg->setPosition(_bg->getContentSize()/2);
        addChild(_bg);
        
        auto titleLabel = Text::create("", "Arial", 24);
        
        //        titleLabel->setAlignment(TextHAlignment::LEFT);
        titleLabel->setContentSize(Size(180, 45));
        titleLabel->setTextHorizontalAlignment(TextHAlignment::LEFT);
        titleLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        titleLabel->setPosition(Vec2(50, 35));
        this->addChild(titleLabel);
        _titleLabel = titleLabel;
        
        
        _sw = CheckBox::create("B_open0.png", "B_open0.png","B_open.png","B_open0.png","B_open0.png");
        _sw->setPosition(Vec2(500, 35));
        _sw->addEventListener([=](Ref*,CheckBox::EventType type){
        
            if (_onSwitch) {
                _onSwitch(this);
            }
        });
        this->addChild(_sw);
        
        _arrow = ImageView::create("B_arrow.png");
        _arrow->setPosition(Vec2(500, 35));
        _arrow->setFlippedX(true);
        addChild(_arrow);
        
        _lock = ImageView::create("lock.png");
        _lock->setPosition(Vec2(420, 35));
        addChild(_lock);
        _lock->setVisible(false);
    }
    
    
    return true;
}
