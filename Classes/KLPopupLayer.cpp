//
//  KLPopupLayer.cpp
//  Klondike
//
//  Created by 宋扬 on 16/7/17.
//
//

#include "KLPopupLayer.hpp"
#include "DataManager.h"
#include "KTDate.hpp"
#include "Localization.hpp"
#include "Trump.h"
#include "Layer_Setting.hpp"
using namespace NSDataManage;

#pragma mark - KLThanksGivingLayer -

bool KLThanksGivingLayer::init(){
    if(!KTPauseLayer::init()){
        return false;
    }

    auto bg = Layout::create();//LayerColor::create(Color4B(0, 0, 0, 175));
    bg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    bg->setContentSize(winSize);
    bg->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    bg->setBackGroundColor(Color3B::BLACK);
    bg->setBackGroundColorOpacity(175);
    
    bg->setPosition(Vec2::ZERO);
    addChild(bg, 0);
    _bg = bg;
    
    auto panel = ImageView::create("9_greenwindow.png");
    panel->setScale9Enabled(true);
    panel->setContentSize(Size(640, 460));
    panel->setPosition(VisibleRect::center() + Vec2(0, 60));
    addChild(panel);
    _panel = panel;
    
    auto bb = panel->getBoundingBox();
    {
        Label *titleLbl = Label::createWithSystemFont(LocalizedString("TID_UI_HAPPY_MERRY_CHRISTMAS"), "Arial", 36);
        titleLbl->setTextColor(Color4B::WHITE);
        titleLbl->setAlignment(TextHAlignment::CENTER);
        titleLbl->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        
        titleLbl->setPosition(ccRectTop(bb) + Vec2(0, -50));
        addChild(titleLbl);
        _titleLbl = titleLbl;
    }
    
    {
        auto btn = KTFactory::createButton("B_green.png", "B_green0.png", "",   LocalizedString("TID_UI_CONFIRM"), 30, "Arial", Size(280, 75));
        btn->setTag(0);
        auto label = btn->getTitleRenderer();
        label->setPosition(label->getPosition() + Vec2(0, 4));
        label->enableShadow(Color4B::GRAY);
        btn->setPosition(ccRectBottom(bb) + Vec2(0, 90));
        btn->addClickEventListener([=](Ref*){
            this->removeFromParentAndCleanup(true);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
            iOSWrapper::showToSetupThemeAppPage(true);
#else
            DataManager::getInstance()->changeThemeSet("theme_set_6");
#endif
        });
        addChild(btn);
        _shareBtn = btn;
        
        auto rect = btn->getBoundingBox();
        rect.origin = Vec2::ZERO;
        KLUtils::addStars(btn, rect, 1);
    }
    
    auto btn = KTFactory::createButton("close.png", "close1.png", "",  "", 30);
    
    btn->setPosition(ccRectRightTop(bb) + Vec2(-60, -50));
    btn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            this->removeFromParentAndCleanup(true);
            
        }
    });
    addChild(btn);
    _clostBtn = btn;
    
    auto sp = Sprite::create("shared/cardback/99.png");
    addChild(sp);
    sp->setPosition(ccRectLeft(bb) + Vec2(110, 0));
    _sp = sp;
    
    auto label = Label::createWithSystemFont(LocalizedString("TID_UI_HAPPY_MERRY_CHRISTMAS_CONTENT"), "Arial", 30);
    label->setOverflow(Label::Overflow::SHRINK);
    auto labelSize = Size(400, 150);
    label->setContentSize(labelSize);
    label->setDimensions(labelSize.width, labelSize.height);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    label->setPosition(ccRectLeft(bb) + Vec2(170, 0));
    addChild(label);
    _label = label;
    
    
    return true;
}

void KLThanksGivingLayer::updateLayout(){
    _bg->setContentSize(winSize);
    _panel->setPosition(VisibleRect::center() + Vec2(0, 60));
    auto bb = _panel->getBoundingBox();
    _titleLbl->setPosition(ccRectTop(bb) + Vec2(0, -50));
    _shareBtn->setPosition(ccRectBottom(bb) + Vec2(0, 90));
    _clostBtn->setPosition(ccRectRightTop(bb) + Vec2(-60, -50));
    _sp->setPosition(ccRectLeft(bb) + Vec2(110, 0));
    _label->setPosition(ccRectLeft(bb) + Vec2(170, 0));
}

#pragma mark - KLUpdateLayer -

bool KLUpdateLayer::show(){
    if(!iOSWrapper::isUpdateEnable()) return false;
    auto lastVersion = iOSWrapper::updateVersion();
    auto thisVersion = iOSWrapper::getVersionName();
    if (lastVersion == thisVersion) {
        //当前版本已是最新版本
        return false;
    }
    
    if (iOSWrapper::isForceUpdate()) {
        vector<string> others;
        
        auto la = KLAlertLayer::show(LocalizedString("TID_UI_HINT"), LocalizedString("TID_UI_PLEASE UPDATE TO CONTINUE PLAYING!"), LocalizedString("TID_UI_NEW VERSION!"), others, nullptr);
        Button* btn = la->getChildByTag<Button *>(0);
        if (btn != nullptr) {
            btn->addTouchEventListener(nullptr);
            btn->addClickEventListener([=](Ref *sender){
            
                Application::getInstance()->openURL(iOSWrapper::updateUrl());

            });
        }
    } else {
        vector<string> others;
        others.push_back(LocalizedString("TID_UI_NEW VERSION!"));
        KLAlertLayer::show(LocalizedString("TID_UI_HINT"), LocalizedString("TID_UI_PLEASE UPDATE TO CONTINUE PLAYING!"), LocalizedString("TID_UI_SKIP UPDATE"), others, [=](KLAlertLayer *layer, int buttonIdx){
            if(buttonIdx == 1){
                Application::getInstance()->openURL(iOSWrapper::updateUrl());
            }
        });
    }
    
    return true;
}

#pragma mark - KLShareSuccessLayer -

void KLShareSuccessLayer::updateLayout(){

    _bg->setContentSize(winSize);
    _panel->setPosition(VisibleRect::center() + Vec2(0, 60));
    auto bb = _panel->getBoundingBox();
    _titleLbl->setPosition(ccRectTop(bb) + Vec2(0, -50));
    _shareBtn->setPosition(ccRectBottom(bb) + Vec2(0, 90));
    _clostBtn->setPosition(ccRectRightTop(bb) + Vec2(-60, -50));
    _sp->setPosition(ccRectLeft(bb) + Vec2(110, 0));
    _label->setPosition(ccRectLeft(bb) + Vec2(170, 0));
}

bool KLShareSuccessLayer::init(){
    if(!KTPauseLayer::init()){
        return false;
    }
    
    auto bg = Layout::create();//LayerColor::create(Color4B(0, 0, 0, 175));
    bg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    bg->setContentSize(winSize);
    bg->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    bg->setBackGroundColor(Color3B::BLACK);
    bg->setBackGroundColorOpacity(175);
    
    bg->setPosition(Vec2::ZERO);
    addChild(bg, 0);
    _bg = bg;
    
    auto panel = ImageView::create("9_greenwindow.png");
    panel->setScale9Enabled(true);
    panel->setContentSize(Size(640, 460));
    panel->setPosition(VisibleRect::center() + Vec2(0, 60));
    addChild(panel);
    _panel = panel;
    
    auto bb = panel->getBoundingBox();
    {
        Label *titleLbl = Label::createWithSystemFont(LocalizedString("TID_CODE_SHARE CARDBACK2"), "Arial", 36);
        titleLbl->setTextColor(Color4B::WHITE);
        titleLbl->setAlignment(TextHAlignment::CENTER);
        titleLbl->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        
        titleLbl->setPosition(ccRectTop(bb) + Vec2(0, -50));
        addChild(titleLbl);
        _titleLbl = titleLbl;
    }
    
    {
        auto btn = KTFactory::createButton("B_green.png", "B_green0.png", "",   LocalizedString("TID_CODE_SHARE CARDBACK BUTTON2"), 30, "Arial", Size(280, 75));
        btn->setTag(0);
        auto label = btn->getTitleRenderer();
        label->setPosition(label->getPosition() + Vec2(0, 4));
        label->enableShadow(Color4B::GRAY);
        btn->setPosition(ccRectBottom(bb) + Vec2(0, 90));
        btn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
            if (type == Widget::TouchEventType::ENDED) {
                if(onClick){
                    onClick();
                }
                this->removeFromParent();
            }
        });
        addChild(btn);
        _shareBtn = btn;
    }
    
    auto btn = KTFactory::createButton("close.png", "close1.png", "",  "", 30);
    
    btn->setPosition(ccRectRightTop(bb) + Vec2(-60, -50));
    btn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            this->removeFromParentAndCleanup(true);
            
        }
    });
    addChild(btn);
    _clostBtn = btn;
    
    auto sp = Sprite::create("shared/cardback/99.png");
    addChild(sp);
    sp->setPosition(ccRectLeft(bb) + Vec2(110, 0));
    _sp = sp;
    
    auto label = Label::createWithSystemFont(LocalizedString("TID_CODE_SHARE CARDBACK DESC2"), "Arial", 20);
    label->setOverflow(Label::Overflow::SHRINK);
    auto labelSize = Size(300, 80);
    label->setContentSize(labelSize);
    label->setDimensions(labelSize.width, labelSize.height);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    label->setPosition(ccRectLeft(bb) + Vec2(170, 0));
    addChild(label);
    _label = label;
    
    return true;
}

#pragma mark - KLShareInfoLayer -

