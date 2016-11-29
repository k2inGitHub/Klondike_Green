//
//  KTParticleManager.cpp
//  colorful
//
//  Created by SongYang on 14-2-17.
//
//

#include "KTParticleManager.h"

static KTParticleManager *g_sharedParticleManager = NULL;

ParticleSystem* KTParticleManager::createParticle(ParticleType type)
{
    ParticleSystem *emitter = NULL;
    switch (type) {
        case ParticleType::TRAIL :{
            emitter = ParticleSystemQuad::createWithTotalParticles(50);
            //粒子数量
            //贴图
            emitter->setTexture(CCTextureCache::sharedTextureCache()->addImage("point.png"));
            emitter->setAnchorPoint(CCPoint(0, 0));
            // duration
            emitter->setDuration(kCCParticleDurationInfinity);
            
            // radius mode
            emitter->setEmitterMode(kCCParticleModeRadius);
            
            // radius mode: start and end radius in pixels
            emitter->setStartRadius(4);
            emitter->setStartRadiusVar(1);
            emitter->setEndRadius(kCCParticleStartRadiusEqualToEndRadius);
            emitter->setEndRadiusVar(0);
            
            // radius mode: degrees per second
            emitter->setRotatePerSecond(100);
            emitter->setRotatePerSecondVar(0);
            
            // angle
            emitter->setAngle(90);
            emitter->setAngleVar(0);
            
            // emitter position
            emitter->setPosVar(CCPointZero);
            
            // life of particles
            emitter->setLife(0.5);
            emitter->setLifeVar(0);
            
            // spin of particles
            emitter->setStartSpin(0);
            emitter->setStartSpinVar(0);
            emitter->setEndSpin(0);
            emitter->setEndSpinVar(0);
            
            // color of particles
            //	ccColor3B startColor(0.0f, 0.8f, 0.9f);
            emitter->setStartColor(ccc4f(0.992, 0.21f, 0.745f, 1.0f));
            
            //	Color4F startColorVar(0, 0, 0, 1.0f);
            emitter->setStartColorVar(ccc4f(0, 0, 0, 1.0f));
            
            //	Color4F endColor(1.0f, 1.0f, 1.0f, 0.1f);
            emitter->setEndColor(ccc4f(1.0f, 1.0f, 1.0f, 0.1f));
            
            //	Color4F endColorVar(0, 0, 0, 0.1f);
            emitter->setEndColorVar(ccc4f(0, 0, 0, 0.1f));
            //	Color4F setStartColor(Color4F(Color4B(50, 50, 50, 50)));
            //	Color4F setEndColor(Color4F(Color4B(0, 0, 0, 0)));
            
            // size, in pixels
            emitter->setStartSize(30);
            emitter->setStartSizeVar(1);
            emitter->setEndSize(0);
            
            // emits per second
            emitter->setEmissionRate(emitter->getTotalParticles() / emitter->getLife());
            
            // additive
            emitter->setBlendAdditive(false);
        }break;
           
        case ParticleType::COMET:{
        
            emitter = ParticleSystemQuad::createWithTotalParticles(400);
            //粒子数量
            //贴图
            emitter->setTexture(CCTextureCache::sharedTextureCache()->addImage("fire.png"));
            emitter->setAnchorPoint(CCPoint(0, 0));
            // duration
            emitter->setDuration(kCCParticleDurationInfinity);
            
            // GRAVITY mode
            emitter->setEmitterMode(ParticleSystem::Mode::GRAVITY);
            
            // GRAVITY mode: start and end radius in pixels
//            emitter->setStartRadius(4);
//            emitter->setStartRadiusVar(1);
//            emitter->setEndRadius(kCCParticleStartRadiusEqualToEndRadius);
//            emitter->setEndRadiusVar(0);
            emitter->setSpeed(200);
            emitter->setSpeedVar(30);
            
            // radius mode: degrees per second
//            emitter->setRotatePerSecond(100);
//            emitter->setRotatePerSecondVar(0);
            
            // angle
            emitter->setAngle(170);
            emitter->setAngleVar(10);
            
            // emitter position
            emitter->setPosVar(CCPointZero);
            

            
            // life of particles
            emitter->setLife(0);
            emitter->setLifeVar(1.9);
            
            // spin of particles
            emitter->setStartSpin(0);
            emitter->setStartSpinVar(0);
            emitter->setEndSpin(0);
            emitter->setEndSpinVar(0);
            
            // color of particles
            //	ccColor3B startColor(0.0f, 0.8f, 0.9f);
            emitter->setStartColor(ccc4f(0.31, 0.09, 0, 1.0f));
            
            //	Color4F startColorVar(0, 0, 0, 1.0f);
            emitter->setStartColorVar(ccc4f(0, 0, 0.1, 0));
            
            //	Color4F endColor(1.0f, 1.0f, 1.0f, 0.1f);
            emitter->setEndColor(ccc4f(0.2, 0, 0.2, 1));
            
            //	Color4F endColorVar(0, 0, 0, 0.1f);
            emitter->setEndColorVar(ccc4f(0.1, 0, 0, 0));
            //	Color4F setStartColor(Color4F(Color4B(50, 50, 50, 50)));
            //	Color4F setEndColor(Color4F(Color4B(0, 0, 0, 0)));
            
            // size, in pixels
            emitter->setStartSize(50);
            emitter->setStartSizeVar(20);
            emitter->setEndSize(20);
            
            
            // emits per second
            emitter->setEmissionRate(emitter->getTotalParticles() / emitter->getLife());
            
            // additive
            emitter->setBlendAdditive(true);
        
        }break;
            
        default:
            break;
    }
    return emitter;
}

///构造
KTParticleManager::KTParticleManager()
{

}
///析构
KTParticleManager::~KTParticleManager()
{

}
///单例
KTParticleManager* KTParticleManager::sharedParticleManager()
{
    if (!g_sharedParticleManager) {
        g_sharedParticleManager = new KTParticleManager();
    }
    return g_sharedParticleManager;
}