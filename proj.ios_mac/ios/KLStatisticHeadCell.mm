//
//  KLStatisticHeadCell.m
//  Klondike
//
//  Created by 宋扬 on 16/7/11.
//
//

#import "KLStatisticHeadCell.h"
#import "AppController.h"

@implementation KLStatisticHeadCell

- (void)awakeFromNib{
    _leftLabel.text = LocalizedNSString(@"TID_UI_1 CARD");
    _rightLabel.text = LocalizedNSString(@"TID_UI_3 CARDS");
}

@end
