//
//  Layer_Setting.cpp
//  Klondike
//
//  Created by 宋扬 on 16/7/25.
//
//

#include "Layer_Setting.hpp"
#include "Cell_Setting.hpp"
#include "DataManager.h"
#include "Localization.hpp"
#include "SolitaireKlondikeScene.h"
#include "KLPopupLayer.hpp"
#include "Audio.h"
using namespace NSDataManage;


const static char* NAME_SOUND = "NAME_SOUND";
const static char* NAME_DRAW3 = "NAME_DRAW3";
const static char* NAME_VEGAS_MODE = "NAME_VEGAS_MODE";
const static char* NAME_VEGAS_ADD = "NAME_VEGAS_ADD";
const static char* NAME_TIMER = "NAME_TIMER";
const static char* NAME_LEFT_HAND = "NAME_LEFT_HAND";
const static char* NAME_LANGUAGE = "NAME_LANGUAGE";
const static char* NAME_AUTO_HINT = "NAME_AUTO_HINT";
const static char* NAME_HOW_TO_PLAY = "NAME_HOW_TO_PLAY";


#pragma mark - Layer_HowToPlay -

void Layer_HowToPlay::updateLayout(){
    
    _bg->setPosition(VisibleRect::center());
    _bg->setScale(winSize.width/640, winSize.height/1136);
    
    auto navBar = getNavigationBar();
    float ht = navBar->getContentSize().height + 35;
    Vector<Node *>nodes;
    for (auto n : _tabButtons) {
        nodes.pushBack(n);
    }
    horizonAliament(nodes, 0, VisibleRect::top().y - ht - 45);
    
    float bottom = winSize.height > winSize.width ? 150 : 0;
    float scHt = winSize.height - (ht + 90 + bottom);
    
    _scrollView->setPosition(Vec2((80)/2, bottom));
    _scrollView->setContentSize(Size(winSize.width - 80, scHt));
//    scrollView->setInnerContainerSize(scrollView->getContentSize());
    
    _label->setDimensions(winSize.width - 80, 0);
    _label->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _label->setPosition(Vec2(0, _scrollView->getContentSize().height));
    
    _scrollView->setInnerContainerSize(Size(_scrollView->getContentSize().width, _label->getContentSize().height));
    
    _label->setPosition(Vec2(0, _scrollView->getInnerContainerSize().height));
}

bool Layer_HowToPlay::init(){
    if (!KTPauseLayer::init()) {
        return false;
    }
    
    auto size = VisibleRect::getVisibleRect().size;
    auto sprite = Sprite::create(BG_FILENAME);
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
    
    auto space = MenuItem::create();
    space->setContentSize(itemQuit->getContentSize());
    leftItems.pushBack(space);
    navBar->setLeftBarItems(leftItems);
    
    float ht = navBar->getContentSize().height + 35;
    Vector<Node *>nodes;
    string normals[] = {"tapback_1.png", "tapback_2.png", "tapback_2.png", "tapback_1.png"};
    string sels[] = {"tapback_1.png","tapback_2.png", "tapback_2.png", "tapback_1.png"};
    string titles[] = {LocalizedString("TID_UI_RULESRULES"),LocalizedString("TID_UI_SCORING"),LocalizedString("TID_UI_GAME"),LocalizedString("TID_UI_QUEST")};
    bool flipX[] = {false, false,false, true};
    
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
                btnLost->setContentSize(Size(140, 52.5));
                btnLost->setOpacity(255);
                btnLost->setColor(Color3B(KL_GREEN));
                btnLost->setTitleColor(Color3B::WHITE);
//                btnLost->getTitleRenderer()->enableShadow(Color4B::GRAY);
            }
            if (btnGet == _tabButtons.front() || btnGet == _tabButtons.back())
            {
                btnGet->loadTextures("tap_1.png", "tap_2.png");
            } else {
                btnGet->loadTextures("tap1_1.png", "tap1_2.png");
            }
            btnGet->setScale9Enabled(true);
            btnGet->setContentSize(Size(140, 60));
            btnGet->setOpacity(255);
            btnGet->setColor(Color3B::WHITE);
            btnGet->setTitleColor(Color3B::WHITE);
//            btnGet->getTitleRenderer()->enableShadow(Color4B::WHITE);
            _tabIdx = (int)_tabButtons.getIndex(btnGet);
            this->loadData(_tabIdx);
            
        }
    };
    for (int i = 0; i < 4; i++) {
        auto btn = KTFactory::createButton(normals[i], sels[i], "", titles[i], 32, "Arial", Size(140, 60));
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

//    LabelTTF::create("", "Arial", 18);

    float scHt = size.height - (ht + 90 + 160);
    
    ui::ScrollView* scrollView = ui::ScrollView::create();
    scrollView->setBounceEnabled(true);
    scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
    scrollView->setContentSize(Size(560, scHt));
    scrollView->setInnerContainerSize(scrollView->getContentSize());
    scrollView->setScrollBarEnabled(false);
    float innerWidth = scrollView->getContentSize().width;
    float innerHeight = scrollView->getContentSize().height;
    
    scrollView->setInnerContainerSize(Size(innerWidth, innerHeight));
    scrollView->setPosition(Vec2((size.width - 560)/2, 160));
    addChild(scrollView);
    _scrollView = scrollView;
    
    
    auto label = Label::createWithSystemFont(LocalizedString("TID_UI_RULES PAGE"), "Arial", 32);
    label->setDimensions(560, 0);
    label->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    label->setPosition(Vec2(0, scrollView->getContentSize().height));
    _label = label;
    scrollView->addChild(label);
    
    
    callback(_tabButtons.at(0), Widget::TouchEventType::ENDED);
    
    return true;
}

void Layer_HowToPlay::loadData(int idx){

    string ids[] = {"TID_UI_RULES PAGE", "TID_UI_SCORE PAGE BASIC", "TID_UI_GAME PAGE BASIC", "TID_UI_DAILY HELP"};
    
    _label->setString(LocalizedString(ids[idx]));
    _scrollView->setInnerContainerSize(Size(_scrollView->getContentSize().width, _label->getContentSize().height));
    
    _label->setPosition(Vec2(0, _scrollView->getInnerContainerSize().height));
//    _scrollView->setInnerContainerPosition(Vec2(0, 0));
}

#pragma mark - Layer_Language -

void Layer_Language::updateLayout(){

    
    _bg->setContentSize(winSize);
    
    _panel->setPosition(VisibleRect::center() + Vec2(0, 0));
    
    _titleLbl->setPosition(VisibleRect::center() + Vec2(0, 196));
    
    _btn->setPosition(VisibleRect::center() + Vec2(256, 196));
    
    _box->setPosition(VisibleRect::center() + Vec2(0, -26));
}

bool Layer_Language::init(){
    if (!KTPauseLayer::init()) {
        return false;
    }
    
    auto bg = LayerColor::create(Color4B(0, 0, 0, 175));
    addChild(bg, -2);
    _bg = bg;
    
    auto panel = ImageView::create("9_greenwindow.png");
    panel->setTag(123);
    panel->setScale9Enabled(true);
    panel->setContentSize(Size(640, 500));
    panel->setPosition(VisibleRect::center() + Vec2(0, 0));
    addChild(panel, -1);
    _panel = panel;
    
    Label *titleLbl = Label::createWithSystemFont(LocalizedString("TID_UI_LANGUAGE"), "Arial", 36);
    titleLbl->setTextColor(Color4B::WHITE);
    titleLbl->setAlignment(TextHAlignment::CENTER);
    titleLbl->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    titleLbl->setPosition(VisibleRect::center() + Vec2(0, 196));
    addChild(titleLbl);
    _titleLbl = titleLbl;
    
    auto btn = KTFactory::createButton("close.png", "close1.png", "",  "", 30);
    btn->setPosition(VisibleRect::center() + Vec2(256, 196));
    btn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            this->removeFromParentAndCleanup(true);
        }
    });
    addChild(btn);
    _btn = btn;
