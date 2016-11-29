//
//  KLStatisticCell.h
//  Klondike
//
//  Created by 宋扬 on 16/7/7.
//
//

#import <UIKit/UIKit.h>

static NSString *const statisticCellIdentifier = @"KLStatisticCell";


@interface KLStatisticCell : UITableViewCell

@property (nonatomic, weak) IBOutlet UILabel *leftLabel;

@property (nonatomic, weak) IBOutlet UILabel *middleLabel;

@property (nonatomic, weak) IBOutlet UILabel *rightLabel;


@end
