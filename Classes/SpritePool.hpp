//
//  SpritePool.hpp
//  Klondike
//
//  Created by 宋扬 on 16/8/1.
//
//

#ifndef SpritePool_hpp
#define SpritePool_hpp

#include "cocos2d.h"
using namespace cocos2d;

namespace KTCocos {
    class SpritePool : public Ref{
        
    private:
        int _preloadCount;
        Vector<Sprite *> _spawned;
        Vector<Sprite *> _despawned;
        
    public:
        
        CC_SYNTHESIZE_PASS_BY_REF(std::string, _spName, SpriteName);
        
        virtual bool init(const std::string &name, int preloadCount);
        
        void preloadInstances();
        
        static SpritePool *create(const std::string &name, int preloadCount){
            SpritePool *inst = new SpritePool();
            inst->init(name, preloadCount);
            inst->autorelease();
            return inst;
        }
        
        int getTotalCount();
        
        bool despawnInstance(Sprite *sp);
        
        bool contains(Sprite *sp);
        
        bool despawnAll();
        
        Sprite *spawnInstance(Vec2 pos);
        
        Sprite *spawnNew(Vec2 pos = Vec2::ZERO);
        
    };
}



#endif /* SpritePool_hpp */