void KLShareInfoLayer::updateLayout(){
    _bg->setContentSize(winSize);
    _panel->setPosition(VisibleRect::center() + Vec2(0, 60));
    auto bb = _panel->getBoundingBox();
    _titleLbl->setPosition(ccRectTop(bb) + Vec2(0, -50));
    _shareBtn->setPosition(ccRectBottom(bb) + Vec2(0, 90));
    _clostBtn->setPosition(ccRectRightTop(bb) + Vec2(-60, -50));
    _sp->setPosition(ccRectLeft(bb) + Vec2(110, 0));
    _label->setPosition(ccRectLeft(bb) + Vec2(170, 0));
}

bool KLShareInfoLayer::init(){
    if(!KTPauseLayer::init()){
        return false;
    }
    
    auto bg = Layout::create();//LayerColor::create(Color4B(0, 0, 0, 175));
    bg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    bg->setContentSize(winSize);
    bg->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    bg->setBackGroundColor(Color3B::BLACK);
    bg->setBackGroundColorOpacity(175);

    bg->setPosition(Vec2::ZERO);
    addChild(bg, 0);
    _bg = bg;
    
    auto panel = ImageView::create("9_greenwindow.png");
    panel->setScale9Enabled(true);
    panel->setContentSize(Size(640, 460));
    panel->setPosition(VisibleRect::center() + Vec2(0, 60));
    addChild(panel);
    _panel = panel;
    
    auto bb = panel->getBoundingBox();
    {
        Label *titleLbl = Label::createWithSystemFont(LocalizedString("TID_CODE_SHARE CARDBACK1"), "Arial", 36);
        titleLbl->setTextColor(Color4B::WHITE);
        titleLbl->setAlignment(TextHAlignment::CENTER);
        titleLbl->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        
        titleLbl->setPosition(ccRectTop(bb) + Vec2(0, -50));
        addChild(titleLbl);
        _titleLbl = titleLbl;
    }
    
    {
        auto btn = KTFactory::createButton("B_green.png", "B_green0.png", "",   LocalizedString("TID_CODE_SHARE CARDBACK BUTTON1"), 30, "Arial", Size(280, 75));
        btn->setTag(0);
        auto label = btn->getTitleRenderer();
        label->setPosition(label->getPosition() + Vec2(0, 4));
        label->enableShadow(Color4B::GRAY);
        btn->setPosition(ccRectBottom(bb) + Vec2(0, 90));
        btn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
            if (type == Widget::TouchEventType::ENDED) {
                iOSWrapper::showShareUI();
            }
        });
        addChild(btn);
        _shareBtn = btn;
        
        auto rect = btn->getBoundingBox();
        rect.origin = Vec2::ZERO;
        KLUtils::addStars(btn, rect, 1);
    }
    
    auto btn = KTFactory::createButton("close.png", "close1.png", "",  "", 30);
    
    btn->setPosition(ccRectRightTop(bb) + Vec2(-60, -50));
    btn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            this->removeFromParentAndCleanup(true);
            
        }
    });
    addChild(btn);
    _clostBtn = btn;

    auto sp = Sprite::create("shared/cardback/99.png");
    addChild(sp);
    sp->setPosition(ccRectLeft(bb) + Vec2(110, 0));
    _sp = sp;
    
    auto label = Label::createWithSystemFont(LocalizedString("TID_CODE_SHARE CARDBACK DESC1"), "Arial", 20);
    label->setOverflow(Label::Overflow::SHRINK);
    auto labelSize = Size(300, 80);
    label->setContentSize(labelSize);
    label->setDimensions(labelSize.width, labelSize.height);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    label->setPosition(ccRectLeft(bb) + Vec2(170, 0));
    addChild(label);
    _label = label;
    
    return true;
}

#pragma mark - KLLoadingLayer -

void KLLoadingLayer::update(float dt){

    _timer += dt;
    float t = clampf(_timer/ 6.f, 0, 1);
    _progressBar->setPercent(t * 100);
}

void KLLoadingLayer::onEnter(){
    KTLayer::onEnter();
    runAction(Sequence::create(DelayTime::create(6), CallFunc::create(this->onFinish), NULL));
}

void KLLoadingLayer::updateLayout(){

    HLAdManagerWrapper::hideBanner();
    _bg->setScale(winSize.width/640, winSize.height/1136);
    _bg->setPosition(VisibleRect::center());
    _icon->setPosition(VisibleRect::center() + Vec2(0, 110));
    _tipBg->setPosition(VisibleRect::bottom() + Vec2(0, 200));
    _label->setPosition(_tipBg->getPosition());
    _progressBg->setPosition(VisibleRect::bottom() + Vec2(0, 80));
    _progressBar->setPosition(VisibleRect::bottom() + Vec2(0, 80));
}

bool KLLoadingLayer::init(){
    if(!KTLayer::init()){
        return false;
    }
    
    _timer = 2;
    
    auto bg = Sprite::create(BG_FILENAME);
    bg->setPosition(VisibleRect::center());
    addChild(bg);
    _bg = bg;
    
    auto icon = Sprite::create("loadingIcon.png");
    icon->setPosition(VisibleRect::center() + Vec2(0, 110));
    addChild(icon);
    _icon = icon;
    
    auto tipBg = ImageView::create("9_white_s.png");
    tipBg->setContentSize(Size(560, 100));
    tipBg->setColor(Color3B::BLACK);
    tipBg->setOpacity(127);
    tipBg->setScale9Enabled(true);
    tipBg->setPosition(VisibleRect::bottom() + Vec2(0, 200));
    
    addChild(tipBg);
    _tipBg = tipBg;
    
    
    int num = random(1, 11);
    string str = StringUtils::format("TID_UI_BASIC_LOADING_TIP_%d", num);
    auto label = Label::createWithSystemFont(LocalizedString(str), "Arial", 20);
    label->setOverflow(Label::Overflow::SHRINK);
    auto labelSize = tipBg->getContentSize() - Size(20, 20);
    label->setContentSize(labelSize);
    label->setDimensions(labelSize.width, labelSize.height);
    label->setPosition(tipBg->getPosition());
    label->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    addChild(label);
    _label = label;
    
    auto progressBg = ImageView::create("loading_line_bg.png");
    progressBg->setPosition(VisibleRect::bottom() + Vec2(0, 120));
    progressBg->setScale9Enabled(true);
    progressBg->setContentSize(Size(440, 24));
    
    addChild(progressBg);
    _progressBg = progressBg;
    
    _progressBar = LoadingBar::create("loading_line.png", 0);
    _progressBar->setScale9Enabled(true);
    _progressBar->setContentSize(Size(440, 24));
    _progressBar->setPosition(VisibleRect::bottom() + Vec2(0, 120));
    _progressBar->setDirection(LoadingBar::Direction::LEFT);
//    _progressBar->setScale(440.f/163.f, 24.f/21.f);
    addChild(_progressBar, 1);
    
    scheduleUpdate();
    
    return true;
}

#pragma mark - KLIntroLayer -

KLIntroLayer::~KLIntroLayer(){
    DataManager::getInstance()->getStarPool()->despawnAll();
}

void KLIntroLayer::updateLayout(){
    _bg->setContentSize(winSize);
    _panel->setPosition(VisibleRect::center() + Vec2(0, 60));
    auto bb = _panel->getBoundingBox();
    
    _titleLbl->setPosition(ccRectTop(bb) + Vec2(0, -50));
    _icon->setPosition(ccRectLeftTop(bb) + Vec2(70, -50));
    _startButton->setPosition(ccRectBottom(bb) + Vec2(0, -50));
    _closeBtn->setPosition(ccRectRightTop(bb) + Vec2(-60, -50));
    
    auto widgetSize = Size(640, 460);
    _pageView->setPosition(Vec2((widgetSize.width - _pageView->getContentSize().width) / 2.0f - 320 + winSize.width/2,
                               VisibleRect::center().y - 90));
    if (_leftBtn) {
        _leftBtn->setPosition(Vec2(25-320, 70) + VisibleRect::center());
    }
    if (_rightBtn) {
        _rightBtn->setPosition(Vec2(-25+320, 70) + VisibleRect::center());
    }
}

KLIntroLayer::KLIntroLayer():
_leftBtn(nullptr),
_rightBtn(nullptr),
onClose(nullptr){

}

bool KLIntroLayer::init(KLIntroLayer::IntroType introType){
    if (!KTPauseLayer::init()) {
        return false;
    }
    
    _introType = introType;
    
    auto bg = LayerColor::create(Color4B(0, 0, 0, 175));
    addChild(bg, -1);
    _bg = bg;
    
    auto panel = ImageView::create("9_greenwindow.png");
    panel->setScale9Enabled(true);
    panel->setContentSize(Size(600, 400));
    panel->setPosition(VisibleRect::center() + Vec2(0, 60));
    addChild(panel);
    _panel = panel;
    
    auto bb = panel->getBoundingBox();
    {
        auto title = LocalizedString("TID_UI_HOW TO PLAY");
//        if (_introType == IntroType::CustomCardbackTheme) {
//            title = LocalizedString("TID_UI_NEW_FEATURE_TITLE");
//        }
        Label *titleLbl = Label::createWithSystemFont(title, "Arial", 36);
        titleLbl->setTextColor(Color4B::WHITE);
        titleLbl->setAlignment(TextHAlignment::CENTER);
        titleLbl->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        titleLbl->setPosition(ccRectTop(bb) + Vec2(0, -50));
        addChild(titleLbl);
        _titleLbl = titleLbl;
    }
    {
        ImageView *icon = ImageView::create("guide_bg1_1.png");
        icon->setPosition(ccRectLeftTop(bb) + Vec2(70, -50));
        addChild(icon);
        _icon = icon;
        
    }
    {
        auto title = _introType == IntroType::First ? LocalizedString("TID_UI_PLAY") : LocalizedString("TID_UI_CHANGE BACKGROUND");
        auto btn = KTFactory::createButton("B_green.png", "B_green0.png", "",  title, 30, "Arial", Size(280, 75));
        btn->setTag(0);
        auto label = btn->getTitleRenderer();
        label->setPosition(label->getPosition() + Vec2(0, 4));
        label->enableShadow(Color4B::GRAY);
        btn->setPosition(ccRectBottom(bb) + Vec2(0, -50));
        btn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
            if (type == Widget::TouchEventType::ENDED) {
               
                if (this->onClose) {
                    this->onClose();
                }
                
//                if (_introType == IntroType::CustomCardbackTheme) {
//                    auto la = Layer_CardBack::create();
//                    auto sc = Scene::create();
//                    sc->addChild(la);
//                    Director::getInstance()->pushScene(sc);
//                }
                
                
                this->removeFromParentAndCleanup(true);
            }
        });
        addChild(btn);
        _startButton = btn;
    }
    
    auto btn = KTFactory::createButton("close.png", "close1.png", "",  "", 30);
    //        btn->setPosition(VisibleRect::center() + Vec2(0, -132));
    
    //        btn->setContentSize(Size(280, 75));
    btn->setPosition(ccRectRightTop(bb) + Vec2(-60, -50));
    btn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            if (onClose) {
                onClose();
            }
            this->removeFromParentAndCleanup(true);
        }
    });
    addChild(btn);
    _closeBtn = btn;
    
    auto widgetSize = Size(640, 460);
    PageView* pageView = PageView::create();
    pageView->setContentSize(Size(500, 220));
    pageView->setBackGroundColor(Color3B::RED);
