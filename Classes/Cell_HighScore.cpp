//
//  Cell_HighScore.cpp
//  Klondike
//
//  Created by 宋扬 on 16/7/21.
//
//

#include "Cell_HighScore.hpp"


Widget* Cell_HighScore::createCloneInstance()
{
    return Cell_HighScore::create();
}

void Cell_HighScore::updateLayout()
{
    auto winSize = Director::getInstance()->getVisibleSize();
    
    _rankButton->setPosition(Vec2(winSize.width/2 - 200, 46));
    _titleLabel->setPosition(Vec2(winSize.width/2 - 120, 46));
    _timeLabel->setPosition(Vec2(winSize.width/2 + 120, 46));
    _line->setPosition(Vec2(winSize.width/2, _line->getContentSize().height/2));
}

bool Cell_HighScore::init(){

    if (!Cell_Base::init()) {
        return false;
    }
    
    auto size = VisibleRect::getVisibleRect().size;
    
    auto cell = this;
    {
        cell->setTouchEnabled(true);
        cell->setContentSize(Size(size.width, 120));
        auto btn = KTFactory::createButton("", "", "", "1", 40, "Arial-BoldMT");
        btn->setContentSize(Size(60, 60));
        btn->setPosition(Vec2(120, 46));
        cell->addChild(btn);
        _rankButton = btn;
        
        auto titleLabel = Label::createWithSystemFont("1220", "Arial-BoldMT", 30);
        titleLabel->setPosition(Vec2(200, 46));
        cell->addChild(titleLabel);
        _titleLabel = titleLabel;
        
        
        auto timeLabel = Label::createWithSystemFont("2016-12-20 08:00", "Arial", 24);
        timeLabel->setPosition(Vec2(440, 46));
        _timeLabel = timeLabel;
        addChild(timeLabel);
    }
    
    
    auto line = Scale9Sprite::create("9_line_white.png");
    line->setScale9Enabled(true);
    line->setContentSize(Size(size.width, line->getContentSize().height));
    line->setOpacity(100);
    line->setPosition(line->getContentSize()/2);
    
    addChild(line);
    _line = line;
    
    return true;
}
