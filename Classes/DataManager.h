#ifndef  _DATAMANAGER_H_
#define  _DATAMANAGER_H_

#include "tinyxml2/tinyxml2.h"
#include "KTDate.hpp"
#include "SpritePool.hpp"
#include "Layer_Setting.hpp"

using namespace KTCocos;
using namespace std;
using namespace tinyxml2;

namespace NSDataManage {
    
    const string constChrFileName = "data.xml";
    
    // 抽3张牌的开关变量
    enum ENUM_CHOU3CARD
    {
        enum_chou_off = 0, //关
        enum_chou_on,
    };
    
    // 右手开关变量
    enum ENUM_RIGHT_HAND
    {
        enum_right_off = 0, //关
        enum_right_on,
    };
    
    // 时间和得分的开关变量
    enum ENUM_TIME_SCORE
    {
        enum_time_off = 0, //关
        enum_time_on,
    };
    
    // 动画设置变量
    enum ENUM_ACTION
    {
        enum_action_1x = 0, //1x 速度
        enum_action_2x,     //2x 速度
        enum_action_no,     //没有动画
    };
    
    // 大型卡开关变量
    enum ENUM_BIG_CARD
    {
        enum_big_off = 0, //关
        enum_big_on,     //开
    };
    
    // 设置数据的结构体
    struct TagSettings{
        ENUM_CHOU3CARD   m_chou3card;
        ENUM_RIGHT_HAND  m_right_hand;
        ENUM_TIME_SCORE  m_time_score;
        ENUM_ACTION      m_action;
        ENUM_BIG_CARD    m_big_card;
    };
    
    // 2种模式下的统计的数据，包括（已胜游戏，已玩游戏，最佳得分， 最少移牌，最佳时间，总时间）
    struct TagStatisticMode{
    public:
        int     winned;      // 已胜游戏
        int     played;      // 已玩游戏
        int     best_score;  // 最佳得分
        int     lease_move;  // 最少移牌
        long    best_time;   // 最佳时间
        long    total_time;  // 总时间
    };
    
    // 统计数据,包括2种模式下的数据
    struct TagStatisticAll{
        TagStatisticMode   statistics_mode[2];
    };
    
    class DataManager{
    public:
        
        void changeTheme(int idx);
        
        void changeCardback(int idx);
        
        ThemeItem* findTheme(std::string name){
            for (auto e : _themes) {
                if (e->getName() == name) {
                    return e;
                }
            }
            return nullptr;
        }
        
        ThemeItem* findSetupTheme(std::string name){
            for (auto e : _setupThemes) {
                if (e->getName() == name) {
                    return e;
                }
            }
            return nullptr;
        }
        
        ThemeItem* findTheme(int idx){
            if(idx < _themes.size()){
                return _themes.at(idx);
            }
            return nullptr;
        }
        
        int getThemeID(ThemeItem *theme){
            return _themes.getIndex(theme)+1;
        }
        
        void addSetupTheme(ThemeItem *item){
            auto current = findTheme(item->getName());
            if(current != nullptr){
                
                _themes.replace(_themes.getIndex(current), item);
            } else {
                _themes.pushBack(item);
            }
            auto current2 = findSetupTheme(item->getName());
            if(current2 != nullptr){
                _setupThemes.replace(_setupThemes.getIndex(current2), item);
            } else {
                _setupThemes.pushBack(item);
            }
            auto setupFile =  FileUtils::getInstance()->getWritablePath() + "AppSetup.plist";
            ValueVector v;
            for(auto e : _setupThemes){
                ValueMap map;
                
                map["name"] = cocos2d::Value(e->getName());
                map["bgFile"] = cocos2d::Value(e->getBgFile());
                map["previewFile"] = cocos2d::Value(e->getPreviewFile());
                v.push_back(cocos2d::Value(map));
            }
            FileUtils::getInstance()->writeValueVectorToFile(v, setupFile);
        }
        
        
        int getThemeCount(){
            return (int)_themes.size();
        }
        
        Vector<ThemeItem *> _setupThemes;
        Vector<ThemeItem *> _themes;
        
        CC_SYNTHESIZE_PASS_BY_REF(string, _userName, UserName);
        
        CC_SYNTHESIZE_RETAIN(SpritePool *, _starPool, StarPool);
        
        bool getFirstShared();
        void setFirstShared(bool value);
        
        bool getAutoTip();
        void setAutoTip(bool value);
        
        
        int getThemeID();
        void setThemeID(int value);
        
        string getPreviewThemeFile();
        string getThemeFile();
        string getCachePreviewThemeFile();
        string getCacheThemeFile(bool portrait);
        
        int getCardbackID();
        void setCardbackID(int value);
        
        string getCardbackFile();
        string getCacheCardbackFile();
        
        int getFaceID();
        void setFaceID(int value);
        
        void setVegasOn(bool value);
        void setVegasScoreAdd(bool value);
        
        bool isVegasOn();
        bool isVegasScoreAdd();
                
        int getVegasScore();
        void setVegasScore(int value);
        void addVegasScore(int add);
        
        CC_SYNTHESIZE(bool, _dailyChanllengeSuccess, DailyChanllengeSuccess);
        CC_SYNTHESIZE(int , _dayOffset, DayOffset);
        CC_SYNTHESIZE_RETAIN(KTDate*, _requstData, RequstData);
        CC_SYNTHESIZE_RETAIN(KTDate*, _lastData, LastData);
        long getTimePast(){
            CCASSERT(_lastData != nullptr, "fuck");
            KTDate *date = KTDate::date();
            return date->timeIntervalSinceLastDate(_lastData);
        }
        CC_SYNTHESIZE(bool, _chanlleged, Chanlleged);
        
        
        DataManager();
        ~DataManager();
        
        static DataManager* getInstance();
        
        bool init();
        // 生成xml文件，并且初始化value
        bool InitLocalXml();
        // 从xml文件中获取数值，并且读取到内存当中
        bool ReadLoaclData();
        
        // 保存设置选项里的数据到xml文件中
        void saveSettingsData(TagSettings &settings);
        // 保存统计数据到xml文件中
        void saveStatisticData(TagStatisticAll &statisAll);
        
        void resetStatisticData();
        
        // 获取当前统计数据
        const TagStatisticAll* getStatisticAll();
        // 获取当前设置数据
        const TagSettings* getSettings();
        
        bool isSoundOn();
        
        void setSoundOn(bool value);
        
    private:
        static DataManager*			m_instance;
        XMLDocument*				m_pXml;
        TagStatisticAll             m_statistic;
        TagSettings                 m_settings;
        string						m_strXml;
    };
}

#endif
