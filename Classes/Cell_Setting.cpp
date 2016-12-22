//
//  Cell_Setting.cpp
//  Klondike
//
//  Created by 宋扬 on 16/7/25.
//
//

#include "Cell_Setting.hpp"
#include "KTLayer.h"

#pragma mark - Cell_Base -

void Cell_Base::onEnter(){
    Layout::onEnter();
    onScreenSizeChanged(nullptr);
}

Widget* Cell_Base::createCloneInstance()
{
    return Cell_Base::create();
}

void Cell_Base::copyClonedWidgetChildren(Widget* model)
{
}

bool Cell_Base::init(){
    if (!Layout::init()) {
        return false;
    }
    
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(Cell_Base::onScreenSizeChanged), kScreenSizeChangedNotification, nullptr);
    
    
    return true;
}

void Cell_Base::onScreenSizeChanged(Ref *sender){
    updateLayout();
}

void Cell_Base::updateLayout(){
    
}

Cell_Base::~Cell_Base(){
    __NotificationCenter::getInstance()->removeObserver(this, kScreenSizeChangedNotification);
}

#pragma mark - Cell_CustomThemeSubView

Widget* Cell_CustomThemeSubView::createCloneInstance()
{
    return Cell_CustomThemeSubView::create();
}

void Cell_CustomThemeSubView::updateLayout()
{
}


bool Cell_CustomThemeSubView::init(){
    if (!Cell_Base::init()) {
        return false;
    }
    setTouchEnabled(true);
    auto winSize = Director::getInstance()->getWinSize();
    auto cellSize = Size(88 + 36, 131 + 36);
    setContentSize(cellSize);
    
    _iconView = Button_Highlight::create(cellSize);
    this->addChild(_iconView);
    _iconView->setBackgroundOutlineWidth(4);
    _iconView->setPosition(Vec2(0,0));
    return true;
}

#pragma mark - Cell_CustomTheme -

Widget* Cell_CustomTheme::createCloneInstance()
{
    return Cell_CustomTheme::create();
}

void Cell_CustomTheme::updateLayout()
{
    float scBarWidth = 15;
    auto winSize = Director::getInstance()->getVisibleSize();
    auto cellSize = Size(winSize.width, 230);
    setContentSize(cellSize);
    
    _layer->setContentSize(Size(winSize.width-30-scBarWidth, 181));
    _listView->setContentSize(Size(winSize.width - 92-scBarWidth, 131 + 50));
    _rightArrow->setPosition(Vec2(winSize.width - 28-scBarWidth, 90));
    refreshBtns();
}

void Cell_CustomTheme::reloadData(Vector<PreviewItem *> dataArray){
    _listView->removeAllItems();
    for(int i = 0; i < dataArray.size(); i++){
        _listView->pushBackDefaultItem();
    }
    for(int i = 0;i < dataArray.size(); i++){
        
        auto cell = static_cast<Cell_CustomThemeSubView *>(_listView->getItem(i));
        CCLOG("cell exit = %d", _listView->getItem(i) != nullptr);
        auto item = dataArray.at(i);
        cell->getIconView()->getIcon()->loadTexture(item->getPreviewFile());
    }
    refreshBtns();
}

void Cell_CustomTheme::refreshBtns(){
    //立即布局 计算宽高 刷新arrow
    _listView->doLayout();
    if(_listView->getInnerContainerSize().width <= _listView->getContentSize().width) {
        _leftArrow->setEnabled(false);
        _rightArrow->setEnabled(false);
    } else {
        _leftArrow->setEnabled(false);
        _rightArrow->setEnabled(true);
    }
}