//    Size backgroundSize = background->getContentSize();
    pageView->setPosition(Vec2((widgetSize.width - pageView->getContentSize().width) / 2.0f,
                               VisibleRect::center().y - 90));
    pageView->setDirection(ui::PageView::Direction::HORIZONTAL);
    pageView->removeAllItems();
    
    pageView->setIndicatorEnabled(true);
    pageView->setIndicatorSpaceBetweenIndexNodes(15);
//    pageView->setIndicatorIndexNodesScale(0.5);
    pageView->setIndicatorIndexNodesTexture("page.png");
    pageView->setIndicatorSelectedIndexColor(Color3B::WHITE);
    pageView->setIndicatorIndexNodesColor(Color3B(100, 100, 255));
    pageView->setBounceEnabled(false);
    if (_introType == IntroType::First) {
        pageView->addEventListener([=](Ref*, PageView::EventType){
            CCLOG("page idx = %zd", pageView->getCurrentPageIndex());
            ssize_t idx = pageView->getCurrentPageIndex();
            if(_leftBtn != nullptr)
                _leftBtn->setVisible(idx != 0);
            if (_rightBtn != nullptr) {
                _rightBtn->setVisible(idx != 4);
            }
            _startButton->setVisible(idx == 4);
            
            if (idx == 4) {
                KLUtils::addStars(this, _startButton->getBoundingBox(), 1);
            } else {
                DataManager::getInstance()->getStarPool()->despawnAll();
            }
        });
    }
    
    //Ref*,PageView::EventType::TURNING
    
    addChild(pageView);
    _pageView = pageView;
    
    if (_introType == IntroType::First) {
        for (int i = 0; i < 5; i++) {
            Layout* layout = Layout::create();
            layout->setContentSize(Size(500, 220));
            layout->setBackGroundColor(Color3B::RED);
            //        Text* label = Text::create(StringUtils::format("高分记录最高分的7条经典牌局得分纪录将会被记入排行榜"), "Arial", 30);
            //        label->setColor(Color3B(192, 192, 192));
            //        label->setPosition(Vec2(layout->getContentSize().width / 2.0f, layout->getContentSize().height / 2.0f));
            //        layout->addChild(label);
            
            Label *label = Label::createWithSystemFont(LocalizedString(StringUtils::format("TID_RULES_CLASSIC_%d", i+1)), "Arial", 30);
            label->setContentSize(Size(500, 220));
            label->setDimensions(500, 220);
            label->setPosition(Vec2(layout->getContentSize().width / 2.0f, layout->getContentSize().height / 2.0f));
            label->setOverflow(Label::Overflow::SHRINK);
            
            layout->addChild(label);
            pageView->insertCustomItem(layout, i);
        }
        
        {
            auto btn = KTFactory::createButton("B_left.png", "B_left1.png", "", "", 0);
            btn->setPosition(Vec2(25-320, 70) + VisibleRect::center());
            btn->addClickEventListener([=](Ref *sender){
                ssize_t idx = MAX(0, pageView->getCurrentPageIndex()-1);
                pageView->scrollToPage(idx);
            });
            addChild(btn);
            _leftBtn = btn;
        }
        
        {
            auto btn = KTFactory::createButton("B_left.png", "B_left1.png", "", "", 0);
            btn->setFlippedX(true);
            btn->setPosition(Vec2(320-25, 70) + VisibleRect::center());
            btn->addClickEventListener([=](Ref *sender){
                ssize_t idx = MIN(4, pageView->getCurrentPageIndex()+1);
                pageView->scrollToPage(idx);
            });
            addChild(btn);
            _rightBtn = btn;
        }
        
        {
            _leftBtn->setVisible(false);
            _startButton->setVisible(false);
        }
    }
//    else if (_introType == IntroType::CustomCardbackTheme) {
//        
//        
//        Layout* layout = Layout::create();
//        layout->setContentSize(Size(500, 220));
//        layout->setBackGroundColor(Color3B::RED);
//        
//        Label *label = Label::createWithSystemFont(LocalizedString("TID_UI_NEW_FEATURE_CARDBACK"), "Arial", 30);
//        label->setContentSize(Size(500, 220));
//        label->setDimensions(500, 220);
//        label->setPosition(Vec2(layout->getContentSize().width / 2.0f, 60));
//        label->setOverflow(Label::Overflow::SHRINK);
//        
//        layout->addChild(label);
//        pageView->insertCustomItem(layout, 0);
//        
//        _startButton->setTitleText(LocalizedString("TID_UI_CONFIRM"));
//        
//        _startButton->setVisible(true);
//        pageView->setIndicatorEnabled(false);
//    }
    
    
    
    return true;
}

#pragma mark - KLInputNameLayer -

void KLInputNameLayer::updateLayout(){
    
    _bg->setContentSize(winSize);
    _panel->setPosition(VisibleRect::center());
    auto bb = _panel->getBoundingBox();
    _titleLbl->setPosition(ccRectLeftTop(bb) + Vec2(60, -110));
    _whiteBg->setPosition(VisibleRect::center());
    _tf->setPosition(Vec2(0, 0) + VisibleRect::center());
    _submitBtn->setPosition(ccRectBottom(bb) + Vec2(0, 90));
}

bool KLInputNameLayer::init(){
    if (!KTPauseLayer::init()) {
        return false;
    }
    
    auto bg = LayerColor::create(Color4B(0, 0, 0, 175));
    addChild(bg, -1);
    _bg = bg;
    
    auto panel = ImageView::create("9_greenwindow_s.png");
    panel->setScale9Enabled(true);
    panel->setContentSize(Size(600, 350));
    panel->setPosition(VisibleRect::center());
    addChild(panel);
    _panel = panel;
    
    auto bb = panel->getBoundingBox();
    {
        Label *titleLbl = Label::createWithSystemFont(LocalizedString("TID_UI_ONTO THE LEADERBOARD"), "Arial", 28);
        titleLbl->setTextColor(Color4B::WHITE);
        titleLbl->setAlignment(TextHAlignment::LEFT);
        titleLbl->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        titleLbl->setPosition(ccRectLeftTop(bb) + Vec2(60, -110));
        titleLbl->setContentSize(Size(450, 80));
        titleLbl->setDimensions(450, 80);
        titleLbl->setOverflow(Label::Overflow::SHRINK);
        addChild(titleLbl);
        _titleLbl = titleLbl;
    }
    
    //9_white_s.png
    
    {
        auto bg = ImageView::create("9_white_s.png");
        bg->setScale9Enabled(true);
        bg->setContentSize(Size(450, 80));
        bg->setColor(Color3B::BLACK);
        bg->setOpacity(100);
        bg->setPosition(VisibleRect::center());
        addChild(bg);
        _whiteBg = bg;
    }
    
    TextField *tf = TextField::create(LocalizedString("TID_CODE_ENTER YOUR NAME"), "Arial", 32);
    tf->setPosition(Vec2(0, 0) + VisibleRect::center());
    tf->setMaxLengthEnabled(true);
    tf->setMaxLength(15);
//    tf->addEventListener(const ccTextFieldCallback &callback)
    addChild(tf);
    _tf = tf;
    
    auto btn = KTFactory::createButton("B_green.png", "B_green0.png", "",  LocalizedString("TID_UI_CONFIRM"), 30, "Arial", Size(280, 75));
    auto label = btn->getTitleRenderer();
    label->setPosition(label->getPosition() + Vec2(0, 4));
    label->enableShadow(Color4B::GRAY);
    btn->setPosition(ccRectBottom(bb) + Vec2(0, 90));
    btn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            auto tf = (UICCTextField *)_tf->getVirtualRenderer();
            
            if (_tf->getString().empty()) {
                return;
            }
            
            tf->closeIME();
            if (onClick) {
                onClick();
            }
            this->removeFromParent();
        }
    });
    addChild(btn);
    _submitBtn = btn;
    
    return true;
}

#pragma mark - KLRateLayer -

void KLRateLayer::reset(){

    UserDefault::getInstance()->setBoolForKey("KL_SHOW_RATE", false);
    UserDefault::getInstance()->setIntegerForKey("KL_SHOW_NUM", 0);
    UserDefault::getInstance()->flush();
}

