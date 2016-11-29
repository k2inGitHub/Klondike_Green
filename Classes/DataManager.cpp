#include "DataManager.h"

USING_NS_CC;




namespace NSDataManage {
    
    DataManager*	DataManager::m_instance = NULL;
    
    void DataManager::changeTheme(int _idx){
        HLAnalsytWrapper::event("SetThemes");
        DataManager::getInstance()->setThemeID(_idx);
        _idx--;
        if (_idx == 0) {
            UserDefault::getInstance()->setBoolForKey("KL_CUSTOM_THEME_CLICK", true);
            UserDefault::getInstance()->flush();
            __NotificationCenter::getInstance()->postNotification(kUpdateBadgeTheme);
        } else if (_idx >= kNewThemeMin && _idx <= kNewThemeMax) {
            UserDefault::getInstance()->setBoolForKey(StringUtils::format("KL_THEME_%d_CLICK", _idx).c_str(), true);
            UserDefault::getInstance()->flush();
        }
        __NotificationCenter::getInstance()->postNotification(NSGlobal:: NTFFactor::NTF_SETTING_REFRESH);
    }
    
    void DataManager::changeCardback(int _idx){
        HLAnalsytWrapper::event("SetCardbacks");
        DataManager::getInstance()->setCardbackID(_idx);
        _idx--;
        if (_idx == 0) {
            UserDefault::getInstance()->setBoolForKey("KL_CUSTOM_CARDBACK_CLICK", true);
            UserDefault::getInstance()->flush();
            __NotificationCenter::getInstance()->postNotification(kUpdateBadgeCardback);
        } else if (_idx >= kNewCardbackMin && _idx <= kNewCardbackMax) {
            UserDefault::getInstance()->setBoolForKey(StringUtils::format("KL_CARDBACK_%d_CLICK", _idx).c_str(), true);
            UserDefault::getInstance()->flush();
        }
        __NotificationCenter::getInstance()->postNotification(NSGlobal:: NTFFactor::NTF_SETTING_REFRESH);
    }
    
    bool DataManager::getFirstShared(){
        return UserDefault::getInstance()->getBoolForKey("KL_FirstShared", false);
    }
    
    void DataManager::setFirstShared(bool value){
        UserDefault::getInstance()->setBoolForKey("KL_FirstShared", value);
        UserDefault::getInstance()->flush();
    }
    
    bool DataManager::getAutoTip(){
        return UserDefault::getInstance()->getBoolForKey("KL_AutoTip", true);
    }

    void DataManager::setAutoTip(bool value){
        UserDefault::getInstance()->setBoolForKey("KL_AutoTip", value);
        UserDefault::getInstance()->flush();
    }
    
    int DataManager::getThemeID(){
        return UserDefault::getInstance()->getIntegerForKey("KL_ThemeID", 2);
    }
    
    void DataManager::setThemeID(int value){
        UserDefault::getInstance()->setIntegerForKey("KL_ThemeID", value);
        UserDefault::getInstance()->flush();
    }
    
    string DataManager::getPreviewThemeFile(){
    
        if (DataManager::getInstance()->getThemeID() == 1) {
            return getCachePreviewThemeFile();
        } else {
            auto theme = _themes.at(DataManager::getInstance()->getThemeID()-1);
            return theme->getPreviewFile();
        }
    }
    
    string DataManager::getThemeFile(){
        if (DataManager::getInstance()->getThemeID() == 1) {
            auto winSize = Director::getInstance()->getWinSize();
            return getCacheThemeFile(winSize.height > winSize.width);
        } else {
            auto theme = _themes.at(DataManager::getInstance()->getThemeID()-1);
//            char str[64];
//            sprintf(str, "shared/theme/%d.jpg", DataManager::getInstance()->getThemeID()-1);
            return theme->getBgFile();
        }
    }
    
    string DataManager::getCachePreviewThemeFile(){
        auto size = Director::getInstance()->getWinSize();
        auto ret = FileUtils::getInstance()->getWritablePath() + "theme.png";
        return ret;
    }

    string DataManager::getCacheThemeFile(bool portrait){
        auto size = Director::getInstance()->getWinSize();
        auto ret = FileUtils::getInstance()->getWritablePath() + "theme";
        if (portrait) {
            ret += "_portrait.jpg";
        } else {
            ret += "_landscape.jpg";
        }
        return ret;
    }
    
    string DataManager::getCacheCardbackFile(){
        return FileUtils::getInstance()->getWritablePath() + "cardback.png";
    }
    
