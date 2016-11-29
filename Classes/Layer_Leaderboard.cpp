//
//  Layer_Leaderboard.cpp
//  Klondike
//
//  Created by 宋扬 on 16/8/9.
//
//

#include "Layer_Leaderboard.hpp"
#include "HttpManager.hpp"
#include "KLPopupLayer.hpp"
#include "DataManager.h"
using namespace NSDataManage;

const int kCellMaxNum = 20;

Widget* Cell_Leaderboard::createCloneInstance()
{
    return Cell_Leaderboard::create();
}

void Cell_Leaderboard::updateLayout(){
    
    
}

bool Cell_Leaderboard::init(){
    
    if (!Cell_Base::init()) {
        return false;
    }
    
    auto size = VisibleRect::getVisibleRect().size;
    
    auto cell = this;
    {
        cell->setTouchEnabled(true);
        cell->setContentSize(Size(size.width, 66));
        auto btn = KTFactory::createButton("", "", "", "1", 40, "Arial-BoldMT");
        btn->setContentSize(Size(50, 50));
        btn->setPosition(Vec2(74, 30));
        cell->addChild(btn);
        _rankButton = btn;
        
        auto titleLabel = Label::createWithSystemFont("---", "Arial-BoldMT", 30);
        titleLabel->setContentSize(Size(320, 50));
        titleLabel->setDimensions(320, 50);
        titleLabel->setOverflow(Label::Overflow::SHRINK);
        titleLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        titleLabel->setPosition(Vec2(180, 24));
        cell->addChild(titleLabel);
        _titleLabel = titleLabel;
        
        
        auto timeLabel = Label::createWithSystemFont("----", "Arial", 24);
        timeLabel->setContentSize(Size(70, 50));
        timeLabel->setDimensions(70, 50);
        timeLabel->setOverflow(Label::Overflow::SHRINK);
        timeLabel->setHorizontalAlignment(TextHAlignment::RIGHT);
        timeLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        timeLabel->setPosition(Vec2(580, 24));
        _timeLabel = timeLabel;
        addChild(timeLabel);
    }
    
    
    auto line = Scale9Sprite::create("9_line_white.png");
    line->setScale9Enabled(true);
    line->setContentSize(Size(size.width, line->getContentSize().height));
    line->setOpacity(100);
    line->setPosition(line->getContentSize()/2);
    
    addChild(line);
    
    return true;
}

void Layer_Leaderboard::loadDetailData(){

    std::unordered_map<std::string, std::string> content;
    
    char str[64];
    sprintf(str, "%ld", _stamp);
    content["time"] = str;
    content["uid"] = iOSWrapper::getUserID();
    std::unordered_map<std::string, std::string> param;
    param["content"] = KTUtils::jsonString(content);
    
    HttpManager::create()->sendGET("http://45.113.69.21/ao2/play_cards_user.php", param, [=](const rapidjson::Document &json){
        
        if (json["status"].GetInt() == 1) {
            
            const rapidjson::Value& data = json["data"];
            _score = atoi(data["score"].GetString());
            _useTime = atoi(data["use_time"].GetString());
            _minUseTime = atoi(data["min_use_time"].GetString());
            
            _firstScore = atoi(data["first_score"].GetString());
            _firstUseTime = atoi(data["first_use_time"].GetString());
            _firstMinUseTime = atoi(data["min_use_time"].GetString());
            
            _moves = atoi(data["moves"].GetString());
            
            this->updateDetailUI();
            if (_firstScore == -1) {
                _startButton->setTitleText(LocalizedString("TID_UI_PLAY"));
            } else {
                //TID_UI_PLAY AGAIN
                _startButton->setTitleText(LocalizedString("TID_UI_PLAY AGAIN"));
            }
        }
        
    },[=](string error){
    
    });
}