bool KLRateLayer::show(Node *parent, function<void()> onClick){
    
    bool flag = UserDefault::getInstance()->getBoolForKey("KL_SHOW_RATE", false);
    int num = UserDefault::getInstance()->getIntegerForKey("KL_SHOW_NUM", 0);
    if (!flag && num >= HLAnalsytWrapper::intValue("KL_RATE_SHOW_NUM", 5)) {
        
        int flag2 = HLAnalsytWrapper::intValue("KL_RATE_STATE", 1);
        if (flag2 != 0) {
            auto la = KLRateLayer::create(flag2 == 2);
            la->onClick = onClick;
            parent->addChild(la, 2048);
            UserDefault::getInstance()->setIntegerForKey("KL_SHOW_NUM", 0);
            return true;
        }
    }
    num++;
    UserDefault::getInstance()->setIntegerForKey("KL_SHOW_NUM", num);
    return false;
}

void KLRateLayer::updateLayout(){
    
    _bg->setContentSize(winSize);
    _panel->setPosition(VisibleRect::center() + Vec2(0, 60));
    auto bb = _panel->getBoundingBox();
    _titleLbl->setPosition(ccRectTop(bb) + Vec2(0, -50));
    _submitBtn->setPosition(ccRectBottom(bb) + Vec2(0, 90));
    _closeBtn->setPosition(ccRectRightTop(bb) + Vec2(-60, -50));
    _titleLbl2->setPosition(ccRectLeftTop(bb) + Vec2(60,_showStar ? -310 : -210));
    if (_showStar) {
        _titleLbl3->setPosition(ccRectLeftTop(bb) + Vec2(60, -140));
        
        Vec2 center = ccRectBottom(bb) + Vec2(0, -250);
        Vec2 start = _panel->getPosition() + Vec2(0, 25);
        start = start.rotateByAngle(center, CC_DEGREES_TO_RADIANS(-10 * 3));
        {
            
            for (int i = 0; i < 5; i++) {
                start.rotate(center, CC_DEGREES_TO_RADIANS(10));
                auto sp = _stars.at(i);
                sp->setPosition(start);
                auto v = start - center;
                float radian = v.getAngle();
                float angle = 90 - CC_RADIANS_TO_DEGREES(radian);
                sp->setRotation(angle);
            }
        }
    }
}

bool KLRateLayer::init(bool showStar){
    if (!KTPauseLayer::init()) {
        return false;
    }
    _showStar = showStar;
    auto bg = LayerColor::create(Color4B(0, 0, 0, 175));
    addChild(bg, -1);
    _bg = bg;
    
    auto panel = ImageView::create("9_greenwindow.png");
    panel->setScale9Enabled(true);
    panel->setContentSize(Size(640, 460));
    panel->setPosition(VisibleRect::center() + Vec2(0, 60));
    addChild(panel);
    _panel = panel;
    auto bb = panel->getBoundingBox();
    {
        Label *titleLbl = Label::createWithSystemFont(LocalizedString("TID_UI_ENJOYING SOLITAIRE MANIA?"), "Arial", 36);
        titleLbl->setTextColor(Color4B::WHITE);
        titleLbl->setAlignment(TextHAlignment::CENTER);
        titleLbl->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        
        titleLbl->setPosition(ccRectTop(bb) + Vec2(0, -50));
        addChild(titleLbl);
        _titleLbl = titleLbl;
    }
    
    {
        auto btn = KTFactory::createButton("B_green.png", "B_green0.png", "",  showStar ? LocalizedString("TID_UI_5 STAR") : LocalizedString("TID_UI_RATE"), 30, "Arial", Size(280, 75));
        btn->setTag(0);
        auto label = btn->getTitleRenderer();
        label->setPosition(label->getPosition() + Vec2(0, 4));
        label->enableShadow(Color4B::GRAY);
        btn->setPosition(ccRectBottom(bb) + Vec2(0, 90));
        btn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
            if (type == Widget::TouchEventType::ENDED) {
                UserDefault::getInstance()->setBoolForKey("KL_SHOW_RATE", true);
                string url = iOSWrapper::getRateURL();
                CCLOG("url = %s", url.c_str());
                Application::getInstance()->openURL(url);
                if (onClick) {
                    onClick();
                }
                this->removeFromParentAndCleanup(true);
            }
        });
        addChild(btn);
        _submitBtn = btn;
        auto rect = btn->getBoundingBox();
        rect.origin = Vec2::ZERO;
        KLUtils::addStars(_submitBtn, rect, 1);
    }
    
    auto btn = KTFactory::createButton("close.png", "close1.png", "",  "", 30);
    //        btn->setPosition(VisibleRect::center() + Vec2(0, -132));
    
    //        btn->setContentSize(Size(280, 75));
    btn->setPosition(ccRectRightTop(bb) + Vec2(-60, -50));
    btn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            if (onClick) {
                onClick();
            }
            this->removeFromParentAndCleanup(true);
            
        }
    });
    addChild(btn);
    _closeBtn = btn;
    
    {
        Label *titleLbl = Label::createWithSystemFont(LocalizedString("TID_UI_YOUR SUPPORT HELPS US MAKE GREAT GAMES"), "Arial", 28);
        titleLbl->setTextColor(Color4B::WHITE);
        titleLbl->setAlignment(TextHAlignment::LEFT);
        titleLbl->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        
        titleLbl->setContentSize(Size(500, 50));
        titleLbl->setDimensions(500, 50);
        titleLbl->setOverflow(Label::Overflow::SHRINK);
        titleLbl->setPosition(ccRectLeftTop(bb) + Vec2(60,showStar ? -310 : -210));
        addChild(titleLbl);
        _titleLbl2 = titleLbl;
    }
    if (showStar) {
        {
            Label *titleLbl = Label::createWithSystemFont(LocalizedString("TID_UI_5-STAR RATINGS FROM YOU ENCOURAGE US TO PROVIDE MORE GREAT CONTENTS!"), "Arial", 28);
            titleLbl->setTextColor(Color4B::WHITE);
            titleLbl->setAlignment(TextHAlignment::LEFT);
            titleLbl->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            
            titleLbl->setContentSize(Size(500, 50));
            titleLbl->setDimensions(500, 50);
            titleLbl->setOverflow(Label::Overflow::SHRINK);
            titleLbl->setPosition(ccRectLeftTop(bb) + Vec2(60, -140));
            addChild(titleLbl);
            _titleLbl3 = titleLbl;
        }
        
        
        Vec2 center = ccRectBottom(bb) + Vec2(0, -250);
        Vec2 start = panel->getPosition() + Vec2(0, 25);
        start = start.rotateByAngle(center, CC_DEGREES_TO_RADIANS(-10 * 3));
        {
            
            for (int i = 0; i < 5; i++) {
                start.rotate(center, CC_DEGREES_TO_RADIANS(10));
                auto sp = Sprite::create("Exp_star.png");
                addChild(sp, 1);
                sp->setPosition(start);
                auto v = start - center;
                float radian = v.getAngle();
                float angle = 90 - CC_RADIANS_TO_DEGREES(radian);
                sp->setRotation(angle);
                _stars.pushBack(sp);
            }
            
        }
    }
    
    return  true;
}

KLRateLayer::~KLRateLayer(){
    DataManager::getInstance()->getStarPool()->despawnAll();
}

#pragma mark - KLProgressLayer -

static KLProgressLayer *layer = nullptr;

KLProgressLayer::~KLProgressLayer(){
    layer = nullptr;
}

void KLProgressLayer::hide(Layer *parent){
    if (layer != nullptr) {
        layer->removeFromParent();
    }
}

void KLProgressLayer::show(Layer *parent){
    if (layer==nullptr) {
        parent->addChild(KLProgressLayer::create(), 2048);
    } else {
        if (layer->getParent() != nullptr){
            layer->retain();
            layer->removeFromParent();
            parent->addChild(layer);
            layer->release();
        } else {
            parent->addChild(layer);
        }
    }
}

void KLProgressLayer::updateLayout(){

    _bg->setContentSize(winSize);
    _icon->setPosition(VisibleRect::center());
}

bool KLProgressLayer::init(){
    if (!KTPauseLayer::init(nullptr)) {
        return false;
    }
    layer = this;
    
    auto bg = LayerColor::create(Color4B(0, 0, 0, 175));
    addChild(bg, -1);
    _bg = bg;
    
    auto icon = Sprite::create("connect_icon.png");
    icon->setAnchorPoint(Vec2(0.418, 0.473));
    icon->setPosition(VisibleRect::center());
    addChild(icon);
    _icon = icon;
    
    
    auto ac = RepeatForever::create(RotateBy::create(0.5, 360));
    icon->runAction(ac);
    
    return true;
}

#pragma mark - KLAlertLayer -

void KLAlertLayer::onEnter(){
    
    
    Text *contentLbl = Text::create(_content, "Arial", 30);
    //Label::createWithSystemFont(_content, "Arial", 30);
    contentLbl->ignoreContentAdaptWithSize(false);
    contentLbl->setContentSize(Size(540, 230));
    contentLbl->setTextHorizontalAlignment(TextHAlignment::LEFT);
    contentLbl->setTextVerticalAlignment(TextVAlignment::CENTER);
    contentLbl->setPosition(VisibleRect::center() + Vec2(0, 60));
    addChild(contentLbl);
    _contentLbl = contentLbl;
    
    int buttonNum = 1 + (int)_otherButtons.size();
    float width = MIN(300.f, (600.f - (buttonNum + 1) * 16) / buttonNum);
    auto callback = [=](Ref*sender,Widget::TouchEventType event){
        if (event == Widget::TouchEventType::ENDED) {
            Node* node = (Node *)sender;
            if (onTap) {
                onTap(this, node->getTag());
            }
            if (this->getParent() != nullptr) {
                this->removeFromParent();
            }
        }
    };
    
    int tag = 0;
    if (!_cancelButton.empty()) {
        auto btn = KTFactory::createButton("B_orange.png", "B_orange1.png", "", _cancelButton, 30, "Arial", Size(width, 90));
//        btn->setContentSize(Size(width, 90));
        btn->addTouchEventListener(callback);
        btn->setTag(tag++);
        addChild(btn);
        buttons.pushBack(btn);
    }
    
    
    for(auto name : _otherButtons) {
        auto btn = KTFactory::createButton("B_green1.png", "B_green2.png", "", name, 30, "Arial", Size(width, 90));
        addChild(btn);
        btn->setTag(tag++);
        buttons.pushBack(btn);
        btn->addTouchEventListener(callback);
    }
    horizonAliament(buttons, 10, VisibleRect::center().y - 100, false);
    
    KTPauseLayer::onEnter();
}

