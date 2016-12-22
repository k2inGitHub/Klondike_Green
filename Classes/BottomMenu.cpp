#include "BottomMenu.h"
#include "Audio.h"
#include "Global.h"
#include "iOSWrapper.hpp"
#include "Localization.hpp"
#include "Layer_Daily.hpp"

USING_NS_CC;
using namespace NSGlobal;

BottomMenu::BottomMenu():
_isHide(false){
//    m_setting_layer = NULL;
}

BottomMenu::~BottomMenu() {
    __NotificationCenter::getInstance()->removeObserver(this, kOnLanguageChangedNotification);
    __NotificationCenter::getInstance()->removeObserver(this, kScreenSizeChangedNotification);
}

void BottomMenu::addTitle(Node *node, std::string title){

    auto label = node->getChildByTag<Label *>(123);
    if (label == nullptr) {
        label = Label::createWithSystemFont(title, "Arial", 16, Size(88, 24),TextHAlignment::CENTER, TextVAlignment::TOP);
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        label->setTag(123);
        //    label->setAlignment(TextHAlignment::CENTER);
        //    node->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        label->setContentSize(Size(88, 24));
        label->setOpacity(175);
//        label->setDimensions(88, 24);
//        label->setHorizontalAlignment(TextHAlignment::CENTER);
//        label->setVerticalAlignment(TextVAlignment::CENTER);
        label->setOverflow(Label::Overflow::SHRINK);
        node->addChild(label);
        
        CCLOG("w = %f", node->getContentSize().width);
        CCLOG("title = %s", title.c_str());
        label->setPosition(node->getContentSize().width * 0.5, 8);
        items.pushBack((MenuItem *)node);
    } else {
        label->setString(title);
    }
}

void BottomMenu::hide(bool isAnimate){
    for (auto item : cornerBtns) {
        auto pos = Vec2(getContentSize().width + item->getContentSize().width/2, 83 + item->getContentSize().height/2);
        item->stopAllActions();
        auto moveTo = MoveTo::create(0.3, pos);
        item->runAction(moveTo);
    }
    _isHide = true;
}

void BottomMenu::show(bool isAnimate){
    _isHide = false;
    for (auto item : cornerBtns) {
        auto pos = Vec2(getContentSize().width - item->getContentSize().width/2, 83 + item->getContentSize().height/2);
        item->stopAllActions();
        auto moveTo = MoveTo::create(0.3, pos);
        item->runAction(moveTo);
    }
}

