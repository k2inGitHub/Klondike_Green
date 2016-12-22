#ifndef  _DATAMANAGER_H_
#define  _DATAMANAGER_H_

#include "tinyxml2/tinyxml2.h"
#include "KTDate.hpp"
#include "SpritePool.hpp"
#include "Layer_Setting.hpp"
#include "Item_Setting.hpp"

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
//    struct TagSettings{
//        ENUM_CHOU3CARD   m_chou3card;
//        ENUM_RIGHT_HAND  m_right_hand;
//        ENUM_TIME_SCORE  m_time_score;
//        ENUM_ACTION      m_action;
//        ENUM_BIG_CARD    m_big_card;
//    };
    
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
        //KT SYN
        KT_SYNTHESIZE_UD_BOOL("KL_CHOW_3_CARD", Chow3Card, false);

        KT_SYNTHESIZE_UD_BOOL("KL_IS_RIGHT_HAND", UseRightHand, true);
        
        KT_SYNTHESIZE_UD_BOOL("KL_SHOW_MATCH_INFO", ShowMatchInfo, true);

//        KT_SYNTHESIZE_UD_BOOL("KL_SHOW_SETUP_THEME", TipForSetupThemeShowed, false);
        
        KT_SYNTHESIZE_UD_BOOL("KL_HappyChrismtas_Show", HappyChrismtasShowed, false);
        
        bool _needUpdateSetupFile;
        
        KT_SYNTHESIZE_UD_BOOL("KL_ThemeSetNew", ThemeSetNew, true);
        
        CC_DEPRECATED_ATTRIBUTE void changeTheme(int idx);
        
        CC_DEPRECATED_ATTRIBUTE void changeCardback(int idx);
        
        void changeCardback(string name, bool refresh = true, bool isCustom = true);
        
        void changeTheme(string name, bool refresh = true, bool isCustom = true);
        
        void changeFace(string name, bool refresh = true, bool isCustom = true);
        
        void changeThemeSet(string name) {
            string key = StringUtils::format("KL_THEME_SET_%s_CLICK", name.c_str());
            UserDefault::getInstance()->setBoolForKey(key.c_str(), true);
            UserDefault::getInstance()->flush();
            
            auto mgr = this;
            
            mgr->setSelectThemeSetName(name);
            mgr->changeTheme(mgr->getSelectThemeName(), false, false);
            mgr->changeFace(mgr->getSelectFaceName(), false, false);
            mgr->changeCardback(mgr->getSelectCardbackName(), true, false);
            mgr->setThemeSetNew(false);
            __NotificationCenter::getInstance()->postNotification(kUpdateBadgeCardback);
        }
        
        ThemeItem* getTheme(std::string name){
            for (auto e : _themes) {
                if (e->getName() == name) {
                    return e;
                }
            }
            return nullptr;
        }
        
        ThemeItem* getSetupTheme(std::string name){
            for (auto e : _setupThemes) {
                if (e->getName() == name) {
                    return e;
                }
            }
            return nullptr;
        }
        
        ThemeItem* getTheme(int idx){
            if(idx < _themes.size()){
                return _themes.at(idx);
            }
            return nullptr;
        }
        
        int getThemeID(ThemeItem *theme){
            return (int)_themes.getIndex(theme)+1;
        }
        
        void addSetupTheme(ThemeItem *item){
            auto current = getTheme(item->getName());
            if(current != nullptr){
                _themes.replace(_themes.getIndex(current), item);
            } else {
                _themes.pushBack(item);
            }
            auto current2 = getSetupTheme(item->getName());
            if(current2 != nullptr){
                _setupThemes.replace(_setupThemes.getIndex(current2), item);
            } else {
                _setupThemes.pushBack(item);
            }
            _needUpdateSetupFile = true;
        }
        
        string getThemeSetupFile(){
            return FileUtils::getInstance()->getWritablePath() + "AppSetup.plist";
        }
        
        int getThemeCount(){
            return (int)_themes.size();
        }
        
        //app主题安装的所有主题背景
        Vector<ThemeItem *> _setupThemes;
        //所有主题背景（包含安装的）
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(Vector<ThemeItem *>, _themes, Themes);
        
        
        
        void addSetupFace(FaceItem *item){
            auto current = getFace(item->getName());
            if(current != nullptr){
                _faces.replace(_faces.getIndex(current), item);
            } else {
                _faces.pushBack(item);
            }
            auto current2 = getSetupFace(item->getName());
            if(current2 != nullptr){
                _setupFaces.replace(_setupFaces.getIndex(current2), item);
            } else {
                _setupFaces.pushBack(item);
            }
            _needUpdateSetupFile = true;
        }
        
        Vector<FaceItem *> _setupFaces;
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(Vector<FaceItem *>, _faces, Faces);
        
        FaceItem* getSetupFace(std::string name){
            for (auto e : _setupFaces) {
                if (e->getName() == name) {
                    return e;
                }
            }
            return nullptr;
        }
        
        FaceItem* getFace(string name){
            for(auto item : _faces){
                if (item->getName() == name) {
                    return item;
                }
            }
            return nullptr;
        }
        
        void addSetupCardback(CardbackItem *item){
            auto current = getCardback(item->getName());
            if(current != nullptr){
                _cardbacks.replace(_cardbacks.getIndex(current), item);
            } else {
                _cardbacks.pushBack(item);
            }
            auto current2 = getSetupCardback(item->getName());
            if(current2 != nullptr){
                _setupCardbacks.replace(_setupCardbacks.getIndex(current2), item);
            } else {
                _setupCardbacks.pushBack(item);
            }
            _needUpdateSetupFile = true;
        }
        
        Vector<CardbackItem *> _setupCardbacks;
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(Vector<CardbackItem *>, _cardbacks, Cardbacks);
        
        CardbackItem* getSetupCardback(std::string name){
            for (auto e : _setupCardbacks) {
                if (e->getName() == name) {
                    return e;
                }
            }
            return nullptr;
        }
        
        CardbackItem *getCardback(string name){
            for(auto item : _cardbacks){
                if (item->getName() == name) {
                    return item;
                }
            }
            return nullptr;
        }
        
        void addSetupThemeSet(ThemeSetItem *item){
            auto current = getThemeSet(item->getName());
            if(current != nullptr){
                _themeSets.replace(_themeSets.getIndex(current), item);
            } else {
                _themeSets.pushBack(item);
            }
            auto current2 = getSetupThemeSet(item->getName());
            if(current2 != nullptr){
                _setupThemeSets.replace(_setupThemeSets.getIndex(current2), item);
            } else {
                _setupThemeSets.pushBack(item);
            }
            _needUpdateSetupFile = true;
        }
        
        Vector<ThemeSetItem *> _setupThemeSets;
        
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(Vector<ThemeSetItem *>, _themeSets, ThemeSets);
        
        ThemeSetItem* getSetupThemeSet(std::string name){
            for (auto e : _setupThemeSets) {
                if (e->getName() == name) {
                    return e;
                }
            }
            return nullptr;
        }
        
        ThemeSetItem *getThemeSet(string name){
            for(auto item : _themeSets){
                if (item->getName() == name) {
                    return item;
                }
            }
            return nullptr;
        }
        
        string getSelectThemeSetName(){
            return UserDefault::getInstance()->getStringForKey("KL_ThemeSetName", "theme_set_2");
        }
        void setSelectThemeSetName(string name){
            UserDefault::getInstance()->setStringForKey("KL_ThemeSetName", name);
            UserDefault::getInstance()->flush();
        }
        
        
        
        CC_SYNTHESIZE_PASS_BY_REF(string, _userName, UserName);
        
        CC_SYNTHESIZE_RETAIN(SpritePool *, _starPool, StarPool);
        
        bool getFirstShared();
        void setFirstShared(bool value);
        
        bool getAutoTip();
        void setAutoTip(bool value);
        
        CC_DEPRECATED_ATTRIBUTE int getThemeID();
        CC_DEPRECATED_ATTRIBUTE void setThemeID(int value);
        
        //当前选中的背景ThemeName（包含自定义 与 主题集合）
        string getSelectThemeName(){
            //debug1
            string name = getSelectThemeSetName();
            CCLOG("name = %s", name.c_str());
            auto themeSet = getThemeSet(name);
            return themeSet->getThemeName();
        }
        //自定义背景选中的ThemeName
        string getCustomSelectThemeName(){
            return UserDefault::getInstance()->getStringForKey("KL_ThemeName", "theme_2");
        }
        void setCustomSelectThemeName(string name){
            
            if (name == "theme_1") {
                auto item = _themes.at(0);
                item->setPreviewFile(getCachePreviewThemeFile());
                item->setBgFile(getCacheThemeFile(true));
            } else {
                auto item = _themes.at(0);
                item->setPreviewFile("shared/theme/0.png");
                item->setBgFile("shared/theme/0.png");
            }
            
            UserDefault::getInstance()->setStringForKey("KL_ThemeName", name);
            UserDefault::getInstance()->flush();
        }
        
        string getPreviewThemeFile();
        string getThemeFile();
        string getCachePreviewThemeFile();
        string getCacheThemeFile(bool portrait);
        
        //depricated
        CC_DEPRECATED_ATTRIBUTE int getCardbackID();
        CC_DEPRECATED_ATTRIBUTE void setCardbackID(int value);
        
        string getCustomSelectCardbackName(){
            return UserDefault::getInstance()->getStringForKey("KL_CardbackName", "cardback_2");
        }
        void setCustomSelectCardbackName(string name){
            if (name == "cardback_1") {
                auto item = _cardbacks.at(0);
                item->setPreviewFile(getCacheCardbackFile());
                item->setFile(getCacheCardbackFile());
            } else {
                auto item = _cardbacks.at(0);
                item->setPreviewFile("shared/cardback/0.png");
                item->setFile("shared/cardback/0.png");
            }
            
            UserDefault::getInstance()->setStringForKey("KL_CardbackName", name);
            UserDefault::getInstance()->flush();
        }
        
        string getSelectCardbackName(){
            //debug2
//            CCLOG("getSelectCardbackName");
            return getThemeSet(getSelectThemeSetName())->getCardbackName();
        }
        
        string getCardbackFile();
        string getCacheCardbackFile();
        
        //depricated
        CC_DEPRECATED_ATTRIBUTE int getFaceID();
        CC_DEPRECATED_ATTRIBUTE void setFaceID(int value);
        
        string getCustomSelectFaceName(){
            return UserDefault::getInstance()->getStringForKey("KL_FaceName", "face_1");
        }
        void setCustomSelectFaceName(string name){
            UserDefault::getInstance()->setStringForKey("KL_FaceName", name);
            UserDefault::getInstance()->flush();
        }
        
        string getSelectFaceName(){
            return getThemeSet(getSelectThemeSetName())->getFaceName();
        }
        
        KT_SYNTHESIZE_UD_BOOL("kl_isVegasScoreAdd", VegasScoreAdd, false);
        
        KT_SYNTHESIZE_UD_BOOL("kl_isVegasOn", VegasOn, false);

        KT_SYNTHESIZE_UD_INT("KL_VegasScore",VegasScore,0);
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
//        void saveSettingsData(TagSettings &settings);
        // 保存统计数据到xml文件中
        void saveStatisticData(TagStatisticAll &statisAll);
        
        void resetStatisticData();
        
        // 获取当前统计数据
        const TagStatisticAll* getStatisticAll();
//        // 获取当前设置数据
//        const TagSettings* getSettings();
        
        bool isSoundOn();
        
        void setSoundOn(bool value);
        
        void update(float dt);
        
    private:
        static DataManager*			m_instance;
        XMLDocument*				m_pXml;
        TagStatisticAll             m_statistic;
//        TagSettings                 m_settings;
        string						m_strXml;
    };
}

#endif