void Layer_Leaderboard::updateDetailUI(){

    char str[64];
    if (_tabIdx == 2) {
        
        _detail2->setVisible(false);
        _d1_l1->setString(LocalizedString("TID_UI_MY MOVES"));
        if (_moves == -1) {
            sprintf(str, "---");
        } else {
            sprintf(str, "%d", _moves);
        }
        _d1_l2->setString(str);
    } else if (_tabIdx == 1){
        _detail2->setVisible(true);
        _d1_l1->setString(LocalizedString("TID_UI_MY SCORE"));
        if (_firstScore == -1) {
            sprintf(str, "---");
        } else {
            sprintf(str, "%d", _firstScore);
        }
        _d1_l2->setString(str);
        
        _d2_l1->setString(LocalizedString("TID_UI_MY TIME"));
        if (_firstUseTime == -1) {
            sprintf(str, "---");
        } else {
            sprintf(str, "%0.2d:%0.2d", _firstUseTime/60, _firstUseTime%60);
        }
        _d2_l2->setString(str);
        
        _d2_l3->setString(LocalizedString("TID_UI_FASTEST TIME"));
        if (_firstMinUseTime == -1) {
            sprintf(str, "---");
        } else {
            sprintf(str, "%0.2d:%0.2d", _firstMinUseTime/60, _firstMinUseTime%60);
        }
        _d2_l4->setString(str);
        
    } else if (_tabIdx == 0){
        _detail2->setVisible(true);
        
        _d1_l1->setString(LocalizedString("TID_UI_MY SCORE"));
        if (_score == -1) {
            sprintf(str, "---");
        } else {
            sprintf(str, "%d", _score);
        }
        _d1_l2->setString(str);
        
        _d2_l1->setString(LocalizedString("TID_UI_MY TIME"));
        if (_useTime == -1) {
            sprintf(str, "---");
        } else {
            sprintf(str, "%0.2d:%0.2d", _useTime/60, _useTime%60);
        }
        _d2_l2->setString(str);
        
        _d2_l3->setString(LocalizedString("TID_UI_FASTEST TIME"));
        if (_minUseTime == -1) {
            sprintf(str, "---");
        } else {
            sprintf(str, "%0.2d:%0.2d", _minUseTime/60, _minUseTime%60);
        }
        _d2_l4->setString(str);
    }
}

void Layer_Leaderboard::loadData(int idx){

    if (idx == 2) {
        _headLabels.at(2)->setString(LocalizedString("TID_UI_MOVES"));
    } else {
        _headLabels.at(2)->setString(LocalizedString("TID_UI_SCORE"));
    }
    
    
    KLProgressLayer::show(this);
    
    auto failure = [=](){
        
        KLProgressLayer::hide(this);
        vector<string> v;
        v.push_back(LocalizedString("TID_UI_CONTINUE"));
        KLAlertLayer::show("", LocalizedString("TID_UI_LOG OUT ERROR"), "", v, [=](KLAlertLayer *layer, int buttonIdx){
            Director::getInstance()->popScene();
            
        });
    };
    
    
    static string urls[] = {"http://45.113.69.21/ao2/play_cards_high_score.php","http://45.113.69.21/ao2/play_cards_first_score.php", "http://45.113.69.21/ao2/play_cards_least_move.php"};
    
    
    std::unordered_map<std::string, std::string> content;
    
    char str[64];
    sprintf(str, "%ld", _stamp);
    content["time"] = str;
    std::unordered_map<std::string, std::string> param;
    param["content"] = KTUtils::jsonString(content);
    
    HttpManager::create()->sendGET(urls[idx], param, [=](const rapidjson::Document &json){
        
        if (json["status"].GetInt() == 1) {
        
            KLProgressLayer::hide(this);
            
            _dataArray.clear();
            
            const rapidjson::Value& data = json["data"];
            
            for (rapidjson::Value::ConstValueIterator itr = data.Begin(); itr != data.End(); ++itr) {
                LeaderboardModel *m = LeaderboardModel::create();
                
                m->setUserID(itr->FindMember("uid")->value.GetString());
                m->setName(itr->FindMember("username")->value.GetString());
                auto it = itr->FindMember("score");
                if (it == itr->MemberEnd()) {
                    it = itr->FindMember("moves");
                }
                m->setScore(atoi(it->value.GetString()));
                _dataArray.pushBack(m);
            }
            
            char str[64] = {0};
            
            std::string normals2[] = {"daily_orange.png", "daily_blue.png", "daily_green.png", "daily_white.png"};
            Color4B titleColors[] = {Color4B::WHITE, KL_RED_FONT};
            auto myID = iOSWrapper::getUserID();
            
            for (int i = 0; i < kCellMaxNum; ++i)
            {
                auto data = i >= _dataArray.size() ? nullptr : _dataArray.at(i);
                sprintf(str, "%d", i+1);
                auto item = (Cell_Leaderboard *)_listView->getItem(i);
                auto btn = item->getRankButton();
                btn->setTitleText(str);
                btn->loadTextures(normals2[MIN(3, i)], "");
                btn->getTitleRenderer()->setPosition(Vec2(btn->getContentSize()/2) + Vec2(0, 4));
                int idx = 0;
                if (data) {
                    if (myID == data->getUserID()) {
                        idx = 1;
                    }
                }
                
                item->getTitleLabel()->setTextColor(titleColors[idx]);
                
                if (data) {
                    item->getTitleLabel()->setString(data->getName());
                    sprintf(str, "%d", data->getScore());
                    item->getTimeLabel()->setString(str);
                } else {
                    item->getTitleLabel()->setString("---");
                    item->getTimeLabel()->setString("----");
                }
                
                
            }
            this->updateDetailUI();
            
        } else {
            failure();
        }
        
    }, [=](std::string errror){
        failure();
    });
    
    
}


