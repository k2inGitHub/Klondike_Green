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

//int DataManager::getVegasScore(){
//    return UserDefault::getInstance()->getIntegerForKey("KL_VegasScore", 0);
//}
//
//void DataManager::setVegasScore(int value){
//    UserDefault::getInstance()->setIntegerForKey("KL_VegasScore", value);
//    UserDefault::getInstance()->flush();
//}
//
//void DataManager::setVegasOn(bool value){
//    UserDefault::getInstance()->setBoolForKey("kl_isVegasOn", value);
//    UserDefault::getInstance()->flush();
//}
//void DataManager::setVegasScoreAdd(bool value){
//    UserDefault::getInstance()->setBoolForKey("kl_isVegasScoreAdd", value);
//    UserDefault::getInstance()->flush();
//}






class KLUtils {
    
public:
    
    static Vector<Sprite *> addStars(Node *parent, cocos2d::Rect rect, int num = 10);

    ///获取玩家能力评价
    static float getUserAbility();
    
    static int getUserPlayNum();
    
    static void showInstertial24Later();
};


#endif /* KLUtils_hpp */
