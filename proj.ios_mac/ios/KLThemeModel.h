//
//  ThemeModel.h
//  ThemeAccess
//
//  Created by 宋扬 on 16/11/16.
//  Copyright © 2016年 handloft. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface KLTheme : NSObject

@property (nonatomic, copy) NSString *name;

@property (nonatomic, strong) NSData *bg;

@property (nonatomic, strong) NSData *preview;

@property (nonatomic, strong) NSData *cardback;

@property (nonatomic, strong) NSData *themeSetPreview;

@property (nonatomic, strong) NSMutableArray *faces;

@property (nonatomic, strong) NSDictionary *itemDict;

@end

@interface KLThemeModel : NSObject

@property (nonatomic, strong) NSMutableArray *themeArray;
//增加
- (void)insert:(KLTheme *)theme;
//查找
- (KLTheme *)find:(NSString *)name;

@end