void Layer_Leaderboard::updateLayout(){

    _bg->setPosition(VisibleRect::center());
    _bg->setScale(winSize.width/640, winSize.height/1136);
    
    auto navBar = getNavigationBar();
    float ht = navBar->getContentSize().height + 35;
    Vector<Node *>nodes;
    for(auto n : _tabButtons){
        nodes.pushBack(n);
    }
    horizonAliament(nodes, 0, VisibleRect::top().y - ht );
    
    ht += 75;
    
    auto defaultCell = Cell_Leaderboard::create();
    Node* layouts[] = {defaultCell->getRankButton(), defaultCell->getTitleLabel(), defaultCell->getTimeLabel()};
    for (int i = 0; i < 3; i++) {
        
        auto label = _headLabels.at(i);
        label->setPosition(layouts[i]->getPositionX()- 320 + winSize.width/2, VisibleRect::top().y - ht);
    }
    
    float bottomHt = winSize.height > winSize.width ? 300 : 190;
    
    float scHt = winSize.height - (ht + 10 + bottomHt);
    _listView->setContentSize(Size(winSize.width, scHt));
    _listView->setPosition(Vec2::ZERO + Vec2(- 320 + winSize.width/2, bottomHt));
    
    bottomHt -= 20;
    
    _d2_bg->setPositionX(320);
    _d1_bg->setPositionX(320);
    _detail1->setPosition(Vec2(- 320 + winSize.width/2, winSize.height > winSize.width ? 0 : -110));
    
    bottomHt -= 45;
    
    _detail2->setPosition(Vec2(- 320 + winSize.width/2, winSize.height > winSize.width ? 0 : -110));
    
    bottomHt -= 80;
    
    _startButton->setPosition(Vec2(winSize.width/2 - 160, bottomHt));
    _submitButton->setPosition(Vec2(winSize.width/2 + 160, bottomHt));
}

