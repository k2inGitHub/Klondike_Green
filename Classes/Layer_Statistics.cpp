//
//  Layer_Statistics.cpp
//  Klondike
//
//  Created by 宋扬 on 16/7/22.
//
//

#include "Layer_Statistics.hpp"
#include "Cell_Statistics.hpp"
#include "DataManager.h"
#include "KLPopupLayer.hpp"
#include "Localization.hpp"

using namespace NSDataManage;

bool Layer_Statistics::init(){

    if (!KTPauseLayer::init()) {
        return false;
    }

    auto size = VisibleRect::getVisibleRect().size;
    auto sprite = Sprite::create("shared/theme/1.jpg");
    
    sprite->setPosition(VisibleRect::center());
    this->addChild(sprite, -100);
    _bg = sprite;
    
    setNavigationBarEnable(true);
    auto navBar = getNavigationBar();
    navBar->setTitle(LocalizedString("TID_UI_STATISTICS"));
    
    Vector<MenuItem *>rightItems;
    auto itemQuit = MenuItemImage::create("close.png", "close1.png", [=](Ref *sender){
        Director::getInstance()->popScene();
    });
    rightItems.pushBack(itemQuit);
    navBar->setRightBarItems(rightItems);
    
    Vector<MenuItem *>leftItems;
    
    auto space = MenuItemImage::create("share.png", "share1.png", [=](Ref *sender){
        iOSWrapper::showShareUI();
    });
    //space->setContentSize(itemQuit->getContentSize());
    leftItems.pushBack(space);
    navBar->setLeftBarItems(leftItems);
    
    float ht = navBar->getContentSize().height + 35;
    Vector<Node *>nodes;
    string normals[] = {"tapback_1.png", "tapback_2.png", "tapback_1.png"};
    string sels[] = {"tapback_1.png","tapback_2.png","tapback_1.png"};
    string titles[] = {LocalizedString("TID_UI_TOTAL 2"),LocalizedString("TID_UI_1 CARD"),LocalizedString("TID_UI_3 CARDS")};
    bool flipX[] = {false, false, true};
    
    auto callback = [=](Ref* sender,Widget::TouchEventType type){
        auto btnGet = static_cast<Button *>(sender);
        
        if (type == Widget::TouchEventType::ENDED) {
            
            for(auto btnLost : _tabButtons){
                
                if (_tabButtons.at(1) == btnLost) {
                    btnLost->loadTextures("tapback_2.png", "tapback_2.png");
                } else {
                    btnLost->loadTextures("tapback_1.png", "tapback_1.png");
                }
                btnLost->setScale9Enabled(true);
                btnLost->setContentSize(Size(190, 63));
                btnLost->setOpacity(255);
                btnLost->setColor(Color3B(KL_GREEN));
                btnLost->setTitleColor(Color3B::WHITE);
                btnLost->getTitleRenderer()->enableShadow(Color4B::GRAY);
            }
            
            if (_tabButtons.at(1) == btnGet) {
                btnGet->loadTextures("tap1_1.png", "tap1_2.png");
            } else {
                btnGet->loadTextures("tap_1.png", "tap_2.png");
            }
            btnGet->setScale9Enabled(true);
            btnGet->setContentSize(Size(190, 72));
            btnGet->setOpacity(255);
            btnGet->setColor(Color3B::WHITE);
            btnGet->setTitleColor(Color3B(KL_BROWN));
            btnGet->getTitleRenderer()->enableShadow(Color4B::WHITE);
            _tabIdx = (int)_tabButtons.getIndex(btnGet);
            this->loadData(_tabIdx);
            
        }
    };
    for (int i = 0; i < 3; i++) {
        auto btn = KTFactory::createButton(normals[i], sels[i], "", titles[i], 32, "Arial", Size(190, 72));
        btn->getTitleRenderer()->setDimensions(190, 45);
        btn->getTitleRenderer()->setOverflow(Label::Overflow::SHRINK);
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
    horizonAliament(nodes, 0, VisibleRect::top().y - ht - 45);
    
//    Helper::doLayout(_horizontalLayout);
    
//    runAction(Sequence::create(DelayTime::create(0.000), CallFunc::create([=](){
//        
//    }), NULL));

    
    ListView* listView = ListView::create();
    // set list view ex direction
    listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(true);
    listView->setScrollBarEnabled(false);
    //    listView->setBackGroundImage("cocosui/green_edit.png");
    //    listView->setBackGroundImageScale9Enabled(true);
    listView->setContentSize(Size(size.width, size.height - ht - 80 - 170));
    listView->setPosition(Vec2(0, 170));
//    listView->addEventListener((ui::ListView::ccListViewCallback)CC_CALLBACK_2(Layer_HighScore::selectedItemEvent, this));
//    listView->addEventListener((ui::ListView::ccScrollViewCallback)CC_CALLBACK_2(Layer_HighScore::selectedItemEventScrollView,this));
    //    listView->setScrollBarPositionFromCorner(Vec2(7, 7));
    this->addChild(listView);
    
    
    listView->setItemModel(Cell_Statistics::create());
    
    
    _listView = listView;
    
    for (int i = 0; i < 6; i++) {
        listView->pushBackDefaultItem();
    }
    //loc
    _titles.push_back(LocalizedString("TID_UI_GAMES WON"));
    _titles.push_back(LocalizedString("TID_UI_WIN RATE"));
    _titles.push_back(LocalizedString("TID_UI_GAMES PLAYED"));
    _titles.push_back(LocalizedString("TID_UI_SHORTEST WINNING TIME"));
    _titles.push_back(LocalizedString("TID_UI_FEWEST WINNING MOVES"));
    _titles.push_back(LocalizedString("TID_UI_HIGH SCORE"));
    
    callback(_tabButtons.at(0), Widget::TouchEventType::ENDED);
    
//    _tabButtons.at(0)->setPositionY(_tabButtons.at(1)->getPositionY());
    //loc
    auto reset = KTFactory::createButton("B_orange.png", "B_orange1.png", "", LocalizedString("TID_UI_RESET"), 32, "Arial", Size(190, 72));
    reset->setPosition(Vec2(320, 130));//110
    addChild(reset);
    _reset = reset;
    reset->addClickEventListener([=](Ref*){
        vector<string> btns;
        btns.push_back(LocalizedString("TID_UI_CONFIRM"));
        
        auto content = LocalizedString("TID_UI_DO YOU REALLY WANT TO RESET YOUR STATISTICS?");
        auto cancel = LocalizedString("TID_UI_CANCEL");
        KLAlertLayer::show("", content, cancel, btns, [=](KLAlertLayer *layer, int buttonIdx){
            if (buttonIdx == 1) {
                DataManager::getInstance()->resetStatisticData();
                this->loadData(_tabIdx);
            }
        });
    });
    
    return true;
}

void Layer_Statistics::updateLayout(){

    auto navBar = getNavigationBar();
    float ht = navBar->getContentSize().height + 35;
    Vector<Node *>nodes;
    for(auto n : _tabButtons){
        nodes.pushBack(n);
    }
    horizonAliament(nodes, 0, VisibleRect::top().y - ht - 45);
    
    float bottom = winSize.height > winSize.width ? 170 : 90;
    _listView->setContentSize(Size(winSize.width, winSize.height - ht - 80 - bottom));
    _listView->setPosition(Vec2(0, bottom));
    
    
    _bg->setPosition(VisibleRect::center());
    _bg->setScale(winSize.width/640, winSize.height/1136);
    
    _reset->setPosition(Vec2(winSize.width/2, bottom-40));
}

void Layer_Statistics::loadData(int idx){
    
    _dataArray1.clear();
    
    auto st = *DataManager::getInstance()->getStatisticAll();
    auto sm1 = st.statistics_mode[0];
    auto sm2 = st.statistics_mode[1];
    
    int winned = idx == 0 ? (sm1.winned + sm2.winned) : (idx == 1 ? sm1.winned : sm2.winned);
    
    int played = idx == 0 ? (sm1.played + sm2.played) : (idx == 1 ? sm1.played : sm2.played);
    
    int best_time = idx == 0 ? int(sm1.best_time + sm2.best_time) : (idx == 1 ? (int)sm1.best_time : (int)sm2.best_time);
    
    int lease_move = idx == 0 ? (sm1.lease_move + sm2.lease_move) : (idx == 1 ? sm1.lease_move : sm2.lease_move);
    
    int best_score = idx == 0 ? (sm1.best_score + sm2.best_score) : (idx == 1 ? sm1.best_score : sm2.best_score);
    
    char str[64];
    sprintf(str, "%d", winned);
    _dataArray1.push_back(str);
    
    float p = played == 0 ? 0 : (float)winned / (float)played;
    sprintf(str, "%0.2f％", p*100);
    _dataArray1.push_back(str);
    
    sprintf(str, "%d", played);
    _dataArray1.push_back(str);
    
    sprintf(str, "%.2d:%.2d", best_time/60, best_time%60);
    _dataArray1.push_back(str);
    
    sprintf(str, "%d", lease_move);
    _dataArray1.push_back(str);
    
    sprintf(str, "%d", best_score);
    _dataArray1.push_back(str);
    
    for (int i = 0; i < _listView->getItems().size(); i++) {
        auto cell = static_cast<Cell_Statistics *>( _listView->getItem(i));
        cell->getTitleLabel()->setString(_titles[i]);
        cell->getTimeLabel()->setString(_dataArray1.at(i));
    }
    
}