void KLAlertLayer::updateLayout(){

    _bg->setContentSize(winSize);
    _panel->setPosition(VisibleRect::center());
    _contentLbl->setPosition(VisibleRect::center() + Vec2(0, 60));
    horizonAliament(buttons, 10, VisibleRect::center().y - 100, false);
}

bool KLAlertLayer::init(){
    if (!KTPauseLayer::init()) {
        return false;
    }
    
    auto bg = LayerColor::create(Color4B(0, 0, 0, 175));
    addChild(bg, -1);
    _bg = bg;
    
    auto panel = ImageView::create("9_greenwindow_s.png");
    panel->setScale9Enabled(true);
    panel->setContentSize(Size(640, 450));
    panel->setPosition(VisibleRect::center());
    addChild(panel);
    _panel = panel;
    
    return true;
}

KLAlertLayer* KLAlertLayer::show(std::string title, std::string content, std::string cancelButton, std::vector<std::string>otherButtons, KLAlertLayerTapCallback onTap){
    KLAlertLayer *layer = KLAlertLayer::create();
    layer->_title = title;
    layer->_content = content;
    layer->_cancelButton = cancelButton;
    layer->_otherButtons = otherButtons;
    layer->onTap = onTap;
    
    auto layers = Director::getInstance()->getRunningScene()->getChildren();
    
    if (layers.size() > 0) {
        
        for (auto la : layers) {
            auto target = dynamic_cast<Layer *>(la);
            if (target) {
                target->addChild(layer, 2048);
                break;
            }
        }
        
        
    } else {
        CCLOGWARN("没有可展示对话框的场景");
    }
    return layer;
}

#pragma mark - KLDailyOpenLayer -


bool KLDailyOpenLayer::onTouchBegan(Touch *touch, Event *unused_event){
    auto panel = getChildByTag<ImageView *>(123);
    if (!panel->boundingBox().containsPoint(touch->getLocation())) {
        //quit
        removeFromParent();
    }
    
    return true;
}

void KLDailyOpenLayer::updateLayout(){
    _bg->setContentSize(winSize);
    _panel->setPosition(VisibleRect::center() + Vec2(0, -10));
    _titleLbl->setPosition(VisibleRect::center() + Vec2(0, 160));
    _helpBtn->setPosition(VisibleRect::center() + Vec2(-230, 160));
    _contentLbl->setPosition(VisibleRect::center() + Vec2(0, 80));
    _crown->setPosition(VisibleRect::center() + Vec2(0, -38));
    _playBtn->setPosition(VisibleRect::center() + Vec2(0, -132));
}

bool KLDailyOpenLayer::init(){
     if (!KTPauseLayer::init()) {
         return false;
     }
    
     auto bg = LayerColor::create(Color4B(0, 0, 0, 175));
     addChild(bg, -2);
    _bg = bg;
     
     auto panel = ImageView::create("9_greenwindow.png");
     panel->setTag(123);
     panel->setScale9Enabled(true);
     panel->setContentSize(Size(640, 450));
     panel->setPosition(VisibleRect::center() + Vec2(0, -10));
     addChild(panel, -1);
    _panel = panel;
     
     Label *titleLbl = Label::createWithSystemFont(LocalizedString("TID_UI_NEW CHALLENGE"), "Arial", 36);
     titleLbl->setPosition(VisibleRect::center() + Vec2(0, 160));
     addChild(titleLbl);
    _titleLbl = titleLbl;
     
     auto btn = KTFactory::createButton("B_help.png", "B_help1.png", "", "", 30);
    btn->setPosition(VisibleRect::center() + Vec2(-230, 160));
    btn->addClickEventListener([=](Ref*){
        auto parent = this->getParent();
        this->removeFromParent();
        auto la = KLDailyInfoLayer::create();
        parent->addChild(la, 2048);
    });
     addChild(btn);
    _helpBtn = btn;
   
     
     Label *contentLbl = Label::createWithSystemFont(LocalizedString("TID_UI_COMPLETE THE CHALLENGE GAME TO GET THE CROWN TODAY"), "Arial", 30);
     contentLbl->setPosition(VisibleRect::center() + Vec2(0, 80));
     addChild(contentLbl);
    _contentLbl = contentLbl;

     {
         _crown = ImageView::create("king0.png");
//         _crown->setScale(64.f/131.f);
         addChild(_crown);
         _crown->setPosition(VisibleRect::center() + Vec2(0, -38));
     }
     
     
     //B_green B_green0
     {
         auto btn = KTFactory::createButton("B_green.png", "B_green0.png", "", LocalizedString("TID_UI_PLAY"), 30, "Arial", Size(280, 75));
         btn->setPosition(VisibleRect::center() + Vec2(0, -132));
         
//         btn->setContentSize(Size(280, 75));
         
         auto label = btn->getTitleRenderer();
         label->setPosition(label->getPosition() + Vec2(0, 4));
         
         btn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
             if (type == Widget::TouchEventType::ENDED && onTap) {
                 onTap(this, 0);
             }
         });
         addChild(btn);
         _playBtn = btn;
     }
     
     return true;
}


#pragma mark - KLNoMoveLayer -


bool KLNoMoveLayer::onTouchBegan(Touch *touch, Event *unused_event){
    auto panel = getChildByTag<ImageView *>(123);
    if (!panel->getBoundingBox().containsPoint(touch->getLocation())) {
        //quit
        onTap(this, 4);
        removeFromParent();
    }
    
    return true;
}

void KLNoMoveLayer::updateLayout(){

    _bg->setContentSize(winSize);
    _panel->setPosition(VisibleRect::center() + Vec2(0, 0));
    _panel2->setPosition(VisibleRect::center() + Vec2(0, 145));
    _titleLbl->setPosition(VisibleRect::center() + Vec2(0, 180));
    _checkBox->setPosition(VisibleRect::center() + Vec2(-160, 100));
    _titleLbl2->setPosition(VisibleRect::center() + Vec2(-140, 100));
    _titleLbl3->setPosition(VisibleRect::center() + Vec2(0, -260));
    _layout->setPosition(VisibleRect::center() + Vec2(-140, -130-85));
}

bool KLNoMoveLayer::init(){
    if (!KTPauseLayer::init()) {
        return false;
    }
    
    //     auto bg = Widget::create();
    //     bg->setPosition(VisibleRect::center());
    //     bg->setContentSize(VisibleRect::getVisibleRect().size);
    
    auto bg = LayerColor::create(Color4B(0, 0, 0, 175));
    addChild(bg, -2);
    _bg = bg;
    
    auto panel = ImageView::create("9_greenwindow_s.png");
    panel->setTag(123);
    panel->setScale9Enabled(true);
    panel->setContentSize(Size(500, 550));
    panel->setPosition(VisibleRect::center() + Vec2(0, 0));
    addChild(panel, -1);
    _panel = panel;
    
    {
        auto panel = Scale9Sprite::create("9_white_s.png");
        panel->setColor(Color3B::BLACK);
        panel->setOpacity(64);
        panel->setScale9Enabled(true);
        panel->setContentSize(Size(400, 185));
        panel->setPosition(VisibleRect::center() + Vec2(0, 145));
        addChild(panel, -1);
        _panel2 = panel;
    }
    
    Label *titleLbl = Label::createWithSystemFont(LocalizedString("TID_UI_YOU ARE OUT OF LEGAL MOVES DO YOU WANT TO START A NEW GAME"), "Arial-BoldMT", 24);
    titleLbl->setPosition(VisibleRect::center() + Vec2(0, 180));
    
    titleLbl->setDimensions(MAX(0, 390), 100);
    titleLbl->setVerticalAlignment(TextVAlignment::CENTER);
    titleLbl->setOverflow(Label::Overflow::SHRINK);
    
    addChild(titleLbl);
    _titleLbl = titleLbl;
    
    
    auto box = CheckBox::create("B_NO.png", "tick.png");
    box->setScale(36.f/80.f);
    box->setPosition(VisibleRect::center() + Vec2(-160, 100));
    addChild(box);
    _checkBox = box;
    {
        Label *titleLbl = Label::createWithSystemFont(LocalizedString("TID_UI_DONT REMIND ME AGAIN DURING THIS HAND"), "Arial", 22);
        titleLbl->setAlignment(TextHAlignment::LEFT);
        titleLbl->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        titleLbl->setDimensions(MAX(0, 320), 38);
        titleLbl->setVerticalAlignment(TextVAlignment::CENTER);
        titleLbl->setOverflow(Label::Overflow::SHRINK);
        titleLbl->setPosition(VisibleRect::center() + Vec2(-140, 100));
        addChild(titleLbl);
        _titleLbl2 = titleLbl;
    }
    
    {
        Label *titleLbl = Label::createWithSystemFont(LocalizedString("TID_UI_TAP ANYWHERE TO CONTINUE GAME"), "Arial", 24);
        titleLbl->setTextColor(KL_GREEN);
        titleLbl->setAlignment(TextHAlignment::CENTER);
        titleLbl->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        titleLbl->setPosition(VisibleRect::center() + Vec2(0, -260));
        addChild(titleLbl);
        _titleLbl3 = titleLbl;
    }
//    auto btn = KTFactory::createButton("B_help.png", "B_help.png", "", "", 30);
//    btn->setPosition(VisibleRect::center() + Vec2(-230, 160));
//    addChild(btn);
    
    
//    Label *contentLbl = Label::createWithSystemFont("通关今日挑战牌局", "Arial", 30);
//    contentLbl->setPosition(VisibleRect::center() + Vec2(0, 80));
//    addChild(contentLbl);
//    
//    {
//        auto _crown = ImageView::create("king0.png");
//        //         _crown->setScale(64.f/131.f);
//        addChild(_crown);
//        _crown->setPosition(VisibleRect::center() + Vec2(0, -38));
//    }
    
    
    Layout *layout = Layout::create();
    layout->setContentSize(Size(280, 265));
    layout->setLayoutType(Layout::Type::VERTICAL);
    addChild(layout);
    _layout = layout;
    
    layout->setPosition(VisibleRect::center() + Vec2(-140, -130-85));
    
    std::string names[] = {LocalizedString("TID_CODE_NEW GAME"), LocalizedString("TID_UI_REPLAY"), LocalizedString("TID_UI_WINNING DEAL")};
    Vector<Node *> nodes;
    for (int i = 0; i < 3; i++) {
    
        auto nor = i == 2 ? "B_orange.png" : "B_green.png";
        auto sel = i == 2 ? "B_orange1.png" : "B_green0.png";
        auto btn = KTFactory::createButton(nor, sel, "", names[i], 30, "Arial", Size(280, 75));
//        btn->setPosition(VisibleRect::center() + Vec2(0, -132));
        btn->setTag(i);
        
        auto label = btn->getTitleRenderer();
        label->setPosition(label->getPosition() + Vec2(0, 4));
        label->enableShadow(Color4B::GRAY);
        
        btn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
            if (type == Widget::TouchEventType::ENDED && onTap) {
                Node *n = (Node *)pSender;
                onTap(this, n->getTag());
                this->removeFromParentAndCleanup(true);
            }
        });
        layout->addChild(btn);
        nodes.pushBack(btn);
        
        LinearLayoutParameter* lp1 = LinearLayoutParameter::create();
        btn->setLayoutParameter(lp1);
        lp1->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
        lp1->setMargin(Margin(0.0f, 10.f, 0.0f, 10.0f));
    }
    
    return true;
}