bool Layer_Leaderboard::init(time_t stamp){
    if(!KTPauseLayer::init()){
        return false;
    }
    
    _stamp = stamp;
    
    auto size = VisibleRect::getVisibleRect().size;
    auto sprite = Sprite::create("shared/theme/1.jpg");
    sprite->setPosition(VisibleRect::center());
    this->addChild(sprite, -100);
    _bg = sprite;
    
    setNavigationBarEnable(true);
    auto navBar = getNavigationBar();
    navBar->setTitle(LocalizedString("TID_UI_LEADERBOARD"));
    
    
    
    Vector<MenuItem *>leftItems;
    
    char str[64];
    
    sprintf(str, "%s:%s", LocalizedString("TID_UI_DATE").c_str(), KTDate::date(_stamp)->localDate()->toString().substr(0,10).c_str());
    auto label = Label::createWithSystemFont(str, "Arial", 30);
    label->setContentSize(Size(160, 30));
    label->setDimensions(160, 30);
    label->setVerticalAlignment(TextVAlignment::CENTER);
    label->setTextColor(KL_YELLOW);
    label->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    label->setOverflow(Label::Overflow::SHRINK);
    auto date = MenuItemLabel::create(label);
//    space->setContentSize(itemQuit->getContentSize());
    leftItems.pushBack(date);
    navBar->setLeftBarItems(leftItems);
    
    Vector<MenuItem *>rightItems;
    auto itemQuit = MenuItemImage::create("close.png", "close1.png", [=](Ref *sender){
        Director::getInstance()->popScene();
    });
    rightItems.pushBack(itemQuit);
    
    auto space = MenuItem::create();
    space->setContentSize(Size(MAX(date->getContentSize().width - itemQuit->getContentSize().width-20, 0), itemQuit->getContentSize().height));
    rightItems.pushBack(space);
    navBar->setRightBarItems(rightItems);
    
    float ht = navBar->getContentSize().height + 35;
    Vector<Node *>nodes;
    string normals[] = {"tapback_1.png", "tapback_2.png", "tapback_1.png"};
    string sels[] = {"tapback_1.png","tapback_2.png", "tapback_1.png"};
    string titles[] = {LocalizedString("TID_UI_HIGH SCORE"),LocalizedString("TID_UI_FIRST GAME HIGH SCORES"),LocalizedString("TID_UI_FEWEST MOVES")};
    bool flipX[] = {false, false, true};
    
    auto callback = [=](Ref* sender,Widget::TouchEventType type){
        auto btnGet = static_cast<Button *>(sender);
        
        if (type == Widget::TouchEventType::ENDED) {
            
            for(auto btnLost : _tabButtons){
                if (btnLost == _tabButtons.front() || btnLost == _tabButtons.back()){
                    btnLost->loadTextures("tapback_1.png", "tapback_1.png");
                } else {
                    btnLost->loadTextures("tapback_2.png", "tapback_2.png");
                }
                btnLost->setScale9Enabled(true);
                btnLost->setContentSize(Size(170, 52.5));
                btnLost->setOpacity(255);
                btnLost->setColor(Color3B(KL_GREEN));
                btnLost->setTitleColor(Color3B::WHITE);
                btnLost->getTitleRenderer()->enableShadow(Color4B::GRAY);
                btnLost->getTitleRenderer()->setPosition(Vec2(Size(170, 52.5)/2));
            }
            if (btnGet == _tabButtons.front() || btnGet == _tabButtons.back())
            {
                btnGet->loadTextures("tap_1.png", "tap_2.png");
            } else {
                btnGet->loadTextures("tap1_1.png", "tap1_2.png");
            }
            btnGet->setScale9Enabled(true);
            btnGet->setContentSize(Size(170, 60));
            btnGet->setOpacity(255);
            btnGet->setColor(Color3B::WHITE);
            btnGet->setTitleColor(Color3B(KL_BROWN));
            btnGet->getTitleRenderer()->enableShadow(Color4B::WHITE);
            btnGet->getTitleRenderer()->setPosition(Vec2(Size(170, 60)/2));
            _tabIdx = (int)_tabButtons.getIndex(btnGet);
            this->loadData(_tabIdx);
            
        }
    };
    for (int i = 0; i < 3; i++) {
        auto btn = KTFactory::createButton(normals[i], sels[i], "", titles[i], 24, "Arial", Size(170, 60));
        btn->setFlippedX(flipX[i]);
        
        if (flipX[i]) {
            btn->getTitleRenderer()->setScaleX(-1);
        }
        
        btn->addTouchEventListener(callback);
        
        btn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        
        this->addChild(btn);
        nodes.pushBack(btn);
        _tabButtons.pushBack(btn);
    }
    horizonAliament(nodes, 0, VisibleRect::top().y - ht );
    
    ht += 75;
    
    auto defaultCell = Cell_Leaderboard::create();
    string names[] = {"TID_UI_RANK", "TID_UI_PLAYER", "TID_UI_SCORE"};
    Node* layouts[] = {defaultCell->getRankButton(), defaultCell->getTitleLabel(), defaultCell->getTimeLabel()};
    for (int i = 0; i < 3; i++) {
        
        auto label = Label::createWithSystemFont(LocalizedString(names[i]), "Arial", 22);
        auto laSize = layouts[i]->getContentSize();
        label->setTextColor(KL_GREEN);
        label->setContentSize(laSize);
        label->setAnchorPoint(layouts[i]->getAnchorPoint());
        label->setDimensions(laSize.width, laSize.height);
        label->setOverflow(Label::Overflow::SHRINK);
        label->setPosition(layouts[i]->getPositionX(), VisibleRect::top().y - ht);
        auto targetLabel = static_cast<Label *>(layouts[i]);
        if (targetLabel) {
            label->setHorizontalAlignment(targetLabel->getHorizontalAlignment());
        }
        addChild(label);
        _headLabels.pushBack(label);
    }
    
    float bottomHt = 300;
    float scHt = size.height - (ht +10 + bottomHt);
    
    ListView* listView = ListView::create();
    // set list view ex direction
    listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(true);
    listView->setScrollBarEnabled(false);
    //    listView->setBackGroundImage("cocosui/green_edit.png");
    //    listView->setBackGroundImageScale9Enabled(true);
    listView->setContentSize(Size(size.width, scHt));
    listView->setPosition(Vec2::ZERO + Vec2(0, bottomHt));
    _listView = listView;
//    listView->addEventListener((ui::ListView::ccListViewCallback)CC_CALLBACK_2(Layer_HighScore::selectedItemEvent, this));
//    listView->addEventListener((ui::ListView::ccScrollViewCallback)CC_CALLBACK_2(Layer_HighScore::selectedItemEventScrollView,this));
    //    listView->setScrollBarPositionFromCorner(Vec2(7, 7));
    this->addChild(listView);
    
    
    listView->setItemModel(defaultCell);
    
    for (int i = 0; i < kCellMaxNum; i++) {
        _listView->pushBackDefaultItem();
    }
    
    //HighScoreManager::getInstance()->getDataArray();
    
//    for(int i = 0; i < 10;i++){
//        auto m = LeaderboardModel::create();
//        m->setName("123");
//        m->setScore(1000);
//        dataArray.pushBack(m);
//    }
    
    
    
    callback(_tabButtons.at(0), Widget::TouchEventType::ENDED);
    
    bottomHt -= 20;
    {
        
        _detail1 = Node::create();
        addChild(_detail1);
        
        auto bg = ImageView::create("9_white_s.png");
        bg->setScale9Enabled(true);
        bg->setContentSize(Size(550, 40));
        bg->setColor(Color3B::BLACK);
        bg->setOpacity(100);
        bg->setPosition(Vec2(size.width/2, bottomHt));
        _detail1->addChild(bg);
        _d1_bg = bg;
        
        {
            auto label = Label::createWithSystemFont(LocalizedString("TID_UI_MY SCORE"), "Arial", 20);
            label->setTextColor(KL_YELLOW);
            label->setContentSize(Size(120, 25));
            label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            label->setHorizontalAlignment(TextHAlignment::LEFT);
            label->setDimensions(120, 25);
            label->setOverflow(Label::Overflow::SHRINK);
            label->setPosition(55, bottomHt);
            _detail1->addChild(label);
            _d1_l1 = label;
        }
        {
            auto label = Label::createWithSystemFont("9705", "Arial", 20);
            label->setTextColor(KL_YELLOW);
            label->setContentSize(Size(120, 25));
            label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            label->setHorizontalAlignment(TextHAlignment::RIGHT);
            label->setDimensions(120, 25);
            label->setOverflow(Label::Overflow::SHRINK);
            label->setPosition(640 - 55, bottomHt);
            _detail1->addChild(label);
            _d1_l2 = label;
        }
    }
    bottomHt -= 45;
    {
        _detail2 = Node::create();
        addChild(_detail2);
        auto bg = ImageView::create("9_white_s.png");
        bg->setScale9Enabled(true);
        bg->setContentSize(Size(550, 40));
        bg->setColor(Color3B::BLACK);
        bg->setOpacity(100);
        bg->setPosition(Vec2(size.width/2, bottomHt));
        _detail2->addChild(bg);
        _d2_bg = bg;
        {
            auto label = Label::createWithSystemFont("TID_UI_MY TIME", "Arial", 20);
            label->setTextColor(KL_YELLOW);
            label->setContentSize(Size(120, 25));
            label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            label->setHorizontalAlignment(TextHAlignment::LEFT);
            label->setDimensions(120, 25);
            label->setOverflow(Label::Overflow::SHRINK);
            label->setPosition(55, bottomHt);
            _detail2->addChild(label);
            _d2_l1 = label;
            
        }
        {
            auto label = Label::createWithSystemFont("01:18", "Arial", 20);
            label->setTextColor(KL_YELLOW);
            label->setContentSize(Size(120, 25));
            label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            label->setHorizontalAlignment(TextHAlignment::CENTER);
            label->setDimensions(120, 25);
            label->setOverflow(Label::Overflow::SHRINK);
            label->setPosition(320-64, bottomHt);
            _detail2->addChild(label);
            _d2_l2 = label;
        }
        {
            auto label = Label::createWithSystemFont("TID_UI_FASTEST TIME", "Arial", 20);
            label->setTextColor(KL_YELLOW);
            label->setContentSize(Size(120, 25));
            label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            label->setHorizontalAlignment(TextHAlignment::CENTER);
            label->setDimensions(120, 25);
            label->setOverflow(Label::Overflow::SHRINK);
            label->setPosition(320+64, bottomHt);
            _detail2->addChild(label);
            _d2_l3 = label;
        }
        {
            auto label = Label::createWithSystemFont("00:30", "Arial", 20);
            label->setTextColor(KL_YELLOW);
            label->setContentSize(Size(120, 25));
            label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            label->setHorizontalAlignment(TextHAlignment::RIGHT);
            label->setDimensions(120, 25);
            label->setOverflow(Label::Overflow::SHRINK);
            label->setPosition(640 - 55, bottomHt);
            _detail2->addChild(label);
            _d2_l4 = label;
        }
    }
    bottomHt -= 100;
    
    {
        auto btn = KTFactory::createButton("B_green1.png", "B_green2.png", "", LocalizedString("TID_UI_PLAY"), 30, "Arial", Size(272, 72));
        addChild(btn);
        btn->addClickEventListener([](Ref*){
            Director::getInstance()->popToRootScene();
            __NotificationCenter::getInstance()->postNotification(NTFFactor::NTF_DAILY_GAME);
        });
        btn->setPosition(Vec2(160, bottomHt));
        _startButton = btn;
    }
    
    {
        auto btn = KTFactory::createButton("B_green.png", "B_green0.png", "", LocalizedString("TID_UI_CONFIRM"), 30, "Arial", Size(272, 72));
        addChild(btn);
        btn->addClickEventListener([](Ref*){
            Director::getInstance()->popScene();
        });
        btn->setPosition(Vec2(480, bottomHt));
        _submitButton = btn;
    }
    
    loadDetailData();
    
    return true;
}
