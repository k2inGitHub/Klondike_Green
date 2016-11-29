#include "BlankCard.h"
#include "Global.h"
#include "KLColor.hpp"

USING_NS_CC;
using namespace NSGlobal;

// 空白文件
const char* NAME_TEXTURE_BLANK = "shared/Blank.png";

const char* NAME_TEXTURE_BLANK_ACE = "shared/Blank_Ace.png";

const char* NAME_TEXTURE_BLANK_STOCK = "shared/Blank_Refresh.png";

// 尺寸
const int SIZE_TEXTURE_X = 98;
const int SIZE_TEXTURE_Y = 141;

// 卡片尺寸
//const float SIZE_SPRITE_X = 0.5f; //
//const float SIZE_SPRITE_Y = 0.5f;

//减去外发光的5像素
cocos2d::Rect BlankCard::getBoundingBox() const{

    Rect box = Sprite::getBoundingBox();
    box.origin += Vec2(5, 5);
    box.size = Size(MAX(0.f, box.size.width - 10.f), MAX(0.f, box.size.height - 10.f));
    return box;
}

Sprite *BlankCard::getIcon(){
    if (!_icon) {
        _icon = Sprite::create("Classic_outofflip.png");
        _icon->setPosition(Vec2(SIZE_TEXTURE_X * 0.5f, SIZE_TEXTURE_Y * 0.5f));
        _icon->setColor(Color3B(KL_YELLOW));
        addChild(_icon);
        
        _label1 = Label::createWithSystemFont("OUT OF\n\n\n\nFLIPS", "Arial", 20);
        _label1->setPosition(_icon->getPosition());
        _label1->setColor(Color3B(KL_YELLOW));
        _label1->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        addChild(_label1);
    }
    return _icon;
}

Sprite *BlankCard::getIcon2(){
    
    if (!_icon2) {
        _icon2 = Sprite::create("Classic_moreflip.png");
        _icon2->setPosition(Vec2(SIZE_TEXTURE_X * 0.5f, SIZE_TEXTURE_Y * 0.5f));
        _icon2->setColor(Color3B::WHITE);
        addChild(_icon2);
        
        _label2 = Label::createWithSystemFont("2 MORE\n\n\n\nFLIPS", "Arial", 20);
        _label2->setPosition(_icon2->getPosition());
        _label2->setColor(Color3B::WHITE);
        _label2->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        addChild(_label2);
    }
    return _icon2;
}

void BlankCard::setType(int type){
//    if (_type == type) {
//        return;
//    }
    
    Rect rect = Rect(0, 0, float(SIZE_TEXTURE_X), float(SIZE_TEXTURE_Y));
    _type = type;
    std::string str;
    if (type == 0) {
        str = NAME_TEXTURE_BLANK;
    } else if (type == 1) {
        str = NAME_TEXTURE_BLANK_ACE;
    }
    else if (type == 2) {
        str = NAME_TEXTURE_BLANK_STOCK;
    }
    else if (type == 3) {
        //vegas out off 1card
        str = NAME_TEXTURE_BLANK;
        
        auto icon = getIcon();
        icon->setVisible(true);
        _label1->setVisible(true);
        if (_icon2) {
            _icon2->setVisible(false);
            _label2->setVisible(false);
        }
        
    } else if (type == 4) {
        //vegas out off 3card
        str = NAME_TEXTURE_BLANK;
        
        auto icon2 = getIcon2();
        icon2->setVisible(true);
        _label2->setVisible(true);
        if (_icon) {
            _icon->setVisible(false);
            _label1->setVisible(false);
        }
    }
    SpriteFrame* spFrame = SpriteFrame::create(str, rect);
    setSpriteFrame(spFrame);
}

BlankCard::BlankCard():
_icon(nullptr),
_icon2(nullptr){


}

bool BlankCard::init(bool isField, int x, int y, int type) {

    
    Rect rect = Rect(0, 0, float(SIZE_TEXTURE_X), float(SIZE_TEXTURE_Y));

    std::string str;
    if (type == 0) {
        str = NAME_TEXTURE_BLANK;
    } else if (type == 1) {
        str = NAME_TEXTURE_BLANK_ACE;
    }
    else if (type == 2) {
        str = NAME_TEXTURE_BLANK_STOCK;
    }
    else if (type == 3) {
        //vegas out off 1card
        str = NAME_TEXTURE_BLANK;
        
        auto icon = getIcon();
        icon->setVisible(true);
        _label1->setVisible(true);
        if (_icon2) {
            _icon2->setVisible(false);
            _label2->setVisible(false);
        }
        
    } else if (type == 4) {
        //vegas out off 3card
        str = NAME_TEXTURE_BLANK;
        
        auto icon2 = getIcon2();
        icon2->setVisible(true);
        _label2->setVisible(true);
        if (_icon) {
            _icon->setVisible(false);
            _label1->setVisible(false);
        }
    }
    SpriteFrame* spFrame = SpriteFrame::create(str, rect);
    spFrame->getTexture()->setAliasTexParameters();

    if (!Sprite::initWithSpriteFrame(spFrame)) {
        return false;
    }
    
    
    float min_scale = fmin(ScaleFactor::scale_x, ScaleFactor::scale_y);
    setScale(1 * min_scale, 1 * min_scale);

    _isField = isField;
    _x = x;
    _y = y;
    _type = type;
    _flipChance = 3;
    
    return true;
}

bool BlankCard::isField() {
    return _isField;
}

int BlankCard::getX() {
    return _x;
}

int BlankCard::getY() {
    return _y;
}

