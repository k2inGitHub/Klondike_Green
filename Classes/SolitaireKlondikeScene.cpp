#include "SolitaireKlondikeScene.h"
#include "BlankCard.h"
#include "Trump.h"
#include "TrumpData.h"
#include "TrumpModel.h"
#include "SolitaireKlondikeViewModel.h"
#include "BottomMenu.h"
#include "Audio.h"
#include "Global.h"
#include "iOSWrapper.hpp"
#include "HLAdManagerWrapper.hpp"
#include "HLAnalsytWrapper.hpp"
#include "Localization.hpp"
#include "Layer_Daily.hpp"
#include "HttpManager.hpp"
#include "KLPopupLayer.hpp"
#include "Layer_HighScore.hpp"
#include "HighScoreManager.hpp"
#include "Layer_Statistics.hpp"
#include "Layer_Setting.hpp"
#include "Layer_Leaderboard.hpp"
#include "AppDelegate.h"

USING_NS_CC;
using namespace NSGlobal;
using namespace cocos2d::ui;

static SolitaireKlondikeScene *_instance;
//static float _focusAdTimer = 0;

// 基础碓坐标
//const float POS_DROP_START_X = 47;
//const float POS_DROP_OFFSET_X = 91;
#define POS_DROP_START_Y (VisibleRect::top().y - 150)
//(iOSWrapper::isPad() ? 180 : 100))

// 牌库坐标
#define POS_STOCK_START_Y  POS_DROP_START_Y
//const float POS_STOCK_START_X = (POS_DROP_START_X //
//                                 + (POS_DROP_OFFSET_X * (SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS - 1)));

//const float POS_STOCK_START_X_LEFT = 47;
//const float POS_DROP_START_X_LEFT = POS_STOCK_START_X_LEFT + 3 * POS_DROP_OFFSET_X;

const float POS_STOCK_OFFSET_X = -30;
const float POS_STOCK_OFFSET_X_LEFT = -30;

// 牌堆坐标
//const float POS_FIELD_START_X = POS_DROP_START_X;
//const float POS_FIELD_OFFSET_X_NOW() = POS_DROP_OFFSET_X;
#define POS_FIELD_START_Y (POS_DROP_START_Y - 161) //30 + ht

const float POS_CLOSE_FIELD_OFFSET_Y = -10; //未翻开牌组间距
const float POS_FIELD_OFFSET_Y = -40;   //open牌组间距


// tween
const float TWEEN_CARD_MOVE_SPEED_1X = 0.3f; // 牌的移动动画速度_1x
const float TWEEN_CARD_MOVE_SPEED_2X = 0.15f; // 牌的移动动画速度_2x
const float TWEEN_CARD_MOVE_SPEED_NO = 0.0f; // 没有移动动画_no

// 拖动过程中的Z值
const int Z_DRAGGING = 999;

float SolitaireKlondikeScene::POS_FIELD_OFFSET_X_NOW(){
    return POS_DROP_OFFSET_X_NOW();
}

float SolitaireKlondikeScene::POS_DROP_OFFSET_X_NOW(){
    if(winSize.height > winSize.width){
        return 88 + 3;
    } else {
        return 88 + 16;
    }
}

float SolitaireKlondikeScene::POS_FIELD_START_X_NOW(){
    float start = winSize.width/2 - 3 * POS_DROP_OFFSET_X_NOW();
    return start;
}

float SolitaireKlondikeScene::POS_DROP_START_X_NOW()
{
    float start = winSize.width/2 - 3 * POS_DROP_OFFSET_X_NOW();
    return m_l_r_mode ? start : start + 3 * POS_DROP_OFFSET_X_NOW();
}

float SolitaireKlondikeScene::POS_STOCK_START_X_NOW()
{
    return POS_DROP_START_X_NOW() + (POS_DROP_OFFSET_X_NOW() * (m_l_r_mode ? 6 : -3));
//    if(winSize.height > winSize.width){
//        return m_l_r_mode ? POS_STOCK_START_X : POS_STOCK_START_X_LEFT;
//    } else {
//        return MAX(0, (winSize.width - 640.f))/2 + (m_l_r_mode ? POS_STOCK_START_X : POS_STOCK_START_X_LEFT);
//    }
}

float SolitaireKlondikeScene::POS_STOCK_OFFSET_X_NOW()
{
//    return POS_STOCK_OFFSET_X;
    auto ret = m_l_r_mode ? POS_STOCK_OFFSET_X : POS_STOCK_OFFSET_X_LEFT;
//    CCLOG("ret = %f", ret);
    return ret;
}

Scene* SolitaireKlondikeScene::createScene(bool isChanllenge) {
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = SolitaireKlondikeScene::create(isChanllenge);

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool SolitaireKlondikeScene::init(){
    return init(false);
}



// on "init" you need to initialize your instance
bool SolitaireKlondikeScene::init(bool ischallenge) {
    //////////////////////////////
    // 1. super init first
    if (!KTLayer::init()) {
        return false;
    }
    
    
    
    
    _instance = this;
    _viewModel = nullptr;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //sound
    Audio::getInstance()->playBgMusic(WordFactor::MUSIC_BGM);
    if (!DataManager::getInstance()->isSoundOn()) {
        Audio::getInstance()->pauseEffects();
        Audio::getInstance()->pauseMusic();
    }
    
    // 背景图片
    //theme
    
    auto sprite = ImageView::create(DataManager::getInstance()->getThemeFile());
    sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprite->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    
    if (DataManager::getInstance()->getThemeID() == 2){
        sprite->setScale(winSize.width/1136, winSize.height/1136);
    } else {
        sprite->setScale(1);
    }
    
    this->addChild(sprite, -100);
    _themeBg = sprite;
    
    // 底部菜单
    auto bottom_menu = BottomMenu::create();
    bottom_menu->onItemClick = [=](int tag){
        
        if (tag == 0) {
            auto la = Layer_Setting::create();
            this->addChild(la, 2048);
        }
        else if (tag == 2) {
            KLRestartLayer *la = KLRestartLayer::create(_isDailyChanllenge);
            
            if (_isVegasOn) {
                la->getItems().at(2)->setEnabled(false);
            }
            
            
            la->onTap = [=](KLRestartLayer *layer, int buttonIdx){
                if (buttonIdx >= 0) {
                    if (buttonIdx <3) {
                        if (_isDailyChanllenge) {
                            if (buttonIdx == 0) {
                                this->startGame(_viewModel->gameType);
                            } else if (buttonIdx == 1) {
                                this->quitDailyChallenge();
                            } else {
                                
                            }
                            
                        } else {
                            this->startGame(buttonIdx);
                        }
                    } else if (buttonIdx == 3) {
                        auto hsLayer = Layer_Statistics::create();
                        auto sc = Scene::create();
                        sc->addChild(hsLayer);
                        Director::getInstance()->pushScene(sc);
                    } else if (buttonIdx == 4){
                        auto hsLayer = Layer_HighScore::create();
                        auto sc = Scene::create();
                        sc->addChild(hsLayer);
                        Director::getInstance()->pushScene(sc);
                    }
                }
            };
            this->addChild(la, 2048);
        }
    };
    bottom_menu->setAnchorPoint(Vec2(0,0));
    bottom_menu->setPosition(Vec2(origin.x, origin.y));
    this->addChild(bottom_menu, Z_DRAGGING);
    _bottomMenu = bottom_menu;
    
    
    auto statusBar = ImageView::create("bar_2.png");
    statusBar->setFlippedY(true);
    statusBar->setScale9Enabled(true);
    statusBar->setOpacity(175);
    addChild(statusBar, Z_DRAGGING + 1);
    _statusBar = statusBar;
    
    _infoParent = Node::create();
    addChild(_infoParent, Z_DRAGGING + 1);
    
    _timeLabel = Label::createWithSystemFont("时间:90:80", "Arial", 24);
    _timeLabel->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    _timeLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    _timeLabel->enableShadow();
    
    _timeLabel->setContentSize(Size(200, 28));
    _timeLabel->setDimensions(200, 34);
//    _timeLabel->setOverflow(Label::Overflow::SHRINK);
    _infoParent->addChild(_timeLabel);
    
    _scoreLabel = Label::createWithSystemFont("得分:1000", "Arial", 24);
    _scoreLabel->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    _scoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
//    _scoreLabel->enableShadow();
    _scoreLabel->setContentSize(Size(200, 28));
    _scoreLabel->setDimensions(200, 34);
    
    this->addChild(_scoreLabel, Z_DRAGGING + 1);
    
    _moveLabel = Label::createWithSystemFont("移牌:90", "Arial", 24);
    _moveLabel->setAlignment(TextHAlignment::RIGHT, TextVAlignment::CENTER);
    _moveLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
//    _moveLabel->enableShadow();
    
    _moveLabel->setContentSize(Size(200, 28));
    _moveLabel->setDimensions(200, 34);
//    _moveLabel->setOverflow(Label::Overflow::SHRINK);
    _infoParent->addChild(_moveLabel);
    
    _batteryBg = ImageView::create("battery.png");
    addChild(_batteryBg, Z_DRAGGING + 1);
    
    int v = iOSWrapper::getBattery() * 100;
    _batteryBar = LoadingBar::create("battery_1.png", v);
    _batteryBar->setScaleX(28.f/_batteryBar->getContentSize().width);
    addChild(_batteryBar, Z_DRAGGING + 1);
    
    
    
    _sysTimeLabel = Label::createWithSystemFont(KTDate::date()->localDate()->toString().substr(11, 5), "Arial", 18);
    _sysTimeLabel->setColor(Color3B::GRAY);
    _sysTimeLabel->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    _sysTimeLabel->setContentSize(Size(60, 28));
    _sysTimeLabel->setDimensions(60, 28);
//    _sysTimeLabel->setOverflow(Label::Overflow::SHRINK);
    addChild(_sysTimeLabel, Z_DRAGGING + 1);
    
    
    //自动收牌
    _autoButton = KTFactory::createButton("B_black.png", "B_black.png", "", LocalizedString("TID_UI_AUTO COLLECT"), 32, "Arial", Size(300, 100));
    _autoButton->setPressedActionEnabled(true);
    //Button::create("B_black.png");
    //_autoButton->setTitleText(LocalizedString("TID_UI_AUTO COLLECT"));
    //_autoButton->setTitleFontSize(36);
    _autoButton->setTitleColor(Color3B::WHITE);
    _autoButton->setOpacity(192);
//    Text::create(LocalizedString("TID_UI_VICTORY!"), "Arial", 200);
    
    //win
    _winLabel = Label::createWithSystemFont(LocalizedString("TID_UI_VICTORY!"), "Arial", 180);
//    Size(700, 500), TextHAlignment::CENTER, TextVAlignment::CENTER
    _winLabel->setDimensions(600, 400);
    _winLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    _winLabel->setVerticalAlignment(TextVAlignment::CENTER);
    _winLabel->setPosition(Vec2(visibleSize.width/2, POS_FIELD_START_Y));
    _winLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _winLabel->enableShadow();
    addChild(_winLabel, 128);
    
//    auto blockSize = Size(600, 400);
//
//    auto colorLayer = LayerColor::create(Color4B(100, 100, 100, 255), blockSize.width, blockSize.height);
//    colorLayer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
//    colorLayer->setPosition(_winLabel->getPosition());
//    
//    this->addChild(colorLayer, 127);
    //tip mask
    _tipMask = LayerColor::create(Color4B(0,0,0,127));
    addChild(_tipMask, 127);
    
    float bg_scale_x =1;// 768.0f/325.f;
    float bg_scale_y =1;// .0f/68.f;
    float sc = MAX(bg_scale_x * ScaleFactor::scale_x, bg_scale_y * ScaleFactor::scale_y);
    _autoButton->setScale(sc);
    addChild(_autoButton, 64);
    _autoButton->addTouchEventListener(CC_CALLBACK_2(SolitaireKlondikeScene::touchEvent, this));

    
    _quitChanllengeButton = KTFactory::createButton("B_lobby.png", "B_lobby1.png", "", LocalizedString("TID_UI_QUIT1"), 15, "Arial",  Size(70, 70));
    _quitChanllengeButton->setTitleColor(Color3B(100,100,100)); //Color3B(238, 238, 238)
    auto lbl = _quitChanllengeButton->getTitleRenderer();
    lbl->setPosition(Vec2(_quitChanllengeButton->getContentSize()/2) + Vec2(0, -22));
    _quitChanllengeButton->setPosition(VisibleRect::leftBottom() + Vec2(45, 126));
    _quitChanllengeButton->addTouchEventListener([=](Ref*,Widget::TouchEventType eventType){
        if (eventType == Widget::TouchEventType::ENDED) {
            std::vector<std::string> others;
            others.push_back(LocalizedString("TID_CODE_BACK"));
            auto la = KLAlertLayer::show("", LocalizedString("TID_UI_THE GAME IS NOT YET OVER. ARE YOU SURE YOU WANT TO EXIT DAILY CHALLENGES?"), LocalizedString("TID_UI_EXIT"), others, [=](KLAlertLayer *layer, int buttonIdx){
                if (buttonIdx == 0) {
                    this->quitDailyChallenge();
                }
            });
            Button* btn = la->getChildByTag<Button*>(1);
            btn->loadTextures("B_green.png", "B_green0.png");
        }
        
    });
    _bottomMenu->addChild(_quitChanllengeButton);
    _isDailyChanllenge = ischallenge;
    _quitChanllengeButton->setVisible(_isDailyChanllenge);
    
    
    auto bottomTouchBtn = KTFactory::createButton("null.png", "", "", "", 0, "", Size(winSize.width, winSize.height/2));
    addChild(bottomTouchBtn, -99);
    this->setBottomVisible(true);
    bottomTouchBtn->addClickEventListener([=](Ref *){
        this->setBottomVisible(!this->_isBottomVisible);
    });
    _bottomTouchBtn = bottomTouchBtn;
    
    /////////////////////////////
    // 3. add your codes below...
    _isTouchCard = true;
    
    
    if (ischallenge) {
        setViewModel(SolitaireKlondikeViewModel::create());
        startGame(3);
    } else {
    
        //继续上次游戏
        auto fileUtils = FileUtils::getInstance();
        auto filePath = fileUtils->getWritablePath() + "/record.plist";
        auto valueMap = FileUtils::getInstance()->getValueMapFromFile(filePath);
        //    CCLOG("string = %s", fileUtils->getStringFromFile(filePath).c_str());
        if (valueMap.size() == 0) {
            // ViewModelの生成
            
            setViewModel(SolitaireKlondikeViewModel::create());
            
            initValues();
            // 初始化空白牌
            initBlank();
            // 开始发牌
            initCarts();
        } else {
            initValues();
            decode(valueMap);
            initBlank();
            decodeInitCarts();
        }
    
    }
    
    preloadFxStar();
    
    // 刷新数据
    refresh();
    
    //touch layer
    auto la = Layer::create();
    addChild(la);
    
    //winner action root
//    _winnerActionParent = Node::create();
//    addChild(_winnerActionParent, 10);
    
    
    // 添加触摸监听
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    // 监听回调设置
    listener->onTouchBegan = CC_CALLBACK_2(SolitaireKlondikeScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(SolitaireKlondikeScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(SolitaireKlondikeScene::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(SolitaireKlondikeScene::onTouchCancelled, this);

    // 优先级100
    la->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, la);
    
    
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SolitaireKlondikeScene::onThemeSetup), kOnThemeSetup, nullptr);
    
    // 新游戏的事件观察
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SolitaireKlondikeScene::NTFNewGameCallback), NTFFactor::NTF_NEW_GAME, nullptr);
    
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SolitaireKlondikeScene::NTFNewGameWinnalbeCallback), NTFFactor::NTF_NEW_GAME_WINNALBE, nullptr);
    
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SolitaireKlondikeScene::NTFRepeateGameCallback), NTFFactor::NTF_REPEATE_GAME, nullptr);
    // 撤销的事件观察
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SolitaireKlondikeScene::NTFUndoCallback), NTFFactor::NTF_UNDO, nullptr);
    
    // 设置后刷新事件观察
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SolitaireKlondikeScene::NTFSettingRefreshCallback), NTFFactor::NTF_SETTING_REFRESH, nullptr);
    
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SolitaireKlondikeScene::OnShowNextMoveCallback), NTFFactor::NTF_ShowNextMove, nullptr);
    
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SolitaireKlondikeScene::onDailyCallback), NTFFactor::NTF_Daily, nullptr);
    
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SolitaireKlondikeScene::NTFDailyGameWinnalbeCallback), NTFFactor::NTF_DAILY_GAME, nullptr);
    
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SolitaireKlondikeScene::onBatteryValueChanged), kBatteryChanged, nullptr);
    
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SolitaireKlondikeScene::onShareFinish), "KL_shareFinish", nullptr);
    
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SolitaireKlondikeScene::onApplicationWillEnterForeground), kOnApplicationWillEnterForeground, nullptr);
    
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SolitaireKlondikeScene::onAdLeaveApp), kOnAdLeaveApplication, nullptr);
    
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SolitaireKlondikeScene::onApplicationDidEnterBackground), kOnApplicationDidEnterBackground, nullptr);
    
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SolitaireKlondikeScene::updateBadge), kUpdateBadgeCardback, nullptr);
    
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SolitaireKlondikeScene::updateBadge), kUpdateBadgeTheme, nullptr);
    
    scheduleUpdate();
    
    schedule(CC_SCHEDULE_SELECTOR(SolitaireKlondikeScene::saveGame), 3);

    
    
    
    
    //=======test======
    
//    Document doc;
//    doc.Parse("12324325467567567{asdada}we2989***");
//    
//    CCLOG("init doc = %d", doc.IsObject());
    
//    DataManager::getInstance()->setDailyChanllengeSuccess(true);
//    auto m = HighScoreModel::create();
//    m->setChallengeDate(KTDate::date());
//    auto winLa = KLWinLayer::create(m);
//    addChild(winLa, 2048);

//    auto layer = KLShareSuccessLayer::create();
//    addChild(layer, 2048);
    
//    auto layer = KLNoMoveLayer::create();
//    addChild(layer, 2048);
    
//    runAction(Sequence::create(DelayTime::create(1), CallFunc::create([=](){
//        vector<string> others;
//        others.push_back("ok");
//        
//        KLAlertLayer::show("titsle", "content", "cancel", others, nullptr);
//    }), NULL));
    
    /*
    std::unordered_map<std::string, std::string> content;
    content["uid"] = iOSWrapper::getUserID();
    content["data"] = TrumpModel::create()->getSequenceString();
    std::unordered_map<std::string, std::string> param;
    
    std::string jsonString = KTUtils::jsonString(content);
    CCLOG("jsonString = %s", jsonString.c_str());
     */
    
    //    runAction(Sequence::create(DelayTime::create(2), CallFunc::create([=](){
    //        KLUtils::addStars(this, Rect(0, 100, 400, 300));
    //    }), NULL));
    //    auto cb2 = [=](){
    //        CCLOG("cb2");
    //    };
    //    KLRateLayer::show(this, cb2);
    
    //    addChild(Layer_Leaderboard::create(), 2048);
    //    addChild(KLInputNameLayer::create(), 2048);
    //=================
    

    auto showNewThemeFunc = [=](){
        
        
        if(UserDefault::getInstance()->getBoolForKey("KL_SHOW_ThanksGiving") == false){
            
            auto la = KLThanksGivingLayer::create();
            this->addChild(la, 2048);
            
            UserDefault::getInstance()->setBoolForKey("KL_SHOW_ThanksGiving", true);
        }
    };

    
    
    if (UserDefault::getInstance()->getBoolForKey("KL_SHOW_INTRO") == false) {
        
        auto introLayer = KLIntroLayer::create(KLIntroLayer::IntroType::First);
        introLayer->onClose = showNewThemeFunc;
        addChild(introLayer, 2048);
        UserDefault::getInstance()->setBoolForKey("KL_SHOW_INTRO", true);
    } else {
        showNewThemeFunc();
    }

    
    
    
    return true;
}

void SolitaireKlondikeScene::updateBadge(Ref *sender){

    bool flag = UserDefault::getInstance()->getBoolForKey("KL_CUSTOM_CARDBACK_CLICK") == false || UserDefault::getInstance()->getBoolForKey("KL_CUSTOM_THEME_CLICK") == false;
    _bottomMenu->setItemBadge(0, flag);
}

void SolitaireKlondikeScene::showAskForSetupTheme(string themeName){

    vector<string> btns;
    btns.push_back("确定");
    KLAlertLayer::show("提示", "应用新的主题?", "取消", btns, [=](KLAlertLayer *layer, int buttonIdx){
        if(buttonIdx == 1){
            auto mgr = DataManager::getInstance();
            ThemeItem *item = new ThemeItem();
            item->setName(themeName);
            auto writablePath = FileUtils::getInstance()->getWritablePath();
            string bgfile = writablePath + themeName + "_bg.jpg";
            string previewfile = writablePath + themeName + "_preview.png";
            item->setBgFile(bgfile);
            item->setPreviewFile(previewfile);
            mgr->addSetupTheme(item);
            item->release();
            
            Director::getInstance()->getTextureCache()->removeTextureForKey(bgfile);
            Director::getInstance()->getTextureCache()->removeTextureForKey(previewfile);

            mgr->changeTheme(mgr->getThemeID(item));
            Director::getInstance()->popToRootScene();
        }
    });
}

