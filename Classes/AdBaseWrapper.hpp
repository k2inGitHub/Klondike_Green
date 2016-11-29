//
//  AdBaseWrapper.hpp
//  Klondike
//
//  Created by 宋扬 on 16/10/14.
//
//

#ifndef AdBaseWrapper_hpp
#define AdBaseWrapper_hpp

#include "cocos2d.h"
using namespace cocos2d;

class AdBaseWrapper : public Ref {
public:
    virtual void showInterstitial(){}
    virtual bool canShowSafe(){return false;}
    virtual bool canShowUnSafe(){return false;}
    virtual bool init(){return true;}
};

#endif /* AdBaseWrapper_hpp */
