////
////  KLSettingController.m
////  Klondike
////
////  Created by 宋扬 on 16/7/7.
////
////
//
//#import "KLSettingController.h"
//#import "KLSettingCell.h"
//#include "DataManager.h"
//#include "Global.h"
//#include "Audio.h"
//#include "KLPopupLayer.hpp"
//#include "SolitaireKlondikeScene.h"
//
//using namespace NSDataManage;
//
//@interface KLSettingController () <UITableViewDelegate, UITableViewDataSource>
//
//@property (nonatomic, weak) IBOutlet UITableView *tableView;
//
//@property (nonatomic, strong) NSMutableArray *dataArray;
//
////@property (nonatomic, copy) NSString *gameRuleString;
//
//@end
//
//@implementation KLSettingController
//
//- (void)viewDidLoad {
//    [super viewDidLoad];
//    
//    
//    [self addBackItem];
//    
////    NSString *path = [[NSBundle mainBundle] pathForResource:@"ConfigData" ofType:@"plist"];
////    NSDictionary *configData = [NSDictionary dictionaryWithContentsOfFile:path];
////    self.gameRuleString = configData[@"gameRule"];
//    
//    
//    auto dataMgr = DataManager::getInstance();
//    auto tagSettings = dataMgr->getSettings();
//    
//    
//    NSArray *arr = @[[@{@"title":LocalizedNSString(@"TID_UI_DRAW3"), @"button" : @"sw", @"value" : @((int)tagSettings->m_chou3card)} mutableCopy],
//                     [@{@"title":LocalizedNSString(@"TID_UI_LEFT HANDED"), @"button" : @"sw", @"value" : @((int)tagSettings->m_right_hand)}mutableCopy],
//                     [@{@"title":LocalizedNSString(@"TID_UI_TIMER MODE"), @"button" : @"sw", @"value" : @((int)tagSettings->m_time_score)}mutableCopy],
//                    
//                     [@{@"title":LocalizedNSString(@"TID_UI_PLAY_SPEED"), @"button" : @"seg", @"value" : @((int)tagSettings->m_action)}mutableCopy],
//                     [@{@"title":LocalizedNSString(@"TID_UI_CARD FACES"), @"button" : @"sw", @"value" : @((int)tagSettings->m_big_card)}mutableCopy],
//                     [@{@"title":LocalizedNSString(@"TID_UI_STATISTICS"), @"button" : @"none", @"value" : @(0)}mutableCopy],
//                     [@{@"title":LocalizedNSString(@"TID_UI_RULESRULES"), @"button" : @"none", @"value" : @(0)}mutableCopy],
//                     [@{@"title":LocalizedNSString(@"TID_UI_SOUND"), @"button" : @"sw", @"value" : @(dataMgr->isSoundOn())}mutableCopy],
//                     [@{@"title":LocalizedNSString(@"TID_UI_VEGAS MODE"), @"button" : @"sw", @"value" : @(dataMgr->isVegasOn())}mutableCopy],
//                     [@{@"title":LocalizedNSString(@"TID_UI_VEGAS CUMULATIVE"), @"button" : @"sw", @"value" : @(dataMgr->isVegasScoreAdd())}mutableCopy],];
//    
//    _dataArray = [NSMutableArray arrayWithArray:arr];
//    
//    
//    NSDate *currentData = [NSDate date];
//    NSTimeInterval t = [currentData timeIntervalSince1970];
//    NSLog(@"utc date = %@", currentData);
//    NSLog(@"t = %f", t);
//}
//
//- (void)viewWillAppear:(BOOL)animated{
//    [super viewWillAppear:animated];
//    self.navigationController.navigationBarHidden = NO;
//    [self setNavTitle:LocalizedNSString(@"TID_UI_SETTINGS")];
//    [[HLAdManager sharedInstance] hideBanner];
//}
//
//- (void)back{
//    [super back];
////    [[HLAdManager sharedInstance] showBanner];
//}
//
//- (void)viewWillDisappear:(BOOL)animated{
//    [super viewWillDisappear:animated];
//    __NotificationCenter::getInstance()->postNotification(NSGlobal:: NTFFactor::NTF_SETTING_REFRESH);
//
//}
//
//#pragma mark - TableView -
//
//- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
//{
//    if (indexPath.row == 6) {
//        NSString *txt = LocalizedNSString(@"TID_UI_SCORE PAGE BASIC");
//        UIAlertView *alertView = [UIAlertView showWithTitle:LocalizedNSString(@"TID_UI_RULESRULES") message:nil cancelButtonTitle:LocalizedNSString(@"TID_UI_CONFIRM") otherButtonTitles:nil tapBlock:nil];
//        UILabel *textLabel = [[UILabel alloc] init];
//        textLabel.font = [UIFont systemFontOfSize:13];
//        textLabel.numberOfLines =0;
//        textLabel.textAlignment =NSTextAlignmentLeft;
//        textLabel.text = txt;
//        [alertView setValue:textLabel forKey:@"accessoryView"];
////        for(UIView *subview in alertView.subviews)
////        {
////            if([[subview class] isSubclassOfClass:[UILabel class]])
////            {
////                UILabel *label = (UILabel*)subview;
////                if([label.text isEqualToString:txt])
////                    label.textAlignment = NSTextAlignmentLeft;
////            }  
////        }
//    } else if (indexPath.row == 5) {
//        UIViewController *controller = [[UIStoryboard storyboardWithName:@"Main" bundle:nil] instantiateViewControllerWithIdentifier:@"KLStatisticController"];
//        [self.navigationController pushViewController:controller animated:YES];
//    }
//}
//
//- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
//    return 50;
//}
//
//- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
//    return [_dataArray count];
//}
//
//- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
//    
//    return [tableView dequeueReusableCellWithIdentifier:homeCellIdentifier];
//}
//
//- (void)tableView:(UITableView *)tableView willDisplayCell:(UITableViewCell *)cell forRowAtIndexPath:(NSIndexPath *)indexPath {
//    NSParameterAssert([cell isKindOfClass:[KLSettingCell class]]);
//    __weak KLSettingCell *homeCell = (KLSettingCell *)cell;
//    
//    __weak NSMutableDictionary *dic = _dataArray[indexPath.row];
//    
//    homeCell.titleLabel.text = dic[@"title"];
////    homeCell.sw.hidden = YES;
////    homeCell.segControl.hidden = YES;
//    NSString *btn = dic[@"button"];
//    NSNumber *value = dic[@"value"];
//    if ([btn isEqualToString:@"sw"]) {
//        homeCell.sw.hidden = NO;
//        homeCell.segControl.hidden = YES;
//        homeCell.arrow.hidden = YES;
////        homeCell.accessoryType = UITableViewCellAccessoryNone;
//        homeCell.sw.on = [value boolValue];
//    } else if ([btn isEqualToString:@"seg"]){
//        homeCell.sw.hidden = YES;
//        homeCell.segControl.hidden = NO;
//        homeCell.arrow.hidden = YES;
////        homeCell.accessoryType = UITableViewCellAccessoryNone;
//        homeCell.segControl.selectedSegmentIndex = [value integerValue];
//    } else {
//        homeCell.sw.hidden = YES;
//        homeCell.segControl.hidden = YES;
//        homeCell.arrow.hidden = NO;
////        homeCell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
//    }
//    
//    __weak KLSettingController *controller = self;
//    homeCell.onControl = ^(KLSettingCell *newcell) {
//        
//        int idx = (int)[controller.tableView indexPathForCell:newcell].row;
//        NSMutableDictionary *dict = controller.dataArray[idx];
//        [dict setObject:@(newcell.segControl.selectedSegmentIndex) forKey:@"value"];
//        auto instance = DataManager::getInstance();
//        TagSettings tagSettings = *instance->getSettings();
//        tagSettings.m_action = (ENUM_ACTION)newcell.segControl.selectedSegmentIndex;
//        instance->saveSettingsData(tagSettings);
//    };
//    
//    homeCell.onSwitch = ^(KLSettingCell *newcell){
//        int idx = (int)[controller.tableView indexPathForCell:newcell].row;
//        NSMutableDictionary *dict = controller.dataArray[idx];
//        int value = newcell.sw.on ? 1 : 0;
//        [dict setObject:@(value) forKey:@"value"];
//        DataManager *instance = DataManager::getInstance();
//        
//        if (idx == 7) {
//            instance->setSoundOn((bool)newcell.sw.on);
//            if(!newcell.sw.on){
//                Audio::getInstance()->pauseEffects();
//                Audio::getInstance()->pauseMusic();
//            }else{
//                Audio::getInstance()->resumeEffects();
//                Audio::getInstance()->resumeMusic();
//            }
//
//        } else if (idx == 8) {
//            bool flag = newcell.sw.on;
//            instance->setVegasOn(flag);
//            bool flag2 = getGameScene()->getIsVegasOn();
//            if (flag2 != flag) {
//                if (flag2) {
//                    
//                    
//                    [UIAlertView showWithTitle:nil message:@"你关闭了维加斯积分，是否立刻开始一局标准积分游戏?" cancelButtonTitle:@"取消" otherButtonTitles:@[@"重开一局"] tapBlock:^(UIAlertView * _Nonnull alertView, NSInteger buttonIndex) {
//                        if (buttonIndex == 1) {
//                            __NotificationCenter::getInstance()->postNotification(NTFFactor::NTF_NEW_GAME);
//                            [self back];
//                        }
//                    }];
//                    
////                    vector<string> btns;
////                    btns.push_back("重开一局");
////                    KLAlertLayer::show("", "你关闭了维加斯积分，是否立刻开始一局标准积分游戏?", "继续本局", btns, [=](KLAlertLayer *layer, int buttonIdx){
////                        if(buttonIdx == 1) {
////                            __NotificationCenter::getInstance()->postNotification(NTFFactor::NTF_NEW_GAME);
////                            [self back];
////                        }
////                    });
//                } else {
//                    [UIAlertView showWithTitle:nil message:@"你开启了维加斯积分，是否立刻开始一局维加斯积分游戏?" cancelButtonTitle:@"取消" otherButtonTitles:@[@"重开一局"] tapBlock:^(UIAlertView * _Nonnull alertView, NSInteger buttonIndex) {
//                        if (buttonIndex == 1) {
//                            
//                            __NotificationCenter::getInstance()->postNotification(NTFFactor::NTF_NEW_GAME);
//                            [self back];
//                        }
//                    }];
////                    vector<string> btns;
////                    btns.push_back("重开一局");
////                    KLAlertLayer::show("", "你开启了维加斯积分，是否立刻开始一局维加斯积分游戏?", "继续本局", btns, [=](KLAlertLayer *layer, int buttonIdx){
////                        if(buttonIdx == 1) {
////                            __NotificationCenter::getInstance()->postNotification(NTFFactor::NTF_NEW_GAME);
////                            [self back];
////                        }
////                    });
//                }
//            }
//            
//        } else if (idx == 9) {
//            instance->setVegasScoreAdd(newcell.sw.on);
//        } else {
//            TagSettings tagSettings = *instance->getSettings();
//            int v = (newcell.sw.on ? 1 : 0);
//            if (idx == 0) {
//                tagSettings.m_chou3card = (ENUM_CHOU3CARD)v;
//            } else if (idx == 1) {
//                tagSettings.m_right_hand = (ENUM_RIGHT_HAND)v;
//            } else if (idx == 2) {
//                tagSettings.m_time_score = (ENUM_TIME_SCORE)v;
//            } else if (idx == 4) {
//                tagSettings.m_big_card = (ENUM_BIG_CARD)v;
//            }
//            instance->saveSettingsData(tagSettings);
//        }
//        
//        
//    };
//}
//
//- (void)didReceiveMemoryWarning {
//    [super didReceiveMemoryWarning];
//    // Dispose of any resources that can be recreated.
//}
//
///*
//#pragma mark - Navigation
//
//// In a storyboard-based application, you will often want to do a little preparation before navigation
//- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
//    // Get the new view controller using [segue destinationViewController].
//    // Pass the selected object to the new view controller.
//}
//*/
//
//@end
