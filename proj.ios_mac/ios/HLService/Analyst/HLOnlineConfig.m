//
//  HLOnlineConfig.m
//  Klondike
//
//  Created by 宋扬 on 16/7/13.
//
//

#import "HLOnlineConfig.h"
#import "AFNetworking.h"
#import "HLADDeviceHelper.h"
#import "NSString+HLAD.h"

@implementation HLOnlineConfig

static NSString* const onlineConfigURL = @"http://45.113.69.21/ao2/online_config.php";
static bool _logEnabled = true;

static NSMutableDictionary *_dict = nil;

+ (NSMutableDictionary *)onlineConfig{
    if (_dict == nil) {
        _dict = [[NSMutableDictionary alloc] init];
    }
    return _dict;
};

+ (void)updateOnlineConfig{

    
    AFHTTPSessionManager *manager = [AFHTTPSessionManager manager];
    //    manager.responseSerializer = [AFHTTPResponseSerializer serializer];
    manager.responseSerializer.acceptableContentTypes = [NSSet setWithObjects:@"text/html", @"application/json", nil];
    manager.requestSerializer.timeoutInterval = 15;
    NSURLSessionDataTask *task = [manager GET:onlineConfigURL parameters:@{@"content":[NSString JSONStringFromObject:@{@"app_bundle_id":[HLADDeviceHelper bundleName],@"app_version" : [HLADDeviceHelper bundleShortVersion]}]} progress:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject) {
        
        if ([responseObject count]!=0) {
           
            NSMutableDictionary *dic = [self onlineConfig];
            [dic removeAllObjects];
            [dic setDictionary:responseObject];
            [[NSUserDefaults standardUserDefaults] setObject:dic forKey:@"HL_OnlineConfig"];
            [[NSUserDefaults standardUserDefaults]synchronize];
            
        } else {
            NSMutableDictionary *dic = [self onlineConfig];
            [dic removeAllObjects];
            [dic setDictionary:[[NSUserDefaults standardUserDefaults] objectForKey:@"HL_OnlineConfig"]];
        }
        if (_logEnabled) {
            NSLog(@"HL online config data = %@", responseObject);
        }
        
    } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error) {
        if (_logEnabled) {
            NSLog(@"HL online error = %@", error);
        }
        NSMutableDictionary *dic = [self onlineConfig];
        [dic removeAllObjects];
        [dic setDictionary:[[NSUserDefaults standardUserDefaults] objectForKey:@"HL_OnlineConfig"]];
    }];
    if (_logEnabled) {
        NSLog(@"task = %@", task.originalRequest.URL);
    }
}

+ (NSDictionary *)getConfigParams{
    return [self onlineConfig];
}
+ (NSString *)getConfigParams:(NSString *)key{
    return [[self onlineConfig] objectForKey:key];
}

+ (void)setLogEnabled:(BOOL)value{
    _logEnabled = value;
}

@end
