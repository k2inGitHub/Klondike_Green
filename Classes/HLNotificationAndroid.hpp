//
//  VungleWrapper.h
//  PowersOfTwo
//
//  Created by Freeman on 14-4-19.
//
//

#ifndef HLNOTIFICATIONANDROID_H_
#define HLNOTIFICATIONANDROID_H_


class HLNotificationAndroid{
    
public:
    static HLNotificationAndroid * getInstance();
    void startGet();
    void addNotification(const char* msg_content, const char* msg_sending_time, int weekID, int msg_id);
};

#endif
