//
//  Cell_Statistics.hpp
//  Klondike
//
//  Created by 宋扬 on 16/7/22.
//
//

#ifndef Cell_Statistics_hpp
#define Cell_Statistics_hpp

#include "KTLayer.h"
#include "Cell_Setting.hpp"

class Cell_Statistics : public Cell_Base{
    
    Node* _line;
    
public:
    
    virtual Widget* createCloneInstance() override;
    virtual void updateLayout() override;
    
//    CC_SYNTHESIZE(Button *, _rankButton, RankButton);
    
    CC_SYNTHESIZE(Text *, _titleLabel, TitleLabel);
    
    CC_SYNTHESIZE(Text *, _timeLabel, TimeLabel);
    
    virtual bool init() override;
    
    CREATE_FUNC(Cell_Statistics);
};

#endif /* Cell_Statistics_hpp */
