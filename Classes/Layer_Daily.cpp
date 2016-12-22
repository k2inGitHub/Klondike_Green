//
//  Layer_Daily.cpp
//  Klondike
//
//  Created by 宋扬 on 16/7/14.
//
//

#include "Layer_Daily.hpp"
#include "HttpManager.hpp"
#include "KLPopupLayer.hpp"
#include "SolitaireKlondikeScene.h"
#include "Localization.hpp"
#include "Layer_Leaderboard.hpp"
using namespace std;

Layer_Daily::Layer_Daily():
//_requestDate(nullptr),
_historyTotal(0),
_selectedDayIdx(0){

}

Layer_Daily::~Layer_Daily(){
//    CC_SAFE_RELEASE_NULL(_requestDate);
    DataManager::getInstance()->getStarPool()->despawnAll();
}

void Layer_Daily::popToLeaderBoard(){
    auto sc = Scene::create();
    
    int offset = getDayOffset(getCurrentMonth(), _selectedButton);
    auto requestDate = DataManager::getInstance()->getRequstData();
    KTDate *utcDate = requestDate->utcDate()->dateByAddingTimeInterval(offset * 24 * 60 * 60);
    auto la = Layer_Leaderboard::create(utcDate->stamp());
    sc->addChild(la);
    Director::getInstance()->pushScene(sc);
}

void Layer_Daily::updateLayout(){

    HLAdManagerWrapper::hideBanner();
    
    _bg->setScale(winSize.width/640, winSize.height/1136);
    _bg->setPosition(VisibleRect::center());
    
    float margin = 30;
    float ht = getNavigationBar()->getContentSize().height + margin;
    
    float centerX = winSize.width/2;
    bool islandscape = winSize.height < winSize.width;
    if (islandscape) {
        //landscape
        centerX = winSize.width/2 - 160;
    }
    
    _leftBtn->setPosition(VisibleRect::leftTop() + Vec2(centerX - 220, -ht));
    _rightButton->setPosition(VisibleRect::leftTop() + Vec2(centerX + 160, -ht));
    _monthLabel->setPosition(VisibleRect::leftTop() + Vec2(centerX, -ht));
    
    auto pos = VisibleRect::leftTop() + Vec2(centerX + 240, -ht);
    _rankBg->setPosition(pos);
    _rankButton->setPosition(pos + Vec2(0, 3));
    _badgeIcon->setPosition(VisibleRect::leftTop() + Vec2(centerX-135, -ht));
    
    ht += 40;
    
    horizonAliament(_labels, 26, VisibleRect::top().y - ht);
    float dt = winSize.width/2 - centerX;
    for(auto n : _labels){
        n->setPositionX(n->getPositionX()-dt);
    }
    
//    _red1->setPosition(VisibleRect::leftTop() + Vec2(centerX, -135));
    _white1->setPosition(VisibleRect::leftTop() + Vec2(centerX, -410));
    
    ht += 60;
    
    for (int i = 0; i < 6; i++) {
        auto nodes = _dailyButtons.at(i);
        
        horizonAliament(nodes, 24, VisibleRect::top().y - ht);
        
        for(auto n : nodes){
            n->setPositionX(n->getPositionX()-dt);
        }
        ht += 56;
    }
    
    ht += 22;
    
    _greenBg->setPosition(VisibleRect::leftTop() + Vec2(centerX, -ht));
    _greenCrown->setPosition(VisibleRect::leftTop() + Vec2(centerX-227, -ht));
    _greenProgressBg->setPosition(VisibleRect::leftTop() + Vec2(centerX+50, -ht));
    _progressBar->setPosition(VisibleRect::leftTop() + Vec2(centerX+50, -ht));
    _badgeRoot->setPosition(_progressBar->getPosition() + Vec2(-5, -23));
    
    if (!islandscape){
        
        ht += 110;
        _dayLabel->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        _dayLabel->setPosition(VisibleRect::leftTop() + Vec2(centerX-120, -ht-10));
        _dayCrown->setPosition(VisibleRect::leftTop() + Vec2(centerX + 45, -ht-10));
        
        _historyLabel->setHorizontalAlignment(TextHAlignment::CENTER);
        _historyLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _historyLabel->setPosition(VisibleRect::top() + Vec2(178, -ht-10));
        
        ht += 90;
        _white2->setPosition(VisibleRect::top() + Vec2(0, -ht));
        _white2->setContentSize(Size(600, 240));
        ht += 20;
        _dayBgGreen->setContentSize(Size(540, 135));
        _dayBgGreen->setPosition(VisibleRect::top() + Vec2(0, -ht));
        _unlockImg->setPosition(VisibleRect::top() + Vec2(-170, -ht+13));
        _unlockLabel->setPosition(VisibleRect::top() + Vec2(-170, -ht - 48));
        _startButton->setPosition(VisibleRect::top() + Vec2(100, -ht - 8));
        
    } else {
        float margin = 50;
        float ht = getNavigationBar()->getContentSize().height + margin;
        centerX = winSize.width/2 + 300;
        
        _dayCrown->setPosition(VisibleRect::leftTop() + Vec2(centerX - 130, -ht));
        _historyLabel->setHorizontalAlignment(TextHAlignment::LEFT);
        _historyLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        _historyLabel->setPosition(VisibleRect::leftTop() + Vec2(centerX - 90 + 33, -ht));
        
        
        ht += 90;
        
        _dayLabel->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        _dayLabel->setPosition(VisibleRect::leftTop() + Vec2(centerX, -ht));
        
        _white2->setPosition(VisibleRect::leftTop() + Vec2(centerX, -410));
        _white2->setContentSize(Size(320, 470));
        
        ht += 220;
        
        _dayBgGreen->setContentSize(Size(300, 350));
        _dayBgGreen->setPosition(VisibleRect::leftTop() + Vec2(centerX, -ht));
        _unlockImg->setPosition(VisibleRect::leftTop() + Vec2(centerX, -ht+80));
        _unlockLabel->setPosition(VisibleRect::leftTop() + Vec2(centerX, -ht));
        _startButton->setPosition(VisibleRect::leftTop() + Vec2(centerX, -ht - 100));
        
    }
    
    
    
    
}

