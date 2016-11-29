//
//  KTDate.hpp
//  Klondike
//
//  Created by 宋扬 on 16/7/16.
//
//

#ifndef KTDate_hpp
#define KTDate_hpp

#include "cocos2d.h"
using namespace cocos2d;
#include <stdio.h>
#include <time.h>
/**
 NSDate cpp版 时间戳创建日期 
 TODO:formatter 创建本地日期
 */
class KTDate : public Ref {

private:
    tm *_time;
public:
    
    KTDate(){
        _time = nullptr;
    }
    
    ~KTDate(){
//        CCLOG("~KTDate");
        if (_time) {
            delete _time;
            _time = nullptr;
        }
    };
    
    //0 equal 1 右边大 -1 右边小
    int compare(KTDate *date);
    
    //必须同一时区 
    int compareDay(KTDate *date);
    
    KTDate* dateByAddingTimeInterval(time_t interval);
    
    time_t timeIntervalSinceLastDate(KTDate *last);
    
    //local date
    KTDate* localDate();
    
    //utc date
    KTDate* utcDate();
    
    //utc date
    static KTDate* date(time_t timestamp);
    
    //utc date
    static KTDate* date(std::string string);
    
    //local system utc date
    static KTDate *date();
    
    std::string toString();
    
    std::string getTimeZone();
    
    time_t stamp();
    
    int day();
    
    int month();
};

#endif /* KTDate_hpp */