void SolitaireKlondikeScene::onThemeSetup(cocos2d::Ref* pSender){
    //显示安装主题提示框
    auto theme = ((__String *)pSender)->getCString();
    showAskForSetupTheme(theme);
}

//用户时长，通过记录游戏启动和退出时间点来计算
void SolitaireKlondikeScene::onApplicationDidEnterBackground(Ref *sender){

    int sec = _sessionTimer;
    HLAnalsytWrapper::event("Session", StringUtils::format("%d", sec));
    HLAnalsytWrapper::event("BannerTime", StringUtils::format("%d", HLAdManagerWrapper::getBannerShowTime()));
    _sessionTimer = 0;
    HLAdManagerWrapper::setBannerShowTime(0);
}

void SolitaireKlondikeScene::onAdLeaveApp(Ref *sender){
    auto now = KTDate::date();
    UserDefault::getInstance()->setFloatForKey("KL_LastShowActiveAd", now->stamp());
    UserDefault::getInstance()->flush();
}

void SolitaireKlondikeScene::onApplicationWillEnterForeground(Ref *sender){
    
    auto stamp = UserDefault::getInstance()->getFloatForKey("KL_LastShowActiveAd", 0);
    
    auto last = KTDate::date(stamp);
    auto now = KTDate::date();
    auto since = stamp == 0 ? 0 : now->timeIntervalSinceLastDate(last);
    
    auto interval = HLAnalsytWrapper::intValue("KL_ShowActiveAdInterval", 120);
    if (since > interval) {
        
        _focusAdTimer.Set(0);
        _focusAdTimer.InterpTo(1, 1);
        
        
        UserDefault::getInstance()->setFloatForKey("KL_LastShowActiveAd", now->stamp());
        UserDefault::getInstance()->flush();
    }
}

void SolitaireKlondikeScene::updateLayout(){
    
    if (_isDeal){
    
        int index = 0;
        for (int x = 0; x < SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS; x++) {
            
            // 牌堆
            Vector<Trump*> rows = _viewModel->getFieldRow(x);
            
            int y = 0;
            float offsetY = 0;
            for (Trump* trump : rows) {
                
                Point distPos = Point((POS_FIELD_OFFSET_X_NOW() * x * ScaleFactor::scale_x) + POS_FIELD_START_X_NOW() * ScaleFactor::scale_x
                                      , offsetY + POS_FIELD_START_Y);
                
                trump->stopAllActions();
                trump->setPosition(distPos);
                
                
                offsetY += trump->getData()->isReverse() ? POS_CLOSE_FIELD_OFFSET_Y : POS_FIELD_OFFSET_Y ;
                y++;
                index++;
            }
        }
        this->flipFieldCard();
        _isInitCardFinish = true;
        _isDeal = false;
    }
    
//    _winnerActionParent->setPosition(Vec2(winSize.width/2 - 320, winSize.height/2 - 320));
    
    HLAdManagerWrapper::hideBanner();
    _themeBg->setPosition(Vec2(winSize.width/2, winSize.height/2));
    auto themeID = DataManager::getInstance()->getThemeID();
    if(themeID == 1){
        _themeBg->loadTexture(DataManager::getInstance()->getThemeFile());
        _themeBg->setScale(1);
    } else if (themeID == 2){
        _themeBg->setScale(winSize.width/1136, winSize.height/1136);
    } else {
        _themeBg->setScale(1);
    }
    
    _tipMask->setContentSize(winSize);
    
    //top bar
    _statusBar->setPosition(VisibleRect::top() + Vec2(0, -32.5));
    _statusBar->setScale(winSize.width/67.f, 65.f/108.f);
    
    _moveLabel->setPosition(VisibleRect::rightTop() + Vec2(-110, -26));
    _timeLabel->setPosition(VisibleRect::top() + Vec2(-50, -26));
    _scoreLabel->setPosition(VisibleRect::leftTop() + Vec2(0, -26));
    _batteryBg->setPosition(VisibleRect::rightTop() + Vec2(-80, -26));
    _batteryBar->setPosition(VisibleRect::rightTop() + Vec2(-82, -26));
    _sysTimeLabel->setPosition(VisibleRect::rightTop() + Vec2(-30, -26));
    
    _autoButton->setPosition(VisibleRect::bottom() + Vec2(0, 300));
    
    _bottomTouchBtn->setContentSize(Size(winSize.width, winSize.height/2));
    _bottomTouchBtn->setPosition(_bottomTouchBtn->getContentSize()/2);
    
    
    // 添加基础碓的空白牌
    for (int x = 0; x < SolitaireKlondikeViewModel::COUNT_DROP_MAX_ROWS; x++) {
        
        BlankCard* blank = _viewModel->getDropBlank(x);
        blank->setPosition(Point((POS_DROP_OFFSET_X_NOW() * x * ScaleFactor::scale_x) + POS_DROP_START_X_NOW() * ScaleFactor::scale_x, POS_DROP_START_Y * ScaleFactor::scale_y));
    }
    
    // 牌库空白牌
    BlankCard* blank = _viewModel->getStockBlank();
    CCLOG("blank = %s", CCStringFromPoint(blank->getPosition())->getCString());
    if (blank != nullptr)
        blank->setPosition(Point( //
                             POS_STOCK_START_X_NOW() * ScaleFactor::scale_x//
                             , POS_STOCK_START_Y * ScaleFactor::scale_y));
    //close
    for (Trump* stock : this->_viewModel->getCloseStockCards()) {
        
        stock->setPosition(Point( //
                                 POS_STOCK_START_X_NOW() * ScaleFactor::scale_x//
                                 , POS_STOCK_START_Y * ScaleFactor::scale_y));
    }
    
    //open
    auto row = this->_viewModel->getFieldRow(SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X);
    Point pos3 = Point(POS_STOCK_START_X_NOW() + (m_l_r_mode ? (-POS_FIELD_OFFSET_X_NOW()) : (POS_FIELD_OFFSET_X_NOW() + 2 * abs(POS_STOCK_OFFSET_X))) , POS_STOCK_START_Y);
    Point pos2 = pos3 + Point(POS_STOCK_OFFSET_X_NOW() * ScaleFactor::scale_x, 0);
    Point pos1 = pos2 + Point(POS_STOCK_OFFSET_X_NOW() * ScaleFactor::scale_x, 0);
    for(int i=0; i<row.size(); i++){
        if(row.size() == 1)
            row.at(i)->setPosition(pos3);
        else if(row.size() == 2){
            row.at(0)->setPosition(pos2);
            row.at(1)->setPosition(pos3);
        }
        else if(row.size() >= 3){
            if(i<row.size()-2)
                row.at(i)->setPosition(pos1);
            else{
                row.at(row.size()-2)->setPosition(pos2);
                row.at(row.size()-1)->setPosition(pos3);
            }
        }
    }
    
    //suit
    for (Trump* card : this->_viewModel->getDropCards()) {
        float snapPosX = (POS_DROP_OFFSET_X_NOW() * ScaleFactor::scale_x //
                          * (card->getData()->getFieldX() - SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS)) //
        + POS_DROP_START_X_NOW() * ScaleFactor::scale_x ;
        float snapPosY = POS_DROP_START_Y * ScaleFactor::scale_y;
        
        card->setPosition(snapPosX, snapPosY);
        CCLOG("xxxx = %d", card->getData()->getFieldX());
    }
    
    for (int x = 0; x < SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS; x++) {
        // 添加牌堆的空白牌
        BlankCard* blank = _viewModel->getFieldBlank(x);
        blank->setPosition(Point((POS_FIELD_OFFSET_X_NOW() * x) + POS_FIELD_START_X_NOW(), POS_FIELD_START_Y * ScaleFactor::scale_y));
    }
    
    int index = 0;
    for (int x = 0; x < SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS; x++) {
        
        // 牌堆
        Vector<Trump*> rows = _viewModel->getFieldRow(x);
        
        int y = 0;
        float offsetY = 0;
        for (Trump* trump : rows) {
            // 初期位置在牌库的位置
//            this->addChild(trump, y);
            // 配置tween
            Point distPos = Point((POS_FIELD_OFFSET_X_NOW() * x * ScaleFactor::scale_x) + POS_FIELD_START_X_NOW() * ScaleFactor::scale_x
                                  , offsetY + POS_FIELD_START_Y);
            
            if ((x == SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS - 1)
                && (trump == rows.back())) {
                
                trump->setPosition(distPos);
            } else {
                trump->setPosition(distPos);
            }
            
            offsetY += trump->getData()->isReverse() ? POS_CLOSE_FIELD_OFFSET_Y : POS_FIELD_OFFSET_Y ;
            
            
            
            
            y++;
            index++;
        }
        if (!_gameStart) {
            auto trump = rows.back();
            //                trump->stopAllActions();
            //                trump->getData()->setReverse(!trump->getData()->isReverse());
            rows.back()->getData()->setReverse(false);
            trump->flip(nullptr, nullptr, false);
        }
        
        adjustRowHeight(x);
    }
    
    
    auto ac = getActionByTag(911);
    if(ac != nullptr){
        stopWinnerAction(1);
        showWinnerActionNormal([=](){
            this->showWinnerActionCallback();
        });
    }
    
}

void SolitaireKlondikeScene::onShareFinish(Ref *sender){

    bool flag = DataManager::getInstance()->getFirstShared();
    if (!flag) {
        
        Director::getInstance()->popToRootScene();
        
        auto la = KLShareSuccessLayer::create();
        la->onClick = [=](){
            auto la = Layer_CardBack::create();
            auto sc = Scene::create();
            sc->addChild(la);
            Director::getInstance()->pushScene(sc);
        };
        addChild(la, 2048);
        DataManager::getInstance()->setFirstShared(true);
    }
}

void SolitaireKlondikeScene::setBottomVisible(bool value){
    if(_isBottomVisible != value){
        _isBottomVisible = value;
        auto indicator = _bottomMenu->getIndicator();
        if (_isBottomVisible) {
            _bottomMenu->stopAllActions();
            auto seq = Sequence::create(MoveTo::create(0.3f, Vec2(0, 0)), NULL);
            _bottomMenu->runAction(seq);
            indicator->runAction(RotateTo::create(0.3f, 0));
            
        } else {
            _bottomMenu->stopAllActions();
            auto seq = Sequence::create(MoveTo::create(0.3f, Vec2(0, -_bottomMenu->getContentSize().height)), NULL);
            _bottomMenu->runAction(seq);
            indicator->runAction(RotateTo::create(0.3f, 180));
        }
    }
}

void SolitaireKlondikeScene::onBatteryValueChanged(Ref *sender){
    int v = iOSWrapper::getBattery() * 100;
    _batteryBar->setPercent(v);
}

void SolitaireKlondikeScene::onEnter(){
    KTLayer::onEnter();
    HLAdManagerWrapper::hideBanner();
    
    std::unordered_map<std::string, std::string> content;
    content["uid"] = iOSWrapper::getUserID();
    content["challenge_time_zone"] = iOSWrapper::getLocalTimeZone();
    std::unordered_map<std::string, std::string> param;
    param["content"] = KTUtils::jsonString(content);
    
    HttpManager::create()->sendGET("http://49.51.8.225/ao2/play_cards_challenge_check.php", param, [=](const rapidjson::Document &json){
        if (json["status"].GetInt() == 1) {
            
            bool awarded = json["is_awarded"].GetInt() == 1;
            if (awarded) {
                _bottomMenu->setItemBadge(1, false);
            } else {
                _bottomMenu->setItemBadge(1, true);
            }
        } else {
            _bottomMenu->setItemBadge(1, false);
        }
    }, [=](std::string error){
        _bottomMenu->setItemBadge(1, false);
    });
    
    bool flag = UserDefault::getInstance()->getBoolForKey("KL_CUSTOM_CARDBACK_CLICK") == false || UserDefault::getInstance()->getBoolForKey("KL_CUSTOM_THEME_CLICK") == false;
    _bottomMenu->setItemBadge(0, flag);
    
    if(!_checkUpdate){
        KLUpdateLayer::show();
        _checkUpdate = true;
    }
    
    auto theme = UserDefault::getInstance()->getStringForKey("KL_NeedSetupTheme");
    CCLOG("SetupTheme = %s", theme.c_str());
    if(!theme.empty()){
        showAskForSetupTheme(theme);
        UserDefault::getInstance()->setStringForKey("KL_NeedSetupTheme", "");
    }
}



void SolitaireKlondikeScene::updateLocalizedUI(){

    if (_viewModel->gameType == 0) {
        _bottomMenu->getDetailLabel()->setString(LocalizedString("TID_CODE_RANDOM MODE"));
    } else if (_viewModel->gameType == 2) {
        _bottomMenu->getDetailLabel()->setString(LocalizedString("TID_CODE_WINNING MODE"));
    } else if (_viewModel->gameType == 1){
    }
    if (_isVegasOn) {
        _bottomMenu->getDetailLabel()->setString(LocalizedString("TID_CODE_VEGAS SCORE MODE"));
    }
    
    _quitChanllengeButton->setTitleText(LocalizedString("TID_UI_QUIT1"));
    auto lbl = _quitChanllengeButton->getTitleRenderer();
    lbl->setPosition(Vec2(_quitChanllengeButton->getContentSize()/2) + Vec2(0, -22));
    
    _autoButton->setTitleText(LocalizedString("TID_UI_AUTO COLLECT"));
    
    
    int min = (int)_timer / 60;
    int sec = (int)_timer % 60;
    char str[64];
    sprintf(str, "%s%.2d:%.2d", LocalizedString("TID_UI_TIMES:").c_str(), min, sec);
    _timeLabel->setString(str);
    
    
    int score = getTotalScore();
    sprintf(str, "%s:%s%d",LocalizedString("TID_UI_SCORE").c_str(), _isVegasOn ? "$" : "", score);
    _scoreLabel->setString(str);
    
    sprintf(str, "%s%d", LocalizedString("TID_CODE_MOVES_WITH_VALUE").c_str(), (int)_viewModel->getTrumpSize());
    _moveLabel->setString(str);
    
}

SolitaireKlondikeScene::SolitaireKlondikeScene():
_viewModel(nullptr),
_currentScore(nullptr),
_sessionTimer(0),
_checkUpdate(false){

}

SolitaireKlondikeScene::~SolitaireKlondikeScene(){
    __NotificationCenter::getInstance()->removeAllObservers(this);
    CC_SAFE_RELEASE(_viewModel);
}

void SolitaireKlondikeScene::showStarFx(Trump *card){

    for (int i = 0; i < 5; i++) {
        int num = max((4 - i), 1) * 2;
        for (int j = 0; j < num; j++) {
            
            auto star = spawnFxStar();
            Rect rect = card->getBoundingBox();
            auto p = KTUtils::getRandomPoint(rect);
            star->setPosition(p);
            Vec2 center = Vec2(rect.getMidX(), rect.getMidY());
            auto target = p + (p - center).getNormalized() * random(20.f, 50.f);
            target.rotate(center, CC_DEGREES_TO_RADIANS(random(-90, 90)));
            star->setVisible(false);
            star->setScale(0.3 + random(0.f, 0.2f));
            star->setOpacity(164);
            auto ac1 = DelayTime::create(i/60.f);
            auto ac2 = Show::create();
            auto ac3 = DelayTime::create(3.f/60.f);
            float t = 1;
            auto ac4 = MoveTo::create(t, target);
            auto ac5 = FadeTo::create(t, 0);
            auto ac6 = ScaleTo::create(t, 0);
            auto spawn = Spawn::create(ac4, ac5, ac6, NULL);
            auto callback = CallFuncN::create([=](Node *sender){
                auto sp = static_cast<Sprite *>(sender);
                sp->setOpacity(255);
                this->despwanFxStar(sp);
            });
            star->runAction(Sequence::create(ac1, ac2,ac3,spawn, callback, NULL));
        }
    }
}

void SolitaireKlondikeScene::preloadFxStar(){

    for (int i = 0; i < 256; i++) {
        auto sp = Sprite::create("star_0.png");
        sp->setLocalZOrder(64);
        sp->setVisible(false);
        _fxStars.pushBack(sp);
        addChild(sp);
    }
}

Sprite *SolitaireKlondikeScene::spawnFxStar(){

//    CCASSERT(_fxStars.size() != 0, "pool 尺寸不够!!");
    if (_fxStars.size() != 0) {
        auto sp = _fxStars.back();
        sp->setVisible(true);
        _fxStars.popBack();
        return sp;
    } else {

        auto sp = Sprite::create("star_0.png");
        sp->setLocalZOrder(64);
        sp->setVisible(true);
        addChild(sp);
        return sp;
    }
}

void SolitaireKlondikeScene::despwanFxStar(Sprite *star){
    _fxStars.pushBack(star);
    star->setVisible(false);
}

void SolitaireKlondikeScene::quitDailyChallenge(){
    
    //退出 每日挑战
    _quitChanllengeButton->setVisible(false);
    _bottomMenu->setItemVisible(1, true);
    _isDailyChanllenge = false;
    //                    this->NTFNewGameCallback(nullptr);
    DataManager::getInstance()->setChanlleged(true);
    auto la = Layer_Daily::create(this);
    la->setGameScene(this);
    auto sc = Scene::create();
    sc->addChild(la);
    Director::getInstance()->pushScene(sc);
}

SolitaireKlondikeViewModel* SolitaireKlondikeScene::getViewModel(){
    return _viewModel;
}

ValueMap SolitaireKlondikeScene::encode(){
    ValueMap root;
    root["viewModel"] = cocos2d::Value(_viewModel->encode());
    root["_gameStart"] = cocos2d::Value(_gameStart);
    root["_timer"] = cocos2d::Value(_timer);
    root["_score"] = cocos2d::Value(_score);
    root["_isWin"] = cocos2d::Value(_isWin);
    root["_showNoMoveStart"] = cocos2d::Value(_showNoMoveStart);
    root["_showNoMoveOpenNum"] = cocos2d::Value(_showNoMoveOpenNum);
    root["_showNoMoveEnable"] = cocos2d::Value(_showNoMoveEnable);
    root["_isVegasScoreAdd"] = cocos2d::Value(_isVegasScoreAdd);
    root["_isVegasOn"] = cocos2d::Value(_isVegasOn);
    root["blanckStock"] = cocos2d::Value(_viewModel->getStockBlank()->encode());
    root["bottomMenu"] = cocos2d::Value(_bottomMenu->encode());

    return root;
}

void SolitaireKlondikeScene::decode(ValueMap map){
    
    SolitaireKlondikeViewModel * model = SolitaireKlondikeViewModel::create();
    model->decode(map["viewModel"].asValueMap());
    setViewModel(model);

    _gameStart = map["_gameStart"].asBool();
    _timer = map["_timer"].asFloat();
    _score = map["_score"].asInt();
    _isWin = map["_isWin"].asBool();
    _showNoMoveStart = map["_showNoMoveStart"].asBool();
    _showNoMoveOpenNum = map["_showNoMoveOpenNum"].asInt();
    _showNoMoveEnable = map["_showNoMoveEnable"].asBool();
    _isVegasOn = map["_isVegasOn"].asBool();
    _isVegasScoreAdd = map["_isVegasScoreAdd"].asBool();
    
    _blankStockModel = map["blanckStock"].asValueMap();
    _bottomMenu->decode(map["bottomMenu"].asValueMap());
}

void SolitaireKlondikeScene::touchEvent(Ref *pSender, Widget::TouchEventType type){
    
    if (type == Widget::TouchEventType::ENDED) {
        
        _isAutoStarted = true;
        _autoButton->setVisible(false);

        _sfx_score11_ID = Audio::getInstance()->playEffect("Music/30_collect_score11.mp3", true);
    }
}

void SolitaireKlondikeScene::saveGame(float){
    auto fileUtils = FileUtils::getInstance();
    auto filePath = fileUtils->getWritablePath() + "/record.plist";
    if ((!_isWin)) {
        //没有胜利 存档
        
        ValueMap valueMap = encode();
        FileUtils::getInstance()->writeToFile(valueMap, filePath);
        
//        CCLOG("string = %s", FileUtils::getInstance()->getStringFromFile(filePath).c_str());
    } else {
        //胜利 清除存档
        ValueMap valueMap;
        FileUtils::getInstance()->writeToFile(valueMap, filePath);
//        CCLOG("ValueMap = %d", valueMap.size());
    }
    
}

