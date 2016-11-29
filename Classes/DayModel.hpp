//
//  DayModel.hpp
//  Klondike
//
//  Created by 宋扬 on 16/7/19.
//
//

#ifndef DayModel_hpp
#define DayModel_hpp

#include "KTLayer.h"

class DayModel : public Ref {
    
public:
    CC_SYNTHESIZE_PASS_BY_REF(int, _day, Day);
    
    CC_SYNTHESIZE_PASS_BY_REF(bool, _awards, Awards);
    
    CC_SYNTHESIZE_PASS_BY_REF(bool, _isGetToday, IsGetToday);
    
};

#endif /* DayModel_hpp */
