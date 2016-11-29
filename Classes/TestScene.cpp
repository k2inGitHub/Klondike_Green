//
//  TestScene.cpp
//  Klondike
//
//  Created by 宋扬 on 16/7/20.
//
//

#include "TestScene.hpp"
#include "KTFactory.h"
#include "Button_Daily.hpp"

bool TestLayer::init(){

    if (!KTLayer::init()) {
        return false;
    }
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Untitled.plist");
//    auto node = SpriteBatchNode::create("Untitled.png");
//    addChild(node);
    
    for (int i = 0; i < 4; i++) {
        auto lbl = Button_Daily::create();
        lbl->setDay(1);
        lbl->setGlobalZOrder(1);
//        lbl->setUnlock(true);
        //Button::create("B_green_st.png", "","",Widget::TextureResType::PLIST);
//        lbl->setTitleFontName("");
        
        //Sprite::createWithSpriteFrameName("B_green_st.png");
        
        TextAtlas::create("123", "labelatlas_st.png", 17, 22, "0");
        
        //Label::createWithBMFont("fonts/bitmapFontTest4.fnt", "string");
        //gl call = 6
        //LabelTTF::create("Alignment", "Helvetica", 12, Size(256, 32), TextHAlignment::LEFT);
        //gl call = 3
        //LabelAtlas::create("123 Test", "fonts/tuffy_bold_italic-charmap.png", 48, 64, ' ');
        //gl call = 6
        //Label::createWithSystemFont("string", "Arial", 18);
        
//        lbl->initWithString("123 Test", "tuffy_bold_italic-charmap.png");
        this->addChild(lbl);
        _labels.pushBack(lbl);
    }
    
    horizonAliament(_labels, 20, VisibleRect::center().y);
    
    auto btn = KTFactory::createButton("B_orange.png", "B_orange1.png", "", "test", 20);
    btn->setPosition(VisibleRect::bottom() + Vec2(0, 90));
    btn->addTouchEventListener([=](Ref*,Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            show = !show;
            for (auto n : _labels) {
                n->setVisible(show);
            }
        }
    });
    addChild(btn);
    return true;
}