//    auto map = Localization::getLanguageDict();
//    for(auto it = map.begin(); it != map.end(); ++it ) {
//        languages.push_back(it->second);
//    }
    
    char str[64];
    auto box = VBox::create(Size(570, 360));
    box->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    box->setPosition(VisibleRect::center() + Vec2(0, -26));
    addChild(box);
    _box = box;
    for (int i = 0; i < 4; i++) {
        auto box2 = HBox::create(Size(570, 90));
        box->addChild(box2);
        
        for (int j = 0; j < 4; j++) {
            int idx = i * 4 + j;
            
            sprintf(str, "shared/flags/%s.png", languages[idx].c_str());
            
            auto widget = Layout::create();
            widget->setContentSize(Size(142.5f, 90));
            
            auto btn = Button::create(str);
            btn->setTag(idx);
            btn->setPosition((widget->getContentSize())/2);
            btn->addClickEventListener([=](Ref*sender){
                auto node = static_cast<Node *>(btn);
                auto name = languages[node->getTag()];
                Localization::setLanguage(name);
                this->removeFromParent();
            });
            
            widget->addChild(btn);
            box2->addChild(widget);
            
        }
    }
    
    return true;
}

#pragma mark - Layer_CustomTheme -

Layer_CustomTheme::~Layer_CustomTheme(){
    CC_SAFE_RELEASE_NULL(_rt);
    __NotificationCenter::getInstance()->removeAllObservers(this);
}

void Layer_CustomTheme::onThemeFileCanceled(Ref *sender){

}
void Layer_CustomTheme::onThemeFileChanged(Ref *sender){
    auto mgr = DataManager::getInstance();
    mgr->setCustomSelectThemeName(mgr->getThemes().at(0)->getName());
    
    Director::getInstance()->getTextureCache()->removeTextureForKey(DataManager::getInstance()->getCacheThemeFile(true));
    Director::getInstance()->getTextureCache()->removeTextureForKey(DataManager::getInstance()->getCacheThemeFile(false));
    Director::getInstance()->getTextureCache()->removeTextureForKey(DataManager::getInstance()->getCachePreviewThemeFile());
    
//    reloadData();
    runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create([=](){

        auto file = DataManager::getInstance()->getCachePreviewThemeFile();
        auto rt = _rt;
        
        auto sp1 = Sprite::create(file);
        sp1->setPosition(44, 131 * 0.5);
        auto sp2 = Sprite::create("_outline.png");
        sp2->setPosition(44, 131 * 0.5);
        //    auto sp3 = Sprite::create("_light.png");
        //    clip->addChild(sp3);
        //    sp3->setPosition(Vec2(44, 131-2));
        
        //this->addChild(clip, 2048);
        
        rt->beginWithClear(0,0,0,0);
        sp1->visit();
        sp2->visit();
        rt->end();
        
        
        //rt->setPosition(100, 100);
        //this->addChild(rt, 2048);
        
        
        rt->saveToFile("theme.png", Image::Format::PNG, true, [&](RenderTexture*, const std::string& fullPath){
            CCLOG("save success!!!");
            Director::getInstance()->getTextureCache()->removeTextureForKey(file);
            reloadData();
            //        this->setClipContainer(nullptr);
        });
        
        //    });
        Director::getInstance()->getRenderer()->render();
    }), NULL));
}

void Layer_CustomTheme::onCardbackFileCanceled(Ref *sender){
//    if (dataArray.size() !=0 && dataArray[0].size() !=0 && dataArray[0][0] == "shared/cardback/0.png") {
//        auto item = static_cast<Cell_CardBack *>(_listView->getItem(0));
//        Button_Highlight*btn = item->getIconViews().at(1);
//        btn->onClick(btn);
//    }
}

void Layer_CustomTheme::onCardbackFileChanged(Ref *sender){
    auto mgr = DataManager::getInstance();
    mgr->setCustomSelectCardbackName(mgr->getCardbacks().at(0)->getName());

    auto file = DataManager::getInstance()->getCacheCardbackFile();
    Director::getInstance()->getTextureCache()->removeTextureForKey(file);
    
//    reloadData();
    
    runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create([=](){
    
        auto sp1 = Sprite::create(file);
        sp1->setPosition(44, 131 * 0.5);
        auto sp2 = Sprite::create("_outline.png");
        sp2->setPosition(44, 131 * 0.5);
        
        _rt->beginWithClear(0,0,0,0);
        sp1->visit();
        sp2->visit();
        _rt->end();
        
        _rt->saveToFile("cardback.png", Image::Format::PNG, true, [&](RenderTexture*, const std::string& fullPath){
            Director::getInstance()->getTextureCache()->removeTextureForKey(fullPath);
            reloadData();
        });
        Director::getInstance()->getRenderer()->render();
        
    }), NULL));
}

void Layer_CustomTheme::updateLayout(){
    
    bool flag = winSize.height > winSize.width;
    _bg->setScale(winSize.width/640.f, winSize.height/1136.f);
    _bg->setPosition(VisibleRect::center());
    auto size = VisibleRect::getVisibleRect().size;
    float bottomOffset = flag ? 90 : 0;
    size.height -= (getNavigationBar()->getContentSize().height + bottomOffset);
    
    _listView->setContentSize(size);
    _listView->setPosition(Vec2(0, bottomOffset));

}

bool Layer_CustomTheme::init(){
    if (!KTPauseLayer::init()) {
        return false;
    }
    
    _rt = RenderTexture::create(88, 131);
    _rt->retain();
    
//    _clipContainer = nullptr;
    auto size = VisibleRect::getVisibleRect().size;
    auto sprite = Sprite::create(BG_FILENAME);
    
    sprite->setPosition(VisibleRect::center());
    this->addChild(sprite, -100);
    _bg = sprite;
    
    setNavigationBarEnable(true);
    auto navBar = getNavigationBar();
    
    float bottomOffset = 90;
    size.height -= (getNavigationBar()->getContentSize().height + bottomOffset);
    getNavigationBar()->setTitle(LocalizedString("TID_UI_CUSTOM"));
    
    Vector<MenuItem *>rightItems;
    auto itemQuit = MenuItemImage::create("close.png", "close1.png", [=](Ref *sender){
        Director::getInstance()->popScene();
    });
    rightItems.pushBack(itemQuit);
    navBar->setRightBarItems(rightItems);
    
    Vector<MenuItem *>leftItems;
    auto space = MenuItemImage::create("null.png", "null.png", [=](Ref *sender){
        
    });
    space->setContentSize(itemQuit->getContentSize());
    leftItems.pushBack(space);
    navBar->setLeftBarItems(leftItems);
    
    ListView* listView = ListView::create();
    // set list view ex direction
    listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(true);
    listView->setScrollBarEnabled(true);
    
    listView->setScrollBarColor(Color3B::BLACK);
    listView->setScrollBarOpacity(64);
    listView->setScrollBarAutoHideEnabled(false);
    
    CCLOG("width = %d", (int)listView->getScrollBarWidth());
    
    listView->setContentSize(size);
    listView->setPosition(Vec2(0, bottomOffset));
    
    this->addChild(listView);
    
    listView->setItemModel(cellForDisplay());
    listView->addEventListener([=](Ref*, ListView::EventType type){
        
        if (type == ListView::EventType::ON_SELECTED_ITEM_END) {
            //            int idx = listView->getCurSelectedIndex();
            //            CCLOG("select = %d", idx);
        }
    });
    _listView = listView;
    
    reloadData();
    
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(Layer_CustomTheme::onCardbackFileChanged), kOnCardbackFileChanged, nullptr);
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(Layer_CustomTheme::onCardbackFileCanceled), kOnCardbackFileCanceled, nullptr);
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(Layer_CustomTheme::onThemeFileChanged), kOnThemeFileChanged, nullptr);
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(Layer_CustomTheme::onThemeFileCanceled), kOnThemeFileCanceled, nullptr);
    
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(Layer_CustomTheme::updateBadge), kUpdateBadgeCardback, nullptr);
    
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(Layer_CustomTheme::updateBadge), kUpdateBadgeTheme, nullptr);
    
    return true;
}

