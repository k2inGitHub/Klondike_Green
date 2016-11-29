//
//  UITextField+KTAddition.h
//  DBD
//
//  Created by 宋扬 on 16/7/1.
//  Copyright © 2016年 handloft. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UITextField (KTAddition)

- (BOOL)checkNumWithMax:(int)max;

- (BOOL)checkNumWithMin:(int)min;

- (void)setNum:(int)num;

- (void)addNum:(int)add;

- (int)getNum;

@end
