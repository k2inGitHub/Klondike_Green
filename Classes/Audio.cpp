#include "Audio.h"
#include "SimpleAudioEngine.h"
#include "Global.h"

USING_NS_CC;
using namespace NSGlobal;
using namespace CocosDenshion;


Audio* Audio::m_instance = nullptr;

Audio::Audio(){
    prepare();
    isEffect = true;
    
    SimpleAudioEngine::getInstance()->setEffectsVolume(1);
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.f);
    
}
Audio::~Audio(){
    
}

void Audio::playButtonSFX(){
    playEffect(WordFactor::CLICK_EFFECT_MOVE_OGG);
}

Audio* Audio::getInstance(){
    if(m_instance == nullptr){
        m_instance = new Audio();
    }
    
    return m_instance;
}

void Audio::pauseMusic(){
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void Audio::resumeMusic(){
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();

}

void Audio::playBgMusic(char* name){
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(name,true);
    
}
int Audio::playEffect(const char* name, bool loop){
    if(!isEffect) return - 1;
    return CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(name,loop);
    return 0;
}
void Audio::pauseEffects(){
    isEffect = false;
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
}
void Audio::resumeEffects(){
    isEffect = true;
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();
}
void Audio::prepare(){
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(WordFactor::CLICK_EFFECT_MOVE_OGG);
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(WordFactor::CLICK_EFFECT_UNDO_OGG);
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(WordFactor::CLICK_EFFECT_FAPAI_OGG);
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(WordFactor::CLICK_EFFECT_FANPAI_OGG);
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(WordFactor::CLICK_EFFECT_WIN_OGG);
}
