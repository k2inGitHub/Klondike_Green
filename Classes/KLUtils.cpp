//
//  KLUtils.cpp
//  Klondike
//
//  Created by 宋扬 on 16/8/2.
//
//

#include "KLUtils.hpp"
#include "DataManager.h"
using namespace NSDataManage;

void KLUtils::showInstertial24Later(){

    auto str = UserDefault::getInstance()->getStringForKey("KL_FirstDate");
    if (str.empty()) {
        //纪录首日日期
        UserDefault::getInstance()->setStringForKey("KL_FirstDate", KTDate::date()->toString());
        UserDefault::getInstance()->flush();
    } else {
        auto firstDate = KTDate::date(str);
        auto now = KTDate::date();
        auto dt = now->timeIntervalSinceLastDate(firstDate);
        if(dt > 24 * 60 *60){
            //次日后开启
            if(HLAnalsytWrapper::boolValue("KL_ShowStartAd")){
                HLAdManagerWrapper::showUnsafeInterstitial();
            }
        }
    }
}

int KLUtils::getUserPlayNum(){
    auto st = *DataManager::getInstance()->getStatisticAll();
    auto sm1 = st.statistics_mode[0];
    auto sm2 = st.statistics_mode[1];
    return sm1.played + sm2.played;
}

float KLUtils::getUserAbility() {
    int idx = 0;
    auto st = *DataManager::getInstance()->getStatisticAll();
    auto sm1 = st.statistics_mode[0];
    auto sm2 = st.statistics_mode[1];
    
    int winned = idx == 0 ? (sm1.winned + sm2.winned) : (idx == 1 ? sm1.winned : sm2.winned);
    
    int played = idx == 0 ? (sm1.played + sm2.played) : (idx == 1 ? sm1.played : sm2.played);
    
//    int best_time = idx == 0 ? int(sm1.best_time + sm2.best_time) : (idx == 1 ? (int)sm1.best_time : (int)sm2.best_time);
//    
//    int lease_move = idx == 0 ? (sm1.lease_move + sm2.lease_move) : (idx == 1 ? sm1.lease_move : sm2.lease_move);
//    
//    int best_score = idx == 0 ? (sm1.best_score + sm2.best_score) : (idx == 1 ? sm1.best_score : sm2.best_score);
    
    float p = played == 0 ? 0 : (float)winned / (float)played;
    
    float num = 0.4f;
    if (played > 20) {
        num = 1;
    } else if (played > 10){
        num = 0.6f;
    } else {
        num = 0.4f;
    }
    
    float ret = num * (1.f + p)/2.f;

    return ret;
}

Vector<Sprite *> KLUtils::addStars(Node *parent, Rect rect, int num){
    Vector<Sprite *> stars;
    for (int i = 0; i < num; i++) {
        
        Vec2 pos = rect.origin + Vec2(random(0.f, rect.size.width), random(0.f, rect.size.height));
        auto sp = DataManager::getInstance()->getStarPool()->spawnInstance(pos);
        parent->addChild(sp, 48);
        sp->setVisible(false);
        sp->setRotation(random(0, 360));
        sp->setOpacity(164);
        auto ac1 = DelayTime::create(0.25 * i);
        auto ac2 = ScaleTo::create(0, random(1.2, 1.5));
        auto ac3 = Show::create();
        auto ac4 = ScaleTo::create(1, 0);
        auto ac5 = RotateBy::create(1, 360);
        auto ac6 = FadeTo::create(1, 20);
        auto spawn = Spawn::create(ac4, ac5, ac6, NULL);
        auto cb = CallFuncN::create([=](Node *sender){
            sender->setRotation(0);
            sender->setOpacity(255);
            DataManager::getInstance()->getStarPool()->despawnInstance(static_cast<Sprite *>(sender));
        });
        
        Vector<FiniteTimeAction *> v;
        v.pushBack(ac1);
        v.pushBack(ac2);
        v.pushBack(ac3);
        v.pushBack(spawn);
        v.pushBack(cb);
        
        if (i == num - 1) {
            auto cb2 = CallFunc::create([=](){
                KLUtils::addStars(parent, rect, num);
            });
            v.pushBack(cb2);
        }
        sp->runAction(Sequence::create(v));
        stars.pushBack(sp);
    }
    return stars;
}