bool Cell_CustomTheme::init(){
    
    if (!Cell_Base::init()) {
        return false;
    }
    
    
    auto winSize = Director::getInstance()->getWinSize();
    
    auto cellSize = Size(winSize.width, 230);
    setContentSize(cellSize);
    
//    auto s = Size(185 + 20, 16);
    
//    auto box = HBox::create(Size(s.width * 3, s.height));
//    box->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    box->setPosition((cellSize)/2);
//    addChild(box);
//    _box = box;
    
    auto label = Label::createWithSystemFont(LocalizedString("NONE"), "Arial", 30);
    label->setOverflow(Label::Overflow::NONE);
    auto labelSize = Size(400, 150);
    label->setContentSize(labelSize);
    label->setDimensions(labelSize.width, labelSize.height);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    label->setPosition(Vec2(30, 205));
    addChild(label);
    _titleLabel = label;
    
    //75
    
    float scBarWidth = 15;
    
    auto layer = ImageView::create("9_white_s.png");
    layer->setScale9Enabled(true);
    layer->setContentSize(Size(winSize.width-30-scBarWidth, 181));
    layer->setColor(Color3B::BLACK);
    layer->setOpacity(64);
    layer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    //LayerColor::create(Color4B(0, 0, 0, 127), 160, 40);
    layer->setPosition(Vec2(15, 0));
    addChild(layer);
    _layer = layer;
    
    ListView* listView = ListView::create();
    // set list view ex direction
    listView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
    listView->setBounceEnabled(true);
    listView->setScrollBarEnabled(false);
    
    
    listView->setContentSize(Size(winSize.width - 92 - scBarWidth, 131 + 50));
    listView->setPosition(Vec2(46, 0));
    
    this->addChild(listView);
    
    listView->setItemModel(Cell_CustomThemeSubView::create());
//    listView->addEventListener([=](Ref*, ListView::EventType type){
//        
//        if (type == ListView::EventType::ON_SELECTED_ITEM_END) {
//            //            int idx = listView->getCurSelectedIndex();
//            //            CCLOG("select = %d", idx);
//        }
//    });
//    listView->addTouchEventListener([=](Ref*,Widget::TouchEventType type){
//        
//    });
    listView->addEventListener([=](Ref*, ScrollView::EventType type){
        
        if(type != ScrollView::EventType::CONTAINER_MOVED) return;
        auto left = _listView->getLeftmostItemInCurrentView();
        auto right = _listView->getRightmostItemInCurrentView();
        auto leftIdx = _listView->getIndex(left);
        auto rightIdx = _listView->getIndex(right);
        
        if (leftIdx == 0) {
            //            CCLOG("BOUNCE_LEFT");
            _leftArrow->setEnabled(false);
        } else {
            _leftArrow->setEnabled(true);
        }
        
        if (rightIdx == _listView->getItems().size()-1) {
            //            CCLOG("BOUNCE_RIGHT");
            _rightArrow->setEnabled(false);
        } else {
            _rightArrow->setEnabled(true);
        }
        
    });
    _listView = listView;
    
    {
        Button *arrow = KTFactory::createButton("B_left.png", "B_left.png", "null.png", "", 0);
        arrow->setPosition(Vec2(28, 90));
        arrow->setScale(0.5f);
        addChild(arrow);
        _leftArrow = arrow;
    }
    
    {
        Button *arrow = KTFactory::createButton("B_left.png", "B_left.png", "null.png", "", 0);
        arrow->setFlippedX(true);
        arrow->setPosition(Vec2(winSize.width - 28 - scBarWidth, 90));
        arrow->setScale(0.5f);
        addChild(arrow);
        _rightArrow = arrow;
    }
    
    return true;
}

#pragma mark - Cell_ThemeSet -

Widget* Cell_ThemeSet::createCloneInstance()
{
    return Cell_ThemeSet::create();
}

void Cell_ThemeSet::updateLayout()
{
    auto winSize = Director::getInstance()->getVisibleSize();
    auto cellSize = Size(winSize.width, 230);
    setContentSize(cellSize);
    _box->setPosition((cellSize)/2);
}


bool Cell_ThemeSet::init(){
    
    if (!Cell_Base::init()) {
        return false;
    }
    
    auto cellSize = Size(640, 230);
    setContentSize(cellSize);
    
    auto s = Size(185 + 20, 201 + 29);
    
    auto box = HBox::create(Size(s.width * 3, s.height));
    box->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    box->setPosition((cellSize)/2);
    addChild(box);
    _box = box;
    
    for (int i = 0; i < 3; i++) {
        auto _iconView = Button_Highlight::create(s);
        box->addChild(_iconView);
        _iconView->setBackgroundOutlineWidth(0);
        _iconViews.pushBack(_iconView);
    }
    
    return true;
}

#pragma mark - Cell_CardBack -

Widget* Cell_CardBack::createCloneInstance()
{
    return Cell_CardBack::create();
}

void Cell_CardBack::updateLayout()
{
    auto winSize = Director::getInstance()->getVisibleSize();
    auto cellSize = Size(winSize.width, 160);
    setContentSize(cellSize);
    _box->setPosition((cellSize)/2);
}