bool Layer_Daily::init(KTPauseLayerDelegate *delegate){

    if (!KTPauseLayer::init(delegate)) {
        return false;
    }
    
    HLAdManagerWrapper::hideBanner();
    
    auto visibleSize = VisibleRect::getVisibleRect().size;
    auto sprite = Sprite::create(BG_FILENAME);
    
    sprite->setPosition(VisibleRect::center());
    this->addChild(sprite, -100);
    _bg = sprite;
    
//    setNavigationBarEnable(true);
//    auto bar = getNavigationBar();
//    bar->setTitle("每日挑战");
    
    Vector<MenuItem *>leftItems;
    auto item1 = MenuItemImage::create("B_help.png", "B_help1.png");
    item1->setCallback([=](Ref *){
        auto la = KLDailyInfoLayer::create();
        this->addChild(la, 2048);
    });
    leftItems.pushBack(item1);
    auto item2 = MenuItemImage::create("Daily.png", "Daily.png");
    leftItems.pushBack(item2);
    
//    bar->setLeftBarItems(leftItems);
    
    Vector<MenuItem *>rightItems;
    auto itemQuit = MenuItemImage::create("close.png", "close1.png", [=](Ref *sender){
        this->doQuit();
        Director::getInstance()->popScene();
//        HLAdManagerWrapper::showBanner();
        
        if (DataManager::getInstance()->getChanlleged()) {
            __NotificationCenter::getInstance()->postNotification(NTFFactor::NTF_NEW_GAME_WINNALBE);
            DataManager::getInstance()->setChanlleged(false);
        }
    });
    rightItems.pushBack(itemQuit);
    
    auto item23 = MenuItem::create();
    item23->setContentSize(item2->getContentSize());
    rightItems.pushBack(item23);
    
    addNavigationBar(LocalizedString("TID_UI_DAILY CHALLENGE"), leftItems, rightItems);
    
    float margin = 40;
    float ht = getNavigationBar()->getContentSize().height + margin;
    
    auto leftBtn = Button::create("B_arrow.png", "B_arrow.png");
    leftBtn->addTouchEventListener(CC_CALLBACK_2(Layer_Daily::touchEvent, this));
    leftBtn->setPosition(VisibleRect::leftTop() + Vec2(100, -ht));
    addChild(leftBtn, 2, 10);
    _leftBtn = leftBtn;
    {
        //right
        auto leftBtn = Button::create("B_arrow.png", "B_arrow.png");
        leftBtn->setScaleX(-1);
        leftBtn->addTouchEventListener(CC_CALLBACK_2(Layer_Daily::touchEvent, this));
        leftBtn->setPosition(VisibleRect::rightTop() + Vec2(-160, -ht));
        addChild(leftBtn, 2, 20);
        _rightButton = leftBtn;
    }
    _monthLabel = Label::createWithSystemFont(LocalizedString(monthIDs[0]), "Arial", 36);
    _monthLabel->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    _monthLabel->setOverflow(Label::Overflow::SHRINK);
    _monthLabel->setContentSize(Size(270, 65));
    _monthLabel->setDimensions(270, 65);
    _monthLabel->setPosition(VisibleRect::top() + Vec2(0, -ht));
    addChild(_monthLabel, 2);
    
    {
        auto pos = VisibleRect::rightTop() + Vec2(-80, -ht);
        auto bg = ImageView::create("daily_white.png");
        bg->setScale9Enabled(true);
        bg->setContentSize(Size(68, 68));
        bg->setPosition(pos);
        addChild(bg);
        _rankBg = bg;
        
        auto leftBtn = Button::create("btn_rankings1.png", "btn_rankings2.png");
        leftBtn->addClickEventListener([=](Ref *){
            this->popToLeaderBoard();
        });
        leftBtn->setPosition(pos + Vec2(0, 3));
        addChild(leftBtn, 2, 20);
        _rankButton = leftBtn;
    }
    
    _badgeIcon = ImageView::create("badge_back.png");
    _badgeIcon->setScale(0.5f);
    _badgeIcon->setPosition(VisibleRect::top() + Vec2(-135, -ht));
    addChild(_badgeIcon);
    //13
    ht += 40;
    {
        std::string names[7] = {LocalizedString("TID_UI_SUN"), LocalizedString("TID_UI_MON"),LocalizedString("TID_UI_TUE"),LocalizedString("TID_UI_WED"),LocalizedString("TID_UI_THU"),LocalizedString("TID_UI_FRI"),LocalizedString("TID_UI_SAT")};
        for (int i = 0; i < 7; i++) {
            CCLOG("week str = %s", names[i].c_str());
            Label *l = Label::createWithSystemFont(names[i], "Arial", 20);
            l->setOverflow(Label::Overflow::SHRINK);
            l->setDimensions(55, 28);
            l->setContentSize(Size(55, 28));
//            float w = l->getContentSize().width;
            addChild(l);
            _labels.pushBack(l);
        }
        
        horizonAliament(_labels, 26, VisibleRect::top().y - ht - 10);
        for(int i = 0;i < 7;i++){
            auto p = _labels.at(i)->getPosition();
            CCLOG("%s", StringUtils::format("label %d pos = %s", i, CCStringFromPoint(p)->getCString()).c_str());
        }
    }
    
    {
        
        auto bg = ImageView::create("_whiteAlpha.png");
        bg->setPosition(VisibleRect::top() + Vec2(0, -420));
        bg->setScale9Enabled(true);
        bg->setContentSize(Size(600, 470));
        addChild(bg , -2);
        _white1 = bg;
    }
    //21

    ht += 60;
    
    for (int i = 0; i < 6; i++) {
        Vector<cocos2d::Node *>nodes;
        _dailyButtons.push_back(nodes);
        for (int j = 0; j < 7; j++) {
            auto btn = Button_Daily::create();
            btn->setDay(j);
//            btn->setPast(true);
//            btn->setEmpty(true);
            _buttons.pushBack(btn);
            _dailyButtons[i].pushBack(btn);
            addChild(btn, 3);
            
            btn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
                if (type == Widget::TouchEventType::ENDED) {
                    _buttons.at(_selectedDayIdx)->setSelected(false);
                    auto button = (Button_Daily *)pSender;
                    button->setSelected(true);
                    _selectedDayIdx = (int)_buttons.getIndex(button);
                    this->onSelectButton(button);
                }
            });
        }
        horizonAliament(_dailyButtons[i], 24, VisibleRect::top().y - ht);
        ht += 56;
    }
    
    ht += 22;
    
    auto bg = ImageView::create("bg_green_daily.png");
    bg->setPosition(VisibleRect::top() + Vec2(0, -ht));
    bg->setScale9Enabled(true);
    bg->setContentSize(Size(540, 78));
    addChild(bg , -1);
    _greenBg = bg;
    
    {
        auto _crown = ImageView::create("king0.png");
        _crown->setScale(64.f/131.f);
        addChild(_crown);
        _crown->setPosition(VisibleRect::top() + Vec2(-227, -ht));
        _greenCrown = _crown;
    }
    
    
    auto progressBg = ImageView::create("loading_line_bg.png");
    progressBg->setPosition(VisibleRect::top() + Vec2(50, -ht));
    progressBg->setScale9Enabled(true);
    progressBg->setContentSize(Size(440, 24));
    
    addChild(progressBg);
    _greenProgressBg = progressBg;
    
    _progressBar = LoadingBar::create("line_green.png", 80);
    _progressBar->setPosition(VisibleRect::top() + Vec2(50, -ht));
    _progressBar->setDirection(LoadingBar::Direction::LEFT);
    _progressBar->setScale(440.f/163.f, 24.f/21.f);
    addChild(_progressBar, 1);
    
    auto badgeRoot= Node::create();
    badgeRoot->setPosition(_progressBar->getPosition() + Vec2(-5, -23));
    badgeRoot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(badgeRoot, 2);
    _badgeRoot = badgeRoot;
    
    Vector<Node *> nodes;
    for (int i = 0; i < 4; i++) {
        if (i == 0) {
            auto n = Node::create();
            n->setContentSize(Size(114, 125));
            n->setScale(0.6f);
            badgeRoot->addChild(n);
            nodes.pushBack(n);
        } else {
            char name[64];
            sprintf(name, "badge%d.png", 4-i);
            auto n = KTFactory::createButton(name, name, "", "", 0);
            n->setScale(0.6f);
            badgeRoot->addChild(n);
            nodes.pushBack(n);
            sprintf(name, "%d", i * 10);
            auto lab = Label::createWithSystemFont(name, "arial", 28);
            lab->setPosition(Vec2(n->getContentSize().width/2, 140));
            n->addChild(lab);
            _badges.pushBack(n);
        }
    }
    
    horizonAliament(nodes, 70, 0, true);
    
    ht += 110;
    
    char str[64];
    sprintf(str, "TID_CODE_DATE%d", 1);
    sprintf(str, "%s %s",LocalizedString(monthIDs[0]).c_str(), LocalizedString(str).c_str());
    
    _dayLabel = Label::createWithSystemFont(str, "Arial", 36);
    _dayLabel->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    _dayLabel->setOverflow(Label::Overflow::SHRINK);
    _dayLabel->setContentSize(Size(280, 65));
    _dayLabel->setDimensions(280, 65);
    _dayLabel->setPosition(VisibleRect::top() + Vec2(-120, -ht-10));
    addChild(_dayLabel);
    
    {
        auto _crown = ImageView::create("king0.png");
        _crown->setContentSize(Size(48, 48));
        _crown->setScale(0.5f);
        addChild(_crown);
        _crown->setPosition(VisibleRect::top() + Vec2(45, -ht-10));
        _dayCrown = _crown;
    }
    
    
    _historyLabel = Label::createWithSystemFont(LocalizedString("TID_UI_ALL TIME:"), "Arial", 24);
    _historyLabel->setContentSize(Size(200, 42));
    _historyLabel->setDimensions(MAX(0, 200), 30);
    _historyLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    _historyLabel->setVerticalAlignment(TextVAlignment::CENTER);
    _historyLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _historyLabel->setOverflow(Label::Overflow::SHRINK);
    _historyLabel->setPosition(VisibleRect::top() + Vec2(178, -ht-10));
    addChild(_historyLabel);
    
    ht += 90;
    {
        auto bg = ImageView::create("_whiteAlpha.png");
        bg->setPosition(VisibleRect::top() + Vec2(0, -ht));
        bg->setScale9Enabled(true);
        bg->setContentSize(Size(600, 240));
        addChild(bg , -2);
        _white2 = bg;
    }
    ht += 20;
    {
        auto bg = ImageView::create("bg_green.png");
        bg->setPosition(VisibleRect::top() + Vec2(0, -ht));
        bg->setScale9Enabled(true);
        bg->setContentSize(Size(540, 135));
        addChild(bg , -1);
        _dayBgGreen = bg;
    }
    
    {
        auto _crown = ImageView::create("king_back.png");
//        _crown->setContentSize(Size(48, 48));
        addChild(_crown);
        _crown->setPosition(VisibleRect::top() + Vec2(-170, -ht+13));
        _unlockImg = _crown;
        _unlockImg->setScale(0.8f);
    }
    
    _unlockLabel = Label::createWithSystemFont(LocalizedString("TID_CODE_UNSOLVED"), "Arial", 34);
    _unlockLabel->setContentSize(Size(190, 42));
    _unlockLabel->setDimensions(MAX(0, 190), 42);
    _unlockLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    _unlockLabel->setVerticalAlignment(TextVAlignment::CENTER);
    _unlockLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _unlockLabel->setOverflow(Label::Overflow::SHRINK);
    _unlockLabel->setPosition(VisibleRect::top() + Vec2(-170, -ht - 48));
    addChild(_unlockLabel);
    
    _startButton = KTFactory::createButton("B_green.png", "B_green0.png", "", LocalizedString("TID_UI_START"), 46, "Arial-BoldMT", Size(296,100));
