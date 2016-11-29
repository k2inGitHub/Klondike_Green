//
//  Layer_NavigationBar.cpp
//  Klondike
//
//  Created by 宋扬 on 16/7/14.
//
//

#include "Layer_NavigationBar.hpp"
#include "KTLayer.h"


void Layer_NavigationBar::updateItems(){

    float leftX = 0;
    float rightX = _barSize.width;
    
    float padding = 20;
    float width = -padding;
    for(const auto &child : _leftItems)
        width += child->getContentSize().width * child->getScaleX() + padding;
    float offset = (_barSize.width - width)/2;
    float x = -width / 2.0f - offset;
    
    for(const auto &child : _leftItems) {
        child->setPosition(x + child->getContentSize().width * child->getScaleX() / 2.0f, 0);
        x += child->getContentSize().width * child->getScaleX() + padding;
        leftX = x + _barSize.width/2;
        
        
        CCLOG("left pos = %s", CCStringFromPoint(child->getPosition())->getCString());
    }
    
    {
        float padding = 20;
        float width = padding;
        for(const auto &child : _rightItems)
            width += child->getContentSize().width * child->getScaleX() + padding;
        float offset = (_barSize.width - width)/2;
        float x = width / 2.0f + offset;
        
        for(const auto &child : _rightItems) {
            child->setPosition(x - child->getContentSize().width * child->getScaleX() / 2.0f, 0);
            x -= child->getContentSize().width * child->getScaleX() + padding;
//            rightX = x;
            rightX = x + _barSize.width/2;
            
            CCLOG("right pos = %s", CCStringFromPoint(child->getPosition())->getCString());

        }
    }
    if (_titleLabel) {
        CCLOG("rightX = %f", rightX);
        CCLOG("leftX = %f", leftX);
        
//        CCLOG("title pos = %s", CCStringFromPoint(pos)->getCString());
        Vec2 pos = Vec2((rightX-leftX)*0.5 + leftX, _barSize.height/2);
        _titleLabel->setPosition(pos);
    }
}

void Layer_NavigationBar::setLeftBarItems(Vector<MenuItem *>& bartItems){

    for (auto item : _leftItems) {
        item->removeFromParent();
    }
    
    _leftItems = bartItems;
    
    updateItems();
    _itemDirty = true;
    
    if (_parent != nullptr) {
        createMenu();
    }
}

void Layer_NavigationBar::setRightBarItems(Vector<MenuItem *>& bartItems){
    for (auto item : _rightItems) {
        item->removeFromParent();
    }
   
    _rightItems = bartItems;
    
    updateItems();
    _itemDirty = true;
    
    if (_parent != nullptr) {
        createMenu();
    }
}

void Layer_NavigationBar::onEnter(){

    Layer::onEnter();
    
    _titleLabel->setString(_title);
    
    if (_itemDirty) {
        createMenu();
    }
    
    Button::create("");
}

void Layer_NavigationBar::touchEvent(Ref *pSender, Widget::TouchEventType type){
    
    CCLOG("nav button");
}

Layer_NavigationBar::Layer_NavigationBar():
_titleLabel(nullptr),
_itemDirty(false),
_menu(nullptr)
{

}

bool Layer_NavigationBar::init(std::string title, Vector<MenuItem *> lefttItems, Vector<MenuItem *> righttItems){
    if (!Layer::init()) {
        return false;
    }
    
    
    setTitle(title);
    setLeftBarItems(lefttItems);
    setRightBarItems(righttItems);
    
    auto size = VisibleRect::getVisibleRect().size;
    auto contentSize = Size(size.width, 88);
    Vec2 target = Vec2::ZERO;
    setPosition(VisibleRect::leftTop() + Vec2(0, -contentSize.height));
    setContentSize(contentSize);
    
    
    _barSize = contentSize;
    setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    
    
    auto bg = Sprite::create("_topbar.png");
    bg->setPosition(Vec2(contentSize/2) + target);
    bg->setScaleY(contentSize.height / 91.f);
    bg->setScaleX(contentSize.width/26.f);
    this->addChild(bg, -100);
    _bg = bg;

    createMenu();

    _titleLabel = Label::createWithSystemFont(_title, "Arial", 36);
    addChild(_titleLabel, 2);

    updateItems();
    
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(Layer_NavigationBar::onScreenSizeChanged), kScreenSizeChangedNotification, nullptr);
    
    return true;
}

void Layer_NavigationBar::onScreenSizeChanged(Ref *sender){

    auto size = VisibleRect::getVisibleRect().size;
    auto contentSize = Size(size.width, 88);
    setPosition(VisibleRect::leftTop() + Vec2(0, -contentSize.height));
    setContentSize(contentSize);
    _barSize = contentSize;
    
    _menu->setContentSize(_barSize);
    _menu->setPosition(_barSize/2);
    
    _bg->setScaleX(contentSize.width/26.f);
    _bg->setPosition(Vec2(contentSize/2));
    
    float leftX = 0;
    float rightX = _barSize.width;
    
    float padding = 20;
    float width = -padding;
    for(const auto &child : _leftItems)
        width += child->getContentSize().width * child->getScaleX() + padding;
    float offset = (_barSize.width - width)/2;
    float x = -width / 2.0f - offset;
    
    for(const auto &child : _leftItems) {
        child->setPosition(x + child->getContentSize().width * child->getScaleX() / 2.0f, 0);
        x += child->getContentSize().width * child->getScaleX() + padding;
        leftX = x + _barSize.width/2;
    }
    
    {
        float padding = 20;
        float width = padding;
        for(const auto &child : _rightItems)
            width += child->getContentSize().width * child->getScaleX() + padding;
        float offset = (_barSize.width - width)/2;
        float x = width / 2.0f + offset;
        
        for(const auto &child : _rightItems) {
            child->setPosition(x - child->getContentSize().width * child->getScaleX() / 2.0f, 0);
            x -= child->getContentSize().width * child->getScaleX() + padding;
            //            rightX = x;
            rightX = x + _barSize.width/2;
            
            CCLOG("right pos = %s", CCStringFromPoint(child->getPosition())->getCString());
            
        }
    }
    
    Vec2 pos = Vec2((rightX-leftX)*0.5 + leftX, _barSize.height/2);
    _titleLabel->setPosition(pos);
}

void Layer_NavigationBar::createMenu(){
    
    if (_menu) {
        _menu->removeFromParent();
    }
    
    Vector<MenuItem *> items = _leftItems;
    for(auto item : _rightItems) {
        items.pushBack(item);
    }
    _menu = Menu::createWithArray(items);
    _menu->setContentSize(_barSize);
    _menu->setPosition(Vec2(_barSize/2));
    addChild(_menu, 3);
    
    
    _itemDirty = false;
}

Layer_NavigationBar::~Layer_NavigationBar(){
    __NotificationCenter::getInstance()->removeObserver(this, kScreenSizeChangedNotification);
}

bool Layer_NavigationBar::init(){
    Vector<cocos2d::MenuItem *> v;
    return init("", v,  v);
}
