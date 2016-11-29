//
//  MonthModel.hpp
//  Klondike
//
//  Created by 宋扬 on 16/7/16.
//
//

#ifndef MonthModel_hpp
#define MonthModel_hpp

#include "KTLayer.h"
#include "DayModel.hpp"


static const char *monthIDs[] = {"TID_UI_JAN", "TID_UI_FEB", "TID_UI_MAR", "TID_UI_APR", "TID_UI_MAY", "TID_UI_JUN", "TID_UI_JUL", "TID_UI_AUG", "TID_UI_SEP", "TID_UI_OCT", "TID_UI_NOV", "TID_UI_DEC"};

class MonthModel : public Ref {
    
public:
    
    int getMonthValue(){
    
        
        int v = atoi(_month.substr(5,2).c_str());
        return v;
    }
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _month, Month);
    
    CC_SYNTHESIZE_PASS_BY_REF(int, _days, Days);
    
    CC_SYNTHESIZE_PASS_BY_REF(int, _dayTotal, DayTotal);
    
    CC_SYNTHESIZE_PASS_BY_REF(int, _firstDayOfWeek, FirstDayOfWeek);
    
    CC_SYNTHESIZE_PASS_BY_REF(Vector<DayModel *>, _challengedDays, ChallengedDays);
    
    std::string getMonthString(){
        struct tm tm = {0};
        strptime(_month.c_str(), "%Y-%m", &tm);
//        char str[64];
//        sprintf(str, "%d月", tm.tm_mon+1);
        
        return LocalizedString(monthIDs[tm.tm_mon]);
    };
    std::string getMonthDayString(int day){
        struct tm tm = {0};
        strptime(_month.c_str(), "%Y-%m", &tm);
        char str[64];
        sprintf(str, "TID_CODE_DATE%d", day);
        sprintf(str, "%s %s",LocalizedString(monthIDs[tm.tm_mon]).c_str(), LocalizedString(str).c_str());
        return str;
    };
};

#endif /* MonthModel_hpp */
