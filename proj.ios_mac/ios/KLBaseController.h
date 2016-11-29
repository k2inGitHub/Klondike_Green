//
//  KLBaseController.h
//  Klondike
//
//  Created by 宋扬 on 16/7/7.
//
//

#import <UIKit/UIKit.h>
#import "KTUIFactory.h"
#import "UIAlertView+Blocks.h"
#import "HLService.h"
#import "AppController.h"

@interface KLBaseController : UIViewController

- (void)addBackItem;

- (void)setNavTitle:(NSString *)string;

- (void)back;

@end