void Layer_CustomTheme::updateBadge(Ref *sender){
    reloadData();
}

Layout *Layer_CustomTheme::cellForDisplay(){
    return Cell_CustomTheme::create();
}

void Layer_CustomTheme::reloadData(){
    _listView->removeAllItems();
    auto mgr = DataManager::getInstance();
    string titles[] = {"TID_UI_CARD FACES", "TID_UI_CARD BACKS", "TID_UI_BACKGROUND"};
    
    for(int i = 0;i < 3; i++){
        _listView->pushBackDefaultItem();
        auto cell = static_cast<Cell_CustomTheme *>(_listView->getItem(i));
        cell->getTitleLabel()->setString(LocalizedString(titles[i]));
        Vector<PreviewItem *> dataArray;
        int defaultIdx = 0;
        if (i == 0) {
            for (auto item : mgr->getFaces()) {
                dataArray.pushBack(item);
            }
            defaultIdx = (int)mgr->getFaces().getIndex(mgr->getFace(mgr->getCustomSelectFaceName()));
        } else if (i == 1) {
            for (auto item : mgr->getCardbacks()) {
                dataArray.pushBack(item);
            }
            
            defaultIdx = (int)mgr->getCardbacks().getIndex(mgr->getCardback(mgr->getCustomSelectCardbackName()));

        } else if (i ==2) {
            for (auto item : mgr->getThemes()) {
                dataArray.pushBack(item);
            }
            defaultIdx = (int)mgr->getThemes().getIndex(mgr->getTheme(mgr->getCustomSelectThemeName()));

        }
        cell->reloadData(dataArray);
       
        auto subList = cell->getListView();
        
        
        
        auto items = subList->getItems();
//        subList->setInnerContainerPosition(Vec2(-20, 0));
//        subList->setInnerContainerPosition(Vec2(20, 0));
//        subList->setInnerContainerPosition(Vec2(0, 0));
        //tag2
        for (int j = 0; j < items.size(); j++) {
            auto subCell = static_cast<Cell_CustomThemeSubView *>(items.at(j));
            
//            subCell->addTouchEventListener([=](Ref*,Widget::TouchEventType type){
//                if (type == Widget::TouchEventType::MOVED) {
//                    _listView->setTouchEnabled(false);
//                }
//            });
            
            if (i == 1) {
                //牌被特别
                auto cardbackItem = mgr->getCardbacks().at(j);
                bool flag = false;
                if (j == 0) {
                    
//                    if (cardbackItem->getPreviewFile() == mgr->getCacheCardbackFile()) {
//                        auto iv = subCell->getIconView()->getIcon();
//                        auto oldSize = iv->getContentSize();
//                        auto newSize = oldSize + Size(-10,-4);
//                        iv->setContentSize(newSize);
//                        {
//                            ImageView *iv2 = ImageView::create("_outline.png");
//                            iv2->setScale9Enabled(true);
//                            iv2->setCapInsets(Rect(0, 0, 88, 131));
//                            iv2->setPosition(iv->getPosition());
//                            iv2->setContentSize(oldSize);
//                            subCell->addChild(iv2, 32);
//                        }
//                    }
                    if (UserDefault::getInstance()->getBoolForKey("KL_CUSTOM_CARDBACK_CLICK") == false) {
                        subCell->getIconView()->setBadgeType(Button_Highlight::BadgeType::Simple);
                        flag = true;
                    }
                } else if (cardbackItem->getIsNew() && UserDefault::getInstance()->getBoolForKey(StringUtils::format("KL_CARDBACK_%s_CLICK", cardbackItem->getName().c_str()).c_str()) == false) {
                    subCell->getIconView()->setBadgeType(Button_Highlight::BadgeType::New);
                    flag = true;
                }
                subCell->getIconView()->setBadgeEnabled(flag);
            } else if (i == 2) {
            
                bool flag = false;
                auto themeItem = mgr->getThemes().at(j);
                if (j == 0) {
                    if (UserDefault::getInstance()->getBoolForKey("KL_CUSTOM_THEME_CLICK") == false) {
                        flag = true;
                        subCell->getIconView()->setBadgeType(Button_Highlight::BadgeType::Simple);
                    }
//                    if (themeItem->getPreviewFile() == mgr->getCachePreviewThemeFile()) {
//                        auto iv = subCell->getIconView()->getIcon();
//                        auto oldSize = iv->getContentSize();
//                        auto newSize = oldSize + Size(-10,-4);
//                        iv->setContentSize(newSize);
//                        {
//                            ImageView *iv2 = ImageView::create("_outline.png");
//                            iv2->setScale9Enabled(true);
//                            iv2->setCapInsets(Rect(0, 0, 88, 131));
//                            iv2->setPosition(iv->getPosition());
//                            iv2->setContentSize(oldSize);
//                            subCell->addChild(iv2, 32);
//                        }
//                        
//                    }
                    
                } else if (themeItem->getIsNew() && UserDefault::getInstance()->getBoolForKey(StringUtils::format("KL_THEME_%s_CLICK", themeItem->getName().c_str()).c_str()) == false) {
                    flag = true;
                    subCell->getIconView()->setBadgeType(Button_Highlight::BadgeType::New);
                }
                subCell->getIconView()->setBadgeEnabled(flag);
            }
            
            
            subCell->getIconView()->onClick = [=](Button_Highlight *sender){
                
                auto current = static_cast<Cell_CustomThemeSubView *>(sender->getParent());

                bool canSelect = true;
                int idx = (int)subList->getIndex(current);
                if (i == 0) {
                    mgr->setCustomSelectFaceName(mgr->getFaces().at(idx)->getName());
                } else if (i == 1) {
                    
                    if(idx == 2 && !DataManager::getInstance()->getFirstShared()){
                        canSelect = false;
                        auto la = KLShareInfoLayer::create();
                        this->addChild(la);
                    }
                    else if(idx == 0){
                        iOSWrapper::showImagePicker();
                        canSelect = false;
                        
                        UserDefault::getInstance()->setBoolForKey("KL_CUSTOM_CARDBACK_CLICK", true);
                        UserDefault::getInstance()->flush();
                        __NotificationCenter::getInstance()->postNotification(kUpdateBadgeCardback);
                    }
                    else {
                        mgr->setCustomSelectCardbackName(mgr->getCardbacks().at(idx)->getName());
                    }
                    
                } else if (i == 2) {
                    
                    if(idx == 0) {
                        iOSWrapper::showThemePicker();
                        
                        UserDefault::getInstance()->setBoolForKey("KL_CUSTOM_THEME_CLICK", true);
                        UserDefault::getInstance()->flush();
                        __NotificationCenter::getInstance()->postNotification(kUpdateBadgeTheme);
                        
                        canSelect = false;
                    }
                    else {
                        mgr->setCustomSelectThemeName(mgr->getThemes().at(idx)->getName());
                    }
                }
                
                if(canSelect){
                    for(auto item : subList->getItems()){
                        auto cell = static_cast<Cell_CustomThemeSubView *>(item);
                        cell->getIconView()->setSelected(false);
                    }
                    sender->setSelected(true);
                }
            };
        }
        static_cast<Cell_CustomThemeSubView *>(subList->getItem(defaultIdx))->getIconView()->setSelected(true);
    }
}

#pragma mark - Layer_Select -

void Layer_Select::updateLayout(){

    bool flag = winSize.height > winSize.width;
    _bg->setScale(winSize.width/640.f, winSize.height/1136.f);
    _bg->setPosition(VisibleRect::center());
    auto size = VisibleRect::getVisibleRect().size;
    float bottomOffset = flag ? 200 : 90;
    size.height -= (getNavigationBar()->getContentSize().height + bottomOffset);

    _listView->setContentSize(size);
    _listView->setPosition(Vec2(0, bottomOffset));
    
    _cancelBtn->setPosition(VisibleRect::bottom() + Vec2(-160, bottomOffset-50));
    _submitBtn->setPosition(VisibleRect::bottom() + Vec2(160, bottomOffset-50));
}

