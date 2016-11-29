//
//  KLManager.h
//  Klondike
//
//  Created by 宋扬 on 16/10/31.
//
//

#import <Foundation/Foundation.h>

@interface KLManager : NSObject

+ (instancetype)sharedManager;

@property (nonatomic, assign) BOOL forceLandscape;

@property (nonatomic, assign) BOOL isStart;

- (NSString *)getThemeCachePath:(BOOL)portrait;

- (NSString *)getThemePreviewCachePath;

- (void)onThemeFileChanged;

- (void)onThemeFileCanceled;

+ (NSString *)getLocalization:(NSString *)key;

-(BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url;

@end
