//
//  KLManager.m
//  Klondike
//
//  Created by 宋扬 on 16/10/31.
//
//

#import "KLManager.h"
#include "cocos2d.h"
#include "DataManager.h"
#include "Global.h"
#import "AppController.h"
#import "KLThemeModel.h"
#import "KLConfig.h"
#include "AppDelegate.h"

using namespace cocos2d;
using namespace NSDataManage;
using namespace NSGlobal;

@implementation KLManager

- (NSString *)themefilePath{
    NSURL *sharedContainerURL = [[NSFileManager defaultManager] containerURLForSecurityApplicationGroupIdentifier:kAppGroupIdentifier];
    return [[sharedContainerURL URLByAppendingPathComponent:kThemeFileName] path];
}

-(BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url{

//    if (!url) {  return NO; }
    NSString *URLString = [url absoluteString];
    if (![URLString containsString:kOpenURLPrefix]) {
        return NO;
    }
    NSLog(@"handleOpenURL = %@",URLString);
    //[[NSUserDefaults standardUserDefaults] setObject:URLString forKey:@"url"];
    //[[NSUserDefaults standardUserDefaults] synchronize];
    
    KLThemeModel *tm = [NSKeyedUnarchiver unarchiveObjectWithFile:self.themefilePath];
    if (tm == nil) {
        return NO;
    }
    NSString *themeName = [URLString substringFromIndex:[kOpenURLPrefix length]];
    if (themeName.length == 0) {
        return NO;
    }
    KLTheme *theme = [tm find:themeName];
    if (theme == nil) {
        return NO;
    }
//    self.imageView.image = [UIImage imageWithData:theme.bg];
    
    NSString *dir = [NSString stringWithUTF8String:FileUtils::getInstance()->getWritablePath().c_str()];
    NSString *bgFile = [dir stringByAppendingPathComponent:[NSString stringWithFormat:@"%@_bg.jpg", themeName]];
    NSString *previewFile = [dir stringByAppendingPathComponent:[NSString stringWithFormat:@"%@_preview.png", themeName]];
    
    [theme.bg writeToFile:bgFile atomically:YES];
    [theme.preview writeToFile:previewFile atomically:YES];
    
    if (!self.isStart) {
        UserDefault::getInstance()->setStringForKey("KL_NeedSetupTheme", themeName.UTF8String);
    } else {
        __NotificationCenter::getInstance()->postNotification(kOnThemeSetup, __String::create(themeName.UTF8String));

    }
    
//    NSFileManager *mgr = [NSFileManager defaultManager];
    
    
    
    return YES;
}

+ (NSString *)getLocalization:(NSString *)key{
    return LocalizedNSString(key);
}

- (void)onThemeFileCanceled{
    __NotificationCenter::getInstance()->postNotification(kOnThemeFileCanceled);

}

- (void)onThemeFileChanged{
    
    __NotificationCenter::getInstance()->postNotification(kOnThemeFileChanged);
}

- (NSString *)getThemePreviewCachePath{
    return [NSString stringWithUTF8String:DataManager::getInstance()->getCachePreviewThemeFile().c_str()];

}

- (NSString *)getThemeCachePath:(BOOL)portrait{

    auto ret = FileUtils::getInstance()->getWritablePath() + "theme";
    if (portrait) {
        ret += "_portrait.jpg";
    } else {
        ret += "_landscape.jpg";
    }
    return [NSString stringWithUTF8String:ret.c_str()];
}

+ (instancetype)sharedManager{
    static KLManager *_instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        if (_instance == nil) {
            _instance = [[KLManager alloc]init];
        }
    });
    return _instance;
}



@end
