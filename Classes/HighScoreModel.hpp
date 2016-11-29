//
//  HighScoreModel.hpp
//  Klondike
//
//  Created by 宋扬 on 16/7/21.
//
//

#ifndef HighScoreModel_hpp
#define HighScoreModel_hpp

#include "KTLayer.h"
#include "KTDate.hpp"

class HighScoreModel : public Ref {
    
public:
    
    CC_SYNTHESIZE_RETAIN(KTDate *, _chanllengeDate, ChallengeDate);
    
    CC_SYNTHESIZE(int, _time, Time);
    
    CC_SYNTHESIZE(int, _moveNum, MoveNum);
    
    CC_SYNTHESIZE(std::string, _timeString, TimeString);
    
    CC_SYNTHESIZE(int, _score, Score);
    
    
    ~HighScoreModel();
    virtual bool init();
    
    CREATE_FUNC(HighScoreModel);
};

#endif /* HighScoreModel_hpp */
