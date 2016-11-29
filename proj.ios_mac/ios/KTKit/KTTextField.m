//
//  KTTextField.m
//  DBD
//
//  Created by 宋扬 on 16/6/30.
//  Copyright © 2016年 handloft. All rights reserved.
//

#import "KTTextField.h"

@implementation KTTextField

- (void)setLeftImage:(UIImage *)leftImage
{
    UIImageView *imageView = [[UIImageView alloc] initWithImage:leftImage];
    imageView.contentMode = UIViewContentModeCenter;
    float wd = imageView.frame.size.width;
    float sp = (self.frame.size.height - wd)/2;
    imageView.frame = CGRectMake(sp, sp, wd, wd);
    self.leftViewMode = UITextFieldViewModeAlways;
    self.leftView = imageView;
}

- (UIImage *)leftImage{
    UIImageView *imageView = (UIImageView *)self.leftView;
    if (imageView != nil) {
        return imageView.image;
    }
    return nil;
}

@end
