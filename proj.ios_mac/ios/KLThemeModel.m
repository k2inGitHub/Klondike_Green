//
//  ThemeModel.m
//  ThemeAccess
//
//  Created by 宋扬 on 16/11/16.
//  Copyright © 2016年 handloft. All rights reserved.
//

#import "KLThemeModel.h"
#import <MJExtension.h>

@implementation KLTheme

MJExtensionCodingImplementation

@end

@implementation KLThemeModel

MJExtensionCodingImplementation

- (instancetype)init{
    self = [super init];
    if (!self) {
        return nil;
    }
    self.themeArray = [[NSMutableArray alloc] init];
    
    return self;
}

//增加 如果存在 更新
- (void)insert:(KLTheme *)theme{
    KLTheme *t = [self find:theme.name];
    if(t != nil){
        //更新
        NSUInteger idx = [self.themeArray indexOfObject:t];
        [self.themeArray replaceObjectAtIndex:idx withObject:theme];
    } else {
        [self.themeArray addObject:theme];
    }
}
//查找
- (KLTheme *)find:(NSString *)name{
    for (KLTheme *theme in self.themeArray) {
        
        if ([theme.name isEqualToString:name]) {
            return theme;
        }
    }
    return nil;
}

@end
