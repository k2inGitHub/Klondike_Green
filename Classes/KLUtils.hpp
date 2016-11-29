//
//  KLUtils.hpp
//  Klondike
//
//  Created by 宋扬 on 16/8/2.
//
//

#ifndef KLUtils_hpp
#define KLUtils_hpp

#include "KTLayer.h"

class KLUtils {
    
public:
    
    static Vector<Sprite *> addStars(Node *parent, cocos2d::Rect rect, int num = 10);

    ///获取玩家能力评价
    static float getUserAbility();
    
    static int getUserPlayNum();
    
    static void showInstertial24Later();
};


#endif /* KLUtils_hpp */
