//
//  NSString+KT.m
//  QZ
//
//  Created by 宋扬 on 16/6/15.
//  Copyright © 2016年 handloft. All rights reserved.
//

#import "NSString+KT.h"

@implementation NSString (KT)
+ (NSString *)JSONStringFromDict:(id)object {
    NSError *error;
    NSData *result = [NSJSONSerialization dataWithJSONObject:object options:0 error:&error];
//    if (error) {
//        NSLog(@"Error occered when JSON deserializate NSData to object: %@", error);
//        return nil;
//    }
    return [[NSString alloc] initWithData:result encoding:NSUTF8StringEncoding];
}

+ (float) heightForString:(NSString *)value fontSize:(float)fontSize andWidth:(float)width{
    UITextView *detailTextView = [[UITextView alloc]initWithFrame:CGRectMake(0, 0, width, 0)];
    detailTextView.font = [UIFont systemFontOfSize:fontSize];
    detailTextView.text = value;
    CGSize deSize = [detailTextView sizeThatFits:CGSizeMake(width,CGFLOAT_MAX)];
    return deSize.height;
}

@end
