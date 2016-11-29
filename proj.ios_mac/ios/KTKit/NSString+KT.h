//
//  NSString+KT.h
//  QZ
//
//  Created by 宋扬 on 16/6/15.
//  Copyright © 2016年 handloft. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NSString (KT)
+ (NSString *)JSONStringFromDict:(id)object;
+ (float) heightForString:(NSString *)value fontSize:(float)fontSize andWidth:(float)width;
@end