bool BottomMenu::init(){
    //////////////////////////////
    // 1. super init first
    if (!LayerColor::initWithColor(Color4B(0,0,0,0))) {
        return false;
    }
    
    // 逻辑大小
//    Size design_size = Size(640, 90.f);
    // 可视大小
    Size visible_size = Size(640 * ScaleFactor::scale_x, 90.f * ScaleFactor::scale_y);
    this->setContentSize(visible_size);
    

    // 背景图片
    auto bg = Sprite::create("bottombar.png");
    bg->setOpacity(175);
    this->addChild(bg, 0);
    _bg = bg;
    
//    float min_scale = fmin(ScaleFactor::scale_x, ScaleFactor::scale_y);
    
    // 设置
    auto setting = MenuItemImage::create( "btn_classic_setting1.png",
                                         "btn_classic_setting2.png",
                                         CC_CALLBACK_1(BottomMenu::menuSettingCallback, this));
    //setting->setScale(0.4f * min_scale, 0.4f * min_scale);
    setting->setPosition(Vec2(visible_size.width/4, visible_size.height/2));
    setting->setScale(1);
    addTitle(setting, LocalizedString("TID_UI_SETTINGS"));
    
    //主题
    auto daily = MenuItemImage::create("btn_classic_skin1.png","btn_classic_skin2.png",
                                       [=](Ref *sender){
                                           Audio::getInstance()->playButtonSFX();
                                           
                                           if (onItemClick != nullptr) {
                                               onItemClick(1);
                                           }
                                       });
    daily->setPosition(Vec2(visible_size.width * 3/4, visible_size.height/2));
    daily->setScale(1);
    addTitle(daily, LocalizedString("TID_UI_BACKGROUND_TITLE"));
    
    
    // 牌局
    auto new_game_label = MenuItemImage::create( "btn_classic_game1.png",
                                           "btn_classic_game2.png",
                                                [=](Ref *sender) {
                                                    Audio::getInstance()->playButtonSFX();
//                                                    iOSWrapper::showNewGameSheet();
                                                    
                                                    if (onItemClick != nullptr) {
                                                        onItemClick(2);
                                                    }
                                                    
                                                });
    //new_game_label->setScale(min_scale);
//    new_game_label->setAnchorPoint(Vec2(1,0.5));
    new_game_label->setPosition(Vec2(visible_size.width - 30, visible_size.height/2));
    new_game_label->setScale(1);
    addTitle(new_game_label, LocalizedString("TID_UI_GAME"));
//    this->addChild(new_game_label, 0);
    
   

    
    // 提示
    auto GameCenterIcon = MenuItemImage::create( "btn_classic_hint1.png","btn_classic_hint2.png",
                                         CC_CALLBACK_1(BottomMenu::menuGameCenterCallback, this));
    //GameCenterIcon->setScale(0.4f * min_scale, 0.4f * min_scale);
    GameCenterIcon->setPosition(Vec2(visible_size.width * 3/4, visible_size.height/2));
    GameCenterIcon->setScale(1);
    addTitle(GameCenterIcon, LocalizedString("TID_UI_HINT"));
    
    
    // 撤销
    auto undo_label = MenuItemImage::create( "btn_undo.png",
                                            "btn_undo1.png",
                                            [=](Ref *sender){
                                                Audio::getInstance()->playButtonSFX();
                                                __NotificationCenter::getInstance()->postNotification(NTFFactor::NTF_UNDO);
                                            });
    undo_label->setPosition(Vec2(30, visible_size.height/2));
    undo_label->setScale(1);
    addTitle(undo_label, LocalizedString("TID_UI_UNDO"));
    
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(setting, daily, new_game_label, GameCenterIcon,undo_label, NULL);
    
    this->addChild(menu, 1);
    _menu = menu;
    updateMenu();
    
    //corner btn
    
    Button *dailyBtn = KTFactory::createButton("btn_daily.png", "btn_daily1.png", "", "", 0);
    dailyBtn->addClickEventListener([=](Ref *sender){
        Audio::getInstance()->playButtonSFX();
        __NotificationCenter::getInstance()->postNotification(NTFFactor::NTF_Daily);

    });
    addChild(dailyBtn);
    dailyBtn->setPosition(Vec2(visible_size.width - dailyBtn->getContentSize().width/2, 83 + dailyBtn->getContentSize().height/2));
    cornerBtns.pushBack(dailyBtn);
    
//    auto daily = MenuItemImage::create("btn_daily.png","btn_daily1.png",
//                                       [=](Ref *sender){
//                                           Audio::getInstance()->playButtonSFX();
//                                           __NotificationCenter::getInstance()->postNotification(NTFFactor::NTF_Daily);
//
//                                       });
//    daily->setPosition(Vec2(visible_size.width * 3/4, visible_size.height/2));
//    daily->setScale(1);
//    addTitle(daily, LocalizedString("TID_UI_QUEST"));
    
    
    auto layer = ImageView::create("9_white_s.png");
    layer->setScale9Enabled(true);
    layer->setContentSize(Size(160, 40));
    layer->setColor(Color3B::BLACK);
    layer->setOpacity(127);
    layer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    //LayerColor::create(Color4B(0, 0, 0, 127), 160, 40);
    layer->setPosition(Vec2(0, 83));
    addChild(layer);
    _detailContainer = layer;
    
    _detailLabel = Label::createWithSystemFont("", "Arial", 24);
    _detailLabel->setContentSize(Size(160, 40));
    _detailLabel->setDimensions(160, 30);
    _detailLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    _detailLabel->setVerticalAlignment(TextVAlignment::CENTER);
    _detailLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _detailLabel->setOverflow(Label::Overflow::SHRINK);
    _detailLabel->setPosition(layer->getContentSize()/2);
    _detailLabel->setOpacity(175);
    layer->addChild(_detailLabel);
    
    _indicator = Sprite::create("down.png");
    
    addChild(_indicator);
    
    // 加入触摸监听
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    
    listener->onTouchBegan = CC_CALLBACK_2(BottomMenu::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(BottomMenu::onTouchEnded, this);
    
    // 优先级为100
    this->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 100);
    
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BottomMenu::onLanguageChangedNotification), kOnLanguageChangedNotification, nullptr);
    
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BottomMenu::onScreenSizeChanged), kScreenSizeChangedNotification, nullptr);

    onScreenSizeChanged (nullptr);
    
    return true;
}

void BottomMenu::onScreenSizeChanged(Ref *sender){
    
    auto size = Director::getInstance()->getVisibleSize();
    Size visible_size = Size(size.width, 90.f);
    float bg_scale_x = size.width/SIZE_TEXTURE_X;
    float bg_scale_y = 90.f/SIZE_TEXTURE_Y;
    _bg->setScale(bg_scale_x * ScaleFactor::scale_x, bg_scale_y * ScaleFactor::scale_y);
    _bg->setPosition(Vec2(visible_size.width/2, visible_size.height/2));
    
    _menu->setPosition(visible_size.width/2.f, visible_size.height/2.f - 8);
    this->setContentSize(visible_size);
    
    updateMenu();
    
    _indicator->setPosition(Vec2(size.width/2, 100));
    
    auto dailyBtn = cornerBtns.at(0);
    dailyBtn->setPosition(Vec2(visible_size.width + (_isHide ? 1 : -1)* dailyBtn->getContentSize().width/2, 83 + dailyBtn->getContentSize().height/2));
    dailyBtn->stopAllActions();
}