bool Layer_Select::init(){
    if (!KTPauseLayer::init()) {
        return false;
    }
    auto size = VisibleRect::getVisibleRect().size;
    auto sprite = Sprite::create(BG_FILENAME);
    
    sprite->setPosition(VisibleRect::center());
    this->addChild(sprite, -100);
    _bg = sprite;
    
    setNavigationBarEnable(true);
    
    float bottomOffset = 200;
    size.height -= (getNavigationBar()->getContentSize().height + bottomOffset);
    
    ListView* listView = ListView::create();
    // set list view ex direction
    listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(true);
    listView->setScrollBarEnabled(false);
    
    listView->setContentSize(size);
    listView->setPosition(Vec2(0, bottomOffset));
    
    this->addChild(listView);
    
    listView->setItemModel(cellForDisplay());
    listView->addEventListener([=](Ref*, ListView::EventType type){
        
        if (type == ListView::EventType::ON_SELECTED_ITEM_END) {
//            int idx = listView->getCurSelectedIndex();
//            CCLOG("select = %d", idx);
        }
    });
    _listView = listView;
    
    reloadData();
    
    {
        auto btn = KTFactory::createButton("B_green1.png", "B_green2.png", "",  LocalizedString("TID_UI_CANCEL"), 36, "Arial", Size(240, 72));
        //        btn->setPosition(VisibleRect::center() + Vec2(0, -132));
        
//        btn->setContentSize(Size(240, 72));
        auto label = btn->getTitleRenderer();
        label->setPosition(label->getPosition() + Vec2(0, 4));
        btn->setPosition(VisibleRect::bottom() + Vec2(-160, 150));
        btn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
            if (type == Widget::TouchEventType::ENDED) {
//                Node *n = (Node *)pSender;
//                this->removeFromParentAndCleanup(true);
                Director::getInstance()->popScene();
                this->onCancel();
            }
        });
        addChild(btn);
        _cancelBtn = btn;
    }
    
    {
        auto btn = KTFactory::createButton("B_green.png", "B_green0.png", "",  LocalizedString("TID_UI_CONFIRM"), 36, "Arial", Size(240, 72));
//        btn->setContentSize(Size(240, 72));
        auto label = btn->getTitleRenderer();
        label->setPosition(label->getPosition() + Vec2(0, 4));
        btn->setPosition(VisibleRect::bottom() + Vec2(160, 150));
        btn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
            if (type == Widget::TouchEventType::ENDED) {
//                Node *n = (Node *)pSender;
                this->onConfirm();
//                this->removeFromParentAndCleanup(true);
                Director::getInstance()->popScene();
            }
        });
        addChild(btn);
        _submitBtn = btn;
    }
    
    return true;
}
Layout *Layer_Select::cellForDisplay(){
    return nullptr;
}
void Layer_Select::reloadData(){

    
    
    
}
void Layer_Select::onConfirm(){

    
}

void Layer_Select::onCancel(){
}

#pragma mark - Layer_ThemeSet -

Layer_ThemeSet::~Layer_ThemeSet(){

}

Layout *Layer_ThemeSet::cellForDisplay(){
    return Cell_ThemeSet::create();
}

void Layer_ThemeSet::reloadData(){
    
    auto mgr = DataManager::getInstance();
    _listView->removeAllItems();

    int num = (int)mgr->getThemeSets().size();
    int cellNum = ceil(num / 3.f);
    function<void()> customThemeFunc = [=](){
        auto la = Layer_CustomTheme::create();
        auto sc = Scene::create();
        sc->addChild(la);
        Director::getInstance()->pushScene(sc);
    };
    
    for (int i = 0; i < cellNum; i++) {
        _listView->pushBackDefaultItem();
        auto cell = static_cast<Cell_ThemeSet *>( _listView->getItem(i) );

        for (int j = 0; j < 3; j++) {
            int idx = i*3+j;
            Button_Highlight* iconView = cell->getIconViews().at(j);

            if (idx < num) {
                auto item = mgr->getThemeSets().at(idx);
                if (item->getName() == "theme_set_1") {
                    auto viewWidth = iconView->getContentSize().width;
                    Button *btn = KTFactory::createButton("B_green.png", "B_green0.png", "B_NO.png", LocalizedString("TID_UI_CUSTOM"), 20, "Arial", Size(viewWidth-40, 60));
                    iconView->addChild(btn, 64);
                    btn->setPosition(Vec2(viewWidth/2, 50));
                    auto titleRender = btn->getTitleRenderer();
                    titleRender->setPositionY(titleRender->getPositionY() + 5);
                    iconView->setCustomButton(btn);
                    
                    btn->addClickEventListener([=](Ref *sender){
                        customThemeFunc();
                    });
                }
                iconView->getIcon()->loadTexture(item->getPreviewFile());
                bool flag = false;
                if (item->getIsNew() && UserDefault::getInstance()->getBoolForKey(StringUtils::format("KL_THEME_SET_%s_CLICK", item->getName().c_str()).c_str()) == false) {
                    flag = true;
                    if(item->getName() == "theme_set_1") {
                        iconView->setBadgeType(Button_Highlight::BadgeType::Simple);

                    } else {
                        iconView->setBadgeType(Button_Highlight::BadgeType::New);
                    }
                }
                iconView->setBadgeEnabled(flag);
                
                iconView->onClick = [=](Button_Highlight *sender){
                    auto current = static_cast<Cell_ThemeSet *>(sender->getParent()->getParent());
                    auto x = current->getIconViews().getIndex(sender);
                    auto y = _listView->getIndex(current);
                    int idx = int(y * 3 + x);
                    auto item = mgr->getThemeSets().at(idx);
                    
                    if (item->getName() == "theme_set_0") {
                        iOSWrapper::showToSetupThemeAppPage();
                    } else {
                        auto customItem = mgr->getThemeSet("theme_set_1");
                        auto customIdx = mgr->getThemeSets().getIndex(customItem);
                        bool flag = _idx == customIdx && idx == customIdx && _inited;
                        
                        _idx = idx;
                        for(auto item : _listView->getItems()){
                            auto cell = static_cast<Cell_ThemeSet *>(item);
                            for(auto btn : cell->getIconViews()){
                                btn->setSelected(false);
                            }
                        }
                        
                        sender->setSelected(true);
                        
                        
                        auto firstCell = static_cast<Cell_ThemeSet *>(_listView->getItem(customIdx/3));
                        auto firstView = firstCell->getIconViews().at(customIdx%3);
                        firstView->getCustomButton()->setEnabled(_idx == customIdx);
                        
                        if(flag) {
                            customThemeFunc();
                        }
                    }
                };
            } else {
                iconView->setVisible(false);
            }
        }
    }
    
    int x = _lastIdx%3;
    int y = _lastIdx/3;
    auto firstCell = static_cast<Cell_ThemeSet *>(_listView->getItem(y));
    auto firstView = firstCell->getIconViews().at(x);
    firstView->onClick(firstView);
}

void Layer_ThemeSet::onConfirm() {
    
    
}

void Layer_ThemeSet::updateLayout(){
    
    Layer_Select::updateLayout();
    bool flag = winSize.height > winSize.width;
    auto size = VisibleRect::getVisibleRect().size;

    float bottomOffset = flag ? 90 : 0;
    size.height -= (getNavigationBar()->getContentSize().height + bottomOffset);
    
    _listView->setContentSize(size);
    _listView->setPosition(Vec2(0, bottomOffset));
}


