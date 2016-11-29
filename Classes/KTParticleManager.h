//
//  KTParticleManager.h
//  colorful
//
//  Created by SongYang on 14-2-17.
//
//

#ifndef __colorful__KTParticleManager__
#define __colorful__KTParticleManager__

#include "cocos2d.h"
USING_NS_CC;

enum class ParticleType {
    ///尾巴粒子
    TRAIL,
    ///MIR彗星粒子
    COMET,
};

/**
 粒子管理类
 */
class KTParticleManager : public Ref{
    
    
public:
    
    ///通过粒子种类获取粒子
    ParticleSystem* createParticle(ParticleType type);
    ///构造
    KTParticleManager();
    ///析构
    ~KTParticleManager();
    ///单例
    static KTParticleManager* sharedParticleManager();
};

#endif /* defined(__colorful__KTParticleManager__) */
