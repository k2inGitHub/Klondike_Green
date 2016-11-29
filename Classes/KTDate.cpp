//
//  KTDate.cpp
//  Klondike
//
//  Created by 宋扬 on 16/7/16.
//
//

#include "KTDate.hpp"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//time_t
//timegm(struct tm *const tm)
//{
//    time_t ret;
//    char *tz;
//    
//    tz = getenv("TZ");
//    setenv("TZ", "", 1);
//    tzset();
//    ret = mktime(tm);
//    if (tz)
//        setenv("TZ", tz, 1);
//    else
//        unsetenv("TZ");
//    tzset();
//    return ret;
//}

int get_utc_offset() {
    
    time_t zero = 24*60*60L;
    struct tm * timeptr;
    int gmtime_hours;
    
    /* get the local time for Jan 2, 1900 00:00 UTC */
    timeptr = localtime( &zero );
    gmtime_hours = timeptr->tm_hour;
    
    /* if the local time is the "day before" the UTC, subtract 24 hours
     from the hours to get the UTC offset */
    if( timeptr->tm_mday < 2 )
        gmtime_hours -= 24;
    
    return gmtime_hours;
    
}

time_t
timegm(struct tm *const tm)
{
    return mktime( tm ) + get_utc_offset() * 3600;
}
#endif

time_t KTDate::timeIntervalSinceLastDate(KTDate *last){
    struct tm tmLeft = *_time;
    struct tm tmRight = *last->_time;
    time_t left = timegm(&tmLeft);
    time_t right = timegm(&tmRight);
    return difftime(left, right);
}

int KTDate::compareDay(KTDate *date){
    
    CCASSERT(_time->tm_gmtoff == date->_time->tm_gmtoff, "fuck");
    
    struct tm l = *_time;
    struct tm r = *date->_time;
    if (l.tm_year == r.tm_year && l.tm_mon == r.tm_mon && l.tm_mday == r.tm_mday) {
        return 0;
    }
    if (r.tm_year > l.tm_year || r.tm_mon > l.tm_mon || r.tm_mday > l.tm_mday) {
        return 1;
    }
    return -1;
}

int KTDate::compare(KTDate *date){
    
    struct tm tmLeft = *_time;
    struct tm tmRight = *date->_time;
    time_t left = timegm(&tmLeft);
    time_t right = timegm(&tmRight);
    double ret = difftime(left, right);
    if (ret < 0) {
        return 1;
    }
    if (ret > 0) {
        return -1;
    }
    return 0;
}

//local date
KTDate* KTDate::localDate(){
    
    time_t st = stamp();
    
    struct tm *localTime = localtime(&st);
    
    struct tm *time = new tm();
    *time = *localTime;
    
    KTDate *date = new KTDate();
    date->_time = time;
    date->autorelease();
    return date;
}

//utc date
KTDate* KTDate::utcDate(){
//    struct tm t = *_time;
    
    time_t st = stamp();

    struct tm *gmtTime = gmtime(&st);
    struct tm *time = new tm();
    *time = *gmtTime;
    
    KTDate *date = new KTDate();
    date->_time = time;
    date->autorelease();
    return date;
}

int KTDate::day(){
    return (*_time).tm_mday;
}

int KTDate::month(){

    return (*_time).tm_mon + 1;
}

time_t KTDate::stamp(){
    
    struct tm t = *_time;
    time_t stamp = 0;
    if (t.tm_gmtoff == 0) {
        stamp = timegm(&t);
    } else {
        stamp=mktime(&t);
    }
    return stamp;
}

std::string KTDate::getTimeZone(){
    
    return (*_time).tm_zone;
}

KTDate* KTDate::dateByAddingTimeInterval(time_t interval){
    
    
    time_t st = stamp();
    st += interval;
    struct tm *gmtTime = gmtime(&st);
    struct tm *time = new tm();
    *time = *gmtTime;
    
    KTDate *date = new KTDate();
    date->_time = time;
    date->autorelease();
    return date;
}

KTDate* KTDate::date(time_t timestamp){
    time_t unix_timestamp = timestamp;
    struct tm *gmtTime = gmtime(&unix_timestamp);
    
//    char str[80];
//    strftime(str, 80, "%Y-%m-%d %H:%M:%S%Z", gmtTime);
//    CCLOG("stamp init str = %s", str);
    
    struct tm *time = new tm();
    *time = *gmtTime;
    
    KTDate *date = new KTDate();
    date->_time = time;
    date->autorelease();
    return date;
}

KTDate *KTDate::date(){
    time_t t = time(0);
    struct tm *gmtTime = gmtime(&t);
    struct tm *time = new tm();
    *time = *gmtTime;

    KTDate *date = new KTDate();
    date->_time = time;
    date->autorelease();
    return date;
}

KTDate* KTDate::date(std::string string){
//    auto str = string.c_str();
    
    char str[64];
    sprintf(str, "%s", string.c_str());
    struct tm tm = {0};
    strptime(str, "%Y-%m-%d %H:%M:%S", &tm);
    tm.tm_gmtoff = 0;
    tm.tm_zone = "UTC";
    
    struct tm *time = new struct tm;
    (*time) = tm;
    
    KTDate *date = new KTDate();
    date->_time = time;
    date->autorelease();
    
    return date;
}

std::string KTDate::toString(){
    
    char str[80];
    
    time_t stamp = 0;
    
    
    struct tm t = *_time;
    if (_time->tm_gmtoff == 0) {
        stamp = timegm(&t);
        struct tm *gmtTime0 = gmtime(&stamp);
        strftime(str, 80, "%Y-%m-%d %H:%M:%S", gmtTime0);
    } else {
        stamp = mktime(&t);
        struct tm *gmtTime0 = localtime(&stamp);
        strftime(str, 80, "%Y-%m-%d %H:%M:%S", gmtTime0);
    }
    
    
//    CCLOG("stamp = %ld", stamp);
//    CCLOG("s1 = %s", str);
    
//    time_t stamp2 = stamp + _time->tm_gmtoff;
//    
//    struct tm *gmtTime = gmtime(&stamp2);
//    strftime(str, 80, "%Y-%m-%d %H:%M:%S%Z", gmtTime);
//
//    CCLOG("off = %ld", _time->tm_gmtoff);
//    CCLOG("s2 = %s", str);
    
    return str;
}