bool Layer_ThemeSet::init(){
    _inited = false;
    auto mgr = DataManager::getInstance();
    _lastIdx = _idx = (int)mgr->getThemeSets().getIndex(mgr->getThemeSet(mgr->getSelectThemeSetName()));
    
    if (!Layer_Select::init()) {
        return false;
    }
    auto navBar = getNavigationBar();
    navBar->setTitle(LocalizedString("TID_UI_CHANGE BACKGROUND"));
    _cancelBtn->setVisible(false);
    _submitBtn->setVisible(false);
    
    Vector<MenuItem *>rightItems;
    auto itemQuit = MenuItemImage::create("close.png", "close1.png", [=](Ref *sender){
        auto mgr = DataManager::getInstance();
        auto item = mgr->getThemeSets().at(_idx);
        mgr->changeThemeSet(item->getName());
        
        Director::getInstance()->popScene();

    });
    rightItems.pushBack(itemQuit);
    navBar->setRightBarItems(rightItems);
    
    Vector<MenuItem *>leftItems;
    auto space = MenuItemImage::create("null.png", "null.png");
    space->setContentSize(itemQuit->getContentSize());
    leftItems.pushBack(space);
    navBar->setLeftBarItems(leftItems);
    
    _inited = true;
    
    return true;
}

