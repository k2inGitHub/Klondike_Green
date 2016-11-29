//
//  KLBaseController.m
//  Klondike
//
//  Created by 宋扬 on 16/7/7.
//
//

#import "KLBaseController.h"

@interface KLBaseController ()

@end

@implementation KLBaseController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}

- (void)addBackItem{
    UIBarButtonItem *negativeSpacer = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFixedSpace target:nil action:nil];
    
    negativeSpacer.width = -11;
    
    UIButton *btn = [KTUIFactory customButtonWithImage:@"back-icon" frame:CGRectMake(0, 0, 30, 30) title:nil titleFont:nil titleColor:nil titleSize:0 tag:0];
    [btn addTarget:self action:@selector(back) forControlEvents:UIControlEventTouchUpInside];
    self.navigationItem.leftBarButtonItems = @[negativeSpacer, [[UIBarButtonItem alloc] initWithCustomView:btn]];
}

- (void)back{
    [self.navigationController popViewControllerAnimated:YES];
}

- (void)setNavTitle:(NSString *)string{
    
    if (self.tabBarController != nil) {
        self.tabBarController.navigationItem.title = string;
    } else {
        self.navigationItem.title = string;
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
