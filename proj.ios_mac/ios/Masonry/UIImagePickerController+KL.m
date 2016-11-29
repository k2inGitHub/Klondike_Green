//
//  UIImagePickerController+KL.m
//  Klondike
//
//  Created by 宋扬 on 16/10/31.
//
//

#import "UIImagePickerController+KL.h"
#import "KLManager.h"

@implementation UIImagePickerController (KL)

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return UIInterfaceOrientationIsLandscape( interfaceOrientation );
}

// For ios6, use supportedInterfaceOrientations & shouldAutorotate instead
- (NSUInteger) supportedInterfaceOrientations{
#ifdef __IPHONE_6_0
    NSLog(@"kforceLandscape = %d", [KLManager sharedManager].forceLandscape);
    if ([KLManager sharedManager].forceLandscape) {
        return UIInterfaceOrientationMaskLandscape;
    } else {
        return UIInterfaceOrientationMaskPortrait;
    }
    
    
#endif
}
/*
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return UIInterfaceOrientationIsLandscape( interfaceOrientation );
}

// For ios6, use supportedInterfaceOrientations & shouldAutorotate instead
- (UIInterfaceOrientationMask) supportedInterfaceOrientations{
#ifdef __IPHONE_6_0
    return UIInterfaceOrientationMaskLandscape;
#endif
}
 
 -(UIInterfaceOrientation)preferredInterfaceOrientationForPresentation{
 return UIInterfaceOrientationLandscapeLeft;//kforceLandscape? UIInterfaceOrientationLandscapeLeft : UIInterfaceOrientationPortrait;
 }
 
 */



- (BOOL) shouldAutorotate {
    return YES;
}

@end
