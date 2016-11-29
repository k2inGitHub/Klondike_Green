//
//  Cell_Statistics.cpp
//  Klondike
//
//  Created by 宋扬 on 16/7/21.
//
//

#include "Cell_Statistics.hpp"

void Cell_Statistics::updateLayout(){
    auto winSize = Director::getInstance()->getWinSize();
    _timeLabel->setPosition(Vec2(winSize.width - 60, 60));
    
    _line->setContentSize(Size(winSize.width, _line->getContentSize().height));
    _line->setPosition(_line->getContentSize()/2);
}

Widget* Cell_Statistics::createCloneInstance()
{
    return Cell_Statistics::create();
}

bool Cell_Statistics::init(){
    
    if (!Cell_Base::init()) {
        return false;
    }
    
    auto size = VisibleRect::getVisibleRect().size;
    
    auto cell = this;
    {
//        cell->setTouchEnabled(true);
        cell->setContentSize(Size(size.width, 120));
        
        auto titleLabel = Text::create("", "Arial", 24);
        
//        titleLabel->setAlignment(TextHAlignment::LEFT);
        titleLabel->setContentSize(Size(180, 45));
        titleLabel->setTextHorizontalAlignment(TextHAlignment::LEFT);
        titleLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        titleLabel->setPosition(Vec2(60, 60));
        cell->addChild(titleLabel);
        _titleLabel = titleLabel;
        
        
        auto timeLabel =Text::create("", "Arial", 24);
        timeLabel->setContentSize(Size(180, 45));
//        titleLabel->setAlignment(TextHAlignment::RIGHT);
        timeLabel->setTextHorizontalAlignment(TextHAlignment::RIGHT);
        timeLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        timeLabel->setPosition(Vec2(580, 60));
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
