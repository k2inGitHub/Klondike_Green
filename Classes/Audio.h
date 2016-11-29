#ifndef _AUDIO_H_
#define _AUDIO_H_

#include "cocos2d.h"

class Audio{
public:
    Audio();
    ~Audio();
    
    void playButtonSFX();
    
    static Audio* getInstance();
    // 播放背景音乐
    void playBgMusic(char* name);
    
    void pauseMusic();
    
    void resumeMusic();
    
    // 播放音效
    int playEffect(const char* name, bool loop = false);
    // 暂停音效
    void pauseEffects();
    // 恢复音效
    void resumeEffects();
    // 加载音效
    void prepare();
private:
    static Audio* m_instance;
    bool isEffect;
};
#endif