void SolitaireKlondikeScene::update(float dt){
    HLAdManagerWrapper::update(dt);
    
    if (!_adTimer.IsDone()){
        _adTimer.Update(dt);
        if (_adTimer.IsDone()) {
            HLAdManagerWrapper::showUnsafeInterstitial();
        }
    }
    
    if (!_focusAdTimer.IsDone()){
        _focusAdTimer.Update(dt);
        if (_focusAdTimer.IsDone()) {
            HLAdManagerWrapper::showUnsafeInterstitial();
        }
    }
    
    _sessionTimer += dt;
//    _focusAdTimer += dt;
    
    _sysTimer += dt;
    if (_sysTimer >= kSysUpdateInteval) {
        _sysTimeLabel->setString(KTDate::date()->localDate()->toString().substr(11, 5));
        _sysTimer = 0;
    }
    
    //time
    if (_gameStart && (!_isWin)) {
        
        _timer += dt;
        
        int min = (int)_timer / 60;
        int sec = (int)_timer % 60;
        char str[64];
        sprintf(str, "%s%.2d:%.2d", LocalizedString("TID_UI_TIMES:").c_str(), min, sec);
        _timeLabel->setString(str);
        
        
        int score = getTotalScore();
        sprintf(str, "%s:%s%d",LocalizedString("TID_UI_SCORE").c_str(), _isVegasOn ? "$" : "", score);
        _scoreLabel->setString(str);
        
        sprintf(str, "%s%d", LocalizedString("TID_CODE_MOVES_WITH_VALUE").c_str(), (int)_viewModel->getTrumpSize());
        _moveLabel->setString(str);
        
        
        _suitSfxTimer += dt;
        if (_suitSfxTimer > 10) {
            _suitSfxIdx = 0;
        }
        
        if (DataManager::getInstance()->getAutoTip()) {
            _autoTipTimer += dt;
            if (_autoTipTimer >= kAutoTipTime) {
                OnShowNextMoveCallback(nullptr);
                _autoTipTimer = 0;
            }
        }
    }
    
    //auto collect
    if (_isAutoStarted && (!_isWin)) {
        autoCollectCardStep();
    }
}

void SolitaireKlondikeScene::refresh(){
    //主题
    _themeBg->loadTexture(DataManager::getInstance()->getThemeFile());
    auto themeID = DataManager::getInstance()->getThemeID();
    if(themeID == 1){
        _themeBg->setScale(1);
    } else if (themeID == 2){
        _themeBg->setScale(winSize.width/1136, winSize.height/1136);
    } else {
        _themeBg->setScale(1);
    }
    // 移动牌的动画时间
    switch (DataManager::getInstance()->getSettings()->m_action) {
        case enum_action_1x:
            m_tweem_card_move_speed = TWEEN_CARD_MOVE_SPEED_1X;
            break;
        case enum_action_2x:
            m_tweem_card_move_speed = TWEEN_CARD_MOVE_SPEED_2X;
            break;
        case enum_action_no:
            m_tweem_card_move_speed = TWEEN_CARD_MOVE_SPEED_NO;
            break;
        default:
            break;
    }
    
    // 设定规则
    SolitaireKlondikeViewModel::_dropCount = DataManager::getInstance()->getSettings()->m_chou3card ? 3 : 1;
    // 设定左右手模式
    m_l_r_mode = DataManager::getInstance()->getSettings()->m_right_hand;
    
    
    // 添加基础碓的空白牌
    for (int x = 0; x < SolitaireKlondikeViewModel::COUNT_DROP_MAX_ROWS; x++) {
        
        BlankCard* blank = _viewModel->getDropBlank(x);
        blank->setPosition(Point((POS_DROP_OFFSET_X_NOW() * x * ScaleFactor::scale_x) + POS_DROP_START_X_NOW() * ScaleFactor::scale_x, POS_DROP_START_Y * ScaleFactor::scale_y));
    }
    
    // 牌库空白牌
    BlankCard* blank = _viewModel->getStockBlank();
    blank->setPosition(Point( //
                             POS_STOCK_START_X_NOW() * ScaleFactor::scale_x//
                             , POS_STOCK_START_Y * ScaleFactor::scale_y));
    //close
    for (Trump* stock : this->_viewModel->getCloseStockCards()) {
        
        stock->setPosition(Point( //
                                 POS_STOCK_START_X_NOW() * ScaleFactor::scale_x//
                                 , POS_STOCK_START_Y * ScaleFactor::scale_y));
    }
    
    //open
    auto row = this->_viewModel->getFieldRow(SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X);
    Point pos3 = Point(POS_STOCK_START_X_NOW() + (m_l_r_mode ? (-POS_FIELD_OFFSET_X_NOW()) : (POS_FIELD_OFFSET_X_NOW() + 2 * abs(POS_STOCK_OFFSET_X))) , POS_STOCK_START_Y);
    //Point(POS_STOCK_START_X_NOW(), POS_STOCK_START_Y) + Point(POS_FIELD_OFFSET_X, 0) * (m_l_r_mode ? -1 : 1);
    Point pos2 = pos3 + Point(POS_STOCK_OFFSET_X_NOW() * ScaleFactor::scale_x, 0);
    Point pos1 = pos2 + Point(POS_STOCK_OFFSET_X_NOW() * ScaleFactor::scale_x, 0);
    for(int i=0; i<row.size(); i++){
        if(row.size() == 1)
            row.at(i)->setPosition(pos3);
        else if(row.size() == 2){
            row.at(0)->setPosition(pos2);
            row.at(1)->setPosition(pos3);
        }
        else if(row.size() >= 3){
            if(i<row.size()-2)
                row.at(i)->setPosition(pos1);
            else{
                row.at(row.size()-2)->setPosition(pos2);
                row.at(row.size()-1)->setPosition(pos3);
            }
        }
    }
    
    //suit
    for (Trump* card : this->_viewModel->getDropCards()) {
        float snapPosX = (POS_DROP_OFFSET_X_NOW() * ScaleFactor::scale_x //
                    * (card->getData()->getFieldX() - SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS)) //
        + POS_DROP_START_X_NOW() * ScaleFactor::scale_x ;
        float snapPosY = POS_DROP_START_Y * ScaleFactor::scale_y;
        
        card->setPosition(snapPosX, snapPosY);
    }
    
    
    // 刷新牌的UI类型
    Vector<Trump*> allCards = _viewModel->getAllCards();
    for (Trump* trump : allCards){
        trump->refresh();
    }
    
    //比赛信息
    _infoParent->setVisible(DataManager::getInstance()->getSettings()->m_time_score == enum_time_on);
}

void SolitaireKlondikeScene::initBlank(){
    
    if (_viewModel->gameType == 0) {
        _bottomMenu->getDetailLabel()->setString(LocalizedString("TID_CODE_RANDOM MODE"));
        _bottomMenu->getDetailContainer()->setVisible(true);
    } else if (_viewModel->gameType == 2) {
        _bottomMenu->getDetailLabel()->setString(LocalizedString("TID_CODE_WINNING MODE"));
        _bottomMenu->getDetailContainer()->setVisible(true);
    } else if (_viewModel->gameType == 1){
        _bottomMenu->getDetailContainer()->setVisible(true);
    }
    if (_isVegasOn) {
        _bottomMenu->getDetailLabel()->setString(LocalizedString("TID_CODE_VEGAS SCORE MODE"));
        _bottomMenu->getDetailContainer()->setVisible(true);
    }
    if (_isDailyChanllenge) {
        _bottomMenu->getDetailContainer()->setVisible(false);
    }
    
    
    for (int x = 0; x < SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS; x++) {
        // 添加牌堆的空白牌
        BlankCard* blank = _viewModel->getFieldBlank(x);
        blank->setPosition(Point((POS_FIELD_OFFSET_X_NOW() * ScaleFactor::scale_x * x) + POS_FIELD_START_X_NOW() * ScaleFactor::scale_x, POS_FIELD_START_Y * ScaleFactor::scale_y));
        
        this->addChild(blank, -1);
    }
    
    // 添加基础碓的空白牌
    for (int x = 0; x < SolitaireKlondikeViewModel::COUNT_DROP_MAX_ROWS; x++) {
        
        BlankCard* blank = _viewModel->getDropBlank(x);
        blank->setPosition(Point((POS_DROP_OFFSET_X_NOW() * x * ScaleFactor::scale_x) + POS_DROP_START_X_NOW() * ScaleFactor::scale_x, POS_DROP_START_Y * ScaleFactor::scale_y));
        
        this->addChild(blank, -1);
    }
    
    // 牌库空白牌
    BlankCard* blank = _viewModel->getStockBlank();
    blank->setPosition(Point( //
                             POS_STOCK_START_X_NOW() * ScaleFactor::scale_x//
                             , POS_STOCK_START_Y * ScaleFactor::scale_y));
    this->addChild(blank, -1);
}

void SolitaireKlondikeScene::decodeInitCarts(){

    int index = 0;
    for (int x = 0; x < SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS; x++) {
        
        // 牌堆
        Vector<Trump*> rows = _viewModel->getFieldRow(x);
        
        int y = 0;
        float offsetY = 0;
        for (Trump* trump : rows) {
            // 初期位置在牌库的位置
            this->addChild(trump, y);
            // 配置tween
            Point distPos = Point((POS_FIELD_OFFSET_X_NOW() * x * ScaleFactor::scale_x) + POS_FIELD_START_X_NOW() * ScaleFactor::scale_x
                                  , offsetY + POS_FIELD_START_Y);
            
            if ((x == SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS - 1)
                && (trump == rows.back())) {
                
                trump->setPosition(distPos);
            } else {
                trump->setPosition(distPos);
            }
            if(trump == rows.back()){
                // 判断最上面一张牌是否翻转
                
                // 设置非翻转
                rows.back()->getData()->setReverse(false);
                // 翻转动作
                rows.back()->flip(nullptr, nullptr, false);
            }
            
            
            
            
            offsetY += trump->getData()->isReverse() ? POS_CLOSE_FIELD_OFFSET_Y : POS_FIELD_OFFSET_Y ;
            y++;
            index++;
        }
        adjustRowHeight(x);
    }
    //close
    for (Trump* stock : this->_viewModel->getCloseStockCards()) {
        
        stock->setPosition(Point( //
                                 POS_STOCK_START_X_NOW() * ScaleFactor::scale_x//
                                 , POS_STOCK_START_Y * ScaleFactor::scale_y));
        
        this->addChild(stock, stock->getData()->getFieldY());
    }
    
    //open
    auto openCards = _viewModel->getOpenStockCards();
    for(auto card : openCards) {
        this->addChild(card);
        
        ajustopenStockCard(true, false);
        // 当前打开牌的移动位置
        float snapPosX = card->getPosition().x + (m_l_r_mode ? (-POS_FIELD_OFFSET_X_NOW()) : (POS_FIELD_OFFSET_X_NOW() + 2 * abs(POS_STOCK_OFFSET_X)));
        //POS_FIELD_OFFSET_X_NOW() * (m_l_r_mode ? -1 : 1);
        float snapPosY = POS_STOCK_START_Y * ScaleFactor::scale_y ;
        card->setPosition(snapPosX, snapPosY);
    }
    
    
    
    //suit
    for (int i = 0; i < 4; i++) {
        for(auto card : _viewModel->getSuitRow(i)) {
            float snapPosX = (POS_DROP_OFFSET_X_NOW() * (card->getData()->getFieldX() - SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS)) + POS_DROP_START_X_NOW();
            float snapPosY = POS_DROP_START_Y ;
            this->addChild(card);
            card->setPosition(snapPosX, snapPosY);
        }
    }
    
    _viewModel->getStockBlank()->decode(_blankStockModel);
    
    _isInitCardFinish = true;
}

void SolitaireKlondikeScene::initValues(){
    
    _needReorderZ = false;
    _isRefreshCard = false;
    
    if (DataManager::getInstance()->isSoundOn()) {
        Audio::getInstance()->resumeMusic();
        Audio::getInstance()->resumeEffects();
    }
    
    bool resetVegasScore = false;
    if (DataManager::getInstance()->isVegasOn()) {
        if (!_isVegasOn) {
            resetVegasScore = true;
        }
        if (!DataManager::getInstance()->isVegasScoreAdd()) {
            resetVegasScore = true;
        }
    }
    
    _isVegasOn = !_isDailyChanllenge && DataManager::getInstance()->isVegasOn();
    _isVegasScoreAdd = DataManager::getInstance()->isVegasScoreAdd();
    
    DataManager::getInstance()->setDailyChanllengeSuccess(false);
    setCurrentScore(nullptr);
    
    _sysTimer = 0;
    
    _autoTipTimer = 0;
    _isInitCardFinish = false;
    _showNoMoveEnable = true;
    _showNoMoveStart = false;
    _showNoMoveOpenNum = 0;
    
    _timer = 0;
    _score = 0;
    
    if (_isVegasOn) {
        
        if (resetVegasScore) {
            DataManager::getInstance()->setVegasScore(0);
        }
        DataManager::getInstance()->addVegasScore(-52);
        _score = DataManager::getInstance()->getVegasScore();
        
        CCLOG("sc = %d", DataManager::getInstance()->getVegasScore());
    }
    
    
    _suitSfxIdx = 0;
    _suitSfxTimer = 0;
    
    _gameStart = false;
    _winLabel->setVisible(false);
    _isTipMove = false;
    _isAutoCollect = false;
    _isWin = false;
    _isAutoStarted = false;
    
    char str[64];
    sprintf(str, "%s00:00", LocalizedString("TID_UI_TIMES:").c_str());
    _timeLabel->setString(str);
    sprintf(str, "%s:%s%d",LocalizedString("TID_UI_SCORE").c_str(), _isVegasOn ? "$" : "", _score);
    _scoreLabel->setString(str);
    sprintf(str, "%s0", LocalizedString("TID_CODE_MOVES_WITH_VALUE").c_str());
    _moveLabel->setString(str);
    _autoButton->setVisible(false);
    _tipMask->setVisible(false);
    
    
   
    
    
    _isTouchCard = false;
}

void SolitaireKlondikeScene::initCarts(){
    
    
    // 发牌音效
    Audio::getInstance()->playEffect(WordFactor::CLICK_EFFECT_FAPAI_OGG);
    
//    Trump* trump = _viewModel->getFieldRow(0).at(0);
//    // 初期位置在牌库的位置
//    trump->setPosition(Point(POS_STOCK_START_X_NOW() * ScaleFactor::scale_x, POS_STOCK_START_Y * ScaleFactor::scale_y));
//    this->addChild(trump, 0);
//    
//    trump->getData()->setReverse(false);
//    trump->refresh();
//    return;
    
    
    int row = 7;
    int x = row-1,y=0;
    for (int i = row - 1; i >= 0; i--) {
        x = row - 1;
        y = x - i;
        while( (x >= 0) && (y >= 0)){
//            card = deckPile.getCard();
//            tablePile[x].addCard(card);
            
            auto trump = _viewModel->getFieldRow(x).at(y);
            // 初期位置在牌库的位置
            trump->setPosition(Point(POS_STOCK_START_X_NOW() * ScaleFactor::scale_x, POS_STOCK_START_Y * ScaleFactor::scale_y));
            this->addChild(trump, y);
            
            x--;
            y = x - i;
        }
    }
    
    _isDeal = true;
    //action 弹广告 引起 cocos bug 自定义计时器
    _adTimer.Set(0);
    bool flag = UserDefault::getInstance()->getBoolForKey("KL_FirstAdTime");
    if (flag) {
        _adTimer.InterpTo(1, 0.1);
    } else {
        UserDefault::getInstance()->setBoolForKey("KL_FirstAdTime", true);
        UserDefault::getInstance()->flush();
    }
    
    
    
    runAction(Sequence::create(DelayTime::create(2.6+m_tweem_card_move_speed), CallFunc::create([=](){
        this->_isDeal = false;
        //HLAdManagerWrapper::showUnsafeInterstitial();
        
    }), NULL));
    
    int index = 0;
    for (int x = 0; x < SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS; x++) {
        
        // 牌堆
        Vector<Trump*> rows = _viewModel->getFieldRow(x);
        
        int y = 0;
        float offsetY = 0;
        for (Trump* trump : rows) {
            
            // 配置tween
            Point distPos = Point((POS_FIELD_OFFSET_X_NOW() * x * ScaleFactor::scale_x) + POS_FIELD_START_X_NOW() * ScaleFactor::scale_x
                                , offsetY + POS_FIELD_START_Y * ScaleFactor::scale_y);
            
            if ((x == SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS - 1)
                && (trump == rows.back())) {
                
                
                trump->tweenOnDeal(index * 0.05, // delay
                                   m_tweem_card_move_speed // duration
                                   , distPos, [=](){
                                       this->flipFieldCard();
                                       _isInitCardFinish = true;
                                   });
                
                
            } else {
                
                trump->tweenOnDeal(index * 0.05, // delay
                                   m_tweem_card_move_speed // duration
                                   , distPos, nullptr);
            }
            
            offsetY += trump->getData()->isReverse() ? POS_CLOSE_FIELD_OFFSET_Y : POS_FIELD_OFFSET_Y ;
            y++;
            index++;
        }
    }
    
    
    // 牌库
    for (Trump* stock : this->_viewModel->getCloseStockCards()) {
        
        stock->setPosition(Point( //
                                 POS_STOCK_START_X_NOW() * ScaleFactor::scale_x//
                                 , POS_STOCK_START_Y * ScaleFactor::scale_y));
        
        this->addChild(stock, stock->getData()->getFieldY());
    }
    
    
    saveGame(0);
}

void SolitaireKlondikeScene::autoCollectCardStep(){

    if (_isAutoCollect) {
        return;
    }
    
    
    //Pile 中转单元合并
    for (int i = 0; i < SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS; i++) {
        Vector<Trump*> rows = _viewModel->getFieldRow(i);
        int rowX = -1;
        bool find = false;
//        for (int i = 0; i < rows.size(); i++) {
//            auto card = rows.at(i);
//            if ((!card->getData()->isReverse())) {
//                //pileOpen底部元素检查
//                
//                _isTouchCard = true;
//                _touchingCard = card;
//                rowX = canTouchCardMove(_viewModel->isLast(card->getData()->getFieldX(), _touchingCard->getData()->getFieldY()), true);
//                if (rowX != -1) {
//                    find = true;
//                    
//                }
//                break;
//            }
//        }
        if((!find) && (rows.size() > 0 )){
            //pile Open顶部元素检查
            auto card = rows.at(rows.size() - 1);
            _isTouchCard = true;
            _touchingCard = card;
            rowX = canTouchCardMove(_viewModel->isLast(card->getData()->getFieldX(), _touchingCard->getData()->getFieldY()), true);
            if (rowX != -1) {
                find = true;
            }
        }
        
        if (find) {
            
            onSingleTapOpenCard(_viewModel->isLast(_touchingCard->getData()->getFieldX(), _touchingCard->getData()->getFieldY()));
            
            _isTouchCard = false;
            _touchingCard = nullptr;
            return;
        }
        _isTouchCard = false;
        _touchingCard = nullptr;
    }
    
    auto cards = _viewModel->getOpenStockCards();
    if (cards.size() > 0) {
        Trump *card = cards.back();
        
        //suit
        for (int i = 0; i < SolitaireKlondikeViewModel::COUNT_DROP_MAX_ROWS; i++) {
            auto suit = _viewModel->getSuitRow(i);
            if (suit.size() > 0) {
                auto card2 = suit.back();
                if (_viewModel->isKlondikeDrop(card2, card)) {
                    openCardMove(card);
                    return;
                }
            }
        }
        // empty suit
        if (card->getData()->getValue() == 1) {
            for (BlankCard* blank: _viewModel->getDropBlanks()) {
                // 是否空白
                if (_viewModel->getDropRow(blank->getX()).size() != 0) {
                    continue;
                }
                openCardMove(card);
                return;
            }
        }
        
        //pile
        for (int i = 0; i < SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS; i++) {
            auto pile = _viewModel->getFieldRow(i);
            if (pile.size() > 0) {
                auto card2 = pile.back();
                if (_viewModel->isKlondikeSequence(card2, card)) {
                    openCardMove(card);
                    return;
                }
            }
        }
        // empty pile
        if (card->getData()->getValue() == 13){
            for (BlankCard* blank: _viewModel->getFieldBlanks()) {
                // 这列是不是空白
                if (_viewModel->getFieldRow(blank->getX()).size() != 0) {
                    continue;
                }
                openCardMove(card);
                return;
            }
        }
    }
    
    
//    auto cards = _viewModel->getOpenStockCards();
//    if (cards->size() > 0) {
//        Trump *card = cards->back();
//        _touchingCard = card;
//        _isTouchCard = true;
//        //???
//        onSingleTapOpenCard(_viewModel->isLast(_touchingCard->getData()->getFieldX(), _touchingCard->getData()->getFieldY()));
//        _isTouchCard = false;
//        _touchingCard = nullptr;
//        
//        return;
//    }
    if (_viewModel->getCloseStockCards().size() != 0) {
        //close open card
        
        onSingleTapClosedStockCard();
    } else {
        auto cards = _viewModel->getOpenStockCards();
        if (cards.size() != 0) {
            onSingleTapBlankStockCard();
        }
    }
}



