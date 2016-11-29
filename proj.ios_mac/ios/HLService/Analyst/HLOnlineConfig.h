//
//  HLOnlineConfig.h
//  Klondike
//
//  Created by 宋扬 on 16/7/13.
//
//

#import <Foundation/Foundation.h>

@interface HLOnlineConfig : NSObject

+ (void)updateOnlineConfig;

+ (NSDictionary *)getConfigParams;

+ (NSString *)getConfigParams:(NSString *)key;

+ (void)setLogEnabled:(BOOL)value;

@end