//    auto _label = _startButton->getTitleRenderer();
    _startButton->getTitleRenderer()->setContentSize(Size(296, 80));
    _startButton->getTitleRenderer()->setDimensions(MAX(0, 296), 80);
    _startButton->getTitleRenderer()->setVerticalAlignment(TextVAlignment::CENTER);
    _startButton->getTitleRenderer()->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    _startButton->getTitleRenderer()->setOverflow(cocos2d::Label::Overflow::SHRINK);
    _startButton->setPosition(VisibleRect::top() + Vec2(100, -ht - 8));
    
    
    
    auto label = _startButton->getTitleRenderer();
    label->setPosition(Vec2(_startButton->getContentSize()/2) + Vec2(0, 4));
    label->enableShadow(KL_GREEN_SHADOW);
    addChild(_startButton);
    _startButton->addTouchEventListener([=](Ref*,Widget::TouchEventType eventType){
        if (eventType == Widget::TouchEventType::ENDED) {
            this->startCardGame();
        }
    });
    auto rect = _startButton->getBoundingBox();
    rect.origin = Vec2::ZERO;
    auto list = KLUtils::addStars(_startButton, rect, 16);
//    for(auto sp : list){
//        sp->removeFromParent();
//        _startButton->addChild(sp);
//    }
    
    return true;
}