void SolitaireKlondikeScene::showWinnerActionNewRecord(function<void()> cb){
    _winnerTrumps.clear();
    for (int i = 0; i < 12; i++) {
        vector<Trump *> v1;
        _winnerTrumps.push_back(v1);
    }
    
    for (int i = 0; i < 4; i++) {
        auto v = _viewModel->getSuitRow(i);
        
        for (int j = 0; j < 12; j++) {
            //row
            
            _winnerTrumps[j].push_back(v.at(j));
            
        }
    }
    
    for (int i = 0; i < 4; i++) {
        _winnerTrumps[i].push_back(_viewModel->getSuitRow(i).back());
    }
    
//    for(auto list : _winnerTrumps){
//        for(auto card : list){
//            card->retain();
//            
//            auto p = _winnerActionParent->convertToNodeSpace(card->getPosition());
//            
//            card->removeFromParent();
//            
//            _winnerActionParent->addChild(card);
//            card->setPosition(p);
//            
//            card->release();
//        }
//    }
    
    Vec2 start = VisibleRect::center() + Vec2(-255, 0);
    start.rotate(VisibleRect::center(), CC_DEGREES_TO_RADIANS(-30));
    
    vector<Vec2> points;
    for (int i = 0; i < 12; i++) {
        start.rotate(VisibleRect::center(), CC_DEGREES_TO_RADIANS(-30));
//        auto p = _winnerActionParent->convertToNodeSpace(start);
        points.push_back(start);
    }
    
    float maxT = 0;
    for (int i = 0; i < 12; i++){
        for (int j = 0; j < _winnerTrumps[i].size(); j++){
            float t = points[i].getDistance(_winnerTrumps[i][j]->getPosition()) / 1200.f;
            maxT = MAX(maxT, t);
        }
    }
    
    for (int i = 0; i < 12; i++) {
        
        int moveIdx = (i + 2)%12;
        if (i > 4 && i < 10) {
            moveIdx = 12 - moveIdx;
        }
        for (int j = 0; j < _winnerTrumps[i].size(); j++) {
            auto card = _winnerTrumps[i][j];
            card->setLocalZOrder(j);
            
            float t =  points[i].getDistance(card->getPosition()) / 1200.f;
            auto ac1 = MoveTo::create(t, points[i]);
            auto ac2 = DelayTime::create(maxT - t);
            auto ac3 = DelayTime::create(2.f/60.f * (j + moveIdx));
            
            Vector<FiniteTimeAction *> actions;
            for (int k = 0; k < 8; k++) {
                auto easeMove = EaseSineOut::create(MoveTo::create(0.25f, points[(i + 1 + k)%12]));
                auto seq = Sequence::create(easeMove, DelayTime::create(0.13f), NULL);
                actions.pushBack(seq);
            }
            
            auto ac4 = Sequence::create(actions);
            auto ac5 = DelayTime::create(2.f/60.f * (18 - j - moveIdx));
            auto ac6 = MoveTo::create(0.25, card->getPosition());
            auto callback = CallFuncN::create([=](Node *sender){
            
                auto cd = static_cast<Trump *>(sender);
                cd->setLocalZOrder(cd->getData()->getFieldY());
            });
            
            card->runAction(Sequence::create(ac1, ac2, ac3, ac4, ac5, ac6, callback, NULL));
            
        }
    }
    
    runAction(Sequence::create(DelayTime::create(4), CallFunc::create([=](){
        _winnerTrumps.clear();
        
        for(auto card : _winnerAdditionTrumps){
            card->removeFromParent();
        }
        
        if (cb) {
            cb();
        }
        
    }), NULL));
    Audio::getInstance()->playEffect("Music/31_all_clear_card1.mp3");
}

void SolitaireKlondikeScene::stopWinnerAction(int num){
    
    
        stopActionByTag(911);
        
        for (auto list : _winnerTrumps) {
            for(auto card : list){
                card->stopAllActions();
                float snapPosX = (POS_DROP_OFFSET_X_NOW() * (card->getData()->getFieldX() - SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS)) + POS_DROP_START_X_NOW();
                float snapPosY = POS_DROP_START_Y ;
                
                card->setPosition(snapPosX, snapPosY);
                
                card->retain();
                card->removeFromParent();
                this->addChild(card);
                card->release();
            }
        }
        
        _winnerTrumps.clear();
        
        for(auto card : _winnerAdditionTrumps){
            card->removeFromParent();
        }
        _winnerAdditionTrumps.clear();
    
}

void SolitaireKlondikeScene::showWinnerActionNormal(function<void()> cb){
    _winnerTrumps.clear();
    _winnerAdditionTrumps.clear();
    
    for (int i = 0; i < 3; i++) {
        vector<Trump *> v1;
        _winnerTrumps.push_back(v1);
    }
    
    int num = 0;
    for (int i = 0; i < 13; i++) {
        for (int j = 2; j < 4; j++) {
            auto data = TrumpData::create(j, i + 1, false);
            Trump *card = Trump::create(data);
            _winnerAdditionTrumps.push_back(card);
            data->setFieldX(num);
            data->setFieldY(i);
            
            float snapPosX = (POS_DROP_OFFSET_X_NOW() * (card->getData()->getFieldX() - SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS)) + POS_DROP_START_X_NOW();
            float snapPosY = POS_DROP_START_Y ;
            
            card->setPosition(snapPosX, snapPosY);
            this->addChild(card, i);
            
            num++;
            if (num >= 4) {
                num = 0;
            }
        }
    }
    for (auto e : _winnerAdditionTrumps) {
        _winnerTrumps.at(2).push_back(e);
    }
    
    for (int j = 0; j < 2; j++) {
        int row1 = j;
        int row2 = j == 0 ? 3 : 2;
        for (int i = 0 ; i < 13 ; i++) {
            int idx = i;
            _winnerTrumps.at(j).push_back(_viewModel->getSuitRow(row1).at(idx));
            _winnerTrumps.at(j).push_back(_viewModel->getSuitRow(row2).at(idx));
        }
    }
    
    //4 7 10 13
    //3 6 9 12
    //2 5 8 11
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 4; k++) {
                
                int y = i + k*3 + 2;
                auto data = TrumpData::create(j, y, false);
                Trump *card = Trump::create(data);
                _winnerAdditionTrumps.push_back(card);
                data->setFieldX(num);
                data->setFieldY(y-1);
                
                float snapPosX = (POS_DROP_OFFSET_X_NOW() * (card->getData()->getFieldX() - SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS)) + POS_DROP_START_X_NOW();
                float snapPosY = POS_DROP_START_Y ;
                
                card->setPosition(snapPosX, snapPosY);
                this->addChild(card, y-1);
                
                _winnerTrumps[i].insert(_winnerTrumps[i].end() - 2 * (2-i), card);
            }
        }
    }
    
//    for(auto list : _winnerTrumps){
//        for(auto card : list){
//            card->retain();
//            
//            auto p = _winnerActionParent->convertToNodeSpace(card->getPosition());
//            
//            card->removeFromParent();
//            
//            _winnerActionParent->addChild(card);
//            card->setPosition(p);
//            
//            card->release();
//        }
//    }
    
    
    Vec2 pos[] = {VisibleRect::center() + Vec2(58-320, -140), VisibleRect::center() + Vec2(320-58, -140), VisibleRect::center() + Vec2(0, 320)};
//    for(int i = 0; i < 3;i++){
//        pos[i] = _winnerActionParent->convertToNodeSpace(pos[i]);
//    }
    
    for (int i = 0; i < 3; i++) {
        for(int j = 0; j < _winnerTrumps[i].size(); j++){
            auto card = _winnerTrumps[i][j];
            card->setOpacity(192);
            card->setLocalZOrder(j+1);
            auto p1 = pos[(i+2)%3];
            auto p2 = pos[(i+1)%3];
            auto p3 = pos[(i)%3];
            
            auto ac = Repeat::create(Sequence::create(MoveTo::create(0.15, p1), DelayTime::create(0.18), MoveTo::create(0.15, p2),DelayTime::create(0.18), MoveTo::create(0.15, p3),DelayTime::create(0.18), NULL), 3);
            
            auto move = MoveBy::create(0.15, pos[i]- card->getPosition());
            
            Vector<FiniteTimeAction*> actions;
            actions.pushBack(move);
            actions.pushBack(DelayTime::create(1.f/120.f * j));
            actions.pushBack(ac);
            actions.pushBack(FadeTo::create(0, 255));
            actions.pushBack(DelayTime::create(1.f/120.f * (_winnerTrumps[i].size() - 1-j)));
            actions.pushBack(move->reverse());
            if (i == 3) {
                actions.pushBack(Hide::create());
            }
            card->runAction(Sequence::create(actions));
        }
    }
    
    auto ac = Sequence::create(DelayTime::create(4), CallFunc::create([=](){
        _winnerTrumps.clear();
        
        for(auto card : _winnerAdditionTrumps){
            card->removeFromParent();
        }
        _winnerAdditionTrumps.clear();
        
        if (cb) {
            cb();
        }
        
    }), NULL);
    ac->setTag(911);
    runAction(ac);
    Audio::getInstance()->playEffect("Music/31_all_clear_card2.mp3");
}

//void SolitaireKlondikeScene::showWinnerAction(){
//    
//    std::vector<int> v;
//    v.push_back(2);
//    v.push_back(1);
//    v.push_back(0);
//    v.push_back(3);
//    Vec2 target = VisibleRect::center();
//    target.y *= 0.7f;
//    for (int i = 0; i < 4; i++) {
//        auto row = _viewModel->getSuitRow(v.at(i));
//        float delay = i;
//        for (int j = 0; j < row.size(); j++) {
//            auto card = row.at(j);
//            auto ac = DelayTime::create(delay);
//            auto ac0 = DelayTime::create((float)j/15.f);
//            auto ac2 = EaseSineOut::create(ac0);
//            auto ac3 = MoveTo::create(0.4, target);
//            card->runAction(Sequence::create(ac, ac2, ac3, NULL));
//        }
//    }
//    auto ac = DelayTime::create(1.8f);
//    auto callback = CallFuncN::create([=](Ref *sender){
//        auto allCards = _viewModel->getAllCards();
//        for (int i = 0; i < allCards.size();i++) {
//            auto card = allCards.at(i);
//            card->stopAllActions();
//            auto size = VisibleRect::getVisibleRect().size;
//            auto target = Vec2(cocos2d::random(0.f, size.width),cocos2d::random(200.f, size.height));
//            auto ac = MoveTo::create(0.7, target);
//            auto sc1 = RotateTo::create(0.7, random(0.f, 360.f));
//            auto ac2 = DelayTime::create(i*2.f/54.f);
//            auto ac3 = MoveTo::create(0.2, Vec2(POS_STOCK_START_X_NOW(), POS_STOCK_START_Y));
//            auto sc2 = RotateTo::create(0.2, 0);
//            card->runAction(Sequence::create(Spawn::createWithTwoActions(ac, sc1), ac2, Spawn::createWithTwoActions(ac3, sc2), NULL));
//        }
//    });
//    runAction(Sequence::create(ac, callback, NULL));
//    
//    Audio::getInstance()->playEffect("Music/31_all_clear_card2.mp3");
//}

void SolitaireKlondikeScene::openCardMove(Trump *card) {
    _touchingCard = card;
    _isTouchCard = true;
    
    onSingleTapOpenCard(_viewModel->isLast(_touchingCard->getData()->getFieldX(), _touchingCard->getData()->getFieldY()));
    _isTouchCard = false;
    _touchingCard = nullptr;
}

void SolitaireKlondikeScene::onDailyCallback(cocos2d::Ref* pSender){

    auto la = Layer_Daily::create(this);
    la->setGameScene(this);
    la->setFromGameMenu(true);
    auto sc = Scene::create();
    sc->addChild(la);
    Director::getInstance()->pushScene(sc);
}

void SolitaireKlondikeScene::pauseLayerDidQuit(KTPauseLayer *layer){
//    setVisible(true);
}

void SolitaireKlondikeScene::stopNextMove(){
    if (_isTipMove) {
        
        
        
        _isTipMove = false;
        for (auto card : _tipCards) {
            card->removeFromParent();
        }
        _tipCards.clear();
        
        _tipMask->setVisible(false);
        
        for (auto ac : actions) {
            stopAction(ac);
        }
    }
}

