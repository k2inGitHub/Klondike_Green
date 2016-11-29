//
//  HighScoreManager.hpp
//  Klondike
//
//  Created by 宋扬 on 16/7/21.
//
//

#ifndef HighScoreManager_hpp
#define HighScoreManager_hpp

#include "KTLayer.h"
#include "HighScoreModel.hpp"

class HighScoreManager :public Ref {
    
public:
    
    bool addHighScore(HighScoreModel *m);
    
    void loadData();
    
    void saveData();
    
    CC_SYNTHESIZE_PASS_BY_REF(Vector<HighScoreModel *>, _dataArray, DataArray);
    
    virtual bool init();
    
    static HighScoreManager* getInstance();
    
//    CREATE_FUNC(HighScoreManager);
};

#endif /* HighScoreManager_hpp */