void Layer_Daily::onEnter(){
    KTPauseLayer::onEnter();
    requestData();
}

void Layer_Daily::startCardGame(){

    Director::getInstance()->popScene();
    __NotificationCenter::getInstance()->postNotification(NTFFactor::NTF_DAILY_GAME);
}

MonthModel* Layer_Daily::getCurrentMonth(){
    return _dataArray.at(_monthIdx);
}

void Layer_Daily::update(float dt){
//    if (_requestDate) {
//        CCLOG("request Date = %s", _requestDate->toString().c_str());
//        CCLOG(" p %p", _requestDate);
//    }
    
}

void Layer_Daily::updateUI(){
    
    auto requestDate = DataManager::getInstance()->getRequstData();
    MonthModel* model = getCurrentMonth();
    int day = 1;
    bool containsToday = false;
    
    _monthLabel->setString(model->getMonthString());
    
    char str[64];
    sprintf(str, "%s%d", LocalizedString("TID_UI_ALL TIME:").c_str(), _historyTotal);
    _historyLabel->setString(str);
    
    auto challengedDays = model->getChallengedDays();
    for (int i = 0; i < _buttons.size(); i++) {
        
        auto btn = _buttons.at(i);
        btn->reset();
        if (i < model->getFirstDayOfWeek()) {
            btn->setEmpty(true);
//            btn->setEnabled(false);
        } else {
            if (day <= model->getDays()) {
                btn->setDay(day);
                
                //??? 服务器加时间戳
//                char utcTime[64];
//                sprintf(utcTime, "%s-%0.2d 00:10:00", model->getMonth().c_str(), day);
                
                int offset = getDayOffset(model, btn);
                KTDate *localDate = requestDate->utcDate()->dateByAddingTimeInterval(offset * 24 * 60 * 60)->localDate();
                auto flag = localDate->compareDay(requestDate);
                btn->setFuture(flag == -1);
                
//                if (btn->isFuture()) {
//                    btn->setEnabled(false);
//                }
                
                if (flag == 0) {
                    containsToday = true;
                    _selectedDayIdx = i;
                }
                if (!containsToday) {
                    _selectedDayIdx = i;
                }
                
                for(auto cd : challengedDays){
                    if (cd->getDay() == day) {
                        btn->setUnlock(true);
                        btn->setGetToday(cd->getIsGetToday());
                        break;
                    }
                }
                
                day++;
            } else {
                btn->setEmpty(true);
            }
        }
    }
    auto button = _buttons.at(_selectedDayIdx);
    button->setSelected(true);
    onSelectButton(button);
    
    int total = model->getDayTotal();
    int days = model->getDays();
    float p = float(total)/float(days) * 100.f;
    _progressBar->setPercent(p);
    auto label = (Label *)_badges.back()->getChildren().at(0);
    
    sprintf(str, "%d", model->getDays());
    label->setString(str);
    
    if (total == days) {
        _badgeIcon->loadTexture("badge1.png");
        _badgeIcon->setOpacity(255);
    } else if (total >= 20) {
        _badgeIcon->loadTexture("badge2.png");
        _badgeIcon->setOpacity(255);
    } else if (total >= 10) {
        _badgeIcon->loadTexture("badge3.png");
        _badgeIcon->setOpacity(255);
    } else {
        _badgeIcon->loadTexture("badge_back.png");
        _badgeIcon->setOpacity(127);
    }
}