void BottomMenu::onLanguageChangedNotification(Ref *sender){
    string ids[] = {"TID_UI_SETTINGS", "TID_UI_BACKGROUND_TITLE", "TID_UI_GAME", "TID_UI_HINT", "TID_UI_UNDO"};
    
    
    for (int i = 0; i < items.size(); i++) {
        addTitle(items.at(i), LocalizedString(ids[i]));
    }
}

void BottomMenu::updateMenu(){
    auto size = Director::getInstance()->getVisibleSize();
    float width = 0;
    auto childern = _menu->getChildren();
    for(const auto &child : childern){
        width += child->getContentSize().width * child->getScaleX();
        auto lable = (Label *)child->getChildByTag(123);
        CCLOG("title = %s", lable->getString().c_str());
        CCLOG("z = %d", child->getLocalZOrder());
    }
    float padding = (size.width - 40 - width)/(_menu->getChildrenCount() - 1);
    _menu->alignItemsHorizontallyWithPadding(padding);
}

void BottomMenu::setConerItemBadge(int idx, bool value){
    auto target = cornerBtns.at(idx);
    auto badge = target->getChildByTag<Sprite *>(1024);
    if (value == true) {
        if (badge == nullptr) {
            badge = Sprite::create("redpoint.png");
            target->addChild(badge, 1024, 1024);
            badge->setPosition(Vec2(target->getContentSize()/2) + Vec2(22, 24));
        }
    } else {
        if (badge) {
            badge->removeFromParent();
        }
    }
}

void BottomMenu::setItemBadge(int idx, bool value){

    auto target = items.at(idx);
    auto badge = target->getChildByTag<Sprite *>(1024);
    if (value == true) {
        if (badge == nullptr) {
            badge = Sprite::create("redpoint.png");
            target->addChild(badge, 1024, 1024);
            badge->setPosition(Vec2(target->getContentSize()/2) + Vec2(22, 24));
        }
    } else {
        if (badge) {
            badge->removeFromParent();
        }
    }
}

void BottomMenu::setConerItemVisible(int idx, bool visible){
    auto target = cornerBtns.at(idx);
    if (visible) {
        if (target->getParent() == nullptr) {
            this->addChild(target);
//            target->setLocalZOrder(idx);
//            _menu->sortAllChildren();
//            updateMenu();
        }
    } else {
        if (target->getParent() != nullptr) {
            target->removeFromParent();
//            updateMenu();
        }
    }
}

void BottomMenu::setItemVisible(int idx,bool visible){
    
    auto target = items.at(idx);
    if (visible) {
        if (target->getParent() == nullptr) {
            _menu->addChild(target);
            target->setLocalZOrder(idx);
            _menu->sortAllChildren();
            updateMenu();
        }
    } else {
        if (target->getParent() != nullptr) {
            target->removeFromParent();
            updateMenu();
        }
    }
}

bool BottomMenu::onTouchBegan(Touch* touch, Event* event) {
//    Point location = touch->getLocation();
//    if(undo_label->getBoundingBox().containsPoint(location)){
//        //撤销,发送通知
//        NotificationCenter::getInstance()->postNotification(NTFFactor::NTF_UNDO);
//    }
//    else if(new_game_label->getBoundingBox().containsPoint(location)){
//        //新游戏,发送通知
//        NotificationCenter::getInstance()->postNotification(NTFFactor::NTF_NEW_GAME);
//    }
    return true;
}

void BottomMenu::onTouchEnded(Touch* touch, Event* event) {
    
}

void BottomMenu::menuSettingCallback(cocos2d::Ref* pSender){
    // 进入设置场景
//    auto scene = SettingLayer::createScene();
//    auto reScene = CCTransitionFlipX::create(0.3f, scene);
//    
//    auto director = Director::getInstance();
//    director->pushScene(reScene);
//    
//
    Audio::getInstance()->playButtonSFX();
//    iOSWrapper::toSettingController();
    
    if (onItemClick != nullptr) {
        onItemClick(0);
    }
}

void BottomMenu::menuMuteCallback(cocos2d::Ref* pSender){
//    int index = mute_toogle->getSelectedIndex();
//    
//    if(index == 1){
//        Audio::getInstance()->pauseEffects();
//    }else if(index == 0){
//        Audio::getInstance()->resumeEffects();
//    }
}

void BottomMenu::menuGameCenterCallback(cocos2d::Ref* pSender){
    Audio::getInstance()->playButtonSFX();
    __NotificationCenter::getInstance()->postNotification(NTFFactor::NTF_ShowNextMove);
}