bool SolitaireKlondikeScene::checkNoMove(){

    for (int i = 0; i < SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS; i++) {
        Vector<Trump*> rows = _viewModel->getFieldRow(i);
        int rowX = -1;
        bool find = false;
        for (int i = 0; i < rows.size(); i++) {
            auto card = rows.at(i);
            if ((!card->getData()->isReverse())) {
                //pileOpen底部元素检查
                
                _isTouchCard = true;
                _touchingCard = card;
                rowX = canTouchCardMove(_viewModel->isLast(card->getData()->getFieldX(), card->getData()->getFieldY()), true);
                if (rowX != -1) {
                    _isTouchCard = false;
                    _touchingCard = nullptr;
                    find = true;
                    return false;
                }
                break;
            }
        }
        _isTouchCard = false;
        _touchingCard = nullptr;
        if((!find) && (rows.size() > 0 )){
            //pile Open顶部元素检查
            auto card = rows.at(rows.size() - 1);
            _isTouchCard = true;
            _touchingCard = card;
            rowX = canTouchCardMove(_viewModel->isLast(card->getData()->getFieldX(), card->getData()->getFieldY()), true);
            if (rowX != -1) {
                _isTouchCard = false;
                _touchingCard = nullptr;
                find = true;
                return false;
            }
        }
        _isTouchCard = false;
        _touchingCard = nullptr;
    }
    
    Vector<Trump*> cards;
    if (DataManager::getInstance()->getSettings()->m_chou3card) {
        auto openCards = _viewModel->getOpenStockCards();
        if (openCards.size() != 0) {
            cards.pushBack(openCards.back());
        }
        auto closeCards = _viewModel->getCloseStockCards();
        
        if (closeCards.size() > 2) {
            for(int i = 2; i < closeCards.size(); i+=3){
                auto card = closeCards.at(i);
                cards.pushBack(card);
            }
        }
        if (closeCards.size() % 3 != 0) {
            cards.pushBack(closeCards.back());
        }
    } else {
        auto openCards = _viewModel->getOpenStockCards();
        if (openCards.size() != 0) {
            cards.pushBack(openCards.back());
        }
        auto closeCards = _viewModel->getCloseStockCards();
        for(auto card : closeCards){
            cards.pushBack(card);
        }
    }
    
//    
//    auto closecards = _viewModel->getOpenStockCards();
//    for(auto it = closecards.rbegin(); it != closecards.rend(); ++it) {
////        closecards.at(it);
//        cards.pushBack(*it);
//    }
//    auto openCards = _viewModel->getCloseStockCards();
//    for(auto card : openCards){
//        cards.pushBack(card);
//    }
//    int step = DataManager::getInstance()->getSettings()->m_chou3card ? 3 : 1;
    for(auto card : cards) {
//        auto card = cards.at(cards.size()-1-i);
//        Trump *card = cards.back();
            //suit
            for (int i = 0; i < SolitaireKlondikeViewModel::COUNT_DROP_MAX_ROWS; i++) {
                auto suit = _viewModel->getSuitRow(i);
                if (suit.size() > 0) {
                    auto card2 = suit.back();
                    if (_viewModel->isKlondikeDrop(card2, card)) {
                        return false;
                    }
                }
            }
            // empty suit
            if (card->getData()->getValue() == 1) {
                for (BlankCard* blank: _viewModel->getDropBlanks()) {
                    // 是否空白
                    if (_viewModel->getDropRow(blank->getX()).size() != 0) {
                        continue;
                    }
                    return false;
                }
            }
        
        //pile
        for (int i = 0; i < SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS; i++) {
            auto pile = _viewModel->getFieldRow(i);
            if (pile.size() > 0) {
                auto card2 = pile.back();
                if (_viewModel->isKlondikeSequence(card2, card)) {
                    return false;
                }
            }
        }
        // empty pile
        if (card->getData()->getValue() == 13){
            for (BlankCard* blank: _viewModel->getFieldBlanks()) {
                // 这列是不是空白
                if (_viewModel->getFieldRow(blank->getX()).size() != 0) {
                    continue;
                }
                return false;
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        auto suit = _viewModel->getSuitRow(i);
        if (suit.size() != 0) {
            auto card2 = suit.back();
            Trump *targetCard = nullptr;
            for (int j = 0; j < SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS; j++) {
                auto pile = _viewModel->getFieldRow(j);
                
                if (pile.size() > 1) {
                    for (int k = 0; k < pile.size()-1; k++) {
                        auto card = pile.at(k);
                        if (!card->getData()->isReverse() &&  _viewModel->isKlondikeDrop(card2, card) ){
                            
                            targetCard = pile.at(k + 1);
                            
                            break;
                        }
                    }
                }
                
            }
            //找到被遮挡的移到suit的目标牌的上一张牌
            if (targetCard) {
                _isTouchCard = true;
                _touchingCard = targetCard;
                int rowX = canTouchCardMove(_viewModel->isLast(targetCard->getData()->getFieldX(), _touchingCard->getData()->getFieldY()), false);
                if (rowX != -1) {
                    
                    _isTouchCard = false;
                    _touchingCard = nullptr;
                    return false;
                }
                _isTouchCard = false;
                _touchingCard = nullptr;
            }
        }
    }
    
    
    return true;
}

void SolitaireKlondikeScene::OnShowNextMoveCallback(cocos2d::Ref* pSender){

    if(!_isInitCardFinish) return;
    
    if(_isWin) return;
    
    _adNextMoveCounter++;
    
    if (_adNextMoveCounter >= HLAnalsytWrapper::intValue("KL_NextMoveCount", 100)) {
        HLAdManagerWrapper::showUnsafeInterstitial();
        _adNextMoveCounter = 0;
    }
    
    stopNextMove();
    
    auto showNoMoveFunc = [=](){
    
        if (_showNoMoveEnable) {
            if (!_isVegasOn) {
                //维加斯模式不出没有移动提示框
                auto la = KLNoMoveLayer::create();
                la->onTap = [=](KLNoMoveLayer *layer, int buttonIdx){
                    _showNoMoveEnable = !la->getCheckBox()->isSelected();
                    
                    if (_isDailyChanllenge) {
                        if (buttonIdx < 3) {
                            this->startGame(_viewModel->gameType);
                        }
                    } else{
                        if (buttonIdx < 3) {
                            this->startGame(buttonIdx);
                        }
                    }
                    
                };
                addChild(la, 2048);
                la->getCheckBox()->setSelected(!_showNoMoveEnable);
            }
            return true;
        }
        return false;
    };
    
    if (_showNoMoveStart == false) {
        if (checkNoMove()) {
            _showNoMoveStart = true;
            _showNoMoveOpenNum = 0;
        }
    } else {
        if (!checkNoMove()) {
            _showNoMoveOpenNum = 0;
        }
        if (_showNoMoveOpenNum > 1) {
            //show ui
            bool ret = showNoMoveFunc();
            if (ret) {
                return;
            }
        }
    }
    
    
    
    //kt
    bool flag = false;
    
    bool hasTipMove = false;
    
    int tipIdx = 0;
    float tipDelayTime = 0;
    
    actions.clear();
    vector<float> times;
    //Pile 中转单元合并
    for (int i = 0; i < SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS; i++) {
        Vector<Trump*> rows = _viewModel->getFieldRow(i);
        int rowX = -1;
        bool find = false;
        for (int i = 0; i < rows.size(); i++) {
            auto card = rows.at(i);
            if ((!card->getData()->isReverse())) {
                //pileOpen底部元素检查
                
                _isTouchCard = true;
                _touchingCard = card;
                rowX = canTouchCardMove(_viewModel->isLast(card->getData()->getFieldX(), _touchingCard->getData()->getFieldY()), true);
                if (rowX != -1) {
                    find = true;
                }
                break;
            }
        }
        if((!find) && (rows.size() > 0 )){
            //pile Open顶部元素检查
            auto card = rows.at(rows.size() - 1);
            _isTouchCard = true;
            _touchingCard = card;
            rowX = canTouchCardMove(_viewModel->isLast(card->getData()->getFieldX(), _touchingCard->getData()->getFieldY()), true);
            if (rowX != -1) {
                find = true;
            }
        }
        
        if (find) {
            
            Vector<Trump*> dragTrumps = *this->getDraggingGroup();
            
            actions.pushBack(Sequence::create(DelayTime::create(tipDelayTime), CallFunc::create([=]{
                for (int i = 0; i < dragTrumps.size(); i++) {
                    Trump* dragging = dragTrumps.at(i);
                    tipMoveCard(dragging, rowX, i);
                }
            }), NULL));
            auto t = getTipMoveTime(dragTrumps.at(0), rowX);
            times.push_back(t);
            
            _isTouchCard = false;
            _touchingCard = nullptr;
            hasTipMove = true;
            tipIdx++;
            tipDelayTime += t;
//            return;
        }
        _isTouchCard = false;
        _touchingCard = nullptr;
    }

    auto cards = _viewModel->getOpenStockCards();
    if (cards.size() > 0) {
        Trump *card = cards.back();
        if (!flag) {
            //suit
            for (int i = 0; i < SolitaireKlondikeViewModel::COUNT_DROP_MAX_ROWS; i++) {
                auto suit = _viewModel->getSuitRow(i);
                if (suit.size() > 0) {
                    auto card2 = suit.back();
                    if (_viewModel->isKlondikeDrop(card2, card)) {
                        
                        int rowX = card->getData()->getSuit() + SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS;
                        
                        actions.pushBack(Sequence::create(DelayTime::create(tipDelayTime), CallFunc::create([=]{
                            tipMoveCard(card, rowX,0);
                        }), NULL));
                        auto t = getTipMoveTime(card, rowX);
                        times.push_back(t);
                        
                        hasTipMove = true;
                        tipIdx++;
                        tipDelayTime += t;
//                        return;
                    }
                }
            }
        }
        if (!flag) {
            // empty suit
            if (card->getData()->getValue() == 1) {
                // 是否空白
                int rowX = card->getData()->getSuit() + SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS;
                
                if (_viewModel->getDropRow(rowX).size() == 0) {
                    actions.pushBack(Sequence::create(DelayTime::create(tipDelayTime), CallFunc::create([=]{
                        tipMoveCard(card , rowX,0);
                    }), NULL));
                    auto t = getTipMoveTime(card, rowX);
                    times.push_back(t);
                    hasTipMove = true;
                    tipIdx++;
                    tipDelayTime += t;
                }
                
                
            }
        }
        
        //pile
        for (int i = 0; i < SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS; i++) {
            auto pile = _viewModel->getFieldRow(i);
            if (pile.size() > 0) {
                auto card2 = pile.back();
                if (_viewModel->isKlondikeSequence(card2, card)) {
                    
                    actions.pushBack(Sequence::create(DelayTime::create(tipDelayTime), CallFunc::create([=]{
                        tipMoveCard(card, i, 0);
                    }), NULL));
                    auto t = getTipMoveTime(card, i);
                    times.push_back(t);
                    hasTipMove = true;
                    tipIdx++;
                    tipDelayTime += t;
//                    return;
                }
            }
        }
        // empty pile
        if (card->getData()->getValue() == 13){
            for (BlankCard* blank: _viewModel->getFieldBlanks()) {
                // 这列是不是空白
                if (_viewModel->getFieldRow(blank->getX()).size() != 0) {
                    continue;
                }
                
                actions.pushBack(Sequence::create(DelayTime::create(tipDelayTime), CallFunc::create([=]{
                    tipMoveCard(card, blank->getX(), 0);
                }), NULL));
                auto t = getTipMoveTime(card, blank->getX());
                times.push_back(t);
                
                hasTipMove = true;
                tipIdx++;
                tipDelayTime += t;
//                return;
            }
        }
    }
    
    //2段suit移牌
    for (int i = 0; i < 4; i++) {
        auto suit = _viewModel->getSuitRow(i);
        if (suit.size() != 0) {
            auto card2 = suit.back();
            Trump *targetCard = nullptr;
            for (int j = 0; j < SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS; j++) {
                auto pile = _viewModel->getFieldRow(j);
                
                if (pile.size() > 1) {
                    for (int k = 0; k < pile.size()-1; k++) {
                        auto card = pile.at(k);
                        if (!card->getData()->isReverse() &&  _viewModel->isKlondikeDrop(card2, card) ){
                            
                            targetCard = pile.at(k + 1);
                            
                            break;
                        }
                    }
                }
                
            }
            //找到被遮挡的移到suit的目标牌的上一张牌
            if (targetCard) {
                _isTouchCard = true;
                _touchingCard = targetCard;
                int rowX = canTouchCardMove(_viewModel->isLast(targetCard->getData()->getFieldX(), _touchingCard->getData()->getFieldY()), false);
                if (rowX != -1) {
                    
                    Vector<Trump*> dragTrumps = *this->getDraggingGroup();
                    actions.pushBack(Sequence::create(DelayTime::create(tipDelayTime), CallFunc::create([=]{
                        for (int i = 0; i < dragTrumps.size(); i++) {
                            Trump* dragging = dragTrumps.at(i);
                            tipMoveCard(dragging, rowX, i);
                        }
                    }), NULL));
                    auto t = getTipMoveTime(dragTrumps.at(0),rowX);
                    times.push_back(t);
                    
                    _isTouchCard = false;
                    _touchingCard = nullptr;
                    hasTipMove = true;
                    tipDelayTime += t;
//                    return;
                }
                _isTouchCard = false;
                _touchingCard = nullptr;
            }
        }
    }
//    if (!hasTipMove) {
        if (_viewModel->getCloseStockCards().size() != 0) {
            //close open card
            Trump* card = _viewModel->getCloseStockCards().at(0);
            
            
            actions.pushBack(Sequence::create(DelayTime::create(tipDelayTime), CallFunc::create([=]{
                tipMoveCard(card, SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X, 0);
            }), NULL));
            auto t = getTipMoveTime(card, SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X);
            times.push_back(t);
            hasTipMove = true;
            //        return;
            //        moveCard(card, SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X);
        } else {
            
            if (!_viewModel->getStockBlank()->isOutOfFlips()) {
                auto cards = _viewModel->getOpenStockCards();
                if (cards.size() != 0) {
                    for (auto card : cards) {
                        
                        actions.pushBack(Sequence::create(DelayTime::create(tipDelayTime), CallFunc::create([=]{
                            tipMoveCard(card, SolitaireKlondikeViewModel::INDEX_STOCK_CLOSE_X, 0);
                        }), NULL));
                        
                        auto t = getTipMoveTime(card, SolitaireKlondikeViewModel::INDEX_STOCK_CLOSE_X);
                        times.push_back(t);
                        
                        hasTipMove = true;
                        
                    }
                }
            }
        }
//    }
    if (actions.size() != 0) {
        auto first = actions.front();
        auto last = actions.back();
        if (first == last) {
            auto seq = Sequence::create(first, CallFunc::create([=]{
                if (!_tipMask->isVisible()) {
                    this->_tipMask->setVisible(true);
                }
                _isTipMove = true;
            }), DelayTime::create(times[0]), CallFunc::create([=]{
                if (_tipMask->isVisible()) {
                    this->_tipMask->setVisible(false);
                }
                _isTipMove = false;
            }), NULL);
            actions.replace(0, seq);
        } else {
            auto seq1 = Sequence::create(first, CallFunc::create([=]{
                if (!_tipMask->isVisible()) {
                    this->_tipMask->setVisible(true);
                }
                _isTipMove = true;
            }), NULL);
            actions.replace(0, seq1);
            
            auto seq2 = Sequence::create(last,DelayTime::create(times[actions.size()-1] ), CallFunc::create([=]{
                if (_tipMask->isVisible()) {
                    this->_tipMask->setVisible(false);
                }
                _isTipMove = false;
            }), NULL);
            actions.replace(actions.size()-1, seq2);
        }
        
        for(auto ac : actions){
            
            runAction(ac);
        }
    }
    if (!hasTipMove) {
        showNoMoveFunc();
    }
    
    
}

void SolitaireKlondikeScene::onGameStart(){
    //统计
    _gameStartChou3 = DataManager::getInstance()->getSettings()->m_chou3card;
    auto st = *DataManager::getInstance()->getStatisticAll();
    if (DataManager::getInstance()->getSettings()->m_chou3card) {
        auto sm = st.statistics_mode[1];
        sm.played++;
        st.statistics_mode[1] = sm;
    } else {
        auto sm = st.statistics_mode[0];
        sm.played++;
        st.statistics_mode[0] = sm;
    }
    DataManager::getInstance()->saveStatisticData(st);
    
    if (_viewModel->gameType == 0) {
        HLAnalsytWrapper::event(_gameStartChou3 ? "game_random_3_card" : "game_random_1_card");
        HLAnalsytWrapper::event("game_random_start");
    } else if (_viewModel->gameType == 2) {
        HLAnalsytWrapper::event(_gameStartChou3 ? "game_winnable_3_card" : "game_winnable_1_card");
        HLAnalsytWrapper::event("game_winnable_start");
    }
}

void SolitaireKlondikeScene::onGameEnd(){
    
    
    //时间得分700000/游戏时间
    if (!_isVegasOn) {
        //维加斯没有时间积分奖励
        _score += 700000/(_timer);
    }
    
    auto st = *DataManager::getInstance()->getStatisticAll();
    int score = getTotalScore();
    char str[64];
    sprintf(str, "%s:%s%d",LocalizedString("TID_UI_SCORE").c_str(), _isVegasOn ? "$" : "", score);
    _scoreLabel->setString(str);
    
    if (_gameStartChou3) {
        auto sm = st.statistics_mode[1];
        sm.winned++;
        sm.best_score = MAX(sm.best_score, score);
        int size = (int)_viewModel->getTrumpSize();
        sm.lease_move = sm.lease_move == 0 ? size : MIN(sm.lease_move, size);
        sm.best_time = sm.best_time == 0 ? _timer : MIN(sm.best_time, _timer);
        sm.total_time += _timer;
        st.statistics_mode[1] = sm;
    } else {
        auto sm = st.statistics_mode[0];
        sm.winned++;
        sm.best_score = MAX(sm.best_score, score);
        int size = (int)_viewModel->getTrumpSize();
        sm.lease_move = sm.lease_move == 0 ? size : MIN(sm.lease_move, size);
        sm.best_time = sm.best_time == 0 ? _timer : MIN(sm.best_time, _timer);
        sm.total_time += _timer;
        st.statistics_mode[0] = sm;
    }
    DataManager::getInstance()->saveStatisticData(st);
    
    if (_isVegasOn) {
        DataManager::getInstance()->setVegasScore(score);
    }
    
    
    if (_viewModel->gameType == 0) {
        HLAnalsytWrapper::event("game_random_win");
    } else if (_viewModel->gameType == 2) {
        HLAnalsytWrapper::event("game_winnable_win");
    }
    
    
    {
        std::unordered_map<std::string, std::string> content;
        content["uid"] = iOSWrapper::getUserID();
        content["data"] = TrumpModel::create()->getSequenceString();
        content["cards"] = DataManager::getInstance()->getSettings()->m_chou3card ? "3" : "1";
        content["status"] = "1";
        
        std::string os = "1";
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        os = "2";
#endif
        content["app_os"] = os;
        
        std::unordered_map<std::string, std::string> param;
        
        std::string jsonString = KTUtils::jsonString(content);
        param["content"] = jsonString;
        
        auto str = content["uid"] + "#" + content["data"] + "#" + content["cards"] + "#" + kMD5_Secret;
        //md5
        param["x_client_data"] = MD5(str).toStr();
        
        CCLOG("origin md5 = %s", str.c_str());
        
        CCLOG("uid = %s", content["uid"].c_str());
        CCLOG("json string = %s", jsonString.c_str());
        HttpManager::create()->sendGET("http://45.113.69.21/ao2/play_cards_games.php", param, [](const rapidjson::Document &json){
            CCLOG("胜利排序上传成功");
        }, [](std::string errror){
            CCLOG("胜利排序上传失败");
        });
    }

    KTDate *utcDate = nullptr;
    if (_isDailyChanllenge)
    {
        
        DataManager::getInstance()->setDailyChanllengeSuccess(true);
        
        std::unordered_map<std::string, std::string> content;
        content["uid"] = iOSWrapper::getUserID();
        
        auto mgr = DataManager::getInstance();
        auto requestUTC = mgr->getRequstData()->utcDate();
        CCLOG("requestUTC = %ld", requestUTC->stamp());
        utcDate = requestUTC->dateByAddingTimeInterval(mgr->getTimePast() + mgr->getDayOffset() * 24 * 60 * 60);
        CCLOG("utcDate = %ld", utcDate->stamp());
        auto sp = utcDate->stamp();
        char str[64];
        sprintf(str, "%ld", sp);
        
        content["award_time"] = str;
        content["device_time_zone"] = iOSWrapper::getLocalTimeZone();
        content["game_id"] = _gameID;
        sprintf(str, "%d", score);
        content["score"] = str;
        sprintf(str, "%d", _viewModel->getTrumpSize());
        content["moves"] = str;
        sprintf(str, "%d", (int)_timer);
        content["use_time"] = str;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        content["app_os"] = "2";
#endif
        std::unordered_map<std::string, std::string> param;
        param["content"] = KTUtils::jsonString(content);
        //md5
        auto str2 = content["uid"] + "#" + content["award_time"] + "#" + content["device_time_zone"] + "#" + kMD5_Secret;
        param["x_client_data"] = MD5(str2).toStr();
        //http://45.113.69.21/ao2/play_cards_awards.php depricated
        
        HttpManager::create()->sendGET("http://45.113.69.21/ao2/play_cards_score.php", param, [](const rapidjson::Document &json){
            
            
        }, [](std::string errror){
        
        });
    }
    _isLastHighScore = false;
    HighScoreModel *m = HighScoreModel::create();
    m->setScore(score);
    m->setTimeString(KTDate::date()->localDate()->toString());
    m->setMoveNum((int)_viewModel->getTrumpSize());
    m->setTime(_timer);
    m->setChallengeDate(utcDate);
    if (!_isDailyChanllenge) {
        auto mgr = HighScoreManager::getInstance();
        if(mgr->addHighScore(m)){
            mgr->saveData();
            _isLastHighScore = true;
        } else {
        }
    }
    setCurrentScore(m);
    
    
    
    
}

void SolitaireKlondikeScene::onGameCancel(){
    
    auto st = *DataManager::getInstance()->getStatisticAll();
    if (_gameStartChou3) {
        auto sm = st.statistics_mode[1];
        sm.total_time += _timer;
        st.statistics_mode[1] = sm;
    }else{
        auto sm = st.statistics_mode[0];
        sm.total_time += _timer;
        st.statistics_mode[0] = sm;
    }
    DataManager::getInstance()->saveStatisticData(st);
    
    int score = getTotalScore();
    if (_isVegasOn) {
        DataManager::getInstance()->setVegasScore(score);
    }
    
    //10次以上移牌操作 且取消游戏 看作游戏失败
    if (_viewModel->getTrumpSize() > 10) {
        std::unordered_map<std::string, std::string> content;
        content["uid"] = iOSWrapper::getUserID();
        content["data"] = TrumpModel::create()->getSequenceString();
        content["cards"] = DataManager::getInstance()->getSettings()->m_chou3card ? "3" : "1";
        content["status"] = "0";
        
        std::string os = "1";
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        os = "2";
#endif
        content["app_os"] = os;
        
        std::unordered_map<std::string, std::string> param;
        
        std::string jsonString = KTUtils::jsonString(content);
        param["content"] = jsonString;
        
        CCLOG("uid = %s", content["uid"].c_str());
        CCLOG("json string = %s", jsonString.c_str());
        HttpManager::create()->sendGET("http://45.113.69.21/ao2/play_cards_games.php", param, [](const rapidjson::Document &json){
            CCLOG("失败排序上传成功");
        }, [](std::string errror){
            CCLOG("失败排序上传失败");
        });
    }
}

int SolitaireKlondikeScene::getTotalScore(){
    //如果是计时游戏， 每玩10秒钟减2分。
    int score = _isVegasOn ? _score : (_score - ((int)_timer/10) * 2);
    for(int i = 0; i < _viewModel->getTrumpSize(); i++) {
        auto tr = _viewModel->getTrumRoute(i);
        score += tr->score;
    }
    return score;
}

//重开本局
void SolitaireKlondikeScene::NTFRepeateGameCallback(cocos2d::Ref* pSender){
    
    startGame(1);
}
//重开活局
void SolitaireKlondikeScene::NTFNewGameWinnalbeCallback(cocos2d::Ref* pSender){
    startGame(2);
}

void SolitaireKlondikeScene::NTFDailyGameWinnalbeCallback(cocos2d::Ref* pSender){
    _quitChanllengeButton->setVisible(true);
    _bottomMenu->setItemVisible(1, false);
    _isDailyChanllenge = true;
    startGame(3);
}

//0 1 2 3每日挑战
void SolitaireKlondikeScene::startGame(int gameType){
    stopNextMove();
    if (_gameStart && !_isWin) {
        //放弃本局
        
        onGameCancel();
    }
    
    
    // 移除所有的空白牌
    Vector<BlankCard*> rows_Blank = _viewModel->getAllBlanks();
    
    for (BlankCard* blankcard : rows_Blank) {
        
        blankcard->removeFromParent();
        
    }
    // 移除所有的牌
    Vector<Trump*> rows_Trump = _viewModel->getAllCards();
    
    for (Trump* trump : rows_Trump) {
        
        trump->removeFromParent();
        
    }
    
    auto callFunc = [=](){
        this->setViewModel(SolitaireKlondikeViewModel::create(gameType));
        
        this->initValues();
        // 初始化空白牌
        this->initBlank();
        // 开始发牌
        this->initCarts();
    };
    
    auto requestFunc = [=](std::string url, std::unordered_map<std::string, std::string> param){
    
        KLProgressLayer::show(this);
        HttpManager::create()->sendGET(url, param, [=](const rapidjson::Document &json){
            
            if (json["status"].GetInt() == 1) {
                if (json.HasMember("game_id")) {
                    _gameID = json["game_id"].GetString();
                }
                std::string seqString = json["data"].GetString();
                this->setViewModel(SolitaireKlondikeViewModel::create(gameType, seqString));
                
                this->initValues();
                // 初始化空白牌
                this->initBlank();
                // 开始发牌
                this->initCarts();
                
            } else {
                callFunc();
            }
            KLProgressLayer::hide(this);
        }, [=](std::string errror){
            //        CCLOG("胜利排序上传失败");
            KLProgressLayer::hide(this);
            callFunc();
        });
    };
    
    //每日牌局固定 接口分离
    
    if(gameType == 0){
        std::unordered_map<std::string, std::string> content;
        content["uid"] = iOSWrapper::getUserID();
        content["cards"] = DataManager::getInstance()->getSettings()->m_chou3card ? "3" : "1";
        content["level"] = StringUtils::format("%f", KLUtils::getUserAbility());
        content["games"] = StringUtils::format("%d", KLUtils::getUserPlayNum());
        std::unordered_map<std::string, std::string> param;
        
        std::string jsonString = KTUtils::jsonString(content);
        param["content"] = jsonString;
        
        requestFunc("http://45.113.69.21/ao2/play_cards_games_random.php", param);
    } else if (gameType == 2) {
        
        std::unordered_map<std::string, std::string> content;
        content["uid"] = iOSWrapper::getUserID();
        content["cards"] = DataManager::getInstance()->getSettings()->m_chou3card ? "3" : "1";
        content["level"] = StringUtils::format("%f", KLUtils::getUserAbility());
        content["games"] = StringUtils::format("%d", KLUtils::getUserPlayNum());
        std::unordered_map<std::string, std::string> param;
        
        std::string jsonString = KTUtils::jsonString(content);
        param["content"] = jsonString;
        
        requestFunc("http://45.113.69.21/ao2/play_cards_games_info.php", param);
        
    } else if (gameType == 3) {
        std::unordered_map<std::string, std::string> content;
        content["uid"] = iOSWrapper::getUserID();
        auto mgr = DataManager::getInstance();
        auto requestUTC = mgr->getRequstData()->utcDate();
        auto utcDate = requestUTC->dateByAddingTimeInterval(mgr->getTimePast() + mgr->getDayOffset() * 24 * 60 * 60);
        auto sp = utcDate->stamp();
        char str[64];
        sprintf(str, "%ld", sp);
        content["challenge_time"] = str;
        
        CCLOG("requestUTC = %ld", requestUTC->stamp());
        CCLOG("utcDate = %ld", utcDate->stamp());
        
        content["challenge_time_zone"] = iOSWrapper::getLocalTimeZone();
        std::unordered_map<std::string, std::string> param;
        
        std::string jsonString = KTUtils::jsonString(content);
        param["content"] = jsonString;
        
        requestFunc("http://49.51.8.225/ao2/play_cards_challenge.php", param);
    } else {
        callFunc();
    }
}

//开始随机
void SolitaireKlondikeScene::NTFNewGameCallback(Ref* pSender) {
    startGame(0);
}

void SolitaireKlondikeScene::NTFUndoCallback(Ref* pSender) {
    // 回退
    
    if (!_isInitCardFinish) {
        return;
    }
    
    if (_isWin) {
        return;
    }
    
    if(_isRefreshCard) return;
    
    if (_adUndoCounter >= HLAnalsytWrapper::intValue("KL_UndoCount", 100)) {
        HLAdManagerWrapper::showUnsafeInterstitial();
        _adUndoCounter = 0;
    }
    
    if(_isUndoingCard)
        return;
    
    if(_viewModel->getTrumpSize() <= 0)
        return;
    
    stopNextMove();
    
    
    _adUndoCounter++;

//    _draggedTrumps = new Vector<Trump*>();
    _draggedTrumps.clear();
    
    // 总共拖动了多少次纸牌
    int dragedMove = _viewModel->getTrumpSize();
    
    // 获得要撤退到的x位置
    int toX = _viewModel->getTrumRoute(dragedMove-1)->fromX;
    
    // 拖动后，是否翻转过牌库的牌
    bool isCauseFlip = _viewModel->getTrumRoute(dragedMove-1)->isCauseFlip;
    
    Trump* dragedTrump = _viewModel->getTrumRoute(dragedMove-1)->dragedTrump;
    
    Vector<Trump*>* dragedTrumps = this->getUndoGroup(dragedTrump);
    
    int dragedX = dragedTrump->getData()->getFieldX();
    
    // 撤销操作前的那一步，总共拖动了多少牌
    int dragedSize = (int)dragedTrumps->size();
    
    // 拖动后的纸牌在牌堆中
    if(dragedX < SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X){
        // 返回到的那个牌堆列的牌的多少
        int size = (int)_viewModel->getFieldRow(toX).size();
        if(isCauseFlip && size>0){
            _viewModel->getFieldRow(toX).at(size-1)->getData()->setReverse(true);
            _viewModel->getFieldRow(toX).at(size-1)->flip(NULL);
        }
        float dtY = getDeltaY(toX, dragedTrumps->size());
        for (int i = 0;i < dragedTrumps->size(); i++) {
            auto dragged = dragedTrumps->at(i);
            undoMove(dragged, toX, false, dtY, i+1);
        }
    }
    else if(dragedX < SolitaireKlondikeViewModel::INDEX_STOCK_CLOSE_X){
        // 拖动后的纸牌在open库中，undoGroup中的牌集合逆向翻转
        for(int i=dragedSize-1; i>=0;i--){
            undoMove(dragedTrumps->at(i), toX, i == dragedSize-1 ? false : true);
        }
    }
    else if(dragedX ==  SolitaireKlondikeViewModel::INDEX_STOCK_CLOSE_X ){
        // 上一次的操作时，点击牌库的空白牌，洗了一下open库
        // 洗open库的牌，到close库中
        Vector<Trump*> row = _viewModel->getFieldRow(SolitaireKlondikeViewModel::INDEX_STOCK_CLOSE_X);
        // close库的位置
        Point pos_close = row.at(0)->getPosition();
        
        while (_viewModel->getFieldRowSize(SolitaireKlondikeViewModel::INDEX_STOCK_CLOSE_X) > 0) {
            
            Trump* moveCard = _viewModel->undoMove(_viewModel->getCard(SolitaireKlondikeViewModel::INDEX_STOCK_CLOSE_X, 0), toX);
            
            moveCard->stopAllActions();
            
            // 翻转过来
            moveCard->getData()->setReverse(false);
//            moveCard->refresh();
            
            // Z order
            moveCard->setLocalZOrder(moveCard->getData()->getFieldY());
            
            moveCard->flip(nullptr, [=](){
            });
        }
        
        // 调整open库牌的位置
        row = _viewModel->getFieldRow(SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X);
        // open库牌的三个位置
        // 从左到右依次为pos1,pos2,pos3
        //kt
        Point pos3 = pos_close + Vec2((m_l_r_mode ? (-POS_FIELD_OFFSET_X_NOW()) : (POS_FIELD_OFFSET_X_NOW() + 2 * abs(POS_STOCK_OFFSET_X))), 0);
        //Point(POS_FIELD_OFFSET_X_NOW() * ScaleFactor::scale_x, 0) * (m_l_r_mode ? -1 : 1);
        //this->moveCard(nullptr, 0);
        
        auto func = [=](Trump *moveCard, Vec2 moveTo, int num, bool islast){
            
            _draggedTrumps.pushBack(moveCard);
            MoveTo* move = MoveTo::create(m_tweem_card_move_speed, moveTo);
            EaseSineOut* ease = EaseSineOut::create(move);
            CallFunc* call1 = CallFuncN::create(CC_CALLBACK_1(SolitaireKlondikeScene::onCompleteDragTween, this));
            CallFunc* call2 = CallFunc::create(CC_CALLBACK_0(SolitaireKlondikeScene::flipFieldCard, this));
            CallFunc* call3 = CallFunc::create(CC_CALLBACK_0(SolitaireKlondikeScene::endTween, this));
            auto call4 = CallFuncN::create([=](Node *sender){
                if (islast) {
                    _isRefreshCard = false;
                }
            });
            Sequence* seq = Sequence::create(DelayTime::create(1/90.f * num), CallFunc::create([=](){
                // 撤销音效
                if(num < 10){
                    Audio::getInstance()->playEffect(WordFactor::CLICK_EFFECT_UNDO_OGG);
                }
            }), call2, ease, call1, call3, call4, nullptr);
            moveCard->runAction(seq);
            _isRefreshCard = true;
        };
        
        Point pos2 = pos3 + Point(POS_STOCK_OFFSET_X_NOW() * ScaleFactor::scale_x, 0);
        Point pos1 = pos2 + Point(POS_STOCK_OFFSET_X_NOW() * ScaleFactor::scale_x, 0);
        if(row.size() == 1){
            func(row.at(0), pos3, 0, true);
        } else if (row.size() == 2){
            func(row.at(0), pos2, 0, false);
            func(row.at(1), pos3, 1, true);
        } else if (row.size() >= 3){
            for(int i = 0; i < row.size(); i++){
                if(i<row.size()-2)
                    func(row.at(i), pos1, i, false);
                else if(i == row.size()-2){
                    func(row.at(i), pos2, i, false);
                } else if (i == row.size() - 1){
                    func(row.at(i), pos3, i, true);
                }
            }
        }
        /*
        for(int i=0; i<row.size(); i++){
            if(row.size() == 1)
                row.at(i)->setPosition(pos3);
            else if(row.size() == 2){
                row.at(0)->setPosition(pos2);
                row.at(1)->setPosition(pos3);
            }
            else if(row.size() >= 3){
                if(i<row.size()-2)
                   row.at(i)->setPosition(pos1);
                else{
                   row.at(row.size()-2)->setPosition(pos2);
                   row.at(row.size()-1)->setPosition(pos3);
                }
            }
        }
         */
    }

    
    _viewModel->removeTrumpRoute(_viewModel->getTrumpSize() - 1);  //getTrumpRoutes().popBack();
}

void SolitaireKlondikeScene::NTFSettingRefreshCallback(cocos2d::Ref* pSender){
    refresh();
}

bool SolitaireKlondikeScene::onTouchBegan(Touch* touch, Event* event) {
    
    _autoTipTimer = 0;
    
    if (_isWin) {
        CCLOG("touch false 11");
        return false;
    }
    if (!_isInitCardFinish) {
        return false;
    }
    if (_isTouchCard) {
        CCLOG("touch false 22");
//        CCLOG("touch return");
        return false;
    }
    
    if (_isRefreshCard) {
        return false;
    }

    _isTouchCard = false;
    _isMovingCard = false;
    _touchingCard = nullptr;

    Point location = touch->getLocation();
    _beginTouchPos = location;
    Trump* dragCandidate = nullptr;
    int lastZOrder = -1;

    // 触摸牌堆中的可动的牌
    for (Trump* trump: _viewModel->getFieldCards()) {

        // 翻牌不能动
        if (trump->getData()->isReverse()) {
            continue;
        }
        // 如果牌正在动，不能动
        if (trump->getNumberOfRunningActions() > 0) {
            continue;
        }

        if (trump->getBoundingBox().containsPoint(location)) {
            // Z値最大的为拖拽对象
            // 比如2张牌都是牌面朝上的，且都在触摸点范围，这时，只能拖动最上面那张牌
            if (trump->getLocalZOrder() > lastZOrder) {
                dragCandidate = trump;
                lastZOrder = trump->getLocalZOrder();
            }
        }
    }

    // 触摸牌库中打开的牌
    if (dragCandidate == nullptr) {
        // 获取open牌库中最上面那张牌
        Trump* stock = _viewModel->getDraggableStockRow();
        if (stock != nullptr) {
            // 如果牌有action的话，不能拖拽
            if (stock->getNumberOfRunningActions() == 0) {
                if (stock->getBoundingBox().containsPoint(location)) {
                    dragCandidate = stock;
                }
            }
        }
    }

    // 触摸牌库中未打开的牌
    if (dragCandidate == nullptr) {
        // 获取close牌库的第一张牌
        for (Trump* trump: _viewModel->getCloseStockCards()) {
            // 如果牌有action的话，不能拖拽
            if (trump->getNumberOfRunningActions() == 0) {
                if (trump->getBoundingBox().containsPoint(location)) {
                    dragCandidate = trump;
                    break;
                }
            }
        }
    }
    
    // 触摸基础堆的牌
    if (dragCandidate == nullptr) {
        // 获取基础堆最上面的牌
        for (Trump* trump: _viewModel->getDropCards()) {
            
            // 如果牌正在动，不能动
            if (trump->getNumberOfRunningActions() > 0) {
                continue;
            }
            
            if (trump->getBoundingBox().containsPoint(location)) {
                // Z値最大的为拖拽对象
                // 比如2张牌都是牌面朝上的，且都在触摸点范围，这时，只能拖动最上面那张牌
                if (trump->getLocalZOrder() > lastZOrder) {
                    dragCandidate = trump;
                    lastZOrder = trump->getLocalZOrder();
                }
            }
        }
    }

    // 开始移动前
    if (dragCandidate != nullptr) {
        // 只要点击，就播放音效
//        Audio::getInstance()->playEffect(WordFactor::CLICK_EFFECT_MOVE_OGG);
        CCLOG("touch 11");
        
        _isTouchCard = true;
        _touchingCard = dragCandidate;
//        CCLOG("touch card value = %d action = %zd",_touchingCard->getData()->getValue(), _touchingCard->getNumberOfRunningActions());
        
        _touchStartPos = dragCandidate->getPosition();
        _touchPos = Point(
                (dragCandidate->getPosition().x  + (dragCandidate->getScaleX() / 2)) - location.x //x中心点
                , (dragCandidate->getPosition().y + (dragCandidate->getScaleY() / 2)) - location.y //y中心点
        );

        int index = 0;
        for (Trump* dragging : *this->getDraggingGroup()) {

            // 拖动时，把拖动的牌z值放到最大，目的是为了在拖动的过程中，不被其他牌遮挡
            dragging->setLocalZOrder(Z_DRAGGING + index);

            index++;
        }
        return true;
    }

//    CCLOG("_viewModel = %p", _viewModel);
//    CCLOG("_viewModel->getStockBlank() = %p", _viewModel->getStockBlank());
    auto stock = _viewModel->getStockBlank();
//    CCLOG("box = %s", CCStringFromRect(stock->getBoundingBox())->getCString());
    // 触摸的牌库的空白牌
    bool flag = !stock->isOutOfFlips();
    auto box =  stock->getBoundingBox();
    bool flag2 = box.containsPoint(location);
    if (flag && flag2) {
        CCLOG("touch 22");
        _isTouchCard = true;
        return true;
    }

    
    CCLOG("touch false 33");
    return false;
}

void SolitaireKlondikeScene::onTouchMoved(Touch* touch, Event* event) {

    if (!_isTouchCard || _touchingCard == nullptr) {
        return;
    }

    if (_touchingCard->getData()->getFieldX() == SolitaireKlondikeViewModel::INDEX_STOCK_CLOSE_X) {
        return;
    }
    
    float xdif = touch->getLocation().x - _beginTouchPos.x;
    float ydif = touch->getLocation().y - _beginTouchPos.y;
    float distance = sqrt(xdif * xdif + ydif * ydif);
    
    if(distance>20){
        _isMovingCard = true;
    }else{
        _isMovingCard = false;
    }

    // 在移动的过程中，动态调整拖动的纸牌的位置
    int index = 0;
    for (Trump* trump : *this->getDraggingGroup()) {
        Point dragOffset = Point(0, index * POS_FIELD_OFFSET_Y * ScaleFactor::scale_y);
        trump->setPosition(touch->getLocation() + _touchPos + dragOffset);
        index++;
    }
}

void SolitaireKlondikeScene::onTouchEnded(Touch* touch, Event* event) {

//    CCLOG("onTouchEnded");
    
    if (!_isTouchCard) {
        if (_touchingCard != nullptr){
            _touchingCard->setLocalZOrder(_touchingCard->getData()->getFieldY());
            
            int toX = _touchingCard->getData()->getFieldX();
            if (toX < SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS) {
                // 移动到牌库中
                auto rows = _viewModel->getFieldRow(toX);
                auto dtY = getDeltaY(toX, 0);
                auto snapPosX = (POS_FIELD_OFFSET_X_NOW() * ScaleFactor::scale_x * toX) + POS_FIELD_START_X_NOW() * ScaleFactor::scale_x ;
//
                auto snapPosY = getRowHeight(toX, -1, dtY) + POS_FIELD_START_Y;
                adjustRowHeight(toX, 0, dtY);
                
                rows.back()->setPositionY(snapPosY);
                
                for(auto card : rows){
                    card->setPositionX(snapPosX);
                    card->setLocalZOrder(card->getData()->getFieldY());
                }
            }
        }
        
        return;
    }

    Vector<Trump*>* dragTrumps = this->getDraggingGroup();

    // 加入这个vector,目的是为了移动了纸牌之后，恢复纸牌的Z值
//    _draggedTrumps = new Vector<Trump*>();
    _draggedTrumps.clear();

    // 点击牌库中的空白牌，把已经打开完的牌库，重新洗一下
    if (!_isMovingCard) {
        if (_touchingCard == nullptr) {
            onSingleTapBlankStockCard();
            _isTouchCard = false;
            return;
        }
    }

    // 拖动到牌堆中
    for (Trump* trump: _viewModel->getFieldCards()) {
        // 扫描的牌堆的这张牌，在正在拖动的纸牌中
        if (dragTrumps->contains(trump)) {
            continue;
        }

        // 拖动的纸牌，移动到的位置，并没有覆盖到扫描的这张纸牌的位置
        if (!_touchingCard->getBoundingBox().intersectsRect(trump->getBoundingBox())) {
            continue;
        }
        // 扫描到的这张牌，是不是牌列中最后一张牌
        if (!_viewModel->isLast(trump->getData()->getFieldX(), trump->getData()->getFieldY())) {
            continue;
        }
        // 判断扫描的这张牌，能否和拖动的牌对接上
        if (!_viewModel->isKlondikeSequence(trump, dragTrumps->at(0))) {
            continue;
        }

        // 移动
        if (dragTrumps->size() > 0) {
            // 记录拖动前的x位置
            int fromX = dragTrumps->at(0)->getData()->getFieldX();
            int toX = trump->getData()->getFieldX();
            float dtY = getDeltaY(toX, dragTrumps->size());
            for (int i = 0;i < dragTrumps->size(); i++) {
                auto dragging = dragTrumps->at(i);
                moveCard(dragging, toX, dtY, i + 1);
            }
            // 加入到步骤记录容器中
            pushTrumpRoutes(dragTrumps->at(0), fromX, toX);
            
            // 移牌音效
            Audio::getInstance()->playEffect(WordFactor::CLICK_EFFECT_MOVE_OGG);
            
            return;
        }
    }

    // 拖动到牌堆空白牌上
    for (BlankCard* blank: _viewModel->getFieldBlanks()) {

        // 拖动到的位置是否符合
        if (!_touchingCard->getBoundingBox().intersectsRect(blank->getBoundingBox())) {
            continue;
        }
        // 这列是不是空白
        if (_viewModel->getFieldRow(blank->getX()).size() != 0) {
            continue;
        }
        // 拖动的牌，是不是13
        if (dragTrumps->at(0)->getData()->getValue() != 13) {
            continue;
        }

        // 移動
        if (dragTrumps->size() > 0) {
            // 记录拖动前的x位置
            int fromX = dragTrumps->at(0)->getData()->getFieldX();
            int toX = blank->getX();
            float dtY = getDeltaY(toX, dragTrumps->size());
            for (int i = 0;i < dragTrumps->size(); i++) {
                auto dragging = dragTrumps->at(i);
                moveCard(dragging, toX, dtY, i+1);
            }
            // 加入到步骤记录容器中
            pushTrumpRoutes(dragTrumps->at(0), fromX, toX);
            
            // 移牌音效
            Audio::getInstance()->playEffect(WordFactor::CLICK_EFFECT_MOVE_OGG);
            return;
        }
    }

    // 拖动到基础堆得空白牌上
    for (BlankCard* blank: _viewModel->getDropBlanks()) {

        // 是否在这个位置上
        if (!_touchingCard->getBoundingBox().intersectsRect(blank->getBoundingBox())) {
            continue;
        }
        // 是否空白
        if (_viewModel->getDropRow(blank->getX()).size() != 0) {
            continue;
        }
        // 2张牌及以上，无法拖动到基础堆
        if (dragTrumps->size() > 1) {
            continue;
        }
        // 拖动的纸牌大小必须是1
        if (dragTrumps->at(0)->getData()->getValue() != 1) {
            continue;
        }

        // 移動
        if (dragTrumps->size() > 0) {
            // 记录拖动前的x位置
            int fromX = dragTrumps->at(0)->getData()->getFieldX();
            for (Trump* dragging : *dragTrumps) {
                moveCard(dragging //
                        , dragging->getData()->getSuit() + SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS);
            }
            // 加入到步骤记录容器中
            pushTrumpRoutes(dragTrumps->at(0), fromX, dragTrumps->at(0)->getData()->getSuit() + SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS);
            
            playSuitSfx();
            
            return;
        }
    }

    // 拖动到基础堆上
    for (Trump* trump: _viewModel->getDropCards()) {
        // 拖动的牌中是否包含扫描的基础堆纸牌
        if (dragTrumps->contains(trump)) {
            continue;
        }
        
        // 是否移动到这个位置
        if (!_touchingCard->getBoundingBox().intersectsRect(trump->getBoundingBox())) {
            continue;
        }
        // 2张牌及以上，无法拖动到基础堆
        if (dragTrumps->size() > 1) {
            continue;
        }
        // 扫描到的这张牌，是不是牌列中最后一张牌
        if (!_viewModel->isLast(trump->getData()->getFieldX(), trump->getData()->getFieldY())) {
            continue;
        }
        // 不符合收牌规则，不能拖动
        if (!_viewModel->isKlondikeDrop(trump, dragTrumps->at(0))) {
            continue;
        }

        // 移動
        if (dragTrumps->size() > 0) {
            // 记录拖动前的x位置
            int fromX = dragTrumps->at(0)->getData()->getFieldX();
            
            for (Trump* dragging : *dragTrumps) {
                moveCard(dragging //
                        , dragging->getData()->getSuit() + SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS);
            }
            // 加入到步骤记录容器中
            pushTrumpRoutes(dragTrumps->at(0), fromX, dragTrumps->at(0)->getData()->getSuit() + SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS);
            // 移牌音效
            playSuitSfx();
            
            return;
        }
    }

    // 没有移动，只有点击
    if (!_isMovingCard) {
        for (Trump* dragged : *dragTrumps) {
            //恢复z值
            dragged->setLocalZOrder(dragged->getData()->getFieldY());
        }

        // 点击的列索引
        int x = _touchingCard->getData()->getFieldX();

        if ((x <= SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X)) {

            // 点击基础堆 牌堆 或者 open库的牌
            // 点击的是不是最后一张牌
            bool canCanMove = onSingleTapOpenCard(_viewModel->isLast(x, _touchingCard->getData()->getFieldY()));
            if(canCanMove == true){
                return;
            }
            else{
                int index = 0;
//                _clickSfxID = Audio::getInstance()->playEffect(WordFactor::CLICK_EFFECT_MOVE_OGG);
                float dtY = getDeltaY(x, 0);
                for (Trump* dragging : *dragTrumps) {
                    
                    Point posOffset = Point(0, index * dtY * ScaleFactor::scale_x);
                    // 返回
                    MoveTo* move = MoveTo::create(m_tweem_card_move_speed, _touchStartPos + posOffset);
                    EaseSineOut* ease = EaseSineOut::create(move);
                    Sequence* seq = Sequence::create(ease, CallFuncN::create(CC_CALLBACK_1(SolitaireKlondikeScene::onCompleteDragTween, this)), nullptr);
                    dragging->runAction(seq);
                    
                    _draggedTrumps.pushBack(dragging);
                    
                    index++;
                }
                return;
            }

        } else if (x == SolitaireKlondikeViewModel::INDEX_STOCK_CLOSE_X) {

            // 点击的close牌库
            onSingleTapClosedStockCard();
            
            return;
        }
    }

    // 没有拖动的牌
    if (dragTrumps->size() == 0) {
        _isTouchCard = false;
        _touchingCard->setLocalZOrder(_touchingCard->getData()->getFieldY());
        return;
    }
    // 操作了半天，废了半天劲，返回原来的位置
    int index = 0;
    Audio::getInstance()->playEffect(WordFactor::CLICK_EFFECT_MOVE_OGG);
    float dtY = getDeltaY(_touchingCard->getData()->getFieldX(), 0);
    for (Trump* dragging : *dragTrumps) {

        dragging->stopAllActions();
        
        Point posOffset = Point(0, index * dtY * ScaleFactor::scale_x);
        // 返回
        MoveTo* move = MoveTo::create(m_tweem_card_move_speed, _touchStartPos + posOffset);
        EaseSineOut* ease = EaseSineOut::create(move);
        CallFunc* call1 = CallFuncN::create(CC_CALLBACK_1(SolitaireKlondikeScene::onCompleteDragTween, this));
        CallFunc* call2 = CallFunc::create(CC_CALLBACK_0(SolitaireKlondikeScene::endTween, this));
        Sequence* seq = Sequence::create(ease, call1, call2, nullptr);
        dragging->runAction(seq);

        _draggedTrumps.pushBack(dragging);

        index++;
    }
}

void SolitaireKlondikeScene::pushTrumpRoutes(Trump* dragtrump, int fromX,int toX){
    bool isCauseFlip = false;
    // 如果拖动前的位置在牌库中，且这列牌库下面还有牌,并且这个牌是翻转的
    if(fromX < SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS &&
       _viewModel->getFieldRow(fromX).size()>0 &&
       _viewModel->getFieldRow(fromX).back()->getData()->isReverse())
        isCauseFlip =true;
    // 记录拖动的纸牌
    //score
    int add = 0;
    
    if (toX < SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X && toX >= SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS) {
        //每向收牌区添加一张牌可以得15分
        if (fromX == SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X) {
            //从手牌区
            add = _isVegasOn ? 5 : 15;
        } else if (fromX < SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS){
            //玩牌区
            add = _isVegasOn? 5 : 10;
        }
        
    } else if (toX < SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS && fromX == SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X) {
        
        //将纸牌从手牌区移到玩牌区的某列可得5分
        add = _isVegasOn? 0 : 5;
    } else if (toX < SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS && fromX < SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X && fromX >= SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS) {
        //每从收牌区移动一张牌到玩牌区减15分
        add = _isVegasOn? -5 : -15;
    }
    auto e = TrumpRoute::create(dragtrump, fromX, isCauseFlip, add);
    _viewModel->addTrumRoute(e);
    
    // 是否移动的open库中的牌
    auto back = e;
    if(back->fromX == SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X){
        // 调整open库牌的位置
        ajustopenStockCard(false);
    }
}

void SolitaireKlondikeScene::playSuitSfx(){

    if (_isAutoStarted || _isWin) {
        return;
    }
    
    _suitSfxTimer = 0;
    char str[64];
    sprintf(str, WordFactor::CLICK_SUIT_FORMATTER, _suitSfxIdx + 1);
    // 移牌音效
    Audio::getInstance()->playEffect(str);
    
    if (_suitSfxIdx < 9) {
        _suitSfxIdx++;
    }
}

//获取某行高度
float SolitaireKlondikeScene::getRowHeight(int x, int y, float dtY){
    auto cards = _viewModel->getFieldRow(x);
    float offsetY = 0;
    float heightMax = winSize.height > winSize.width ? 2000 : 300;
    int idx = 0;
    
    for(auto trump : cards){
        if (idx == y) {
            break;
        }
        if (idx < (cards.size()-1)) {
            
            if(!trump->getData()->isReverse()){
                break;
            }
            offsetY += POS_CLOSE_FIELD_OFFSET_Y ;
        }
        idx++;
    }
    if (dtY == -1){
    dtY = -MIN((heightMax + offsetY)/(cards.size() - (idx+1)), abs(POS_FIELD_OFFSET_Y));
    }
    CCLOG("dtY = %0.2f",dtY);
    for (int i = idx; i < cards.size()-1; i++) {
        if (i == y) {
            break;
        }
        
        offsetY += dtY ;
    }
    return offsetY;
}

float SolitaireKlondikeScene::getTipMoveTime(Trump* card, int toX){
    int toYSize = (int)_viewModel->getDropRow(toX).size();
    float snapPosX;
    float snapPosY;
    if (toX < SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS) {
        // 移动到牌库中
        snapPosX = (POS_FIELD_OFFSET_X_NOW() * ScaleFactor::scale_x * toX) + POS_FIELD_START_X_NOW() * ScaleFactor::scale_x ;
        float ht = getRowHeight(toX);
        snapPosY = (POS_FIELD_OFFSET_Y * ((toYSize==0?0:1))) + ht + POS_FIELD_START_Y;
    } else if (toX < SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X) {
        // 移动到基础堆中
        snapPosX = (POS_DROP_OFFSET_X_NOW() * ScaleFactor::scale_x //
                    * (toX - SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS)) //
        + POS_DROP_START_X_NOW() * ScaleFactor::scale_x ;
        snapPosY = POS_DROP_START_Y * ScaleFactor::scale_y;
    } else if (toX < SolitaireKlondikeViewModel::INDEX_STOCK_CLOSE_X) {
        // 牌库中翻开牌的移动
        
        // 调整open库牌的位置
        //        ajustopenStockCard(false);
        
        // 当前打开牌的移动位置
        snapPosX = card->getPosition().x + (m_l_r_mode ? (-POS_FIELD_OFFSET_X_NOW()) : (POS_FIELD_OFFSET_X_NOW() + 2 * abs(POS_STOCK_OFFSET_X)));
        //POS_FIELD_OFFSET_X_NOW() * (m_l_r_mode ? -1 : 1) ;
        snapPosY = POS_STOCK_START_Y * ScaleFactor::scale_y ;
    } else if (toX == SolitaireKlondikeViewModel::INDEX_STOCK_CLOSE_X) {
        
        snapPosX = POS_STOCK_START_X_NOW() * ScaleFactor::scale_x;
        snapPosY = POS_STOCK_START_Y * ScaleFactor::scale_y ;
    }
    
    auto tarPos = Point(snapPosX, snapPosY);
//    auto t = (tarPos - card->getPosition()).getLength() / 500.f;
    return 1.2f;
}

void SolitaireKlondikeScene::adjustRowHeight(int x, int ignoreLastNum, float dtY){

    auto cards = _viewModel->getFieldRow(x);
    float offsetY = 0;
    float heightMax = winSize.height > winSize.width ? 2000 : 300;
    int idx = 0;
    
    for(auto trump : cards){
        
        if (idx < (cards.size()-1)) {
            
            if(!trump->getData()->isReverse()){
                break;
            }
            trump->setPositionY(POS_FIELD_START_Y + offsetY);
            offsetY += POS_CLOSE_FIELD_OFFSET_Y ;
            
            CCLOG("trump : %s y = %f", trump->getData()->getString().c_str(), trump->getPositionY());
        }
        idx++;
    }
    if (dtY == -1){
        dtY = -MIN((heightMax + offsetY)/(cards.size() - (idx+1)), abs(POS_FIELD_OFFSET_Y));
    }
//    CCLOG("adjustRowHeight dtY = %0.2f",dtY);
    for (int i = idx; i < cards.size() - ignoreLastNum; i++) {
        auto trump = _viewModel->getCard(x, i);
        trump->setPositionY(POS_FIELD_START_Y + offsetY);
        offsetY += dtY ;
        CCLOG("trump : %s y = %f", trump->getData()->getString().c_str(), trump->getPositionY());
    }
//    if(ignoreLast){
//        
//    } else
//    {
//        for (int i = idx; i < cards.size(); i++) {
//            _viewModel->getCard(x, i)->setPositionY(POS_FIELD_START_Y + offsetY);
//            offsetY += dtY ;
//        }
//    }
}

void SolitaireKlondikeScene::tipMoveCard(Trump* card, int toX, int groupIdx){

    
    
    Trump *moveCard = card->clone();
    moveCard->setLocalZOrder(1024);
    this->addChild(moveCard);
    _tipCards.pushBack(moveCard);
    // 先逻辑移动牌
//    Trump* moveCard = _viewModel->moveCard(card, toX);
    int toYSize = (int)_viewModel->getDropRow(toX).size();
    
    // 移动目标位置的坐标
    float snapPosX;
    float snapPosY;
    
    if (toX < SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS) {
        // 移动到牌库中
        snapPosX = (POS_FIELD_OFFSET_X_NOW() * ScaleFactor::scale_x * toX) + POS_FIELD_START_X_NOW() * ScaleFactor::scale_x ;
        float ht = getRowHeight(toX);
        snapPosY = (POS_FIELD_OFFSET_Y * (groupIdx + (toYSize==0?0:1))) + ht + POS_FIELD_START_Y;
        
    } else if (toX < SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X) {
        // 移动到基础堆中
        snapPosX = (POS_DROP_OFFSET_X_NOW() * ScaleFactor::scale_x //
                    * (toX - SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS)) //
        + POS_DROP_START_X_NOW() * ScaleFactor::scale_x ;
        snapPosY = POS_DROP_START_Y * ScaleFactor::scale_y;
    } else if (toX < SolitaireKlondikeViewModel::INDEX_STOCK_CLOSE_X) {
        // 牌库中翻开牌的移动
        
        // 调整open库牌的位置
//        ajustopenStockCard(false);
        
        // 当前打开牌的移动位置
        snapPosX = card->getPosition().x + (m_l_r_mode ? (-POS_FIELD_OFFSET_X_NOW()) : (POS_FIELD_OFFSET_X_NOW() + 2 * abs(POS_STOCK_OFFSET_X)));
        //POS_FIELD_OFFSET_X_NOW() * (m_l_r_mode ? -1 : 1) ;
        snapPosY = POS_STOCK_START_Y * ScaleFactor::scale_y ;
    } else if (toX == SolitaireKlondikeViewModel::INDEX_STOCK_CLOSE_X) {
        
        snapPosX = POS_STOCK_START_X_NOW() * ScaleFactor::scale_x;
        snapPosY = POS_STOCK_START_Y * ScaleFactor::scale_y ;
    }
    
    // tween
    auto tarPos = Point(snapPosX, snapPosY);
    //auto t = 0.3f;//(tarPos - moveCard->getPosition()).getLength() / 500.f;
    MoveTo* move = MoveTo::create(0.45f, tarPos);
    EaseSineOut* ease = EaseSineOut::create(move);
//    time = &t;
    
//    CallFunc* call1 = CallFunc::create(CC_CALLBACK_0(SolitaireKlondikeScene::onCompleteDragTween, this));
//    CallFunc* call2 = CallFunc::create(CC_CALLBACK_0(SolitaireKlondikeScene::flipFieldCard, this));
//    CallFunc* call3 = CallFunc::create(CC_CALLBACK_0(SolitaireKlondikeScene::endTween, this));
    
    //tipIdx  4 2第一次 1最后一次
//    bool flag1 =  ((tipIdx&2)>>1) == 1;
//    bool flag2 = (tipIdx&1) == 1;
    
//    if (flag1) {
//        if (!_tipMask->isVisible()) {
//            this->_tipMask->setVisible(true);
//        }
//    }
//    
//    
    auto callback = CallFuncN::create([=](Ref *sender){
//        if (flag2) {
//            if (_tipMask->isVisible()) {
//                this->_tipMask->setVisible(false);
//            }
//        }
//        this->_isTipMove = false;
        _tipCards.eraseObject((Sprite *)sender);
    });
    Sequence* seq = Sequence::create(ease, EaseExponentialIn::create(FadeTo::create(0.45, 0)), RemoveSelf::create(),callback, nullptr);
    moveCard->runAction(seq);
}

// 移动单个纸牌
void SolitaireKlondikeScene::moveCard(Trump* card, int toX, float dtY, int ignoreLastNum, function<void()> cb) {

    stopNextMove();
    
    // 先逻辑移动牌
    Trump* moveCard = _viewModel->moveCard(card, toX);
    _isAutoCollect = true;

    // 移动目标位置的坐标
    float snapPosX;
    float snapPosY;
    bool showStar = false;
    if (toX < SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS) {
        // 移动到牌库中
        snapPosX = (POS_FIELD_OFFSET_X_NOW() * ScaleFactor::scale_x * toX) + POS_FIELD_START_X_NOW() * ScaleFactor::scale_x ;
        snapPosY = getRowHeight(toX, -1, dtY) + POS_FIELD_START_Y;
        
        adjustRowHeight(toX, ignoreLastNum, dtY);
    } else if (toX < SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X) {
        // 移动到基础堆中
        snapPosX = (POS_DROP_OFFSET_X_NOW() * (moveCard->getData()->getFieldX() - SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS)) + POS_DROP_START_X_NOW();
        snapPosY = POS_DROP_START_Y ;
        showStar = true;
    } else if (toX < SolitaireKlondikeViewModel::INDEX_STOCK_CLOSE_X) {
        // 牌库中翻开牌的移动
        
        // 调整open库牌的位置
        ajustopenStockCard(true);
        
        // 当前打开牌的移动位置
        snapPosX = card->getPosition().x + (m_l_r_mode ? (-POS_FIELD_OFFSET_X_NOW()) : (POS_FIELD_OFFSET_X_NOW() + 2 * abs(POS_STOCK_OFFSET_X)));
        //POS_FIELD_OFFSET_X_NOW() * (m_l_r_mode ? -1 : 1);
        
        snapPosY = POS_STOCK_START_Y * ScaleFactor::scale_y ;
    } else if (toX == SolitaireKlondikeViewModel::INDEX_STOCK_CLOSE_X) {
        
        snapPosX = POS_STOCK_START_X_NOW() * ScaleFactor::scale_x;
        snapPosY = POS_STOCK_START_Y * ScaleFactor::scale_y ;
    }

    Vec2 moveTo = Point(snapPosX, snapPosY);
    float t = _isAutoStarted ? ((moveTo-card->getPosition()).length() / 3000.f): m_tweem_card_move_speed;
    
    // tween
    MoveTo* move = MoveTo::create(t, moveTo);
    EaseSineOut* ease = EaseSineOut::create(move);
    CallFunc* call1 = CallFuncN::create(CC_CALLBACK_1(SolitaireKlondikeScene::onCompleteDragTween, this));
    CallFunc* call2 = CallFunc::create(CC_CALLBACK_0(SolitaireKlondikeScene::flipFieldCard, this));
    CallFunc* call3 = CallFunc::create(CC_CALLBACK_0(SolitaireKlondikeScene::endTween, this));
    auto call4 = CallFuncN::create([=](Node *sender){
        auto card = static_cast<Trump *>(sender);
        if (!_gameStart) {
            //移动第一张牌开始
            _gameStart = true;
            onGameStart();
        }
        if (showStar) {
            this->showStarFx(card);
        }
        if (cb != nullptr) {
            cb();
        }
    });
    Sequence* seq = Sequence::create(call2, ease, call1, call3, call4, nullptr);
    moveCard->runAction(seq);
    
    _draggedTrumps.pushBack(moveCard);
}

void SolitaireKlondikeScene::undoMove(Trump* card, int toX, bool isDelay, float dtY, int ignoreLastNum){
    
    _isUndoingCard = true;
    
    //
    card->stopAllActions();

    int fromX = card->getData()->getFieldX();
    
    // 逻辑回退牌
    Trump* moveCard = _viewModel->undoMove(card, toX);
    
    int zOrder = Z_DRAGGING;
    
    moveCard->setLocalZOrder(zOrder);
    
    // 撤销到的目标位置
    float snapPosX;
    float snapPosY;
    
//    if (fromX < (SolitaireKlondikeViewModel::INDEX_DROP_X + SolitaireKlondikeViewModel::COUNT_DROP_MAX_ROWS) && fromX >= SolitaireKlondikeViewModel::INDEX_DROP_X) {
////        _suitSfxIdx--;
////        if (_suitSfxIdx < 0) {
////            _suitSfxIdx = 0;
////        }
//    }
    if(fromX < SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS){
        adjustRowHeight(fromX);
    }
    if (toX < SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS) {
        // 回退到牌堆
        snapPosX = (POS_FIELD_OFFSET_X_NOW() * ScaleFactor::scale_x * moveCard->getData()->getFieldX()) + POS_FIELD_START_X_NOW() * ScaleFactor::scale_x ;
        snapPosY = getRowHeight(toX, -1, dtY) + POS_FIELD_START_Y;
        
        // 如果退回操作时，之前翻过去的牌要翻回来
        adjustRowHeight(toX, ignoreLastNum, dtY);
    } else if (toX < SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X) {
        // 回退到基础堆
        snapPosX = (POS_DROP_OFFSET_X_NOW() * ScaleFactor::scale_x //
                    * (moveCard->getData()->getFieldX() - SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS)) //
        + POS_DROP_START_X_NOW() * ScaleFactor::scale_x ;
        snapPosY = POS_DROP_START_Y * ScaleFactor::scale_y;
        
    } else if (toX < SolitaireKlondikeViewModel::INDEX_STOCK_CLOSE_X)  {
 
        // 调整open库牌的位置
        ajustopenStockCard(true);
        
        // 需要回退的位置
        
        snapPosX = POS_STOCK_START_X_NOW() + (m_l_r_mode ? (-POS_FIELD_OFFSET_X_NOW()) : (POS_FIELD_OFFSET_X_NOW() + 2 * abs(POS_STOCK_OFFSET_X)));
        //POS_DROP_OFFSET_X * (m_l_r_mode ? -1 : 1);
        snapPosY = POS_STOCK_START_Y * ScaleFactor::scale_y ;
    }
    else if (toX == SolitaireKlondikeViewModel::INDEX_STOCK_CLOSE_X)
    {

        // 回退到close库
        snapPosX = POS_STOCK_START_X_NOW() * ScaleFactor::scale_x ;
        snapPosY = POS_STOCK_START_Y * ScaleFactor::scale_y ;
        
        // 回退open库中的牌到close库，调整open库牌的位置
        ajustopenStockCard(false);
        
    }
    
    // tween
    // 3张牌模式的时候，撤销操作，open库中的牌，每回退一张，延迟一下
    DelayTime *delaytime = DelayTime::create(isDelay ? m_tweem_card_move_speed : 0);
    MoveTo* move = MoveTo::create(isDelay ? 0 : m_tweem_card_move_speed, Point(snapPosX, snapPosY));
    EaseSineOut* ease = EaseSineOut::create(move);
    CallFunc* call1 = CallFuncN::create(CC_CALLBACK_1(SolitaireKlondikeScene::onCompleteDragTween, this));
    CallFunc* call2 = CallFunc::create(CC_CALLBACK_0(SolitaireKlondikeScene::flipOpenStock, this));
    CallFunc* call3 = CallFunc::create(CC_CALLBACK_0(SolitaireKlondikeScene::endTween, this));
    Sequence* seq = Sequence::create(delaytime, ease, call1, call2, call3, nullptr);
    moveCard->runAction(seq);
    
    _draggedTrumps.pushBack(moveCard);
}

void SolitaireKlondikeScene::ajustopenStockCard(bool isAdd, bool animate){
    // 翻开牌的个数
    int num_open = _viewModel->getOpenStockCards().size();
    
    if(num_open >= 2 && isAdd){
        
        for(int i=3; i>1; i--){
            //  open库打开两张,只需要移动第一张
            if(num_open == 2)
                i--;
            
            // open库打开>=3张，只需要移动后两张，之前的覆盖
            Trump*  Card = _viewModel->getOpenStockCards().at(num_open - i);
            if (animate) {
                MoveBy* move = MoveBy::create(m_tweem_card_move_speed, Point(POS_STOCK_OFFSET_X_NOW() * ScaleFactor::scale_x, 0));
                EaseSineOut* ease = EaseSineOut::create(move);
                Sequence* seq = Sequence::create(ease, nullptr);
                Card->runAction(seq);
            } else {
                Card->setPosition(Point(POS_STOCK_OFFSET_X_NOW() * ScaleFactor::scale_x, 0) + Card->getPosition()) ;
            }
            
        }
    }
    
    if(num_open >= 1 && isAdd == false){
        for(int i=1; i<=2; i++){
            //  剩下一张，只需要移动这一张
            if(num_open-i < 0)
                return;
            // 剩下>=2张,只需要移动最上面两张牌，即可
            Trump*  Card = _viewModel->getOpenStockCards().at(num_open - i);
            MoveBy* move = MoveBy::create(m_tweem_card_move_speed, Point(-POS_STOCK_OFFSET_X_NOW() * ScaleFactor::scale_x, 0));
            EaseSineOut* ease = EaseSineOut::create(move);
            Sequence* seq = Sequence::create(ease, nullptr);
            Card->runAction(seq);
        }
    }
}

int SolitaireKlondikeScene::canTouchCardMove(bool isLast, bool isTip){

    // 是否找到移动的目标位置
    bool ableMove = false;
    int targetX = -1;
    // 先看看能否移动到 基础堆
    for (int i = 0; i < SolitaireKlondikeViewModel::COUNT_DROP_MAX_ROWS && isLast; i++) {
        
        // 寻找点击的牌，对应的基础堆
        if (_touchingCard->getData()->getSuit() != i) {
            continue;
        }
        targetX = i + SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS;
        
        Vector<Trump*> rows = _viewModel->getFieldRow(targetX);
        if (rows.size() == 0) {
            
            if (_touchingCard->getData()->getValue() != 1) {
                // 基础堆没有牌，点击的纸牌不是1的话，不用操作
                continue;
            }
        } else {
            if (!_viewModel->isKlondikeDrop(rows.back(), _touchingCard)) {
                // 基础堆上的纸牌跟点击的纸牌是否衔接
                continue;
            }
        }
        
        ableMove = true;
    }
    
    // 不能移动到基础堆的话，看看能不能移动到牌堆中
    for (int i = 0; i < SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS && !ableMove; i++) {
        targetX = i;
        Vector<Trump*> rows = _viewModel->getFieldRow(i);
        if(i == _touchingCard->getData()->getFieldX())
            continue;
        if (rows.size() == 0) {
            if (isTip) {
                //Tip下13不需要移动
                if (!(_touchingCard->getData()->getValue() == 13 && _touchingCard->getData()->getFieldY() != 0)) {
                    // 牌堆没有牌，点击的纸牌不是13的话，不用操作
                    continue;
                }
            } else {
                if (_touchingCard->getData()->getValue() != 13) {
                    // 牌堆没有牌，点击的纸牌不是13的话，不用操作
                    continue;
                }
            }
            
            
        } else {
            
            if (isTip) {
                
                bool srcSeq = false;
                if (_touchingCard->getData()->getFieldY() != 0) {
                    auto back = _viewModel->getFieldRow(_touchingCard->getData()->getFieldX()).at(_touchingCard->getData()->getFieldY()-1);
                    if (!back->getData()->isReverse()) {
                        srcSeq = _viewModel->isKlondikeSequence(back, _touchingCard);
                    }
                }
                if ((!_viewModel->isKlondikeSequence(rows.back(), _touchingCard)) || srcSeq) {
                    continue;
                }
            } else {
                if (!_viewModel->isKlondikeSequence(rows.back(), _touchingCard)) {
                    // 牌堆上的纸牌跟点击的纸牌是否衔接
                    continue;
                }
            }
        }
        ableMove = true;
    }
    if (ableMove) {
        return targetX;
    } else {
        return -1;
    }
}

float SolitaireKlondikeScene::getDeltaY(int x, int addNum){

    auto cards = _viewModel->getFieldRow(x);
    float offsetY = 0;
    float heightMax = winSize.height > winSize.width ? 2000 : 300;
    int idx = 0;
    
    for(auto trump : cards){
        if (idx < (cards.size()-1)) {
            
            if(!trump->getData()->isReverse()){
                break;
            }
            offsetY += POS_CLOSE_FIELD_OFFSET_Y ;
        }
        idx++;
    }
    float dtY = -MIN((heightMax + offsetY)/(cards.size() - (idx+1) + addNum), abs(POS_FIELD_OFFSET_Y));
    return dtY;
}

bool SolitaireKlondikeScene::onSingleTapOpenCard(bool isLast) {
    int targetX = canTouchCardMove(isLast);
    if(targetX != -1){
        
        Vector<Trump*>* dragTrumps = this->getDraggingGroup();
        
        // 记录拖动前的x位置
        int fromX = _touchingCard->getData()->getFieldX();
        float dtY = getDeltaY(targetX, (int)dragTrumps->size());
        CCLOG("onSingleTapOpenCard dtY = %0.2f", dtY);
        for (int i = 0; i < dragTrumps->size(); i++) {
            auto dragging = dragTrumps->at(i);
            // Z値先放到最大
            // 移动完了之后，再恢复Z值
            dragging->setLocalZOrder(Z_DRAGGING);
            //移动
            moveCard(dragging, targetX, dtY, i+1);
        }
        
        // 加入到步骤记录容器中
        pushTrumpRoutes(_touchingCard, fromX, targetX);
        
        if (targetX >= SolitaireKlondikeViewModel::INDEX_DROP_X && targetX < (SolitaireKlondikeViewModel::COUNT_DROP_MAX_ROWS + SolitaireKlondikeViewModel::INDEX_DROP_X)) {
            playSuitSfx();
        } else{
        // 移牌音效
            Audio::getInstance()->playEffect(WordFactor::CLICK_EFFECT_MOVE_OGG);
        }
        
        return true;
    }
    else {
        //shake
//        Audio::getInstance()->playEffect(WordFactor::CLICK_EFFECT_MOVE_OGG);
//        SimpleAudioEngine::getInstance()->stopEffect(_clickSfxID);
        Audio::getInstance()->playEffect(WordFactor::CLICK_EFFECT_CANT_MOVE);
        Vector<Trump*>* dragTrumps = this->getDraggingGroup();
        auto ac1 = MoveBy::create(0.03, Vec2(-20, 0));
        auto ac2 = MoveBy::create(0.06, Vec2(40, 0));
        auto ac3 = MoveBy::create(0.03, Vec2(-20, 0));
        auto seq = Sequence::create(ac1, ac2, ac3, NULL);
        
        int x = _touchingCard->getData()->getFieldX();
        int toX = x;
        auto dtY = getDeltaY(x, 0);
        auto cards = _viewModel->getFieldRow(x);
        if (x < (SolitaireKlondikeViewModel::COUNT_DROP_MAX_ROWS + SolitaireKlondikeViewModel::INDEX_DROP_X) && (x >= SolitaireKlondikeViewModel::INDEX_DROP_X)) {
            
            int idx = 0;
            for (Trump* dragging : cards) {
                auto card = dragging;
                dragging->stopAllActions();
                float snapPosX,snapPosY;
                if (toX < SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS) {
                    // 移动到牌库中
                    snapPosX = (POS_FIELD_OFFSET_X_NOW() * ScaleFactor::scale_x * toX) + POS_FIELD_START_X_NOW() * ScaleFactor::scale_x ;
                    adjustRowHeight(toX, 0, dtY);
                    snapPosY = getRowHeight(toX, -1, dtY) + POS_FIELD_START_Y;
                   
                } else if (toX < SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X) {
                    // 移动到基础堆中
                    snapPosX = (POS_DROP_OFFSET_X_NOW() * ScaleFactor::scale_x //
                                * (toX - SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS)) //
                    + POS_DROP_START_X_NOW() * ScaleFactor::scale_x ;
                    snapPosY = POS_DROP_START_Y * ScaleFactor::scale_y;
                }  else if (toX == SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X) {
                    
                    auto row = cards;
                    Point pos3 = Point(POS_STOCK_START_X_NOW() + (m_l_r_mode ? (-POS_FIELD_OFFSET_X_NOW()) : (POS_FIELD_OFFSET_X_NOW() + 2 * abs(POS_STOCK_OFFSET_X))) , POS_STOCK_START_Y);
                    Point pos2 = pos3 + Point(POS_STOCK_OFFSET_X_NOW() * ScaleFactor::scale_x, 0);
                    Point pos1 = pos2 + Point(POS_STOCK_OFFSET_X_NOW() * ScaleFactor::scale_x, 0);
                    if(row.size() == 1)
                    {
                        snapPosX = pos3.x;
                        snapPosY = pos3.y;
                    }
                    else if(row.size() == 2){
                        if (idx == 0){
                            snapPosX = pos2.x;
                            snapPosY = pos2.y;
                        } else if (idx == 1){
                            snapPosX = pos3.x;
                            snapPosY = pos3.y;
                        }
                        
                    }
                    else if(row.size() >= 3){
                        if(idx<row.size()-2){
                            snapPosX = pos1.x;
                            snapPosY = pos1.y;
                        }
                        else if (row.size()-2){
                            snapPosX = pos2.x;
                            snapPosY = pos2.y;
                        } else if (row.size()-1){
                            snapPosX = pos3.x;
                            snapPosY = pos3.y;
                        }
                    }
                } else if (toX < SolitaireKlondikeViewModel::INDEX_STOCK_CLOSE_X) {
                    // 牌库中翻开牌的移动
                    
                    // 调整open库牌的位置
                    //        ajustopenStockCard(false);
                    
                    // 当前打开牌的移动位置
                    snapPosX = card->getPosition().x + (m_l_r_mode ? (-POS_FIELD_OFFSET_X_NOW()) : (POS_FIELD_OFFSET_X_NOW() + 2 * abs(POS_STOCK_OFFSET_X)));
                    //POS_FIELD_OFFSET_X_NOW() * (m_l_r_mode ? -1 : 1) ;
                    snapPosY = POS_STOCK_START_Y * ScaleFactor::scale_y ;
                }
                dragging->setPosition(Vec2(snapPosX, snapPosY));
                dragging->runAction(seq->clone());
                idx++;
            }
        } else {
            int idx = 0;
            for (Trump* dragging : *dragTrumps) {
                auto card = dragging;
                
                dragging->stopAllActions();
                float snapPosX,snapPosY;
                if (toX < SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS) {
                    // 移动到牌库中
                    snapPosX = (POS_FIELD_OFFSET_X_NOW() * ScaleFactor::scale_x * toX) + POS_FIELD_START_X_NOW() * ScaleFactor::scale_x ;
                    adjustRowHeight(toX, 0, dtY);
                    snapPosY = getRowHeight(toX, -1, dtY) + POS_FIELD_START_Y;
                    
                } else if (toX < SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X) {
                    // 移动到基础堆中
                    snapPosX = (POS_DROP_OFFSET_X_NOW() * ScaleFactor::scale_x //
                                * (toX - SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS)) //
                    + POS_DROP_START_X_NOW() * ScaleFactor::scale_x ;
                    snapPosY = POS_DROP_START_Y * ScaleFactor::scale_y;
                } else if (toX == SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X) {
                    
                    auto row = cards;
                    Point pos3 = Point(POS_STOCK_START_X_NOW() + (m_l_r_mode ? (-POS_FIELD_OFFSET_X_NOW()) : (POS_FIELD_OFFSET_X_NOW() + 2 * abs(POS_STOCK_OFFSET_X))) , POS_STOCK_START_Y);
                    Point pos2 = pos3 + Point(POS_STOCK_OFFSET_X_NOW() * ScaleFactor::scale_x, 0);
                    Point pos1 = pos2 + Point(POS_STOCK_OFFSET_X_NOW() * ScaleFactor::scale_x, 0);
                    snapPosX = pos3.x;
                    snapPosY = pos3.y;
                }
                else if (toX < SolitaireKlondikeViewModel::INDEX_STOCK_CLOSE_X) {
                    // 牌库中翻开牌的移动
                    
                    // 调整open库牌的位置
                    //        ajustopenStockCard(false);
                    
                    // 当前打开牌的移动位置
                    snapPosX = card->getPosition().x + (m_l_r_mode ? (-POS_FIELD_OFFSET_X_NOW()) : (POS_FIELD_OFFSET_X_NOW() + 2 * abs(POS_STOCK_OFFSET_X)));
                    //POS_FIELD_OFFSET_X_NOW() * (m_l_r_mode ? -1 : 1) ;
                    snapPosY = POS_STOCK_START_Y * ScaleFactor::scale_y ;
                }
                CCLOG("x = %f, y= %f", snapPosX, snapPosY);
                dragging->setPosition(Vec2(snapPosX, snapPosY));
                dragging->runAction(seq->clone());
                idx++;
            }
        }
    }
    _isTouchCard = false;
    return false;
}

void SolitaireKlondikeScene::onSingleTapClosedStockCard() {

    // 暂时不用
    // hideOpenStockCard();
    
//    for(int i = MAX(0, _viewModel->getFieldRowSize(SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X) - 5); i < _viewModel->getFieldRowSize(SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X); i++) {
//        auto card = _viewModel->getCard(SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X, i);
//        card->setLocalZOrder(card->getData()->getFieldY());
//    }
    
    Trump* dragedTrump = _viewModel->getCloseStockCards().at(0);

    // Z值调整
    int zOrder = Z_DRAGGING;
    for (int i = 0; i < _viewModel->getDropCount(); i++) {

        // close牌库没有牌
        if (_viewModel->getCloseStockCards().size() == 0) {
            break;
        }

        Trump* card = _viewModel->getCloseStockCards().at(0);

//        if (card->getNumberOfRunningActions() > 0) {
////            card->stopAllActions();
//            
//            CCLOG("remove actions card value = %d", card->getData()->getValue());
//        }
        
        
        // 翻转，刷新
        card->getData()->setReverse(false);
//        card->refresh();
        card->setLocalZOrder(zOrder);
        card->flip(nullptr, [=](){
        });
        this->moveCard(card, SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X);
        zOrder++;
    }
    
    // 翻牌音效
    Audio::getInstance()->playEffect(WordFactor::CLICK_EFFECT_FANPAI_OGG);
    
    // 记录拖动的纸牌
    _viewModel->addTrumRoute(TrumpRoute::create(dragedTrump, SolitaireKlondikeViewModel::INDEX_STOCK_CLOSE_X, false));
    
    _isTouchCard = false;
//    runAction(Sequence::create(DelayTime::create(0.01), CallFunc::create([=](){
//        //翻盘效果体验
//        this->_isTouchCard = false;
//    }), NULL));
    
    if (_isVegasOn) {
        //vegas out off
        if(_viewModel->getCloseStockCards().size() == 0) {
            if (_gameStartChou3) {
                _viewModel->getStockBlank()->setType(4);
                int num = _viewModel->getStockBlank()->getFlipchance()-1;
                if (num == 0) {
                    _viewModel->getStockBlank()->setType(3);
                } else {
                    _viewModel->getStockBlank()->setFlipChance(num);
                }
            } else {
                _viewModel->getStockBlank()->setType(3);
            }
        }
    }
}

void SolitaireKlondikeScene::onSingleTapBlankStockCard() {

    // 暂时不做隐藏
    // hideOpenStockCard();
    
    // 洗牌库音效
    

    Vector<Trump*> row = _viewModel->getFieldRow(SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X);
    
    Trump* dragedTrump = row.size() == 0 ? nullptr : row.at(0);

    int num = 0;
    while (_viewModel->getFieldRowSize(SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X) > 0) {

        Trump* moveCard = _viewModel->moveCard(_viewModel->getCard(SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X, 0), SolitaireKlondikeViewModel::INDEX_STOCK_CLOSE_X);

        moveCard->stopAllActions();

        // 设置翻转
        moveCard->getData()->setReverse(true);
        //moveCard->refresh();

        // Z order
        moveCard->setLocalZOrder(moveCard->getData()->getFieldY());
        moveCard->flip(nullptr, [=](){
        });

        //moveCard->setPosition(Point(POS_STOCK_START_X_NOW() * ScaleFactor::scale_x , POS_STOCK_START_Y * ScaleFactor::scale_y));
        bool last = _viewModel->getFieldRowSize(SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X) == 0;
        _isRefreshCard = true;
        auto seq = Sequence::create(DelayTime::create(1/90.f * num), CallFunc::create([=](){
            if(num < 10){
                Audio::getInstance()->playEffect(WordFactor::CLICK_EFFECT_FANPAI_OGG);
            }
            
            auto cb = [=](){
                if (last) {
                    _isRefreshCard = false;
                }
            };
            
            this->moveCard(moveCard, SolitaireKlondikeViewModel::INDEX_STOCK_CLOSE_X, -1, 0, cb);
        }), NULL);
        moveCard->runAction(seq);
        
        num++;
    }
    // 记录拖动的纸牌
    //在翻三张牌的模式下，每翻完一遍手牌区所有的牌减20分。
    //在翻一张牌的模式下，每翻完一遍手牌区所有的牌减100分。
    // 洗open库的牌，到close库中
    
    
    if (dragedTrump) {
//        int add = DataManager::getInstance()->getSettings()->m_chou3card ? -20 : -100;
        _viewModel->addTrumRoute(TrumpRoute::create(dragedTrump, 11, false));
    }
    
    
    if (_showNoMoveStart) {
        _showNoMoveOpenNum++;
    }
}

void SolitaireKlondikeScene::hideOpenStockCard() {

    //暂时不做纸牌隐藏
    Vector<Trump*> row = _viewModel->getFieldRow(SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X);

    while (_viewModel->getFieldRowSize(SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X) > 0) {

        Trump* moveCard = _viewModel->moveCard(_viewModel->getCard(SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X, 0), SolitaireKlondikeViewModel::INDEX_STOCK_HIDDEN_X);

        // 隐藏
        DelayTime* delay = DelayTime::create(m_tweem_card_move_speed);
        CallFunc* hideFunc = CallFunc::create(CC_CALLBACK_0(SolitaireKlondikeScene::hideCard, this, moveCard));
        Sequence* seq = Sequence::create(delay, hideFunc, nullptr);

        moveCard->runAction(seq);
    }
}

void SolitaireKlondikeScene::hideCard(Trump* card) {

    card->setPosition(Point(-200, -200));
}

void SolitaireKlondikeScene::onCompleteDragTween(Ref *sender) {

    auto dragged = (Trump *)sender;
    // 恢复z值
    dragged->setLocalZOrder(dragged->getData()->getFieldY());
    _draggedTrumps.eraseObject(dragged);
    
    _needReorderZ = true;
}

void SolitaireKlondikeScene::flipFieldCard() {

    for (int x = 0; x < SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS; x++) {
        
//        if (x == 2) {
//            break;
//        }
        
        // 牌堆
        Vector<Trump*> rows = _viewModel->getFieldRow(x);

        if (rows.size() == 0) {
            continue;
        }
        // 判断最上面一张牌是否翻转
        if (!rows.back()->getData()->isReverse()) {
            continue;
        }

        // 设置非翻转
        rows.back()->getData()->setReverse(!rows.back()->getData()->isReverse());

        // 翻转动作
        rows.back()->flip(CC_CALLBACK_0(SolitaireKlondikeScene::endTween, this));
        
    }
}

void SolitaireKlondikeScene::flipOpenStock(){

        Vector<Trump*> rows = _viewModel->getFieldRow(SolitaireKlondikeViewModel::INDEX_STOCK_CLOSE_X);
        if (rows.size() == 0) {
          return;
        }
    
        for(int i=0; i<_viewModel->getDropCount() && i<=rows.size()-1;i++){
          // 翻转了，刷新一下
          rows.at(i)->refresh();
        }
    
}

void SolitaireKlondikeScene::endTween() {
    
    _isTouchCard = false;
    _isUndoingCard =false;
    _isAutoCollect = false;
    
    
    
    //检查是否可以直接胜利
    if ((!_isWin) && (!_isAutoStarted) ) {
        bool flag = true;
        for (int i = 0; i < SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS; i++) {
            for(Trump *trump : _viewModel->getFieldRow(i)){
                if (trump->getData()->isReverse()) {
                    flag = false;
                    break;
                }
            }
        }
        
        if (DataManager::getInstance()->getSettings()->m_chou3card || _isVegasOn) {
            int stock = 0;
            stock += _viewModel->getFieldRowSize(SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X);
            stock += _viewModel->getFieldRowSize(SolitaireKlondikeViewModel::INDEX_STOCK_CLOSE_X);
            if (stock >1) {
                flag = false;
            }
        }
        
        if (flag == true) {
            CCLOG("游戏可以直接胜利");
            _autoButton->setVisible(true);
            
        } else {
            _autoButton->setVisible(false);
        }
    }
    if (!_isWin) {
        //检查游戏是否已经胜利
        bool flag2 = true;
        for (int i = 0; i < 4; i++) {
            auto row = _viewModel->getSuitRow(i);
            if (row.size() != 13) {
                flag2 = false;
                break;
            }
        }
        if (flag2) {
            CCLOG("游戏胜利");
//            TrumpModel* model = new TrumpModel();
//            model->saveWinData();
//            delete model;
            
            SimpleAudioEngine::getInstance()->stopEffect(_sfx_score11_ID);
//            SimpleAudioEngine::getInstance()->stopAllEffects();
            SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
            
//            _winLabel->setVisible(true);
            
            
            
            _isWin = true;
            _isAutoStarted = false;
            onGameEnd();
            
            auto cb = [=](){
                this->showWinnerActionCallback();
            };
            
            if (random(0, 1) == 1)
            {
                showWinnerActionNewRecord(cb);
            }
            else
            {
                showWinnerActionNormal(cb);
            }
            
            
//            runAction(Sequence::create(DelayTime::create(6), cb, NULL));
        }
    }
}

void SolitaireKlondikeScene::showWinnerActionCallback(){
    auto la = KLAllClearLayer::create();
    Audio::getInstance()->playEffect(WordFactor::CLICK_EFFECT_WIN_OGG);
    la->onRemove = [=](){
        if (!_isDailyChanllenge) {
            auto la = KLWinLayer::create(getCurrentScore());
            la->onTap = [=](KLWinLayer*, int buttonIdx){
                if (buttonIdx == 0) {
                    
                    auto cb2 = [=](){
                        this->startGame(this->getViewModel()->gameType);
                    };
                    
                    if( !KLRateLayer::show(this, [=](){
                        this->runAction(Sequence::create(DelayTime::create(2.f), CallFunc::create(cb2), NULL));
                    })){
                        cb2();
                    }
                } else {
                    
                    auto hsLayer = Layer_HighScore::create();
                    auto sc = Scene::create();
                    sc->addChild(hsLayer);
                    Director::getInstance()->pushScene(sc);
                }
            };
            this->addChild(la, 2048);
        } else {
            this->quitDailyChallenge();
        }
    };
    this->addChild(la, 2048);
}

void SolitaireKlondikeScene::onTouchCancelled(Touch* touch, Event* event) {
    //Point location = touch->getLocation();
    //CCLOG("onTouchCancelled x:%f, y:%f", location.x, location.y);
    onTouchEnded(touch, event);
}

Vector<Trump*>* SolitaireKlondikeScene::getDraggingGroup() {

    Vector<Trump*>* trumps = new Vector<Trump*>();

    if (!_isTouchCard || _touchingCard == nullptr) {
        return trumps;
    }
    // 目标牌的x索引值
    int x = _touchingCard->getData()->getFieldX();

    if (x < SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS) {

        Vector<Trump*> rows = _viewModel->getFieldRow(x);

        // 牌堆位置上的情况
        for (int y = 0; y < rows.size(); y++) {
            if (rows.at(y)->getData()->getFieldY() >= _touchingCard->getData()->getFieldY()) {
                trumps->pushBack(rows.at(y));
            }
        }

    } else if ((x >= SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X)
            && (x < SolitaireKlondikeViewModel::INDEX_STOCK_CLOSE_X)) {

        // open库位置上的情况
        trumps->pushBack(_touchingCard);
    }else if ((x >= SolitaireKlondikeViewModel::COUNT_FIELD_MAX_ROWS)
              && (x < SolitaireKlondikeViewModel::INDEX_STOCK_OPEN_X)) {
        
        // 基础堆位置上的情况
        trumps->pushBack(_touchingCard);
    }
    return trumps;
}

Vector<Trump*>* SolitaireKlondikeScene::getUndoGroup(Trump* movedTrump){
    if(!movedTrump) return NULL;
    
    Vector<Trump*>* trumps = new Vector<Trump*>();
    
    int x = movedTrump->getData()->getFieldX();
        
        Vector<Trump*> rows = _viewModel->getFieldRow(x);
        
        // 获取纸牌以及它上面的牌
        for (int y = 0; y < rows.size(); y++) {
            if (rows.at(y)->getData()->getFieldY() >= movedTrump->getData()->getFieldY()) {
                trumps->pushBack(rows.at(y));
            }
        }
    
    return trumps;
}


extern "C"{
    
    SolitaireKlondikeScene* getGameScene (){
        return _instance;
    }
    
}