    string DataManager::getCardbackFile(){
        
        if(DataManager::getInstance()->getCardbackID() == 1){
            return getCacheCardbackFile();
        } else {
            char str[128];
            sprintf(str, "shared/cardback/%d.png", DataManager::getInstance()->getCardbackID()-1);
            return str;
        }
    }
    
    int DataManager::getCardbackID(){
        return UserDefault::getInstance()->getIntegerForKey("KL_CardbackID", 2);
    }
    
    void DataManager::setCardbackID(int value){
        UserDefault::getInstance()->setIntegerForKey("KL_CardbackID", value);
        UserDefault::getInstance()->flush();
    }
    
    int DataManager::getFaceID(){
        return UserDefault::getInstance()->getIntegerForKey("KL_FaceID", 1);
    }
    
    void DataManager::setFaceID(int value){
        UserDefault::getInstance()->setIntegerForKey("KL_FaceID", value);
        UserDefault::getInstance()->flush();
    }
    
    int DataManager::getVegasScore(){
        return UserDefault::getInstance()->getIntegerForKey("KL_VegasScore", 0);
    }
    
    void DataManager::addVegasScore(int add){
        setVegasScore(getVegasScore() + add);
    }
    
    void DataManager::setVegasScore(int value){
        UserDefault::getInstance()->setIntegerForKey("KL_VegasScore", value);
        UserDefault::getInstance()->flush();
    }
    
    void DataManager::setVegasOn(bool value){
        UserDefault::getInstance()->setBoolForKey("kl_isVegasOn", value);
        UserDefault::getInstance()->flush();
    }
    void DataManager::setVegasScoreAdd(bool value){
        UserDefault::getInstance()->setBoolForKey("kl_isVegasScoreAdd", value);
        UserDefault::getInstance()->flush();
    }
    
    bool DataManager::isVegasOn(){
    return UserDefault::getInstance()->getBoolForKey("kl_isVegasOn", false);
    }
    bool DataManager::isVegasScoreAdd(){
    return UserDefault::getInstance()->getBoolForKey("kl_isVegasScoreAdd", false);
    }
    
    
    bool DataManager::isSoundOn(){
        return UserDefault::getInstance()->getBoolForKey("kl_sound", true);
    }
    
    void DataManager::setSoundOn(bool value){
        UserDefault::getInstance()->setBoolForKey("kl_sound", value);
        UserDefault::getInstance()->flush();
    }
    
    DataManager::DataManager():
    _lastData(nullptr),
    _chanlleged(false),
    _requstData(nullptr)
    {
        m_pXml = NULL;
        m_strXml = CCFileUtils::getInstance()->getWritablePath() + constChrFileName;
        
        
        // 初始化设置数据
        m_settings.m_chou3card  = (ENUM_CHOU3CARD)0;
        m_settings.m_right_hand = (ENUM_RIGHT_HAND)1;
        m_settings.m_time_score = (ENUM_TIME_SCORE)1;
        m_settings.m_action     = (ENUM_ACTION)0;
        m_settings.m_big_card   = (ENUM_BIG_CARD)1;
        
        // 初始化统计数据
        for(int i=0;i<2;i++){
            m_statistic.statistics_mode[i].winned     = 0;
            m_statistic.statistics_mode[i].played     = 0;
            m_statistic.statistics_mode[i].best_score = 0;
            m_statistic.statistics_mode[i].lease_move = 0;
            m_statistic.statistics_mode[i].best_time  = 0;
            m_statistic.statistics_mode[i].total_time = 0;
        }
        
        
        _starPool = SpritePool::create("star_w.png", 60);
        _starPool->retain();
    }
    
    DataManager::~DataManager(){
        CC_SAFE_RELEASE(_requstData);
        CC_SAFE_RELEASE(_lastData);
    }
    
    DataManager* DataManager::getInstance(){
        if(m_instance == nullptr){
            m_instance = new DataManager();
            m_instance->init();
        }
        return m_instance;
    }
    
