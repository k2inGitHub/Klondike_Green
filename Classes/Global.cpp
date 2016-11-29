#include "Global.h"

namespace NSGlobal {

       //  可是区域缩放因子
       float ScaleFactor::scale_x = 0;
       float ScaleFactor::scale_y = 0;
    
       void ScaleFactor::initScale(float visible_width, float visible_height)
       {
           scale_x = 1;//visible_width / designResolutionSize.width;
           scale_y = 1;//visible_height / designResolutionSize.height;
       }
     const char * WordFactor::CLICK_SUIT_FORMATTER = "Music/30_collect_score%d.mp3";
    
       char * WordFactor::WORD_UNDO = "撤销";
       char * WordFactor::WORD_NEW_GAME = "新游戏";
    
       char * WordFactor::CLICK_EFFECT_MOVE_OGG = "Music/8_tableau.mp3";
       char * WordFactor::CLICK_EFFECT_UNDO_OGG = "Music/8_tableau.mp3";
       char * WordFactor::CLICK_EFFECT_FAPAI_OGG = "Music/26_new_game.mp3";
       char * WordFactor::CLICK_EFFECT_FANPAI_OGG = "Music/8_tableau.mp3";
       char * WordFactor::CLICK_EFFECT_WIN_OGG = "Music/29_all_clear_classic.mp3";
    
    char * WordFactor::CLICK_EFFECT_CANT_MOVE = "Music/25_shake_card.mp3";
     char * WordFactor::MUSIC_BGM = "Music/bgm.mp3";;
    
    
       char * WordFactor::WORD_SETTING_TITLE = "设置";
       char * WordFactor::WORD_SETTING_SURE = "完成";
       char * WordFactor::WORD_SETTING_DROP = "抽3张牌";
       char * WordFactor::WORD_SETTING_LR_MODE = "右手";
       char * WordFactor::WORD_SETTING_SCORE_TIME = "时间和得分";
       char * WordFactor::WORD_SETTING_ACTION = "动画";
       char * WordFactor::WORD_SETTING_CARD_TYPE = "大型卡";
       char * WordFactor::WORD_SETTING_SHARE = "分享应用";
       char * WordFactor::WORD_SETTING_STATISTIC = "统计";
       char * WordFactor::WORD_SETTING_SCORE_INTRO = "积分";
    
       char * WordFactor::WORD_STATISTICS_TITLE = "统计";
       char * WordFactor::WORD_STATISTICS_SETTING = "设置";
       char * WordFactor::WORD_STATISTICS_1_CARD = "1张牌";
       char * WordFactor::WORD_STATISTICS_3_CARD = "3张牌";
       char * WordFactor::WORD_STATISTICS_WINNED = "已胜游戏";
       char * WordFactor::WORD_STATISTICS_PLAYED = "已玩有戏";
       char * WordFactor::WORD_STATISTICS_BEST_SCORE = "最佳得分";
       char * WordFactor::WORD_STATISTICS_LEAST_MOVE = "最少移牌";
       char * WordFactor::WORD_STATISTICS_BEST_TIME = "最佳时间";
       char * WordFactor::WORD_STATISTICS_TOTAL_TIME = "总时间";
       char * WordFactor::WORD_STATISTICS_SHARE_SCORE = "分享得分";
    
       char * NTFFactor::NTF_NEW_GAME = "new game";
    
    
    char * NTFFactor::NTF_DAILY_GAME = "NTF_DAILY_GAME";
    char * NTFFactor::NTF_NEW_GAME_WINNALBE = "NTF_NEW_GAME_WINNALBE";
    char * NTFFactor::NTF_REPEATE_GAME = "NTF_REPEATE_GAME";
    
       char * NTFFactor::NTF_UNDO = "undo";
       char * NTFFactor::NTF_SETTING_REFRESH = "setting_refresh";
        char * NTFFactor::NTF_ShowNextMove = "showNextMove";
    char * NTFFactor::NTF_Daily = "NTF_Daily";
    
}