/*

#pragma mark - Layer_Themes -

Layer_Themes::~Layer_Themes(){
    __NotificationCenter::getInstance()->removeObserver(this, kOnThemeFileChanged);
    __NotificationCenter::getInstance()->removeObserver(this, kOnThemeFileCanceled);

}

bool Layer_Themes::init(){
    auto mgr = DataManager::getInstance();
    _idx = (int)mgr->getThemes().getIndex(mgr->getTheme(mgr->getCustomSelectThemeName()));
    
    if (!Layer_Select::init()) {
        return false;
    }
    
//    {
//        auto btn = KTFactory::createButton("B_green.png", "B_green0.png", "",  LocalizedString("More"), 36, "Arial", Size(240, 72));
//        //        btn->setContentSize(Size(240, 72));
//        auto label = btn->getTitleRenderer();
//        label->setPosition(label->getPosition() + Vec2(0, 4));
//        btn->setPosition(VisibleRect::bottom() + Vec2(0, 280));
//        btn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
//            if (type == Widget::TouchEventType::ENDED) {
//                iOSWrapper::showImagePicker();
//            }
//        });
//        addChild(btn);
//    }
    
    getNavigationBar()->setTitle(LocalizedString("TID_UI_CHANGE BACKGROUND"));
    
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(Layer_Themes::onThemeFileChanged), kOnThemeFileChanged, nullptr);
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(Layer_Themes::onThemeFileCanceled), kOnThemeFileCanceled, nullptr);

    
    return true;
}
void Layer_Themes::onThemeFileCanceled(Ref *sender){
    if(_idx == 0) {
        auto item = static_cast<Cell_CardBack *>(_listView->getItem(0));
        Button_Highlight*btn = item->getIconViews().at(1);
        btn->onClick(btn);
        {
            auto item = DataManager::getInstance()->getTheme(0);
            item->setBgFile("shared/theme/0.jpg");
            item->setPreviewFile("shared/theme/0.png");
        }
    }
}
void Layer_Themes::onThemeFileChanged(Ref *sender){

    DataManager *mgr = DataManager::getInstance();
    auto item = DataManager::getInstance()->getTheme(0);
    item->setBgFile(mgr->getCacheThemeFile(true));
    item->setPreviewFile(mgr->getCachePreviewThemeFile());
    
    Director::getInstance()->getTextureCache()->removeTextureForKey(DataManager::getInstance()->getCacheThemeFile(true));
    Director::getInstance()->getTextureCache()->removeTextureForKey(DataManager::getInstance()->getCacheThemeFile(false));
    Director::getInstance()->getTextureCache()->removeTextureForKey(DataManager::getInstance()->getCachePreviewThemeFile());
    
    auto file = DataManager::getInstance()->getCachePreviewThemeFile();
    auto rt = RenderTexture::create(88, 131,Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
    
    auto container = Node::create();
    container->retain();
    auto stencil = Node::create();
    auto sp1 = Sprite::create("_stencil.png");
    sp1->setPosition(44, 131 * 0.5);
    stencil->addChild(sp1);
    auto clip = ClippingNode::create(stencil);
    clip->setAlphaThreshold(0);
    container->addChild(clip);
    
    auto sp2 = Sprite::create(file);
    sp2->setPosition(44, 131 * 0.5);
    clip->addChild(sp2);

    auto sp3 = Sprite::create("_light.png");
    clip->addChild(sp3);
    sp3->setPosition(Vec2(44, 131-2));
    
    //this->addChild(clip, 2048);
    
    rt->begin();
    //auto renderer = _director->getRenderer();
    //auto& parentTransform = _director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    //clip->visit(renderer, parentTransform, true);
    container->visit();
    rt->end();
    

    //rt->setPosition(100, 100);
    //this->addChild(rt, 2048);
   
    
    rt->saveToFile("theme.png", Image::Format::PNG, true, [&](RenderTexture*, const std::string& fullPath){
        Director::getInstance()->getTextureCache()->removeTextureForKey(fullPath);
        reloadData();
    });
    Director::getInstance()->getRenderer()->render();
}

Layout *Layer_Themes::cellForDisplay(){
    return Cell_CardBack::create();
}

//tag1
void Layer_Themes::reloadData(){
    
    auto mgr = DataManager::getInstance();
    _listView->removeAllItems();

    int num = mgr->getThemeCount();
    int cellNum = ceil(num * 0.25);
    CCLOG("%s", StringUtils::format("num = %d", num).c_str());
    CCLOG("%s", StringUtils::format("cellNum = %d", cellNum).c_str());

    for (int i = 0; i < cellNum; i++) {
        _listView->pushBackDefaultItem();
    }
    
    for (int i = 0; i < cellNum; i++) {
        auto cell = static_cast<Cell_CardBack *>( _listView->getItem(i) );
        
        for(int j = 0; j < 4;j++){
            auto iconView = cell->getIconViews().at(j);
            
            int dataIdx = (i * 4 + j);
            
            if (dataIdx < num) {
                
                auto item = mgr->getThemes().at(dataIdx);
                
                bool flag = false;
                if (dataIdx== 0 && (UserDefault::getInstance()->getBoolForKey("KL_CUSTOM_THEME_CLICK") == false)) {
                    flag = true;
                    iconView->setBadgeType(Button_Highlight::BadgeType::Simple);
                } else if (item->getIsNew() && UserDefault::getInstance()->getBoolForKey(StringUtils::format("KL_THEME_%s_CLICK", item->getName().c_str()).c_str()) == false) {
                    flag = true;
                    iconView->setBadgeType(Button_Highlight::BadgeType::New);
                }
                iconView->setBadgeEnabled(flag);
                
                iconView->getAddIcon()->setVisible(i==0 && j == 0);
                iconView->getIcon()->loadTexture(item->getPreviewFile());
                iconView->onClick = [=](Button_Highlight *sender){
                    
                    for(auto item : _listView->getItems()){
                        auto cell = static_cast<Cell_CardBack *>(item);
                        for(auto btn : cell->getIconViews()){
                            btn->setSelected(false);
                        }
                    }
                    auto current = static_cast<Cell_CardBack *>(sender->getParent()->getParent());
                    sender->setSelected(true);
                    auto x = current->getIconViews().getIndex(sender);
                    auto y = _listView->getIndex(current);
                    _idx = int(y * 4 + x);
                    
                    if(_idx == 0){
                        iOSWrapper::showThemePicker();
                    }
                };
            } else {
                iconView->setVisible(false);
            }
        }
    }
    int x = _idx%4;
    int y = _idx/4;
    auto cell = static_cast<Cell_CardBack *>( _listView->getItem(y) );
    cell->getIconViews().at(x)->setSelected(true);
}

void Layer_Themes::onCancel(){
    auto mgr = DataManager::getInstance();
    string name = mgr->getCustomSelectThemeName();
    auto item = mgr->getTheme(0);
    if(name == "theme_1"){
        item->setBgFile(mgr->getCacheThemeFile(true));
        item->setPreviewFile(mgr->getCachePreviewThemeFile());
    } else {
        item->setBgFile("shared/theme/0.jpg");
        item->setPreviewFile("shared/theme/0.png");
    }
}

void Layer_Themes::onConfirm(){
    auto mgr = DataManager::getInstance();
    string name = mgr->getThemes().at(_idx)->getName();
    CCLOG("name = %s", name.c_str());
    mgr->changeTheme(name);
    
    auto item = DataManager::getInstance()->getTheme(0);
    
    if(name == "theme_1"){
        item->setBgFile(mgr->getCacheThemeFile(true));
        item->setPreviewFile(mgr->getCachePreviewThemeFile());
    } else {
        item->setBgFile("shared/theme/0.jpg");
        item->setPreviewFile("shared/theme/0.png");
    }
}


#pragma mark - Layer_CardBack -

Layer_CardBack::~Layer_CardBack(){
    __NotificationCenter::getInstance()->removeObserver(this, kOnCardbackFileChanged);
    __NotificationCenter::getInstance()->removeObserver(this, kOnCardbackFileCanceled);

}

bool Layer_CardBack::init(){
    auto mgr = DataManager::getInstance();
    _idx = (int)mgr->getCardbacks().getIndex(mgr->getCardback(mgr->getSelectCardbackName()));
    if (!Layer_Select::init()) {
        return false;
    }
    
    getNavigationBar()->setTitle(LocalizedString("TID_UI_CHANGE CARD BACKS"));
//    _listView->addEventListener([=](Ref*, ListView::EventType type){
//        
//        if (type == ListView::EventType::ON_SELECTED_ITEM_END) {
//            auto idx = this->_listView->getCurSelectedIndex();
//            this->_listView->getItem(idx);
//            //            CCLOG("select = %d", idx);
//        }
//    });
    
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(Layer_CardBack::onCardbackFileChanged), kOnCardbackFileChanged, nullptr);
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(Layer_CardBack::onCardbackFileCanceled), kOnCardbackFileCanceled, nullptr);

    
    return true;
}

void Layer_CardBack::onCardbackFileCanceled(Ref *sender){
    if (dataArray.size() !=0 && dataArray[0].size() !=0 && dataArray[0][0] == "shared/cardback/0.png") {
        auto item = static_cast<Cell_CardBack *>(_listView->getItem(0));
        Button_Highlight*btn = item->getIconViews().at(1);
        btn->onClick(btn);
    }
}

void Layer_CardBack::onCardbackFileChanged(Ref *sender){

    auto file = DataManager::getInstance()->getCacheCardbackFile();
    Director::getInstance()->getTextureCache()->removeTextureForKey(file);
    auto rt = RenderTexture::create(88, 131);
    auto sp1 = Sprite::create(file);
    sp1->setPosition(44, 131 * 0.5);
    auto sp2 = Sprite::create("_outline.png");
    sp2->setPosition(44, 131 * 0.5);
    
    rt->begin();
    sp1->visit();
    sp2->visit();
    rt->end();
    
    rt->saveToFile("cardback.png", Image::Format::PNG, true, [&](RenderTexture*, const std::string& fullPath){
        Director::getInstance()->getTextureCache()->removeTextureForKey(fullPath);
        reloadData();
    });
    Director::getInstance()->getRenderer()->render();
}

Layout *Layer_CardBack::cellForDisplay(){
    return Cell_CardBack::create();
}


void Layer_CardBack::reloadData(){
    auto mgr = DataManager::getInstance();
    dataArray.clear();
    _listView->removeAllItems();
    
    int num = (int)mgr->getCardbacks().size();
    bool flag = mgr->getFirstShared();
    int max = ceil(num * 0.25) * 4;
    for (int i = 0; i < max; i++) {
        
        int x = i % 4;
        int y = i / 4;
        
        if (x == 0) {
            vector<string> row;
            dataArray.push_back(row);
        }
        if (i < num) {
            auto item = mgr->getCardbacks().at(i);
            if(i == 0 && _idx == 0){
                dataArray[y].push_back(mgr->getCacheCardbackFile());
            } else {
                dataArray[y].push_back(item->getPreviewFile());
            }
        } else {
            dataArray[y].push_back("");
        }
    }
    
    if(!flag){
        dataArray[0][2] = "shared/cardback/99.png";
    }
    
    for (int i = 0; i < dataArray.size(); i++) {
        _listView->pushBackDefaultItem();
    }
    
    for (int i = 0; i < dataArray.size(); i++) {
        auto cell = static_cast<Cell_CardBack *>( _listView->getItem(i) );
        
        for(int j = 0; j < 4;j++){
            int dataIdx = (i * 4 + j);
            
            
            auto iconView = cell->getIconViews().at(j);
            
            string name = dataArray[i][j];
            if (name.empty()) {
                iconView->setVisible(false);
            } else {
                auto item = mgr->getCardbacks().at(dataIdx);
                iconView->getAddIcon()->setVisible(i==0 && j == 0);
                bool flag = false;
                if (i == 0 && j == 0 && (UserDefault::getInstance()->getBoolForKey("KL_CUSTOM_CARDBACK_CLICK") == false)) {
                    flag = true;
                    iconView->setBadgeType(Button_Highlight::BadgeType::Simple);
                } else if (item->getIsNew() && UserDefault::getInstance()->getBoolForKey(StringUtils::format("KL_CARDBACK_%s_CLICK", item->getName().c_str()).c_str()) == false) {
                    flag = true;
                    iconView->setBadgeType(Button_Highlight::BadgeType::New);
                }
                iconView->setBadgeEnabled(flag);
                iconView->getIcon()->loadTexture(name);
                iconView->onClick = [=](Button_Highlight *sender){
                    
                    for(auto item : _listView->getItems()){
                        auto cell = static_cast<Cell_CardBack *>(item);
                        for(auto btn : cell->getIconViews()){
                            btn->setSelected(false);
                        }
                    }
                    auto current = static_cast<Cell_CardBack *>(sender->getParent()->getParent());
                    sender->setSelected(true);
                    auto x = current->getIconViews().getIndex(sender);
                    auto y = _listView->getIndex(current);
                    _idx = int(y * 4 + x);
                    
                    if(_idx == 2 && !DataManager::getInstance()->getFirstShared()){
                        auto la = KLShareInfoLayer::create();
                        this->addChild(la);
                    }
                    if(_idx == 0){
                        iOSWrapper::showImagePicker();
                    }
                };
            }
        }
    }
    int x = _idx%4;
    int y = _idx/4;
    auto cell = static_cast<Cell_CardBack *>( _listView->getItem(y) );
    if (cell) {
        cell->getIconViews().at(x)->setSelected(true);
    }
    
}
void Layer_CardBack::onConfirm(){
    auto mgr = DataManager::getInstance();
    mgr->changeCardback(mgr->getCardbacks().at(_idx)->getName());
}

#pragma mark - Layer_Faces -

bool Layer_Faces::init(){
    if (!Layer_Select::init()) {
        return false;
    }

    getNavigationBar()->setTitle(LocalizedString("TID_UI_CHANGE CARD FACES"));
    
    return true;
}

Layout *Layer_Faces::cellForDisplay(){
    return Cell_Faces::create();
}
void Layer_Faces::reloadData(){
    auto mgr = DataManager::getInstance();
    auto dataArray = mgr->getFaces();
    auto item = mgr->getFace(mgr->getSelectFaceName());
    
    
    for (int i = 0; i < dataArray.size(); i++) {
        _listView->pushBackDefaultItem();
    }
    
    for (int i = 0; i < dataArray.size(); i++) {
        auto cell = static_cast<Cell_Faces *>( _listView->getItem(i) );
        auto item = dataArray.at(i);
        cell->getIconView()->getIcon()->loadTexture(item->getPreviewFile());
        cell->getIconView()->onClick = [=](Button_Highlight *sender){
        
            for(auto item : _listView->getItems()){
                auto cell = static_cast<Cell_Faces *>(item);
                cell->getIconView()->setSelected(false);
            }
            auto current = static_cast<Cell_Faces *>(sender->getParent());
            sender->setSelected(true);
            _idx = (int)_listView->getIndex(current);
        };
    }
    
    _idx = (int)dataArray.getIndex(item);;
    auto cell = static_cast<Cell_Faces *>( _listView->getItem(_idx) );
    cell->getIconView()->setSelected(true);
}
void Layer_Faces::onConfirm(){
    auto mgr = DataManager::getInstance();
    auto dataArray = mgr->getFaces();
    HLAnalsytWrapper::event("SetFaces");
    DataManager::getInstance()->setCustomSelectFaceName(dataArray.at(_idx)->getName());
    __NotificationCenter::getInstance()->postNotification(NSGlobal:: NTFFactor::NTF_SETTING_REFRESH);
}

 */

