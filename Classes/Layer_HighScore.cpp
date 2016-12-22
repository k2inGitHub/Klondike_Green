//
//  Layer_HighScore.cpp
//  Klondike
//
//  Created by 宋扬 on 16/7/21.
//
//

#include "Layer_HighScore.hpp"
#include "Cell_HighScore.hpp"
#include "HighScoreManager.hpp"

void Layer_HighScore::updateLayout(){

    _bg->setPosition(VisibleRect::center());
    _bg->setScale(winSize.width/640, winSize.height/1136);
    
    auto navBar = getNavigationBar();
    _listView->setContentSize(Size(winSize.width, winSize.height - navBar->getContentSize().height));

}

bool Layer_HighScore::init(){
    if (!KTLayer::init()) {
        return false;
    }
    auto size = VisibleRect::getVisibleRect().size;
    auto sprite = Sprite::create(BG_FILENAME);
    sprite->setPosition(VisibleRect::center());
    this->addChild(sprite, -100);
    _bg = sprite;
    
    setNavigationBarEnable(true);
    auto navBar = getNavigationBar();
    navBar->setTitle(LocalizedString("TID_RULES_CLASSIC_8"));
    
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
    space->setContentSize(itemQuit->getContentSize());
    leftItems.pushBack(space);
    navBar->setLeftBarItems(leftItems);
    
    
    ListView* listView = ListView::create();
    // set list view ex direction
    listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(false);
    listView->setScrollBarEnabled(false);
//    listView->setBackGroundImage("cocosui/green_edit.png");
//    listView->setBackGroundImageScale9Enabled(true);
    listView->setContentSize(Size(size.width, size.height - navBar->getContentSize().height));
    listView->setPosition(Vec2::ZERO);
    listView->addEventListener((ui::ListView::ccListViewCallback)CC_CALLBACK_2(Layer_HighScore::selectedItemEvent, this));
    listView->addEventListener((ui::ListView::ccScrollViewCallback)CC_CALLBACK_2(Layer_HighScore::selectedItemEventScrollView,this));
//    listView->setScrollBarPositionFromCorner(Vec2(7, 7));
    this->addChild(listView);
    _listView = listView;

    listView->setItemModel(Cell_HighScore::create());
    
    char str[64] = {0};
    
    auto dataArray = HighScoreManager::getInstance()->getDataArray();
    
    for (int i = 0; i < 7; i++) {
        listView->pushBackDefaultItem();
    }
    
    std::string normals[] = {"daily_orange.png", "daily_blue.png", "daily_green.png", "daily_white.png"};
    Color4B titleColors[] = {KL_RED_FONT, KL_BLUE_FONT, KL_GREEN_FONT, Color4B::WHITE};
    for (int i = 0; i < 7; ++i)
    {
        auto data = i >= dataArray.size() ? nullptr : dataArray.at(i);
        sprintf(str, "%d", i+1);
        Cell_HighScore* item = (Cell_HighScore *)listView->getItem(i);
        auto btn = item->getRankButton();
        btn->setTitleText(str);
        btn->loadTextures(normals[MIN(3, i)], "");
        btn->getTitleRenderer()->setPosition(Vec2(btn->getContentSize()/2) + Vec2(0, 4));
        if (data) {
            sprintf(str, "%d", data->getScore());
            item->getTitleLabel()->setString(str);
            item->getTimeLabel()->setString(data->getTimeString());
        } else {
            item->getTitleLabel()->setString("---");
            item->getTimeLabel()->setString("----");
        }
        
        item->getTitleLabel()->setTextColor(titleColors[MIN(3, i)]);
    }
    
    
    return true;
}

void Layer_HighScore::selectedItemEvent(Ref *pSender, ListView::EventType type)
{
    switch (type)
    {
        case cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_START:
        {
            ListView* listView = static_cast<ListView*>(pSender);
            CC_UNUSED_PARAM(listView);
            CCLOG("select child start index = %ld", listView->getCurSelectedIndex());
            break;
        }
        case cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_END:
        {
            ListView* listView = static_cast<ListView*>(pSender);
            CC_UNUSED_PARAM(listView);
            CCLOG("select child end index = %ld", listView->getCurSelectedIndex());
            break;
        }
        default:
            break;
    }
}

void Layer_HighScore::selectedItemEventScrollView(Ref* pSender, ui::ScrollView::EventType type)
{
    switch (type) {
        case ui::ScrollView::EventType::SCROLL_TO_BOTTOM:
            CCLOG("SCROLL_TO_BOTTOM");
            break;
        case ui::ScrollView::EventType::SCROLL_TO_TOP:
            CCLOG("SCROLL_TO_TOP");
            break;
        default:
            break;
    }
}
