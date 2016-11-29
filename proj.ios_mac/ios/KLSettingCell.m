//
//  KLSettingCell.m
//  Klondike
//
//  Created by 宋扬 on 16/7/7.
//
//

#import "KLSettingCell.h"

@implementation KLSettingCell

- (IBAction)onSWValueChange:(id)sender{
    
    if (_onSwitch) {
        _onSwitch(self);
    }
}

- (IBAction)onControlChange:(id)sender{

    if (_onControl) {
        _onControl(self);
    }
}

- (void)awakeFromNib {
    [super awakeFromNib];
    // Initialization code
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

@end
