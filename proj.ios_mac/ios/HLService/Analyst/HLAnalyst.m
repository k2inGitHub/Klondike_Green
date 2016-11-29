//
//  HLAnalyst.m
//  VPN-iOS
//
//  Created by 宋扬 on 16/2/28.
//  Copyright © 2016年 handloft. All rights reserved.
//

#import "HLAnalyst.h"
#import "UMmobClick/MobClick.h"

#import "HLInterface.h"
#import "HLOnlineConfig.h"
#if USE_OVERSEAS_AD
#import "Flurry.h"
#import "Firebase.h"
#import <AppsFlyer/AppsFlyer.h>
#else
#import "UMOnlineConfig.h"
#endif

@implementation HLAnalyst

+ (void)event:(NSString *)event{
    [MobClick event:event];
#if USE_OVERSEAS_AD
    [Flurry logEvent:event];
    if ([self useFirebase]) {
        [FIRAnalytics logEventWithName:event parameters:nil];
    }
    
    [[AppsFlyerTracker sharedTracker] trackEvent:event withValues:@{@"value" : @"1"}];
    
#endif
}

+ (void)event:(NSString *)event withValue:(NSString *)value{

    [[AppsFlyerTracker sharedTracker] trackEvent:event withValues:@{@"value" : value}];
}

+ (BOOL)useFirebase{
    return [HLInterface sharedInstance].firebase_Switch == 1;
}

+ (void)start{
    HLInterface *hlInterface = [HLInterface sharedInstance];
    
    UMConfigInstance.appKey = hlInterface.umeng_code;
    UMConfigInstance.channelId = hlInterface.umeng_Channel;
//    [MobClick startWithAppkey:hlInterface.umeng_code reportPolicy:REALTIME channelId:hlInterface.umeng_Channel];
    [MobClick startWithConfigure:UMConfigInstance];
    
#if USE_OVERSEAS_AD
    [Flurry startSession:[HLInterface sharedInstance].flurry_key];
    //!!! Firebase Key 都写在配置文件里 无法在线更改 每次打包需要去后台下载配置文件GoogleService-Info.plist 并替换
    if ([self useFirebase]) {
        [FIRApp configure];
    }
    
    if([hlInterface.flyerDevKey isEqualToString:@""]||hlInterface.flyerDevKey == nil){
    }
    else{
        [AppsFlyerTracker sharedTracker].appsFlyerDevKey = hlInterface.flyerDevKey;
        [AppsFlyerTracker sharedTracker].appleAppID = hlInterface.flyerAppID;
        [[AppsFlyerTracker sharedTracker] trackAppLaunch];
    }
    
    
    [HLOnlineConfig updateOnlineConfig];
#else
    [UMOnlineConfig updateOnlineConfigWithAppkey:hlInterface.umeng_code];
    [UMOnlineConfig setLogEnabled:NO];
#endif
    
}

+ (NSString *)stringValue:(NSString *)key{
    return [self stringValue:key defaultValue:nil];
}
+ (NSString *)stringValue:(NSString *)key defaultValue:(NSString *)value{
#if USE_OVERSEAS_AD
    NSString *p = [HLOnlineConfig getConfigParams:key];
#else
    NSString *p = [UMOnlineConfig getConfigParams:key];
#endif
    if ([p length] > 0) {
        return p;
    }
    return value;
}

+ (int)intValue:(NSString *)key{
    return [self intValue:key defaultValue:0];
}

+ (int)intValue:(NSString *)key defaultValue:(int)value{
#if USE_OVERSEAS_AD
    NSString *p = [HLOnlineConfig getConfigParams:key];
#else
    NSString *p = [UMOnlineConfig getConfigParams:key];
#endif
    if ([p length] > 0) {
        return [p intValue];
    }
    return value;
}

+ (BOOL)boolValue:(NSString *)key{
    return [self boolValue:key defaultValue:NO];
}

+ (BOOL)boolValue:(NSString *)key defaultValue:(BOOL)value{
#if USE_OVERSEAS_AD
    NSString *p = [HLOnlineConfig getConfigParams:key];
#else
    NSString *p = [UMOnlineConfig getConfigParams:key];
#endif
    if ([p length] > 0) {
        return [p isEqualToString:@"1"];
    }
    return value;
}

+ (float)floatValue:(NSString *)key{
    return [self floatValue:key defaultValue:0];
}

+ (float)floatValue:(NSString *)key defaultValue:(float)value{
#if USE_OVERSEAS_AD
    NSString *p = [HLOnlineConfig getConfigParams:key];
#else
    NSString *p = [UMOnlineConfig getConfigParams:key];
#endif
    if ([p length] > 0) {
        return [p floatValue];
    }
    return value;
}

@end
