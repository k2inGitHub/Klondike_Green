//
//  KLEditThemeController.m
//  Klondike
//
//  Created by 宋扬 on 16/10/31.
//
//

#import "KLEditThemeController.h"
#import "Masonry.h"
#import "PhotoTweaksViewController.h"
#import "UIImagePickerController+KL.h"
#import "UIImage+KT.h"
#import "KLManager.h"
//#import "AppController.h"
//#include "DataManager.h"

@interface KLEditThemeController () <PhotoTweaksViewControllerDelegate>

@property (nonatomic, strong) IBOutlet UIImageView *imgView1;

@property (nonatomic, strong) IBOutlet UIImageView *imgView2;

@property (nonatomic, strong) IBOutlet UIButton *touchButton1;

@property (nonatomic, strong) IBOutlet UIButton *touchButton2;

@property (nonatomic, strong) IBOutlet UILabel *label1;

@property (nonatomic, strong) IBOutlet UILabel *label2;

@property (nonatomic, weak) IBOutlet UIButton *submitBtn;

@property (nonatomic, assign) CGSize winSize;

@property (nonatomic, assign) float aspectRatio;

@property (nonatomic, assign) CGSize cropImageSize;

@property (nonatomic, assign) BOOL isVertical;

@end

@implementation KLEditThemeController

- (void)photoTweaksController:(PhotoTweaksViewController *)controller didFinishWithCroppedImage:(UIImage *)croppedImage
{
    croppedImage = [UIImage imageWithImage:croppedImage scaledToSize:    CGSizeMake(self.cropImageSize.width * 2, self.cropImageSize.height * 2)];
    
    if (_isVertical) {
        self.imgView1.image = croppedImage;
    } else {
        self.imgView2.image = croppedImage;
    }
    
    //    [controller.navigationController popViewControllerAnimated:YES];
    [controller.navigationController popViewControllerAnimated:YES];

}

- (void)photoTweaksControllerDidCancel:(PhotoTweaksViewController *)controller
{
    [controller.navigationController popViewControllerAnimated:YES];
    //    [self dismissViewControllerAnimated:YES completion:nil];
}

- (IBAction)onRecieve:(id)sender{
    NSString *pPath = [[KLManager sharedManager]getThemeCachePath:YES];
    NSString *lPath = [[KLManager sharedManager]getThemeCachePath:NO];
    NSString *sPath = [[KLManager sharedManager]getThemePreviewCachePath];
    UIImage *preview = [UIImage imageWithImage:self.imgView1.image scaledToSize:CGSizeMake(78, 124)];
    if ([UIImageJPEGRepresentation(self.imgView1.image,1) writeToFile:pPath atomically:YES] && [UIImageJPEGRepresentation(self.imgView2.image,1) writeToFile:lPath atomically:YES] && [UIImagePNGRepresentation(preview) writeToFile:sPath atomically:YES]){
        [[KLManager sharedManager] onThemeFileChanged];
        [self dismissViewControllerAnimated:YES completion:nil];
    }
}

- (void)onCancel{
    [[KLManager sharedManager] onThemeFileCanceled];
    [self.navigationController.presentingViewController dismissViewControllerAnimated:YES completion:nil];
    
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    self.navigationController.navigationBarHidden = NO;
    [KLManager sharedManager].forceLandscape = NO;
    [[UIDevice currentDevice] setValue:@(UIInterfaceOrientationPortrait) forKey:@"orientation"];
}

- (IBAction)onVertical:(id)sender{

    self.isVertical = YES;
    
    [KLManager sharedManager].forceLandscape = NO;
    [[UIDevice currentDevice] setValue:@(UIInterfaceOrientationPortrait) forKey:@"orientation"];
    
    PhotoTweaksViewController *photoTweaksViewController = [[PhotoTweaksViewController alloc] initWithImage:_sourceImg];
    photoTweaksViewController.delegate = self;
    photoTweaksViewController.autoSaveToLibray = NO;
    photoTweaksViewController.maxRotationAngle = M_PI;
    self.cropImageSize = CGSizeMake(320.f, 320.f * self.aspectRatio);

    photoTweaksViewController.cropSize = _cropImageSize;
    photoTweaksViewController.cancelButtonTitleColor = [UIColor whiteColor];
    photoTweaksViewController.resetButtonTitleColor = [UIColor whiteColor];
    photoTweaksViewController.saveButtonTitleColor = [UIColor whiteColor];
    photoTweaksViewController.sliderTintColor = [UIColor whiteColor];
    
    
    UIImagePickerController *picker = (UIImagePickerController*)self.navigationController;
    
    [picker pushViewController:photoTweaksViewController animated:YES];
}

