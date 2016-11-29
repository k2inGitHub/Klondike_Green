//
//  UITextField+KTAddition.m
//  DBD
//
//  Created by 宋扬 on 16/7/1.
//  Copyright © 2016年 handloft. All rights reserved.
//

#import "UITextField+KTAddition.h"

@implementation UITextField (KTAddition)

- (BOOL)checkNumWithMax:(int)max{
    if ([self getNum] > max) {
        return NO;
    }
    return YES;
}

- (BOOL)checkNumWithMin:(int)min{
    if ([self getNum] < min) {
        return NO;
    }
    return YES;
}

- (void)setNum:(int)num{
    self.text = [NSString stringWithFormat:@"%d", num];
}

- (void)addNum:(int)add{
    int value = [self getNum] + add;
    if (value <= 1) {
        value = 1;
    }
    [self setNum:value];
}

- (int)getNum{
    return [self.text intValue];
}

@end