#pragma mark - Layer_Setting -



void Layer_Setting::updateLocalizedUI(){
    this->loadData();
    
    _titleLabel->setString(LocalizedString("TID_UI_SETTINGS"));
    _sharedBtn->setTitleText(LocalizedString("TID_UI_SHARE1"));
}

void Layer_Setting::onEnter(){

    KTPauseLayer::onEnter();
    
    loadData();
}

void Layer_Setting::loadData(){
    
    dataArray.clear();
    _listView->removeAllItems();
    
    auto dataMgr = DataManager::getInstance();
    
    char str[64];
    {
        ValueMap map;
        map["name"] = Value(NAME_SOUND);
        map["title"] = Value(LocalizedString("TID_UI_SOUND"));
        map["button"] = Value("sw");
        map["value"] = Value((int)dataMgr->isSoundOn());
        dataArray.push_back(Value(map));
    }
    {
        ValueMap map;
        map["name"] = Value(NAME_DRAW3);
        map["title"] = Value(LocalizedString("TID_UI_DRAW3"));
        map["button"] = Value("sw");
        map["value"] = Value((int)dataMgr->isChow3Card());
        dataArray.push_back(Value(map));
    }
    
    {
        ValueMap map;
        map["name"] = Value(NAME_VEGAS_MODE);
        map["title"] = Value(LocalizedString("TID_UI_VEGAS MODE"));
        map["button"] = Value("sw");
        map["value"] = Value((int)DataManager::getInstance()->isVegasOn());
        dataArray.push_back(Value(map));
    }
    
    {
        ValueMap map;
        map["name"] = Value(NAME_VEGAS_ADD);
        map["title"] = Value(LocalizedString("TID_UI_VEGAS CUMULATIVE"));
        map["button"] = Value("sw");
        map["value"] = Value((int)DataManager::getInstance()->isVegasScoreAdd());
        dataArray.push_back(Value(map));
    }
    
    {
        ValueMap map;
        map["name"] = Value(NAME_TIMER);
        map["title"] = Value(LocalizedString("TID_UI_TIMER MODE"));
        map["button"] = Value("sw");
        map["value"] = Value((int)dataMgr->isShowMatchInfo());
        dataArray.push_back(Value(map));
    }
    {
        ValueMap map;
        map["name"] = Value(NAME_LEFT_HAND);
        map["title"] = Value(LocalizedString("TID_UI_LEFT HANDED"));
        map["button"] = Value("sw");
        map["value"] = Value((int)dataMgr->isUseRightHand());
        dataArray.push_back(Value(map));
    }
//    {
//        ValueMap map;
//        map["title"] = Value(LocalizedString("TID_UI_CARD FACES"));
//        map["button"] = Value("img");
//        map["value"] = Value(0);
////        sprintf(str, "shared/cardstyle_%d.png", DataManager::getInstance()->getFaceID());
//        map["img"] = Value(dataMgr->getFace(dataMgr->getSelectFaceName())->getPreviewFile().c_str());
//        dataArray.push_back(Value(map));
//    }
//    {
//        ValueMap map;
//        map["title"] = Value(LocalizedString("TID_UI_CARD BACKS"));
//        map["button"] = Value("img");
//        map["value"] = Value(0);
//        map["img"] = Value(DataManager::getInstance()->getCardbackFile());
//        string cardback = DataManager::getInstance()->getCardbackFile();
//        CCLOG("cardback = %s", cardback.c_str());
//        dataArray.push_back(Value(map));
//    }
//    {
//        ValueMap map;
//        map["title"] = Value(LocalizedString("TID_UI_BACKGROUND"));
//        map["button"] = Value("img");
//        map["value"] = Value(0);
//        map["img"] = Value(DataManager::getInstance()->getPreviewThemeFile());
//        dataArray.push_back(Value(map));
//    }
    {
        ValueMap map;
        map["name"] = Value(NAME_LANGUAGE);
        map["title"] = Value(LocalizedString("TID_UI_LANGUAGE"));
        map["button"] = Value("img");
        map["value"] = Value(0);
        sprintf(str, "shared/flags/%s.png", Localization::getLanguage().c_str());
        map["img"] = Value(str);
        dataArray.push_back(Value(map));
    }
    
    {
        ValueMap map;
        map["name"] = Value(NAME_AUTO_HINT);
        map["title"] = Value(LocalizedString("TID_UI_AUTO HINTS"));
        map["button"] = Value("sw");
        map["value"] = Value(dataMgr->getAutoTip());
        dataArray.push_back(Value(map));
    }
    
    //TID_UI_RULESRULES
    {
        ValueMap map;
        map["name"] = Value(NAME_HOW_TO_PLAY);
        map["title"] = Value(LocalizedString("TID_UI_HOW TO PLAY"));
        map["button"] = Value("none");
        map["value"] = Value(0);
        dataArray.push_back(Value(map));
    }
    
    for (int i = 0; i < dataArray.size(); i++) {
        _listView->pushBackDefaultItem();
    }
    
    auto onSwitchFunc = [=](Cell_Setting *cell, bool showTip){
        
        auto instance = DataManager::getInstance();
        int idx = (int)_listView->getIndex(cell);
        auto dic = dataArray[idx].asValueMap();
        dic["value"] = Value((int)cell->isSwitchSelected());
        bool v = cell->isSwitchSelected();
        
        string name = dic["name"].asString();
        if (name == NAME_SOUND) {
            dataMgr->setSoundOn(v);
            
            if(!v){
                Audio::getInstance()->pauseEffects();
                Audio::getInstance()->pauseMusic();
            }else{
                Audio::getInstance()->resumeEffects();
                Audio::getInstance()->resumeMusic();
            }
            
        } else if (name == NAME_DRAW3) {
            dataMgr->setChow3Card(v);
        } else if (name == NAME_VEGAS_MODE) {
            instance->setVegasOn(v);
            
            auto sub = static_cast<Cell_Setting *>(_listView->getItem(idx+1));
            sub->setForceSwitchOff(!v);
            
            
            bool flag2 = getGameScene()->getIsVegasOn();
            if (flag2 != v) {
                if (flag2) {
                    
                    
                    if (showTip) {
                        vector<string> btns;
                        btns.push_back(LocalizedString("TID_CODE_NEW GAME"));
                        KLAlertLayer::show("", LocalizedString("TID_CODE_CLASSIC MODE CHANGE"), LocalizedString("TID_CODE_BACK"), btns, [=](KLAlertLayer *layer, int buttonIdx){
                            if(buttonIdx == 1) {
                                __NotificationCenter::getInstance()->postNotification(NTFFactor::NTF_NEW_GAME);
                                this->removeFromParent();
                            }
                        });
                    }
                    
                } else {
                    
                    if (showTip) {
                        vector<string> btns;
                        btns.push_back(LocalizedString("TID_CODE_NEW GAME"));
                        KLAlertLayer::show("", LocalizedString("TID_CODE_VEGAS MODE CHANGE"), LocalizedString("TID_CODE_BACK"), btns, [=](KLAlertLayer *layer, int buttonIdx){
                            if(buttonIdx == 1) {
                                __NotificationCenter::getInstance()->postNotification(NTFFactor::NTF_NEW_GAME);
                                this->removeFromParent();
                            }
                        });
                    }
                }
            }
        } else if (name == NAME_VEGAS_ADD) {
            instance->setVegasScoreAdd(v);
        } else if (name == NAME_TIMER) {
            instance->setShowMatchInfo(v);
        } else if (name == NAME_LEFT_HAND) {
            instance->setUseRightHand(v);
        } else if (name == NAME_AUTO_HINT) {
            instance->setAutoTip(v);
        }
        
    };
    
    for (int i = 0; i < dataArray.size(); i++) {
        ValueMap dic = dataArray.at(i).asValueMap();
        Cell_Setting *cell = static_cast<Cell_Setting *>( _listView->getItem(i) );
        cell->getTitleLabel()->setString(dic["title"].asString());
        string btn = dic["button"].asString();
        if (btn == "sw") {
            cell->setSwitchVisible(true);
            cell->getArrow()->setVisible(false);
            bool value = dic["value"].asInt() != 0;
            cell->setSwitchSelected(value);
            onSwitchFunc(cell, false);
            //            if (value) {
            //                cell->getSwitch()->getRendererBackground()->setVisible(false);
            //            }
            
        } else if (btn == "img") {
            string name = dic["img"].asString();
            cell->getArrow()->loadTexture(name);
            float sc = cell->getContentSize().height / cell->getArrow()->getContentSize().height;
            cell->getArrow()->setScale(sc);
            cell->setSwitchVisible(false);
            cell->getArrow()->setFlippedX(false);
//            if (i == 6) {
//                cell->getArrow()->setPositionX(cell->getArrow()->getPositionX() - 70);
//            }
            
        } else if (btn == "none") {
            
            cell->setSwitchVisible(false);
        }
        cell->getBg()->setVisible(i % 2);
        cell->setOnSwitch([=](Cell_Setting *cell){
            onSwitchFunc(cell, true);
        });
        bool flag = false;
//        if (i == 7 && UserDefault::getInstance()->getBoolForKey("KL_CUSTOM_CARDBACK_CLICK") == false) {
//            flag = true;
//        } else if (i == 8 && UserDefault::getInstance()->getBoolForKey("KL_CUSTOM_THEME_CLICK") == false) {
//            flag = true;
//        }
        cell->setBadgeEnabled(flag);
    }
}