#pragma mark - KLRestartLayer -

bool KLRestartLayer::onTouchBegan(Touch *touch, Event *unused_event){
//    auto panel = getChildByTag(123);
//    if (!panel->boundingBox().containsPoint(touch->getLocation())) {
//        //quit
//        onTap(this, -1);
//        removeFromParent();
//    }
    
    return true;
}

void KLRestartLayer::updateLayout(){
    _bg->setContentSize(winSize);
    _panel->setPosition(VisibleRect::center() + Vec2(0, 0));
    
    _layout->setPosition(VisibleRect::center() + Vec2(-140, _isDailyChallege ? (-115) : -200));
}

bool KLRestartLayer::init(bool isDailyChallege){
    if (!KTPauseLayer::init()) {
        return false;
    }
    _isDailyChallege = isDailyChallege;
    auto bg = Layout::create();//LayerColor::create(Color4B(0, 0, 0, 175));
    bg->setTouchEnabled(true);
    bg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    bg->setContentSize(winSize);
    bg->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    bg->setBackGroundColor(Color3B::BLACK);
    bg->setBackGroundColorOpacity(175);
    bg->addClickEventListener([=](Ref *){
        this->onTap(this, -1);
        this->removeFromParent();
    });
    bg->setPosition(Vec2::ZERO);
    addChild(bg, 0);
    _bg = bg;
    
    auto panel = ImageView::create("9_greenwindow_s.png");
    panel->setTag(123);
    panel->setScale9Enabled(true);
    panel->setContentSize(Size(500, _isDailyChallege ? 420 : 550));
    panel->setPosition(VisibleRect::center() + Vec2(0, 0));
    addChild(panel, 0);
    _panel = panel;
    
    int num = _isDailyChallege ? 3 : 5;
    
    Layout *layout = Layout::create();
    layout->setContentSize(Size(280, (75 + 10) * num + 10));
    layout->setLayoutType(Layout::Type::VERTICAL);
    addChild(layout);
    layout->setPosition(VisibleRect::center() + Vec2(-140, _isDailyChallege ? (-115) : -200));
    _layout = layout;
    
    std::string names[] = {LocalizedString("TID_CODE_NEW GAME"), LocalizedString("TID_UI_REPLAY"), LocalizedString("TID_UI_WINNING DEAL"), LocalizedString("TID_UI_STATISTICS"), LocalizedString("TID_UI_SCORES")};
    std::string dailyNames[] = {LocalizedString("TID_UI_PLAY AGAIN"), LocalizedString("TID_UI_EXIT"), LocalizedString("TID_CODE_BACK")};
    Vector<Node *> nodes;
    for (int i = 0; i < num; i++) {
        auto nor = "B_green.png";
        auto sel = "B_green0.png";
        bool flag = (!isDailyChallege) && (i > 2);
        if(flag){
            nor = "B_yellow.png";
            sel = "B_yellow1.png";
        } else if (isDailyChallege){
            if(i == 1){
                nor = "B_orange.png";
                sel = "B_orange1.png";
            } else if (i == 2){
                nor = "B_green1.png";
                sel = "B_green2.png";
            }
        }
        auto btn = KTFactory::createButton(nor, sel, "",  isDailyChallege ? dailyNames[i] : names[i], 30, "Arial", Size(280, 75));
        //        btn->setPosition(VisibleRect::center() + Vec2(0, -132));
        btn->setTag(i);
        
        auto label = btn->getTitleRenderer();
        float offX = 0;
        if(flag ){
            if(i == 3){
                offX = 20;
                //scores.png
                Sprite *sp = Sprite::create("scores.png");
                btn->addChild(sp);
                sp->setPosition(50,label->getPositionY()+4);
            } else if (i == 4){
                //statistics.png
                offX = 20;
                Sprite *sp = Sprite::create("statistics.png");
                btn->addChild(sp);
                sp->setPosition(50,label->getPositionY()+4);
            }
        }
        label->setPosition(label->getPosition() + Vec2(offX, 4));
        label->enableShadow(Color4B::GRAY);
        
        btn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
            if (type == Widget::TouchEventType::ENDED && onTap) {
                Node *n = (Node *)pSender;
                onTap(this, n->getTag());
                this->removeFromParentAndCleanup(true);
            }
        });
        layout->addChild(btn);
        nodes.pushBack(btn);
        _items.pushBack(btn);
        LinearLayoutParameter* lp1 = LinearLayoutParameter::create();
        btn->setLayoutParameter(lp1);
        lp1->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
        lp1->setMargin(Margin(0.0f, 10.f, 0.0f, 10.0f));
    }
    
    
    return true;
}


#pragma mark - KLWinLayer -

KLWinLayer::~KLWinLayer(){
//    DataManager::getInstance()->getStarPool()->despawnAll();


}

void KLWinLayer::updateLayout(){
    _bg->setContentSize(winSize);
    for (auto n : _lights) {
        n->setPosition(VisibleRect::center() + Vec2(0, 10));
    }
    _panel->setPosition(VisibleRect::center() + Vec2(0, 0));
    _titleLbl->setPosition(VisibleRect::center() + Vec2(0, 145));
    for(int i = 0; i < 3; i++) {
        
        {
            Label *titleLbl = labels.at(i);
            
            titleLbl->setPosition(VisibleRect::center() + Vec2(-50, (newRecord ? 75 : 45) - i * 44.f));
        }
        
        {
            Label *titleLbl = detailLabels.at(i);
            
            titleLbl->setPosition(VisibleRect::center() + Vec2(10, (newRecord ? 75 : 45) - i * 44.f));
            
        }
    }
    
    if (DataManager::getInstance()->getDailyChanllengeSuccess()) {
        
        _titleLbl2->setPosition(VisibleRect::center() + Vec2(0,85));
        _clostBtn->setPosition(VisibleRect::center() + Vec2(196, 145));
        
        _shareBtn->setPosition(VisibleRect::center() + Vec2(0, -180));
        _leaderBtn->setPosition(VisibleRect::center() + Vec2(110, -100));
        
        _panel->setPositionY(_panel->getPositionY() - 35);
    }
    
    if (newRecord) {
        _scoreBtn->setPosition(VisibleRect::center() + Vec2(-145, -100));
    }
    
    if(newRecord || DataManager::getInstance()->getDailyChanllengeSuccess()) {
        DataManager::getInstance()->getStarPool()->despawnAll();
        KLUtils::addStars(this, Rect(100 + winSize.width/2 - 320, VisibleRect::center().y - 40, 440, 130));
    }
    
    againButton->setPosition(VisibleRect::center() + Vec2(newRecord ? 90 : (DataManager::getInstance()->getDailyChanllengeSuccess() ? -110 : 0), -100));
}