bool Cell_CardBack::init(){
    
    if (!Cell_Base::init()) {
        return false;
    }
    
    auto cellSize = Size(640, 160);
    setContentSize(cellSize);
    
    auto s = Size(88 + 36, 131 + 36);
    
    auto box = HBox::create(Size(s.width * 4, s.height));
    box->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    box->setPosition((cellSize)/2);
    addChild(box);
    _box = box;
    
    for (int i = 0; i < 4; i++) {
        auto _iconView = Button_Highlight::create(s);
        box->addChild(_iconView);
        _iconView->setBackgroundOutlineWidth(4);
        _iconViews.pushBack(_iconView);
    }
    
    return true;
}

#pragma mark - Cell_Faces -

Widget* Cell_Faces::createCloneInstance()
{
    return Cell_Faces::create();
}

void Cell_Faces::updateLayout()
{
    auto winSize = Director::getInstance()->getVisibleSize();
    auto cellSize = Size(winSize.width, 160);
    setContentSize(cellSize);
    auto s = Size(476, 168);
    _iconView->setPosition((cellSize - s)/2);
}

bool Cell_Faces::init(){
    
    if (!Cell_Base::init()) {
        return false;
    }
    
    auto cellSize = Size(640, 160);
    setContentSize(cellSize);
    
    auto s = Size(476, 168);
    _iconView = Button_Highlight::create(s);
    _iconView->setPosition((cellSize - s)/2);
    addChild(_iconView);
    
    
    return true;
}

#pragma mark - Cell_Setting -

Cell_Setting::Cell_Setting():
_badge(nullptr){

}

void Cell_Setting::setBadgeEnabled(bool value){

    if(value){
        if (_badge == nullptr) {
            _badge = Sprite::create("redpoint.png");
            _badge->setPosition(Vec2(40, 35));
            this->addChild(_badge);
        }
        _badge->setVisible(true);
    } else {
        if(_badge != nullptr){
            _badge->setVisible(false);
        }
    }
}

void Cell_Setting::setForceSwitchOff(bool value){
    _forceSwitchOff = value;
    if (_forceSwitchOff) {
        _lock->setVisible(true);
        _sw->setEnabled(false);
        _sw->getRendererBackgroundDisabled()->setColor(Color3B::GRAY);
        _sw->getRendererFrontCrossDisabled()->setColor(Color3B::GRAY);
    } else {
        _lock->setVisible(false);
        _sw->setEnabled(true);
        _sw->getRendererBackgroundDisabled()->setColor(Color3B::WHITE);
        _sw->getRendererFrontCrossDisabled()->setColor(Color3B::WHITE);
    }
}

void Cell_Setting::setSwitchSelected(bool value){
    if (!_forceSwitchOff) {
        _sw->setSelected(value);
    }
}
bool Cell_Setting::isSwitchSelected(){
    return _sw->isSelected();
}

Widget* Cell_Setting::createCloneInstance()
{
    return Cell_Setting::create();
}

void Cell_Setting::updateLayout()
{
    
}

bool Cell_Setting::init()
{
    if (!Cell_Base::init()) {
        return false;
    }
    Size size = Size(576, 70);
    setContentSize(size);
    {
        setTouchEnabled(true);
        _bg = ImageView::create("9_line_white.png");
        _bg->setContentSize(size);
        _bg->setScale9Enabled(true);
        _bg->setColor(Color3B::BLACK);
        _bg->setOpacity(100);
        _bg->setPosition(_bg->getContentSize()/2);
        addChild(_bg);
        
        auto titleLabel = Text::create("", "Arial", 24);
        
        //        titleLabel->setAlignment(TextHAlignment::LEFT);
        titleLabel->setContentSize(Size(180, 45));
        titleLabel->setTextHorizontalAlignment(TextHAlignment::LEFT);
        titleLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        titleLabel->setPosition(Vec2(50, 35));
        this->addChild(titleLabel);
        _titleLabel = titleLabel;
        
        
        _sw = CheckBox::create("B_open0.png", "B_open0.png","B_open.png","B_open0.png","B_open0.png");
        _sw->setPosition(Vec2(500, 35));
        _sw->addEventListener([=](Ref*,CheckBox::EventType type){
        
            if (_onSwitch) {
                _onSwitch(this);
            }
        });
        this->addChild(_sw);
        
        _arrow = ImageView::create("B_arrow.png");
        _arrow->setPosition(Vec2(500, 35));
        _arrow->setFlippedX(true);
        addChild(_arrow);
        
        _lock = ImageView::create("lock.png");
        _lock->setPosition(Vec2(420, 35));
        addChild(_lock);
        _lock->setVisible(false);
    }
    
    
    return true;
}
