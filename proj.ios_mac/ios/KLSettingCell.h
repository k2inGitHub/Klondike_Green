//
//  KLSettingCell.h
//  Klondike
//
//  Created by 宋扬 on 16/7/7.
//
//

#import <UIKit/UIKit.h>

static NSString *const homeCellIdentifier = @"KLSettingCell";

@class KLSettingCell;

typedef void (^KLSettingCellBlock) (KLSettingCell * cell);

@interface KLSettingCell : UITableViewCell

@property (nonatomic, weak) IBOutlet UILabel *titleLabel;

@property (nonatomic, weak) IBOutlet UISwitch *sw;

@property (nonatomic, weak) IBOutlet UISegmentedControl *segControl;

@property (nonatomic, weak) IBOutlet UIImageView *arrow;

@property (nonatomic, copy) KLSettingCellBlock onSwitch;

@property (nonatomic, copy) KLSettingCellBlock onControl;


@end