void Layer_Setting::updateLayout(){
    
    _bg->setContentSize(winSize);
    
    if (winSize.height > winSize.width) {
        //portrait
        _panel->setContentSize(Size(640, 900));
        _panel->setPosition(VisibleRect::center() + Vec2(0, 0));
        _titleLabel->setPosition(VisibleRect::center() + Vec2(0, 390));
        _closeBtn->setPosition(VisibleRect::center() + Vec2(250, 390));
        
        Size size = Size(576, 750 - 90);
        _listView->setContentSize(size);
        _listView->setPosition(Vec2((VisibleRect::getVisibleRect().size - size) * 0.5f) + Vec2(0, -30 + 45));
        
        _sharedBtn->setPosition(Vec2(winSize.width/2, _listView->getPositionY() - 45));

        auto items = _listView->getItems();
        
    } else {
        _panel->setContentSize(Size(640, 640));
        _panel->setPosition(VisibleRect::center() + Vec2(0, 0));
        _titleLabel->setPosition(VisibleRect::center() + Vec2(0, 268));
        _closeBtn->setPosition(VisibleRect::center() + Vec2(250, 268));
        
        Size size = Size(576, 490 - 90);
        _listView->setContentSize(size);
        _listView->setPosition(Vec2((VisibleRect::getVisibleRect().size - size) * 0.5f) + Vec2(0, -30 + 45));
        
        _sharedBtn->setPosition(Vec2(winSize.width/2, _listView->getPositionY() - 45));

    }
}

void Layer_Setting::updateBadge(Ref *sender){
    loadData();
}

bool Layer_Setting::init(){
    if (!KTPauseLayer::init()) {
        return false;
    }

    auto bg = LayerColor::create(Color4B(0, 0, 0, 175));
    addChild(bg, -2);
    _bg = bg;
    
    auto panel = ImageView::create("9_greenwindow.png");
    panel->setTag(123);
    panel->setScale9Enabled(true);
    panel->setContentSize(Size(640, 900));
    panel->setPosition(VisibleRect::center() + Vec2(0, 0));
    addChild(panel, -1);
    _panel = panel;
    
    {
        
        auto *titleLbl =Text::create(LocalizedString("TID_UI_SETTINGS"), "Arial", 36);
        titleLbl->setTextColor(Color4B::WHITE);
        titleLbl->setTextHorizontalAlignment(TextHAlignment::CENTER);
        titleLbl->setTextVerticalAlignment(TextVAlignment::CENTER);
        titleLbl->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        titleLbl->setPosition(VisibleRect::center() + Vec2(0, 390));
        addChild(titleLbl);
        _titleLabel = titleLbl;
    }
    
    auto btn = KTFactory::createButton("close.png", "close1.png", "",  "", 30);
    //        btn->setPosition(VisibleRect::center() + Vec2(0, -132));
    
    //        btn->setContentSize(Size(280, 75));
    btn->setPosition(VisibleRect::center() + Vec2(250, 390));
    btn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            
            
            __NotificationCenter::getInstance()->postNotification(NSGlobal:: NTFFactor::NTF_SETTING_REFRESH);
            this->removeFromParentAndCleanup(true);
        }
    });
    addChild(btn);
    _closeBtn = btn;
    
    Size size = Size(576, 750 - 90);
    
    ListView* listView = ListView::create();
    // set list view ex direction
    listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(true);
    listView->setScrollBarEnabled(false);
    
    listView->setContentSize(size);
    listView->setPosition(Vec2((VisibleRect::getVisibleRect().size - size) * 0.5f) + Vec2(0, -30 + 45));

    this->addChild(listView);

    listView->setItemModel(Cell_Setting::create());
    listView->addEventListener([=](Ref*, ListView::EventType type){
    
        if (type == ListView::EventType::ON_SELECTED_ITEM_END) {
            auto idx = listView->getCurSelectedIndex();
            string name = dataArray[idx].asValueMap()["name"].asString();
//            if (idx == 6) {
//                auto la = Layer_Faces::create();
//                auto sc = Scene::create();
//                sc->addChild(la);
//                Director::getInstance()->pushScene(sc);
//            } else if (idx == 7) {
//                auto la = Layer_CardBack::create();
//                auto sc = Scene::create();
//                sc->addChild(la);
//                Director::getInstance()->pushScene(sc);
//            } else if (idx == 8) {
//                auto la = Layer_Themes::create();
//                auto sc = Scene::create();
//                sc->addChild(la);
//                Director::getInstance()->pushScene(sc);
//            } else
            if (name == NAME_LANGUAGE){
                //localization
                auto la = Layer_Language::create();
                this->addChild(la);
                
            } else if (name == NAME_HOW_TO_PLAY) {
                auto la = Layer_HowToPlay::create();
                auto sc = Scene::create();
                sc->addChild(la);
                Director::getInstance()->pushScene(sc);
            }
        }
    });
    _listView = listView;
    
    
    auto sharedBtn = KTFactory::createButton("B_green1.png", "B_green2.png", "", LocalizedString("TID_UI_SHARE1"), 30, "Arial", Size(272, 72));
    sharedBtn->setPosition(Vec2(winSize.width/2, _listView->getPositionY() - 45));
    addChild(sharedBtn);
    sharedBtn->addClickEventListener([=](Ref *){
        iOSWrapper::showShareUI();
    });
    _sharedBtn = sharedBtn;
    
    
//    bool flag = UserDefault::getInstance()->getBoolForKey("KL_CUSTOM_BACKCARD_SHOWED");
//    if (!flag) {
//        auto la = KLIntroLayer::create(KLIntroLayer::IntroType::CustomCardbackTheme);
//        addChild(la, 2048);
//        UserDefault::getInstance()->setBoolForKey("KL_CUSTOM_BACKCARD_SHOWED", true);
//    }
    
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(Layer_Setting::updateBadge), kUpdateBadgeCardback, nullptr);
    
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(Layer_Setting::updateBadge), kUpdateBadgeTheme, nullptr);

    
    return true;
}

Layer_Setting::~Layer_Setting(){

    __NotificationCenter::getInstance()->removeObserver(this, kUpdateBadgeCardback);
    __NotificationCenter::getInstance()->removeObserver(this, kUpdateBadgeTheme);
}
