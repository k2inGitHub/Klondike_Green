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
    
    auto fu = FileUtils::getInstance();
    
    std::string themeDir = FileUtils::getInstance()->getWritablePath() + themeName.UTF8String + "/";
    fu->createDirectory(themeDir);
    NSString *dir = [NSString stringWithUTF8String:themeDir.c_str()];
    NSString *bgFile = [dir stringByAppendingPathComponent:[NSString stringWithFormat:@"theme_bg.jpg"]];
    NSString *previewFile = [dir stringByAppendingPathComponent:[NSString stringWithFormat:@"theme_preview.png"]];
    NSString *cardbackFile = [dir stringByAppendingPathComponent:[NSString stringWithFormat:@"cardback.png"]];
    NSString *themeSetFile = [dir stringByAppendingPathComponent:[NSString stringWithFormat:@"theme_set_preview.png"]];
    NSString *dataFile = [dir stringByAppendingPathComponent:[NSString stringWithFormat:@"data.plist"]];
    NSLog(@"cardbackFile = %@", cardbackFile);

    [theme.bg writeToFile:bgFile atomically:YES];
    [theme.preview writeToFile:previewFile atomically:YES];
    if (theme.cardback) {
        [theme.cardback writeToFile:cardbackFile atomically:YES];
    }
    [theme.themeSetPreview writeToFile:themeSetFile atomically:YES];
    [theme.itemDict writeToFile:dataFile atomically:YES];
    
    if (theme.faces.count != 0) {
        std::string facesDir = themeDir + "faces/";
        fu->createDirectory(facesDir);
        for (int i = 0; i < 4; i++) {
            std::string facesSubDir = StringUtils::format("%s%d/", facesDir.c_str(), i);
            fu->createDirectory(facesSubDir);
            NSMutableArray *subArr = theme.faces[i];
            for (int j = 0; j < 13; j++) {
                NSData *data = subArr[j];
                std::string file = StringUtils::format("%s%d.png",facesSubDir.c_str(), j+1);
                [data writeToFile:[NSString stringWithUTF8String:file.c_str()] atomically:YES];
            }
        }
    }
    
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
