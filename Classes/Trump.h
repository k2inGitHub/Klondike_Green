#ifndef Trump_hpp
#define Trump_hpp

#include "cocos2d.h"
#include "KTUtils.h"
#include "TrumpData.h"
using namespace cocos2d;

//class TrumpData;

#define USE_Z_LABEL    (0)

class CC_DLL Trump : public cocos2d::Sprite {
public:
    
    ~Trump(){
        CC_SAFE_RELEASE_NULL(_data);
    }
    
    virtual Trump* clone() const{
        Trump *ret = Trump::create(_data);//new Trump(_data);
        ret->setLocalZOrder(getLocalZOrder());
        ret->setPosition(getPosition());
        return ret;
    };
    // 尺寸
    static const int SIZE_TEXTURE_X = 88;
    static const int SIZE_TEXTURE_Y = 131;

    CREATE_FUNC_PARAM(Trump, TrumpData*, data);
    
    bool init(TrumpData* data);
//    Trump(TrumpData* trump);

//    TrumpData* getData();

    void refresh();

//    virtual void stopAllActions();
    
    /**
    * 发牌
    */
    void tweenOnDeal(float delayTime, float duration, cocos2d::Point distPos, const std::function<void ()> &func);

    /**
    * 翻牌
    */
    void flip(const std::function<void ()> &func, const std::function<void()> &onHalf = nullptr, bool animated = true);

    void setLocalZOrder(int localZOrder);

    CC_SYNTHESIZE_RETAIN(TrumpData*, _data, Data);
    
private:
//    TrumpData* _data;
#if USE_Z_LABEL
    Label *_label;
#endif
};

#endif