bool KLWinLayer::init(HighScoreModel *m){
    if (!KTPauseLayer::init()) {
        return false;
    }
    
    size_t recordIdx = HighScoreManager::getInstance()->getDataArray().getIndex(m);
    newRecord = recordIdx != -1;
    
    auto bg = LayerColor::create(Color4B(0, 0, 0, 175));
    addChild(bg, -2);
    _bg = bg;
    
    if (newRecord || DataManager::getInstance()->getDailyChanllengeSuccess()) {
        for (int i = 0; i < 2; i++) {
            int sig = (i - 0.5f < 0) ? -1 : 1;
            auto light = Sprite::create("biglight_1.png");
            light->setScale(3);
            light->setPosition(VisibleRect::center() + Vec2(0, 10));
            auto ac = RotateBy::create(1, 45 * sig);
            light->runAction(RepeatForever::create(ac));
            addChild(light,-2);
            _lights.pushBack(light);
        }
    }

    auto panel = ImageView::create("9_greenwindow.png");
    panel->setTag(123);
    panel->setScale9Enabled(true);
    panel->setContentSize(Size(550, 400));
    panel->setPosition(VisibleRect::center() + Vec2(0, 0));
    addChild(panel, -1);
    _panel = panel;
    
    {
        
        Label *titleLbl = Label::createWithSystemFont(newRecord ? LocalizedString("TID_UI_NEW RECORD") : (DataManager::getInstance()->getDailyChanllengeSuccess() ? LocalizedString("TID_UI_VICTORY!") :LocalizedString("TID_UI_YOU WON!") ), "Arial", 36);
        titleLbl->setTextColor(Color4B::WHITE);
        titleLbl->setAlignment(TextHAlignment::CENTER);
        titleLbl->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        titleLbl->setPosition(VisibleRect::center() + Vec2(0, 145));
        addChild(titleLbl);
        _titleLbl = titleLbl;
    }
    std::string names[] = {LocalizedString("TID_CODE_VEGAS_SCORE_WITH_VALUE"), LocalizedString("TID_CODE_MOVES_WITH_VALUE"), LocalizedString("TID_UI_RANK1")};
    int nums[] = {m->getScore(), m->getMoveNum(), static_cast<int>(recordIdx) + 1};
    char str[64];
    
    
    for(int i = 0; i < 3; i++) {
    
        {
            Label *titleLbl = Label::createWithSystemFont(names[i], "Arial", 30);
            titleLbl->setTextColor(KL_GREEN);
            titleLbl->setAlignment(TextHAlignment::CENTER);
            titleLbl->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            titleLbl->setPosition(VisibleRect::center() + Vec2(-50, (newRecord ? 75 : 45) - i * 44.f));
            addChild(titleLbl);
            
            if (!newRecord && i == 2) {
                titleLbl->setVisible(false);
            }
            labels.pushBack(titleLbl);
        }
        
        {
            sprintf(str, "%d", nums[i]);
            Label *titleLbl = Label::createWithSystemFont(str, "Arial", 30);
            titleLbl->setTextColor(Color4B::WHITE);
            titleLbl->setAlignment(TextHAlignment::LEFT);
            titleLbl->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            titleLbl->setPosition(VisibleRect::center() + Vec2(10, (newRecord ? 75 : 45) - i * 44.f));
            addChild(titleLbl);
            
            if (!newRecord && i == 2) {
                titleLbl->setVisible(false);
            }
            detailLabels.pushBack(titleLbl);
        }
    }
    
    if (DataManager::getInstance()->getDailyChanllengeSuccess()) {
        labels.back()->setString(LocalizedString("TID_UI_TIMES:"));
        labels.back()->setVisible(true);
        sprintf(str, "%0.2d:%0.2d", m->getTime()/60, m->getTime()%60);
        detailLabels.back()->setString(str);
        detailLabels.back()->setVisible(true);
        {
            string timeStr = m->getChallengeDate()->toString();
            timeStr = timeStr.substr(2,8);
            sprintf(str, "%s %s", LocalizedString("TID_UI_DATE").c_str(), timeStr.c_str());
            Label *titleLbl = Label::createWithSystemFont(str, "Arial", 30);
            titleLbl->setTextColor(KL_GREEN);
            titleLbl->setAlignment(TextHAlignment::CENTER);
            titleLbl->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            titleLbl->setPosition(VisibleRect::center() + Vec2(0,85));
            addChild(titleLbl);
            _titleLbl2 = titleLbl;
        }
        
        
        auto btn = KTFactory::createButton("close.png", "close1.png", "",  "", 30);
        //        btn->setPosition(VisibleRect::center() + Vec2(0, -132));
        
//        btn->setContentSize(Size(280, 75));
        btn->setPosition(VisibleRect::center() + Vec2(196, 145));
        btn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
            if (type == Widget::TouchEventType::ENDED) {
                this->removeFromParentAndCleanup(true);
            }
        });
        addChild(btn);
        _clostBtn = btn;
    }
    
    auto btn = KTFactory::createButton("B_green1.png", "B_green2.png", "",  LocalizedString("TID_CODE_NEW GAME"), 30, "Arial", Size(280, 75));
    btn->setTag(0);
    auto label = btn->getTitleRenderer();
    label->setPosition(label->getPosition() + Vec2(0, 4));
    label->enableShadow(Color4B::GRAY);
    btn->setPosition(VisibleRect::center() + Vec2(newRecord ? 90 : (DataManager::getInstance()->getDailyChanllengeSuccess() ? -110 : 0), -100));
    btn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            Node *n = (Node *)pSender;
            if (onTap) {
                onTap(this, n->getTag());
            }
            this->removeFromParentAndCleanup(true);
        }
    });
    addChild(btn);
    againButton = btn;
    
    if(DataManager::getInstance()->getDailyChanllengeSuccess()){
    
        panel->setContentSize(Size(550, 470));
        panel->setPositionY(panel->getPositionY() - 35);
        
        
        auto btn = KTFactory::createButton("B_green.png", "B_green0.png", "",  LocalizedString("TID_UI_SHARE1"), 30, "Arial", Size(280, 75));
        auto label = btn->getTitleRenderer();
        label->setPosition(label->getPosition() + Vec2(0, 4));
        label->enableShadow(Color4B::GRAY);
        btn->setPosition(VisibleRect::center() + Vec2(0, -180));
        btn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
            if (type == Widget::TouchEventType::ENDED) {
                iOSWrapper::showShareUI();
            }
        });
        addChild(btn);
        _shareBtn = btn;
        
        againButton->setContentSize(Size(210, 75));
        
        btn = KTFactory::createButton("B_green1.png", "B_green2.png", "",  LocalizedString("TID_UI_LEADERBOARD"), 30, "Arial", Size(210, 75));
        btn->setTag(5);
        label = btn->getTitleRenderer();
        label->setPosition(label->getPosition() + Vec2(0, 4));
        label->enableShadow(Color4B::GRAY);
        btn->setPosition(VisibleRect::center() + Vec2(110, -100));
        btn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
            if (type == Widget::TouchEventType::ENDED && onTap) {
                Node *n = (Node *)pSender;
                onTap(this, n->getTag());
                this->removeFromParentAndCleanup(true);
            }
        });
        addChild(btn);
        _leaderBtn = btn;
    }
    
    
    if (newRecord) {
        auto btn = KTFactory::createButton("B_yellow.png", "B_yellow1.png", "",  LocalizedString("TID_UI_SCORES"), 30, "Arial", Size(175, 75));
        btn->setTag(1);
        auto label = btn->getTitleRenderer();
        label->setPosition(label->getPosition() + Vec2(0, 4));
        label->enableShadow(Color4B::GRAY);
        btn->setPosition(VisibleRect::center() + Vec2(-145, -100));
        btn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
            if (type == Widget::TouchEventType::ENDED && onTap) {
                Node *n = (Node *)pSender;
                onTap(this, n->getTag());
//                this->removeFromParentAndCleanup(true);
            }
        });
        addChild(btn);
        _scoreBtn = btn;
        
        
    }
    
    if(newRecord || DataManager::getInstance()->getDailyChanllengeSuccess()) {
        KLUtils::addStars(this, Rect(100, VisibleRect::center().y - 40, 440, 130));
    }
    
    return true;
}

#pragma mark - KLDailyInfoLayer -

bool KLDailyInfoLayer::onTouchBegan(Touch *touch, Event *unused_event){
    
    auto panel = getChildByTag(123);
    if (!panel->boundingBox().containsPoint(touch->getLocation())) {
        //quit
        removeFromParent();
    }
    
    return true;
}

void KLDailyInfoLayer::updateLayout(){

    _bg->setContentSize(winSize);
    _panel->setPosition(VisibleRect::center() + Vec2(0, 0));
    _titleLbl->setPosition(VisibleRect::center() + Vec2(0, 285));
    _bg2->setPosition(VisibleRect::center() + Vec2(0, 175));
    _titleLbl2->setPosition(VisibleRect::center() + Vec2(0, 175));
    _box->setPosition(Vec2((VisibleRect::getVisibleRect().size-_box->getContentSize())/2) + Vec2(0, -50));
    _submitBtn->setPosition(VisibleRect::center() + Vec2(0, -250));
}

