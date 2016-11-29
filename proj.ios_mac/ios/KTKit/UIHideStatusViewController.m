//
//  UIHideStatusViewController.m
//  DBD
//
//  Created by 宋扬 on 16/7/5.
//  Copyright © 2016年 handloft. All rights reserved.
//

#import "UIHideStatusViewController.h"

@implementation UIHideStatusViewController

- (UIStatusBarStyle)preferredStatusBarStyle
{
    return UIStatusBarStyleLightContent;
    //UIStatusBarStyleDefault = 0 黑色文字，浅色背景时使用
    //UIStatusBarStyleLightContent = 1 白色文字，深色背景时使用
}

- (BOOL)prefersStatusBarHidden
{
    return YES; //返回NO表示要显示，返回YES将hiden
}

@end
