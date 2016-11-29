//
//  HighScoreManager.cpp
//  Klondike
//
//  Created by 宋扬 on 16/7/21.
//
//

#include "HighScoreManager.hpp"

HighScoreManager* HighScoreManager::getInstance(){
    static HighScoreManager*mgr = nullptr;
    if (!mgr) {
        mgr = new HighScoreManager();
        mgr->init();
    }
    return mgr;
}

bool HighScoreManager::addHighScore(HighScoreModel *m){

    bool ret = false;
    if (_dataArray.size() < 7 ) {
        _dataArray.pushBack(m);
        ret = true;
    } else {
        if (m->getScore() > _dataArray.back()->getScore()) {
            _dataArray.replace(_dataArray.size() - 1, m);
        }
        ret = true;
    }
    
    if (ret == true) {
        std::sort(_dataArray.begin(), _dataArray.end(), [](HighScoreModel *a, HighScoreModel *b){
            return a->getScore() > b->getScore();
        });
    }
    
    return ret;
}

void HighScoreManager::loadData(){

    auto fu = FileUtils::getInstance();
    auto array = fu->getValueVectorFromFile(fu->getWritablePath() + "/localHishScore.plist");
    for(auto e : array){
        
        HighScoreModel *m = HighScoreModel::create();
        
        auto dic = e.asValueMap();
        m->setScore(dic["score"].asInt());
        m->setTimeString(dic["time"].asString());
        _dataArray.pushBack(m);
    }
}

void HighScoreManager::saveData(){

    ValueVector v;
    for(auto e : _dataArray){
    
        ValueMap dic;
        dic["score"] = Value(e->getScore());
        dic["time"] = Value(e->getTimeString());
        v.push_back(Value(dic));
    }
    FileUtils::getInstance()->writeValueVectorToFile(v, FileUtils::getInstance()->getWritablePath() + "/localHishScore.plist");
}

bool HighScoreManager::init(){
    
    loadData();
    
    
    
    return true;
}