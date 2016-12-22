#include "Trump.h"
#include "TrumpData.h"
#include "DataManager.h"
#include "Global.h"

USING_NS_CC;
using namespace NSGlobal;
using namespace NSDataManage;

// Copyright
//


// 牌的尺寸
const float SIZE_SPRITE_X = 1.0f; //
const float SIZE_SPRITE_Y = 1.0f;

// tween
//const float TWEEN_CARD_MOVE_SPEED = 0.1f; //
const float TWEEN_CARD_FLIP_SPEED_1X = 0.3f; // 翻牌的动画时间_1x
const float TWEEN_CARD_FLIP_SPEED_2X = 0.2f; // 翻牌的动画时间_2x
const float TWEEN_CARD_FLIP_SPEED_NO = 0.0f; // 没有翻拍动画_no

bool Trump::init(TrumpData* data){

    if (!Sprite::init()) {
        return false;
    }
    setData(data);
    refresh();
    
    
#if USE_Z_LABEL
    _label = Label::createWithBMFont("bitmapFontTest4.fnt", "");//Label::createWithSystemFont("z:10", "Arial", 20);
    
    _label->setAlignment(TextHAlignment::CENTER);
    _label->setTextColor(Color4B::BLACK);
    addChild(_label);
    _label->setPosition(44, 120);
#endif
    
    return true;
}

//void Trump::stopAllActions(){
//
//    Node::stopAllActions();
//    setAdditionalTransform(nullptr);
//}

void Trump::refresh() {
    
//    Sprite::init();

    auto mgr = DataManager::getInstance();
    
    std::string textureName;
    // 牌的风格类型，目前有两种,分为0和1
    //int card_type = mgr->getSettings()->m_big_card;

    if (_data->isReverse()) {
        textureName = mgr->getCardbackFile();
    } else {
        
//        char buf[64];
//        sprintf(buf,"shared/classic_%d/%d/%d.png",mgr->getFaceID(),_data->getSuit(), _data->getValue());
        textureName = mgr->getFace(mgr->getSelectFaceName())->getFile(_data->getSuit(), _data->getValue());
    }

    Rect rect = Rect(0, 0, float(SIZE_TEXTURE_X), float(SIZE_TEXTURE_Y));

    SpriteFrame* spFrame = SpriteFrame::create(textureName, rect);
    //设置不抗锯齿
    //spFrame->getTexture()->setAliasTexParameters();

    Sprite::initWithSpriteFrame(spFrame);
    float min_scale = fmin(ScaleFactor::scale_x, ScaleFactor::scale_y);
    Sprite::setScale(SIZE_SPRITE_X * min_scale, SIZE_SPRITE_Y * min_scale);
}

void Trump::tweenOnDeal(float delayTime, float duration, Point distPos, const std::function<void ()> &func) {

    DelayTime* delay = DelayTime::create(delayTime);
    MoveTo* move = MoveTo::create(duration, distPos);
    EaseSineOut* ease = EaseSineOut::create(move);

    Sequence* seq;
    if (func == nullptr) {
        seq = Sequence::create(delay, ease, nullptr);
    } else {
        CallFunc* callBack = CallFunc::create(func);
        seq = Sequence::create(delay, ease, callBack, nullptr);
    }
    Sprite::runAction(seq);
}

void Trump::flip(const std::function<void ()> &func, const std::function<void()> &onHalf, bool animated) {

    // 翻转时间
    float flipSpeed;
    switch (1) {
        case enum_action_1x:
            flipSpeed = TWEEN_CARD_FLIP_SPEED_1X;
            break;
        case enum_action_2x:
            flipSpeed = TWEEN_CARD_FLIP_SPEED_2X;
            break;
        case enum_action_no:
            flipSpeed = TWEEN_CARD_FLIP_SPEED_NO;
            break;
        default:
            break;
    }
    
    // 翻到一半
//    OrbitCamera* cam1 = OrbitCamera::create(flipSpeed / 2.0f, 1, 0, 0.0f, 90.0f, 0, 0);
    auto cam1 = ScaleTo::create(animated ? (flipSpeed / 2.0f) : 0, 0,1);
    Hide* hide = Hide::create();

    // sprite再設定
    CallFunc* refreshFunc = CallFunc::create(CC_CALLBACK_0(Trump::refresh, this));

    CallFunc *onHalfFunc = CallFunc::create(onHalf);
    
    // 翻了一半后
    Show* show = Show::create();
//    OrbitCamera* cam2 = OrbitCamera::create(flipSpeed / 2.0f, 1, 0, 90, -90, 0, 0);
    auto cam2 = ScaleTo::create(animated ? (flipSpeed / 2.0f) : 0, 1,1);

    CallFunc* callBack = CallFunc::create(func);
    Sequence* seq = Sequence::create(cam1, hide, refreshFunc,onHalfFunc,ScaleTo::create(0, 0, 1), show, cam2, callBack, nullptr);
    this->runAction(seq);
}

void Trump::setLocalZOrder(int localZOrder) {

    Sprite::setLocalZOrder(localZOrder);
    
    if (_data) {
        _data->zOrder = localZOrder;
    }
#if USE_Z_LABEL
    char str[64];
    sprintf(str, "z:%d", localZOrder);
    _label->setString(str);
#endif
    
//    CCLOG("Trump Suit: %d, Value: %d, Zorder: %d", _data->getSuit(), _data->getValue(), localZOrder);
}