void Layer_Daily::onSelectButton(Button_Daily *button){
    _selectedButton = button;
    MonthModel* model = getCurrentMonth();
    _dayLabel->setString(model->getMonthDayString(button->getDay()));
    bool flag = button->isUnlock();
    if (flag) {
        if (button->isGetToday()) {
            _unlockImg->loadTexture("king0.png");
        } else {
            _unlockImg->loadTexture("king.png");
        }
        
        _unlockImg->setOpacity(255);
        _unlockLabel->setString(LocalizedString("TID_CODE_SOLVED"));
        _startButton->setTitleText(LocalizedString("TID_UI_PLAY AGAIN"));
        _startButton->getTitleRenderer()->setPosition(Vec2(_startButton->getContentSize()/2) + Vec2(0, 4));
        _startButton->loadTextures("B_orange.png", "B_orange1.png");
    } else {
        _unlockImg->loadTexture("king_back.png");
        _unlockImg->setOpacity(127);
        _unlockLabel->setString(LocalizedString("TID_CODE_UNSOLVED"));
        _startButton->setTitleText(LocalizedString("TID_UI_START"));
        _startButton->getTitleRenderer()->setPosition(Vec2(_startButton->getContentSize()/2) + Vec2(0, 4));
        _startButton->loadTextures("B_green.png", "B_green1.png");
    }
    
    DataManager::getInstance()->setDayOffset(getDayOffset(model, button));
}