- (IBAction)onHorizon:(id)sender{

    self.isVertical = NO;
    
    [KLManager sharedManager].forceLandscape = YES;
    [[UIDevice currentDevice] setValue:@(UIInterfaceOrientationLandscapeLeft) forKey:@"orientation"];
    PhotoTweaksViewController *photoTweaksViewController = [[PhotoTweaksViewController alloc] initWithImage:_sourceImg];
    photoTweaksViewController.delegate = self;
    photoTweaksViewController.autoSaveToLibray = NO;
    photoTweaksViewController.maxRotationAngle = M_PI;
    self.cropImageSize = CGSizeMake(320.f * self.aspectRatio, 320.f);
    photoTweaksViewController.cropSize = _cropImageSize;
    
    photoTweaksViewController.cancelButtonTitleColor = [UIColor whiteColor];
    photoTweaksViewController.resetButtonTitleColor = [UIColor whiteColor];
    photoTweaksViewController.saveButtonTitleColor = [UIColor whiteColor];
    photoTweaksViewController.sliderTintColor = [UIColor whiteColor];
    [self.navigationController pushViewController:photoTweaksViewController animated:YES];
}

- (UIImage *)cropImage:(UIImage *)image frame:(CGRect)clippedRect
{
    // Crop logic
    CGImageRef imageRef = CGImageCreateWithImageInRect([image CGImage], clippedRect);
    UIImage * croppedImage = [UIImage imageWithCGImage:imageRef];
    CGImageRelease(imageRef);
    return croppedImage;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    
    UIBarButtonItem *btnBack = [[UIBarButtonItem alloc]
                                initWithTitle:[KLManager getLocalization:@"TID_UI_BACK"]
                                style:UIBarButtonItemStylePlain
                                target:self
                                action:nil];
    self.navigationController.navigationBar.topItem.backBarButtonItem=btnBack;
    
    
    UIBarButtonItem *cancel = [[UIBarButtonItem alloc] initWithTitle:[KLManager getLocalization:@"TID_UI_CANCEL"] style:UIBarButtonItemStylePlain target:self action:@selector(onCancel)];
    
    self.label1.text = [KLManager getLocalization:@"TID_UI_EDIT_PORTRT"];
    self.label2.text = [KLManager getLocalization:@"TID_UI_EDIT_LANDSCAPE"];
    [self.submitBtn setTitle:[KLManager getLocalization:@"TID_UI_CONFIRM"] forState:UIControlStateNormal];
    
    self.navigationItem.rightBarButtonItem = cancel;
    
    // Do any additional setup after loading the view.
    _imgView1.image = _sourceImg;
    _imgView2.image = _sourceImg;
    CGRect frame = [[UIScreen mainScreen]bounds];
    self.aspectRatio = frame.size.height/frame.size.width;
    NSLog(@"frame = %@", NSStringFromCGRect(frame));
    self.winSize = frame.size;
    
    
    float imgAspRat = _sourceImg.size.height/_sourceImg.size.width;
    if (imgAspRat > _aspectRatio) {
        float w = _sourceImg.size.width;
        float h = w * _aspectRatio;
        frame = CGRectMake(0, (_sourceImg.size.height-h)*0.5, w, h);
    } else {
        float h = _sourceImg.size.height;
        float w = h/_aspectRatio;
        frame = CGRectMake((_sourceImg.size.width-w)*0.5, 0, w, h);
    }
    UIImage *pImg = [self cropImage:self.sourceImg frame:frame];
    pImg = [UIImage imageWithImage:pImg scaledToSize:CGSizeMake(640.f, 640.f * self.aspectRatio)];
    self.imgView1.image = pImg;
    
    if (imgAspRat > 1/_aspectRatio) {
        float w = _sourceImg.size.width;
        float h = w / _aspectRatio;
        frame = CGRectMake(0, (_sourceImg.size.height-h)*0.5, w, h);
    } else {
        float h = _sourceImg.size.height;
        float w = h*_aspectRatio;
        frame = CGRectMake((_sourceImg.size.width-w)*0.5, 0, w, h);
    }
    {
        UIImage *pImg = [self cropImage:self.sourceImg frame:frame];
        pImg = [UIImage imageWithImage:pImg scaledToSize:CGSizeMake(640.f * self.aspectRatio, 640.f)];
        self.imgView2.image = pImg;
    }
    
    
    //layout
    [self.imgView1 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.height.equalTo(self.self.imgView1.mas_width).multipliedBy(self.aspectRatio);
        
        make.width.and.height.lessThanOrEqualTo(self.touchButton1);
        make.width.and.height.equalTo(self.touchButton1).with.priorityLow();
        
        make.center.equalTo(self.touchButton1);
    }];
    
    [self.imgView2 mas_makeConstraints:^(MASConstraintMaker *make) {
        
        make.width.equalTo(self.imgView2.mas_height).multipliedBy(self.aspectRatio);
        
        make.width.and.height.lessThanOrEqualTo(self.touchButton2);
        make.width.and.height.equalTo(self.touchButton2).with.priorityLow();
        
        make.center.equalTo(self.touchButton2);
    }];
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
