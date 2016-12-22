#ifndef  _Global_H_
#define  _Global_H_
#include <vector>
#include "cocos2d.h"
// 全局变量
namespace NSGlobal {
    //remove
    CC_DEPRECATED_ATTRIBUTE const int kNewCardbackMin = 21;
    
    CC_DEPRECATED_ATTRIBUTE const int kNewCardbackMax = 22;
    
    CC_DEPRECATED_ATTRIBUTE const int kNewThemeMin = 8;
    
    CC_DEPRECATED_ATTRIBUTE const int kNewThemeMax = 10;
    
//    const int kCardbackCount = 22;
//    
//    const int kFaceCount = 4;
//    
//    const int kThemeCount = 10;
    
    const cocos2d::Size cardbackImageSize = cocos2d::Size(78, 124);
    
    static const char* BG_FILENAME = "back.jpg";
    
    static const char* kUpdateBadgeCardback = "UpdateBadgeCardback";
    
    static const char* kUpdateBadgeTheme = "UpdateBadgeTheme";
    
    static const char* kOnThemeSetup = "OnThemeSetup";
    
    static const char* kOnThemeFileCanceled = "OnThemeFileCanceled";
    
    static const char* kOnThemeFileChanged = "OnThemeFileChanged";
    
    static const char* kOnCardbackFileCanceled = "OnCardbackFileCanceled";
    
    static const char* kOnCardbackFileChanged = "OnCardbackFileChanged";
    
    static const char *kMD5_Secret = "9214dcda95134ef6555a8e2573b3e669";
    
     // 逻辑视图宽高定义
     static cocos2d::Size designResolutionSize = cocos2d::Size(640, 960);
    
     class ScaleFactor{
         
       public:
       //  可是区域缩放因子
       static float scale_x;
       static float scale_y;
    
       static void initScale(float visible_width, float visible_height);
         
     };
    
     // 所有常量字符串在此定义
     class WordFactor{
        
     public:
         static const char * CLICK_SUIT_FORMATTER;
        static char * WORD_UNDO;
        static char * WORD_NEW_GAME;
        // 音效名字
        static char * CLICK_EFFECT_MOVE_OGG;
        static char * CLICK_EFFECT_UNDO_OGG;
        static char * CLICK_EFFECT_FAPAI_OGG;
        static char * CLICK_EFFECT_FANPAI_OGG;
        static char * CLICK_EFFECT_WIN_OGG;
         static char * CLICK_EFFECT_CANT_MOVE;
         static char * MUSIC_BGM;
         
         
        // 设置页面的文字
        static char* WORD_SETTING_TITLE;
        static char* WORD_SETTING_SURE;
        static char* WORD_SETTING_DROP;
        static char* WORD_SETTING_LR_MODE;
        static char* WORD_SETTING_SCORE_TIME;
        static char* WORD_SETTING_ACTION;
        static char* WORD_SETTING_CARD_TYPE;
        static char* WORD_SETTING_SHARE;
        static char* WORD_SETTING_STATISTIC;
        static char* WORD_SETTING_SCORE_INTRO;
         
         // 统计页面的文字
        static char* WORD_STATISTICS_TITLE;
        static char* WORD_STATISTICS_SETTING;
        static char* WORD_STATISTICS_1_CARD;
        static char* WORD_STATISTICS_3_CARD;
        static char* WORD_STATISTICS_WINNED;
        static char* WORD_STATISTICS_PLAYED;
        static char* WORD_STATISTICS_BEST_SCORE;
        static char* WORD_STATISTICS_LEAST_MOVE;
        static char* WORD_STATISTICS_BEST_TIME;
        static char* WORD_STATISTICS_TOTAL_TIME;
        static char* WORD_STATISTICS_SHARE_SCORE;
     };
    
     // 观察事件字符串在此定义
     class NTFFactor{
     public:
         static char * NTF_NEW_GAME_WINNALBE;
         static char * NTF_REPEATE_GAME;
        static char * NTF_NEW_GAME;
         static char * NTF_DAILY_GAME;
        static char * NTF_UNDO;
        static char * NTF_SETTING_REFRESH;
        static char *NTF_ShowNextMove;
         static char *NTF_Daily;
     };
}

#endif