int Layer_Daily::getDayOffset(MonthModel* model, Button_Daily* button){
    int num = model->getMonthValue();
    int num2 = DataManager::getInstance()->getRequstData()->month();
    int offset = 0;
    if (num < num2) {
        offset = -(model->getDays() + DataManager::getInstance()->getRequstData()->day() - button->getDay());
    } else if (num == num2) {
        offset = button->getDay() - DataManager::getInstance()->getRequstData()->day();
    } else {
        auto m = _dataArray.at(_monthIdx - 1);
        offset = m->getDays() - DataManager::getInstance()->getRequstData()->day() + button->getDay();
    }
    return offset;
}

void Layer_Daily::requestData(){
    std::unordered_map<std::string, std::string> content;
    content["uid"] = iOSWrapper::getUserID();
    content["device_time_zone"] = iOSWrapper::getLocalTimeZone();
    std::unordered_map<std::string, std::string> param;
    param["content"] = KTUtils::jsonString(content);
    
    KLProgressLayer::show(this);
    
    auto failure = [=](){
    
        KLProgressLayer::hide(this);
        vector<string> v;
        v.push_back(LocalizedString("TID_UI_CONTINUE"));
        KLAlertLayer::show("", LocalizedString("TID_UI_LOG OUT ERROR"), "", v, [=](KLAlertLayer *layer, int buttonIdx){
            Director::getInstance()->popScene();
            if (_gameScene->getIsWin()) {
                __NotificationCenter::getInstance()->postNotification(NTFFactor::NTF_NEW_GAME);
            }
        });
    };
    
    HttpManager::create()->sendGET("http://45.113.69.21/ao2/play_cards_awards_list.php", param, [=](const rapidjson::Document &json){
        
        int status = json["status"].GetInt();
        
        if (status == 1) {
            KLProgressLayer::hide(this);
            _dataArray.clear();
            
            _historyTotal = json["total"].GetInt();
            
            time_t systemTime = json["system_time"].GetUint();
            //        auto systemTimeStr = json["system_time_str"].GetString();
            KTDate *date = KTDate::date(systemTime);
            
            CCLOG("date = %ld", date->stamp());
            //        CCLOG("systemTimeStr = %s", systemTimeStr);
            //        CCLOG("timzeon = %s", date->getTimeZone().c_str());
            
            KTDate* localDate = date->localDate();
            CCLOG("localDate = %ld", localDate->stamp());
            DataManager::getInstance()->setRequstData(localDate);
            DataManager::getInstance()->setLastData(KTDate::date());
            //        CCLOG("local date = %s", localDate->toString().c_str());
            //        CCLOG("local timzeon = %s", localDate->getTimeZone().c_str());
            
            
            DataManager::getInstance()->setUserName(json["username"].GetString());
            CCLOG("username = %s", DataManager::getInstance()->getUserName().c_str());
            const rapidjson::Value& data = json["data"];
            
            for (rapidjson::Value::ConstValueIterator itr = data.Begin(); itr != data.End(); ++itr) {
                //遍历数组
                
                MonthModel *m = new MonthModel();
                m->setMonth(itr->FindMember("month")->value.GetString());
                m->setDays(itr->FindMember("days")->value.GetInt());
                //??? 服务器修改为int
                
                m->setFirstDayOfWeek(atoi(itr->FindMember("first_day_of_week")->value.GetString()));
                
                m->setDayTotal(itr->FindMember("day_total")->value.GetInt());
                
                Vector<DayModel *> days;
                const rapidjson::Value& list = itr->FindMember("list")->value;
                for (rapidjson::Value::ConstValueIterator itr2 = list.Begin(); itr2 != list.End(); ++itr2){
                    DayModel *day = new DayModel();
                    day->setDay(atoi(itr2->FindMember("day")->value.GetString()));
                    bool awards = atoi(itr2->FindMember("awards")->value.GetString()) != 0 ? true : false;
                    day->setAwards(awards);
                    day->setIsGetToday(itr2->FindMember("is_today")->value.GetInt() != 0);
                    days.pushBack(day);
                    day->release();
                }
                m->setChallengedDays(days);
                _dataArray.pushBack(m);
                m->release();
            }
            this->_monthIdx = json["current_month"].GetInt();
            this->updateUI();
            CCLOG("finish");
            
            if (_fromGameMenu && (!_buttons.at(_selectedDayIdx)->isUnlock())) {
                auto la = KLDailyOpenLayer::create();
                la->onTap = [=](KLDailyOpenLayer *layer, int buttonIdx){
                    
                    //开启
                    layer->removeFromParent();
                    this->startCardGame();
                    
                };
                this->addChild(la, 1024);
            }
            
            if (DataManager::getInstance()->getDailyChanllengeSuccess()) {
                
                if (DataManager::getInstance()->getUserName().empty()) {
                    
                    auto la = KLInputNameLayer::create();
                    la->onClick = [=](){
                        auto username = la->getTextField()->getString();
                        this->uploadUserName(username, [=](){
                            this->popToLeaderBoard();
                        });
                    };
                    this->addChild(la, 1024);
                    
                } else {
                    auto la = KLWinLayer::create(_gameScene->getCurrentScore());
                    la->onTap = [=](KLWinLayer*, int buttonIdx){
                        
                        if (buttonIdx == 0) {
                            this->startCardGame();
                        } else if (buttonIdx == 5){
                            this->popToLeaderBoard();
                        }
                    };
                    this->addChild(la, 1024);
                }
                
                DataManager::getInstance()->setDailyChanllengeSuccess(false);
            }
        } else {
            failure();
        }
    }, [=](std::string error){
        failure();
    });
}

