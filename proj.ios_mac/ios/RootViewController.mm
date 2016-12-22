/****************************************************************************
 Copyright (c) 2013      cocos2d-x.org
 Copyright (c) 2013-2014 Chukong Technologies Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#import "RootViewController.h"
#import "cocos2d.h"
#import "platform/ios/CCEAGLView-ios.h"
#import "KLSettingController.h"
#include "DataManager.h"
#include "Global.h"
#include "iOSWrapper.hpp"
#include "KTLayer.h"
#include "AppDelegate.h"
#import "PhotoTweaksViewController.h"
#import "UIImage+KT.h"
#import "KLEditThemeController.h"
#import "UIImagePickerController+KL.h"
#import "AppController.h"

using namespace NSDataManage;

@interface RootViewController () <UIActionSheetDelegate, UIImagePickerControllerDelegate, UINavigationControllerDelegate, PhotoTweaksViewControllerDelegate>

@property (nonatomic, assign) UIInterfaceOrientation lastInterfaceOrientation;

@property (nonatomic, retain) UIPopoverController *popoverController;

@end

@implementation RootViewController


 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
        
        [UIDevice currentDevice].batteryMonitoringEnabled = YES;
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onBatteryChanged) name:UIDeviceBatteryLevelDidChangeNotification object:nil];
        
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(toSettingController) name:@"toSettingController" object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(showNewGameSheet) name:@"showNewGameSheet" object:nil];
    }

    return self;
}


/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/


- (void)onBatteryChanged{
    __NotificationCenter::getInstance()->postNotification(kBatteryChanged);
}

- (void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    
    self.navigationController.navigationBarHidden = YES;
    [self updateLayoutForNewOrientation];
}

- (void)viewDidAppear:(BOOL)animated{
    [super viewDidAppear:animated];
    
}

- (void)toSettingController{
    
    UIViewController *controller = [[UIStoryboard storyboardWithName:@"Main" bundle:nil] instantiateViewControllerWithIdentifier:@"KLSettingController"];
    [self.navigationController pushViewController:controller animated:YES];
}

- (void)showNewGameSheet{
    UIActionSheet *sheet = [[UIActionSheet alloc]initWithTitle:nil delegate:self cancelButtonTitle:LocalizedNSString(@"TID_UI_CANCEL") destructiveButtonTitle:nil otherButtonTitles:LocalizedNSString(@"TID_CODE_NEW GAME"), LocalizedNSString(@"TID_UI_REPLAY"), LocalizedNSString(@"TID_UI_WINNING DEAL"), nil];
    [sheet showInView:self.view];
    [sheet release];
}

- (void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex{
    
    if (buttonIndex == 0) {
        cocos2d::__NotificationCenter::getInstance()->postNotification(NSGlobal:: NTFFactor::NTF_NEW_GAME);

    } else if (buttonIndex == 1) {
        cocos2d::__NotificationCenter::getInstance()->postNotification(NSGlobal:: NTFFactor::NTF_REPEATE_GAME);

    } else if (buttonIndex == 2) {
        cocos2d::__NotificationCenter::getInstance()->postNotification(NSGlobal:: NTFFactor::NTF_NEW_GAME_WINNALBE);

    }
}

// Override to allow orientations other than the default portrait orientation.
// This method is deprecated on ios6
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return UIInterfaceOrientationIsLandscape( interfaceOrientation );
}

// For ios6, use supportedInterfaceOrientations & shouldAutorotate instead
- (NSUInteger) supportedInterfaceOrientations{
#ifdef __IPHONE_6_0
    return UIInterfaceOrientationMaskAllButUpsideDown;
#endif
}

- (BOOL) shouldAutorotate {
    return YES;
}

- (void) updateLayoutForNewOrientation{
    
    
    
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    
    if (glview)
    {
        CCEAGLView *eaglview = (__bridge CCEAGLView *)glview->getEAGLView();
        
        if (eaglview)
        {
            UIInterfaceOrientation currentOrient = [UIApplication  sharedApplication].statusBarOrientation;
            
            if(self.lastInterfaceOrientation != currentOrient){
                AppDelegate* app = (AppDelegate *) cocos2d::Application::getInstance();
                bool flag = currentOrient == UIInterfaceOrientationPortrait;
                app->onScreenSizeChanged(flag);
                self.lastInterfaceOrientation = currentOrient;
            }
        }
    }
}

- (void)willAnimateRotationToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration{
    
    [super willAnimateRotationToInterfaceOrientation:toInterfaceOrientation duration:duration];
    
    [self updateLayoutForNewOrientation];
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
    [super didRotateFromInterfaceOrientation:fromInterfaceOrientation];

//    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
//
//    if (glview)
//    {
//        CCEAGLView *eaglview = (CCEAGLView*) glview->getEAGLView();
//
//        if (eaglview)
//        {
//            
//            UIInterfaceOrientation currentOrient = [UIApplication  sharedApplication].statusBarOrientation;
//
//            AppDelegate* app = (AppDelegate *) cocos2d::Application::getInstance();
//            bool flag = currentOrient == UIInterfaceOrientationPortrait;
//            app->onScreenSizeChanged(flag);
//            __NotificationCenter::getInstance()->postNotification(kScreenSizeChangedNotification);
//            
//        }
//    }
}

//fix not hide status on ios7
- (BOOL)prefersStatusBarHidden
{
    return YES;
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];

    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (void)presentImagePickerWithSourceType:(UIImagePickerControllerSourceType)sourceType sender:(id)sender
{
    
    UIImagePickerController *picker = [[[UIImagePickerController alloc] init] autorelease];
    picker.sourceType = sourceType;
    picker.allowsEditing = NO;
    picker.delegate = self;
//    picker.cropMode = DZNPhotoEditorViewControllerCropModeCircular;
    
//    picker.finalizationBlock = ^(UIImagePickerController *picker, NSDictionary *info) {
//        
//        // Dismiss when the crop mode was disabled
//        if (picker.cropMode == DZNPhotoEditorViewControllerCropModeNone) {
//            [weakSelf handleImagePicker:picker withMediaInfo:info];
//        }
//    };
    
//    picker.cancellationBlock = ^(UIImagePickerController *picker) {
//        [weakSelf dismissController:picker];
//    };
    
    [self presentController:picker sender:sender];
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker{
    if (self.curPickerType == PickerTypeCardback){
        __NotificationCenter::getInstance()->postNotification(kOnCardbackFileCanceled);
    } else {
        __NotificationCenter::getInstance()->postNotification(kOnThemeFileCanceled);
    }

    [self dismissViewControllerAnimated:YES completion:nil];
}

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    UIImage *image = [info objectForKey:UIImagePickerControllerOriginalImage];
    if (self.curPickerType == PickerTypeCardback) {
        
        PhotoTweaksViewController *photoTweaksViewController = [[PhotoTweaksViewController alloc] initWithImage:image];
        photoTweaksViewController.delegate = self;
        photoTweaksViewController.autoSaveToLibray = NO;
        photoTweaksViewController.maxRotationAngle = M_PI;
        photoTweaksViewController.cropSize = CGSizeMake(cardbackImageSize.width, cardbackImageSize.height);
        photoTweaksViewController.cancelButtonTitleColor = [UIColor whiteColor];
        photoTweaksViewController.resetButtonTitleColor = [UIColor whiteColor];
        photoTweaksViewController.saveButtonTitleColor = [UIColor whiteColor];
        photoTweaksViewController.sliderTintColor = [UIColor whiteColor];
        [picker pushViewController:photoTweaksViewController animated:YES];
    } else {
        KLEditThemeController *vc = [[UIStoryboard storyboardWithName:@"Main" bundle:[NSBundle mainBundle]] instantiateViewControllerWithIdentifier:@"KLEditThemeController"];
        vc.sourceImg = image;
        [picker pushViewController:vc animated:YES];
    }
}

- (void)photoTweaksController:(PhotoTweaksViewController *)controller didFinishWithCroppedImage:(UIImage *)croppedImage
{
    croppedImage = [UIImage imageWithImage:croppedImage scaledToSize:CGSizeMake(cardbackImageSize.width, cardbackImageSize.height)];
    NSString *pngPath = [NSString stringWithUTF8String:DataManager::getInstance()->getCacheCardbackFile().c_str()];
    if ([UIImagePNGRepresentation(croppedImage) writeToFile:pngPath atomically:YES]){
        [self dismissViewControllerAnimated:YES completion:nil];

        __NotificationCenter::getInstance()->postNotification(kOnCardbackFileChanged);
    }
//    [controller.navigationController popViewControllerAnimated:YES];
}

- (void)photoTweaksControllerDidCancel:(PhotoTweaksViewController *)controller
{
    [controller.navigationController popViewControllerAnimated:YES];
//    [self dismissViewControllerAnimated:YES completion:nil];
}

- (void)presentController:(UIViewController *)controller sender:(id)sender
{
    if (_popoverController.isPopoverVisible) {
        [_popoverController dismissPopoverAnimated:YES];
        self.popoverController = nil;
    }
    
//    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
//        controller.preferredContentSize = CGSizeMake(320.0, 520.0);
//        
//        _popoverController = [[UIPopoverController alloc] initWithContentViewController:controller];
//        [_popoverController presentPopoverFromBarButtonItem:sender permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
//    }
//    else
    {
        [self presentViewController:controller animated:YES completion:NULL];
    }
}

- (void)dealloc {
    [super dealloc];
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}


@end