    bool DataManager::init(){
        bool bResult = false;
        do
        {
            m_pXml = new XMLDocument();
            if (!m_pXml) {
                return false;
            }
            if (!CCFileUtils::getInstance()->isFileExist(m_strXml))
            {
                // 没有xml文件，生成一个
                CC_BREAK_IF(!InitLocalXml());
            }
            
            bResult = ReadLoaclData();
            
            //主题数据
            //本地
            for (int i = 0; i < kThemeCount+1; i++) {
                ThemeItem *item = new ThemeItem();
                if (i == 0 && getThemeID() == 0) {
                    item->setName("cache");
                    item->setBgFile(getCacheThemeFile(true));
                    item->setPreviewFile(getCachePreviewThemeFile());
                } else {
                    item->setName(StringUtils::format("theme_%d", i));
                    item->setBgFile(StringUtils::format("shared/theme/%d.jpg", i));
                    item->setPreviewFile(StringUtils::format("shared/theme/%d.png", i));
                }
                _themes.pushBack(item);
                item->release();
            }
            //安装
            auto setupFile =  FileUtils::getInstance()->getWritablePath() + "AppSetup.plist";
            ValueVector themeVector = FileUtils::getInstance()->getValueVectorFromFile(setupFile);
            for (auto e : themeVector) {
                auto map = e.asValueMap();
                ThemeItem *item = new ThemeItem();
                item->setName(map["name"].asString());
                item->setBgFile(map["bgFile"].asString());
                item->setPreviewFile(map["previewFile"].asString());
                
                
                _themes.pushBack(item);
                _setupThemes.pushBack(item);
                item->release();
            }
            
            
        }while(0);
        return bResult;
    }
    bool DataManager::InitLocalXml(){
        // 生成xml文件
        bool bResult = false;
        do
        {
            //xml声明
            XMLDeclaration *pDel = m_pXml->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
            if (NULL==pDel) {
                return false;
            }
            m_pXml->LinkEndChild(pDel);
            //节点root
            XMLElement *rootElement = m_pXml->NewElement("root");
            rootElement->SetAttribute("version", "1.0");//给节点设置属性
            m_pXml->LinkEndChild(rootElement);
            
            /////////////////////////settings//////////////////////////
            //节点settings
            XMLElement *settingsElement = m_pXml->NewElement("settings");
            
            const char* attribute[5]={"chou3zhang","righthand","time_score","action","bigcard"};
            //默认
            int defaultValues[5] = {0,1,1,1,1};
            
            for (int i = 0; i<5; i++) {
                settingsElement->SetAttribute(attribute[i], defaultValues[i]);
            }
            rootElement->LinkEndChild(settingsElement);
            ////////////////////////////////////////////////////////////
            
            
            ////////////////////////////分数统计/////////////////////////
            //节点statistics
            XMLElement *statisticsElemet = m_pXml->NewElement("statistics");
            rootElement->LinkEndChild(statisticsElemet);
            
            const char* attribuName[6] = {"winned","played","best_score","lease_move","best_time","total_time"};
            
            //子节点card1
            //属性包括（已胜游戏，已玩游戏，最佳得分， 最少移牌，最佳时间，总时间）
            XMLElement *card1Elemet = m_pXml->NewElement("card1");
            for (int i = 0; i<6; i++) {
                card1Elemet->SetAttribute(attribuName[i], "0");
            }
            statisticsElemet->LinkEndChild(card1Elemet);
            
            //子节点card3
            //属性包括（已胜游戏，已玩游戏，最佳得分， 最少移牌，最佳时间，总时间）
            XMLElement *card3Elemet = m_pXml->NewElement("card3");
            for (int i = 0; i<6; i++) {
                card3Elemet->SetAttribute(attribuName[i], "0");
            }
            statisticsElemet->LinkEndChild(card3Elemet);
            ////////////////////////////////////////////////////////////
            
            m_pXml->SaveFile(m_strXml.c_str());//保存文件 参数：路径
            bResult = true;
        }while(0);
        return bResult;
    }
    bool DataManager::ReadLoaclData(){
        // 读取xml上的数据，到内存
        bool bResult = false;
        do
        {
            XMLError errorId = m_pXml->LoadFile(m_strXml.c_str());
            if (errorId != 0) {
                //xml格式错误
                return false;
            }
            XMLElement *rootEle = m_pXml->RootElement();
            
            //////////////////解析settings节点,并且对内存数据赋值//////////////////////////
            XMLElement *settingsEle = rootEle->FirstChildElement("settings");
            
            m_settings.m_chou3card = (ENUM_CHOU3CARD)settingsEle->IntAttribute("chou3zhang");
            m_settings.m_right_hand = (ENUM_RIGHT_HAND)settingsEle->IntAttribute("righthand");
            m_settings.m_time_score = (ENUM_TIME_SCORE)settingsEle->IntAttribute("time_score");
            m_settings.m_action = (ENUM_ACTION)settingsEle->IntAttribute("action");
            m_settings.m_big_card = (ENUM_BIG_CARD)settingsEle->IntAttribute("bigcard");
            ///////////////////////////////////////////////////////////////////
            
            //////////////////解析统计分值节点，并且对内存数据赋值/////////////////////////////////////
            XMLElement *statisticEle = rootEle->FirstChildElement("statistics");
            XMLElement *cardEle = statisticEle->FirstChildElement("card1");
            
            for(int i=0;i<2;i++){
                m_statistic.statistics_mode[i].winned = (int)atoi(cardEle->Attribute("winned"));
                m_statistic.statistics_mode[i].played = (int)atoi(cardEle->Attribute("played"));
                m_statistic.statistics_mode[i].best_score = (int)atoi(cardEle->Attribute("best_score"));
                m_statistic.statistics_mode[i].lease_move = (int)atoi(cardEle->Attribute("lease_move"));
                m_statistic.statistics_mode[i].best_time = (long)atoi(cardEle->Attribute("best_time"));
                m_statistic.statistics_mode[i].total_time = (long)atoi(cardEle->Attribute("total_time"));
                
                cardEle = cardEle->NextSiblingElement();
            }
            /////////////////////////////////////////////////////////////////////
            
            bResult = true;
        }while(0);
        return bResult;
    }
    