void Layer_Daily::uploadUserName(string name, function<void()> onFinish){

    std::unordered_map<std::string, std::string> content;
    content["uid"] = iOSWrapper::getUserID();
    content["username"] = name;
    #if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    content["app_os"] = "2";
    #endif
    std::unordered_map<std::string, std::string> param;
    param["content"] = KTUtils::jsonString(content);
    
    KLProgressLayer::show(this);
    
    HttpManager::create()->sendGET("http://45.113.69.21/ao2/play_cards_user_update.php", param, [=](const rapidjson::Document &json){
        
        DataManager::getInstance()->setUserName(name);
        if (onFinish) {
            onFinish();
        }
        KLProgressLayer::hide(this);
    }, [=](string error){
        KLProgressLayer::hide(this);
    });
}

void Layer_Daily::touchEvent(Ref *pSender, Widget::TouchEventType type){

    if (type == Widget::TouchEventType::ENDED) {
        Node *node = static_cast<Node *>(pSender);
        if (node->getTag() == 10) {
            
            if (_monthIdx <= 0) {
                std::vector<std::string> v;
                v.push_back(LocalizedString("TID_UI_CONTINUE"));
                KLAlertLayer::show("", LocalizedString("TID_UI_FIRST MONTH"), "", v, nullptr);
                return;
            }
            _monthIdx--;
            updateUI();
        } else if (node->getTag() == 20) {
            if (_monthIdx >= (_dataArray.size() - 1)) {
                std::vector<std::string> v;
                v.push_back(LocalizedString("TID_UI_CONTINUE"));
                KLAlertLayer::show("", LocalizedString("TID_UI_LAST MONTH"), "", v, nullptr);
                return;
            }
            
            _monthIdx++;
            updateUI();
        }
    }
}
