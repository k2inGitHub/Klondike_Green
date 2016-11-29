//
//  SpritePool.cpp
//  Klondike
//
//  Created by 宋扬 on 16/8/1.
//
//

#include "SpritePool.hpp"
namespace KTCocos{
    
    bool SpritePool::init(const std::string &name, int preloadCount){
        _spName = name;
        _preloadCount = preloadCount;
        
        preloadInstances();
        
        return true;
    }
    
    void SpritePool::preloadInstances(){
        
        Sprite *sp = nullptr;
        while (getTotalCount() < _preloadCount) {
            sp = spawnNew();
            despawnInstance(sp);
        }
        
        CCLOG("des size = %ld", _despawned.size());
        
    }
    
    int SpritePool::getTotalCount(){
        
        return (int)_spawned.size() + (int)_despawned.size();
    };
    
    bool SpritePool::despawnInstance(Sprite *sp){
        
        //            if (!_spawned.contains(sp)) {
        //                return false;
        //            }
        sp->stopAllActions();
        if (sp->getParent() != nullptr) {
            sp->removeFromParent();
        }
        _despawned.pushBack(sp);
        _spawned.eraseObject(sp);
        sp->setVisible(false);
        return true;
    };
    
    bool SpritePool::contains(Sprite *sp){
        if(_spawned.contains(sp)) {
            return true;
        }
        if (_despawned.contains(sp)) {
            return true;
        }
        return false;
    }
    
    bool SpritePool::despawnAll(){
        auto spwaned = _spawned;
        for (auto sp : spwaned) {
            despawnInstance(sp);
        }
        return true;
    }
    
    Sprite *SpritePool::spawnInstance(Vec2 pos){
        
        Sprite *inst;
        if (_despawned.size() == 0) {
            inst = spawnNew(pos);
        } else {
            inst = _despawned.at(0);
            _spawned.pushBack(inst);
            _despawned.erase(0);
            CCASSERT(inst != nullptr, "error");
            inst->setPosition(pos);
            inst->setVisible(true);
        }
        return inst;
    }

    Sprite * SpritePool::spawnNew(Vec2 pos){
        auto sp = Sprite::create(_spName);
        sp->setPosition(pos);
        _spawned.pushBack(sp);
        return sp;
    }
    
    
    
}