bool KLDailyInfoLayer::init(){

    if (!KTPauseLayer::init()) {
        return false;
    }
    
    auto bg = LayerColor::create(Color4B(0, 0, 0, 175));
    addChild(bg, -2);
    _bg = bg;
    
    auto panel = ImageView::create("9_greenwindow.png");
    panel->setTag(123);
    panel->setScale9Enabled(true);
    panel->setContentSize(Size(680, 680));
    panel->setPosition(VisibleRect::center() + Vec2(0, 0));
    addChild(panel, -1);
    _panel = panel;
    
    {
        
        auto *titleLbl =Text::create(LocalizedString("TID_UI_DAILY CHALLENGE"), "Arial", 36);
        // Label::createWithSystemFont("每日挑战", "Arial", 36);
        titleLbl->setTextColor(Color4B::WHITE);
        titleLbl->setTextHorizontalAlignment(TextHAlignment::CENTER);
        titleLbl->setTextVerticalAlignment(TextVAlignment::CENTER);
        titleLbl->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        titleLbl->setPosition(VisibleRect::center() + Vec2(0, 285));
        addChild(titleLbl);
        _titleLbl = titleLbl;
    }
    {
        auto bg = ImageView::create("9_line_white.png");
        bg->setContentSize(Size(614, 140));
        bg->setScale9Enabled(true);
//        bg->setScaleX(614.f/534.f);
//        bg->setScaleY(140.f/2.f);
        bg->setColor(Color3B::BLACK);
        bg->setOpacity(100);
        bg->setPosition(VisibleRect::center() + Vec2(0, 175));
        addChild(bg);
        _bg2 = bg;
    }
    {
        
        auto *titleLbl = Text::create(LocalizedString("TID_UI_BASIC CHALLENGE HELP DESC"), "Arial", 24);
        
        titleLbl->ignoreContentAdaptWithSize(false);
        titleLbl->setContentSize(Size(560, 100));
        
        titleLbl->setTextColor(KL_GREEN);
        titleLbl->setTextHorizontalAlignment(TextHAlignment::LEFT);
        titleLbl->setTextVerticalAlignment(TextVAlignment::CENTER);
        titleLbl->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        titleLbl->setPosition(VisibleRect::center() + Vec2(0, 175));
        addChild(titleLbl);
        _titleLbl2 = titleLbl;
    }
    
//    Vector<Node *>nodes;
    auto box = VBox::create(Size(614, 300));
    box->setPosition(Vec2((VisibleRect::getVisibleRect().size-box->getContentSize())/2) + Vec2(0, -50));
    _box = box;
    addChild(box);
    string images[] = {"king0.png", "king.png", "badge1.png"};
    string texts[] = {LocalizedString("TID_UI_BASIC CHALLENGE HELP1"),LocalizedString("TID_UI_BASIC CHALLENGE HELP2"),LocalizedString("TID_UI_BASIC CHALLENGE HELP2")};
    for (int i = 0; i < 3; i++) {
        auto layout = Layout::create();
        layout->setContentSize(Size(614, 100));
        
        auto image = ImageView::create(images[i]);
        image->setScale(0.6);
        image->setPosition(Vec2(80, 50));
        layout->addChild(image);
        
        auto text = Text::create(texts[i], "Arial", 24);
        text->ignoreContentAdaptWithSize(false);
        text->setContentSize(Size(420, 60));
        
        text->setTextHorizontalAlignment(TextHAlignment::LEFT);
        text->setTextVerticalAlignment(TextVAlignment::CENTER);
        text->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        text->setPosition(Vec2(130, 50));
        layout->addChild(text);
        
        auto line = Scale9Sprite::create("9_line_white.png");
        line->setScale9Enabled(true);
        line->setContentSize(Size(614, line->getContentSize().height));
        line->setOpacity(100);
        line->setPosition(line->getContentSize()/2);
        
        layout->addChild(line);
//        nodes.pushBack(line);
        box->addChild(layout);
    }
    
    auto btn = KTFactory::createButton("B_green.png", "B_green0.png", "",  LocalizedString("TID_UI_CONFIRM"), 30, "Arial", Size(280, 75));
    //        btn->setPosition(VisibleRect::center() + Vec2(0, -132));
    btn->setTag(0);
    
//    btn->setContentSize(Size(280, 75));
    auto label = btn->getTitleRenderer();
    label->setPosition(label->getPosition() + Vec2(0, 4));
    label->enableShadow(Color4B::GRAY);
    btn->setPosition(VisibleRect::center() + Vec2(0, -250));
    btn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            this->removeFromParentAndCleanup(true);
        }
    });
    addChild(btn);
    _submitBtn = btn;
    
    return true;
}

#pragma mark - KLAllClearLayer -

void KLAllClearLayer::updateLayout(){

    _bg->setContentSize(winSize);
    _starParent->setPosition(Vec2(0, 125) + VisibleRect::center());
    _root->setPosition(VisibleRect::center());
}

bool KLAllClearLayer::init(){

    if (!KTPauseLayer::init()) {
        return false;
    }
    
    auto bg = LayerColor::create(Color4B(0, 0, 0, 175));
    addChild(bg, -2);
    _bg = bg;
    
    auto starParent = Node::create();
    starParent->setPosition(Vec2(0, 125) + VisibleRect::center());
    starParent->setScale(0);
    addChild(starParent, 0);
    _starParent = starParent;
    //jump 1.8s
    
    for (int i = 0; i < 20; i++) {
        auto star = Sprite::create("star_w.png");
        starParent->addChild(star);
        auto v = Vec2(random(-1.f, 1.f), random(-1.f, 1.f)).getNormalized();
        star->setPosition(Vec2(-50, 0) + v * random(20.f, 50.f));
        star->setScale(random(1.2f, 1.6f));
        star->setRotation(random(0, 360));
        star->setColor(Color3B(255, 255, 176));
        leftStars.pushBack(star);
    }
    
    for (int i = 0; i < 20; i++) {
        auto star = Sprite::create("star_w.png");
        starParent->addChild(star);
        auto v = Vec2(random(-1.f, 1.f), random(-1.f, 1.f)).getNormalized();
        star->setPosition(Vec2(50, 0) + v * random(20.f, 50.f));
        star->setScale(random(1.2f, 1.6f));
        star->setRotation(random(0, 360));
        star->setColor(Color3B(255, 255, 176));
        rightStars.pushBack(star);
    }
    
    auto ac1 = ScaleTo::create(0.1f, 1);
    auto callback = CallFunc::create([=](){
    
        for(auto star : leftStars) {
            auto pos = star->getPosition();
            float sc = abs(pos.x)/100.f;
            float sc2 = (pos.y + 20)/40.f;
            float ht = 300+100*sc2;
            pos.x = -sc * 640.f;
            pos.y = -20-starParent->getPositionY();
            auto ac = JumpTo::create(1.5 + 0.3 * sc2, pos, ht, 1);
            star->runAction(ac);
        }
        
        for(auto star : rightStars) {
            auto pos = star->getPosition();
            float sc = abs(pos.x)/100.f;
            float sc2 = (pos.y + 20)/40.f;
            float ht = 300+100*sc2;
            pos.x = sc * 640.f;
            pos.y = -20-starParent->getPositionY();
            auto ac = JumpTo::create(1.5 + 0.3 * sc2, pos, ht, 1);
            star->runAction(ac);
        }
    });
    starParent->runAction(Sequence::create(ac1, callback, DelayTime::create(2), CallFunc::create([=](){
        
        if (onRemove) {
            onRemove();
        }
        this->removeFromParent();
    }), NULL));
    
    auto root = Node::create();
    root->setPosition(VisibleRect::center());
    addChild(root, 1);
    _root = root;
    
    auto icon = Sprite::create("JS_02.png");
    root->addChild(icon, 2);
    
    auto label = Label::createWithSystemFont("ALL\nCLEARED!", "Arial", 40);
    label->enableOutline(Color4B(159, 91, 23, 255), 3);
    label->setContentSize(Size(320, 125));
    label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    label->setPosition(Vec2(icon->getContentSize()/2) + Vec2(0, 5));
    icon->addChild(label);
    
    root->setScale(0.001);
    {
        auto ac1 = ScaleBy::create(1.f, 1000);
        auto ac2 = EaseElasticOut::create(ac1);
        
        
        auto seq = Sequence::create(ac2, ac2->reverse(), NULL);
        root->runAction(seq);
    }
    
    char str[64];
    Vec2 anchors[] = {Vec2::ANCHOR_MIDDLE_RIGHT, Vec2::ANCHOR_BOTTOM_RIGHT, Vec2::ANCHOR_TOP_RIGHT};
    //3 stars
    for (int i = 0; i < 2; i++) {
        
        for (int j = 0; j < 3; j++) {
            sprintf(str, "JS_03_%d.png", j + 1);
            auto sp = Sprite::create(str);
            sp->setAnchorPoint(anchors[j]);
            int sig = i == 0 ? 1 : -1;
            sp->setScale(0.01);
            sp->setPosition(VisibleRect::center() + Vec2(sig * (-120), 0) + (j == 1 ? (Vec2(20 * sig, -20)) : Vec2::ZERO));
            addChild(sp, 0);
            
            auto ac = ScaleBy::create(0.2f, sig * 100, 100);
            auto ac2 = EaseElasticOut::create(ac);
            auto seq = Sequence::create(Hide::create(), DelayTime::create(0.2f + 0.05 * j),Show::create(), ac2, NULL);
            sp->runAction(Sequence::create(seq, DelayTime::create(1), seq->reverse(), NULL));
        }
        
    }
    
    
    //ace
    auto offset = Vec2(24,-200);//-220, 100
    auto center = offset ;
    auto start = offset + Vec2(-170, 236);
    for (int i = 0; i < 4; i++) {
        //ace
        Trump *card = Trump::create(TrumpData::create(i, 1, false));
        start.rotate(center, CC_DEGREES_TO_RADIANS(-12.5));
        
        auto v = (start-center);
        
        auto posEnd = start + v.getNormalized() * 2 * i;
        float radian = v.getAngle();
        float angle = 90 - CC_RADIANS_TO_DEGREES(radian);
        auto rotationEnd = angle;
        
        auto posStart = start + Vec2(32, -62);
        auto rotationStart = rotationEnd + 34;
        card->setPosition(posStart);
        card->setRotation(rotationStart);
        root->addChild(card);
        
        auto sp = Spawn::create(MoveBy::create(0.2, posEnd-posStart), RotateBy::create(0.2, rotationEnd - rotationStart), NULL);
        auto easeBack = EaseBackOut::create(sp);
        auto show = Show::create();
        auto seq = Sequence::create(Hide::create(), DelayTime::create(0.1 * (4 - i)), show, easeBack, NULL);
        
        
        card->runAction(Sequence::create(seq, DelayTime::create(0.9f), Sequence::create(easeBack->reverse(), show->reverse(), NULL), NULL));
        
    }
    
    
    return true;
}