    void DataManager::saveSettingsData(TagSettings &settings){
        // 保存设置数据
        m_settings.m_chou3card  = settings.m_chou3card;
        m_settings.m_right_hand = settings.m_right_hand;
        m_settings.m_time_score = settings.m_time_score;
        m_settings.m_action     = settings.m_action;
        m_settings.m_big_card   = settings.m_big_card;
        
        XMLElement *rootEle = m_pXml->RootElement();
        
        //////////////////解析settings节点,并且对内存数据赋值//////////////////////////
        XMLElement *settingsEle = rootEle->FirstChildElement("settings");
        
        settingsEle->SetAttribute("chou3zhang", m_settings.m_chou3card);
        settingsEle->SetAttribute("righthand", m_settings.m_right_hand);
        settingsEle->SetAttribute("time_score", m_settings.m_time_score);
        settingsEle->SetAttribute("action", m_settings.m_action);
        settingsEle->SetAttribute("bigcard", m_settings.m_big_card);
        // 保存xml文件
        m_pXml->SaveFile(m_strXml.c_str());
    }
    
    void DataManager::resetStatisticData(){
    
        for(int i=0;i<2;i++){
            m_statistic.statistics_mode[i].winned     = 0;
            m_statistic.statistics_mode[i].played     = 0;
            m_statistic.statistics_mode[i].best_score = 0;
            m_statistic.statistics_mode[i].lease_move = 0;
            m_statistic.statistics_mode[i].best_time  = 0;
            m_statistic.statistics_mode[i].total_time = 0;
        }
        saveStatisticData(m_statistic);
    }

    void DataManager::saveStatisticData(TagStatisticAll &statisAll){
        // 保存统计数据
        for(int i=0;i<2;i++){
            m_statistic.statistics_mode[i].winned     = statisAll.statistics_mode[i].winned;
            m_statistic.statistics_mode[i].played     = statisAll.statistics_mode[i].played;
            m_statistic.statistics_mode[i].best_score = statisAll.statistics_mode[i].best_score;
            m_statistic.statistics_mode[i].lease_move = statisAll.statistics_mode[i].lease_move;
            m_statistic.statistics_mode[i].best_time  = statisAll.statistics_mode[i].best_time;
            m_statistic.statistics_mode[i].total_time = statisAll.statistics_mode[i].total_time;
        }
        
        XMLElement *rootEle = m_pXml->RootElement();
        
        char buf[5];
        
        XMLElement *statisticEle = rootEle->FirstChildElement("statistics");
        XMLElement *cardEle = statisticEle->FirstChildElement("card1");
        
        for(int i=0;i<2;i++){
            sprintf(buf,"%d",m_statistic.statistics_mode[i].winned);
            cardEle->SetAttribute("winned", buf);
            
            sprintf(buf,"%d",m_statistic.statistics_mode[i].played);
            cardEle->SetAttribute("played", buf);
            
            sprintf(buf,"%d",m_statistic.statistics_mode[i].best_score);
            cardEle->SetAttribute("best_score", buf);
            
            sprintf(buf,"%d",m_statistic.statistics_mode[i].lease_move);
            cardEle->SetAttribute("lease_move", buf);
            
            sprintf(buf,"%ld",m_statistic.statistics_mode[i].best_time);
            cardEle->SetAttribute("best_time", buf);
            
            sprintf(buf,"%ld",m_statistic.statistics_mode[i].total_time);
            cardEle->SetAttribute("total_time", buf);
            
            cardEle = cardEle->NextSiblingElement();
        }
        
        // 保存xml文件
        m_pXml->SaveFile(m_strXml.c_str());
    }
    
    const TagStatisticAll* DataManager::getStatisticAll(){
        return &m_statistic;
    }
    
    const TagSettings* DataManager::getSettings(){
        return &m_settings;
    }
}
