//
//  KTScene.cpp
//  HelloCpp
//
//  Created by Men on 13-9-27.
//
//

#include "KTLayer.h"
#include "Layer_NavigationBar.hpp"
#include "SolitaireKlondikeScene.h"
//#include "MIRTipLayer.h"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#endif

void KTLayer::playEffect(const char*file)
{
//    if (!data->getSoundOff()) {
//        SimpleAudioEngine::getInstance()->playEffect(file);
//    }
}

void KTLayer::playBackgroundMusic(const char* file)
{
//    if (!data->getSoundOff()) {
//        SimpleAudioEngine::getInstance()->playBackgroundMusic(file);
//    }
}

bool KTLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    _scale = IS_IPAD_HD ? 2.0 : 1.0;
    
    winSize = Director::getInstance()->getWinSize();
    
//    data = MIRDynamicData::getInstance();
    
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(KTLayer::onLanguageChanged), kOnLanguageChangedNotification, nullptr);
    
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(KTLayer::onScreenSizeChanged), kScreenSizeChangedNotification, nullptr);
    
    return true;
}

void KTLayer::onScreenSizeChanged(cocos2d::Ref *sender){
    winSize = Director::getInstance()->getWinSize();
    if (winSize.height > winSize.width) {
        HLAdManagerWrapper::showBanner();
    } else {
        HLAdManagerWrapper::hideBanner();
    }
    updateLayout();
}

void KTLayer::updateLayout(){
    
}

void KTLayer::onLanguageChanged(Ref *sender){
    updateLocalizedUI();
}

void KTLayer::updateLocalizedUI(){

    
}

void KTLayer::showTipLayer(std::string file, std::string content)
{
//    auto la = MIRTipLayer::create();
//    la->setFile(file);
//    la->setContent(content);
//    addChild(la, 1024);
}

KTLayer::KTLayer(): _navigationBar(nullptr){

}

KTLayer::~KTLayer(){
    __NotificationCenter::getInstance()->removeObserver(this, kOnLanguageChangedNotification);
    __NotificationCenter::getInstance()->removeObserver(this, kScreenSizeChangedNotification);
    CC_SAFE_RELEASE_NULL(_navigationBar);
}

void KTLayer::onEnter(){
    Layer::onEnter();
    if (_navigationBar && _navigationBar->getParent() == nullptr) {
        addChild(_navigationBar);
//        _navigationBar->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        
    }
    HLAdManagerWrapper::showBanner();
    onScreenSizeChanged(nullptr);
}

void KTLayer::onExit(){
    Layer::onExit();

    
    if (getParent() == getGameScene()) {
        HLAdManagerWrapper::hideBanner();
    }
    
    
}

Layer_NavigationBar * KTLayer::getNavigationBar(){
    if (_navigationBar == nullptr) {
        _navigationBar = Layer_NavigationBar::create();
        _navigationBar->retain();
//        addChild(_navigationBar);
    }
    return _navigationBar;
}

void KTLayer::setNavigationBarEnable(bool value){
    if (value) {
        getNavigationBar()->setVisible(true);
    } else {
        if (_navigationBar) {
            _navigationBar->setVisible(false);
        }
    }
}

void KTLayer::addNavigationBar(std::string title, Vector<MenuItem *> lefttItems, Vector<MenuItem *> righttItems){
    if (_navigationBar == nullptr) {
        _navigationBar = Layer_NavigationBar::create(title, lefttItems, righttItems);
        _navigationBar->retain();
        addChild(_navigationBar);
//        _navigationBar->setPosition(VisibleRect::leftTop() + Vec2(0, -100));
    }
}

void KTLayer::horizonAliament(Vector<Node *> nodes, float padding, float y, bool useCenter){

    float width = -padding;
    for(const auto &child : nodes){
        width += child->getContentSize().width * child->getScaleX() + padding;
    }
    float offset = useCenter ? 0 : (winSize.width)/2;
    float x = -width / 2.0f + offset;
    
    for(const auto &child : nodes) {
        child->setPosition(x + child->getContentSize().width * child->getScaleX() / 2.0f, y);
        x += child->getContentSize().width * child->getScaleX() + padding;
        //            leftX = x + designResolutionSize.width/2;
    }
}

void KTLayer::verticalAliament(Vector<Node *> nodes, float padding, Vec2 center){
    float width = -padding;
    for(const auto &child : nodes)
        width += child->getContentSize().height * child->getScaleY() + padding;
    float offset = center.y;
    float y = -width / 2.0f + offset;
    
    for(const auto &child : nodes) {
        child->setPosition(center.x, y + child->getContentSize().width * child->getScaleY() / 2.0f);
        y += child->getContentSize().height * child->getScaleY() + padding;
        //            leftX = x + designResolutionSize.width/2;
    }
}

bool KTLayer::isNavigationBarEnable(){
    return _navigationBar && _navigationBar->isVisible();
}


