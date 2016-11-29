//
//  Cell_HighScore.hpp
//  Klondike
//
//  Created by 宋扬 on 16/7/21.
//
//

#ifndef Cell_HighScore_hpp
#define Cell_HighScore_hpp

#include "KTLayer.h"
#include "Cell_Setting.hpp"

class Cell_HighScore : public Cell_Base{
    Node *_line;
public:
    
    virtual Widget* createCloneInstance() override;
    virtual void updateLayout() override;
    
    CC_SYNTHESIZE(Button *, _rankButton, RankButton);
    
    CC_SYNTHESIZE(Label *, _titleLabel, TitleLabel);
    
    CC_SYNTHESIZE(Label *, _timeLabel, TimeLabel);
    
    virtual bool init()override;
    
    CREATE_FUNC(Cell_HighScore);
};

#endif /* Cell_HighScore_hpp */
