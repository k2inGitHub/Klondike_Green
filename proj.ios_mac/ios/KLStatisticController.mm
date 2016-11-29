//
//  KLStatisticController.m
//  Klondike
//
//  Created by 宋扬 on 16/7/7.
//
//

#import "KLStatisticController.h"
#import "KLStatisticCell.h"
#include "DataManager.h"
#include "Global.h"
using namespace NSDataManage;

@interface KLStatisticController ()<UITableViewDelegate, UITableViewDelegate>

@property (nonatomic, weak) IBOutlet UITableView *tableView;

@property (nonatomic, strong) NSMutableArray *dataArray;

@end

@implementation KLStatisticController

- (void)addRightItem{

//    UIBarButtonItem *negativeSpacer = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFixedSpace target:nil action:nil];
    
//    negativeSpacer.width = -11;
    
//    UIButton *btn = [KTUIFactory customButtonWithImage:@"back-icon" frame:CGRectMake(0, 0, 30, 30) title:nil titleFont:nil titleColor:nil titleSize:0 tag:0];
//    [btn addTarget:self action:@selector(back) forControlEvents:UIControlEventTouchUpInside];
    
    UIBarButtonItem *item = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemRefresh target:self action:@selector(reset)];
    
    self.navigationItem.rightBarButtonItems = @[item];
}

- (void)reset{
    [UIAlertView showWithTitle:LocalizedNSString(@"TID_UI_DO YOU REALLY WANT TO RESET YOUR STATISTICS?") message:nil cancelButtonTitle:LocalizedNSString(@"TID_UI_CANCEL") otherButtonTitles:@[LocalizedNSString(@"TID_UI_CONFIRM")] tapBlock:^(UIAlertView * _Nonnull alertView, NSInteger buttonIndex) {
        if(buttonIndex == 1){
            
            DataManager::getInstance()->resetStatisticData();
            [self reloadData];
        }
    }];
    
}

- (void)reloadData{
    
    auto st = *DataManager::getInstance()->getStatisticAll();
    auto sm1 = st.statistics_mode[0];
    auto sm2 = st.statistics_mode[1];
    NSArray *arr = @[@{@"title" : LocalizedNSString(@"TID_UI_GAMES WON"), @"left" : @(sm1.winned), @"right" : @(sm2.winned)},
                     @{@"title" : LocalizedNSString(@"TID_UI_GAMES PLAYED"), @"left" : @(sm1.played), @"right" : @(sm2.played)},
                     @{@"title" : LocalizedNSString(@"TID_UI_HIGH SCORE"), @"left" : @(sm1.best_score), @"right" : @(sm2.best_score)},
                     @{@"title" : LocalizedNSString(@"TID_UI_FEWEST WINNING MOVES"), @"left" : @(sm1.lease_move), @"right" : @(sm2.lease_move)},
                     @{@"title" : LocalizedNSString(@"TID_UI_SHORTEST WINNING TIME"), @"left" : @(sm1.best_time), @"right" : @(sm2.best_time)},
                     @{@"title" : LocalizedNSString(@"TID_UI_TOTAL"), @"left" : @(sm1.total_time), @"right" : @(sm2.total_time)},
                     ];
    _dataArray = [NSMutableArray arrayWithArray:arr];
    [_tableView reloadData];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    [self addBackItem];
    [self addRightItem];
    [self reloadData];
}

- (void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    self.navigationController.navigationBarHidden = NO;
    [self setNavTitle:LocalizedNSString(@"TID_UI_STATISTICS")];
}

- (void)viewWillDisappear:(BOOL)animated{
    [super viewWillDisappear:animated];
    
}

#pragma mark - TableView -

//- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
//
//{
//    if (indexPath.row == 6) {
////        [UIAlertView showWithTitle:@"游戏规则" message:self.gameRuleString cancelButtonTitle:@"确定" otherButtonTitles:nil tapBlock:nil];
//    }
//}

- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
{
    return 60;
}

- (UIView*)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section
{
    return [tableView dequeueReusableCellWithIdentifier:@"HeadCell"];
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    return 50;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return [_dataArray count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    return [tableView dequeueReusableCellWithIdentifier:statisticCellIdentifier];
}

- (void)tableView:(UITableView *)tableView willDisplayCell:(UITableViewCell *)cell forRowAtIndexPath:(NSIndexPath *)indexPath {
    NSParameterAssert([cell isKindOfClass:[KLStatisticCell class]]);
    __weak KLStatisticCell *homeCell = (KLStatisticCell *)cell;
    
    __weak NSMutableDictionary *dic = _dataArray[indexPath.row];
    NSNumber *left = dic[@"left"];
    NSNumber *right = dic[@"right"];
    homeCell.middleLabel.text = dic[@"title"];
    if (indexPath.row > 3) {
        homeCell.leftLabel.text = [NSString stringWithFormat:@"%.2d:%.2d", [left intValue]/60, [left intValue]%60];
        homeCell.rightLabel.text = [NSString stringWithFormat:@"%.2d:%.2d", [right intValue]/60, [right intValue]%60];
    } else {
        homeCell.leftLabel.text = [NSString stringWithFormat:@"%@", left];
        homeCell.rightLabel.text = [NSString stringWithFormat:@"%@", right];
    }
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
