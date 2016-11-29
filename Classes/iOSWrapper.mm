//
//  iOSWrapper.cpp
//  Klondike
//
//  Created by 宋扬 on 16/7/7.
//
//

#include "iOSWrapper.hpp"
#import <UIKit/UIKit.h>
#import "AppController.h"
#import "MBProgressHUD.h"
#import "HLADDeviceHelper.h"
#import "HLService.h"
#include "Localization.hpp"
#import "RootViewController.h"
#import "KLManager.h"

void iOSWrapper::setStart(bool value){
    [KLManager sharedManager].isStart = value;
}

void iOSWrapper::showMoreTheme(){

}

void iOSWrapper::showThemePicker(){
    RootViewController *vc = (RootViewController *)getCustomViewController();
    vc.curPickerType = PickerTypeTheme;
    [vc presentImagePickerWithSourceType:UIImagePickerControllerSourceTypePhotoLibrary sender:nil];
}

void iOSWrapper::showImagePicker(){
    RootViewController *vc = (RootViewController *)getCustomViewController();
    vc.curPickerType = PickerTypeCardback;
    [vc presentImagePickerWithSourceType:UIImagePickerControllerSourceTypePhotoLibrary sender:nil];
}

std::string iOSWrapper::updateUrl(){

    auto str = [HLInterface sharedInstance].itunes_updated_url;
    return str.length > 0 ? str.UTF8String : "";
}

bool iOSWrapper::isForceUpdate(){
    return [HLInterface sharedInstance].itunes_is_forced == 1;
}

std::string iOSWrapper::updateVersion(){
    auto str = [HLInterface sharedInstance].latest_version;
    return str.length > 0 ? str.UTF8String : "";
}

bool iOSWrapper::isUpdateEnable(){
    return [HLInterface sharedInstance].itunes_update_ctrl_switch == 1;
}

std::string iOSWrapper::updateContent(){
    auto str = [HLInterface sharedInstance].itunes_update_content;
    return str.length > 0 ? str.UTF8String : "";
}

std::string iOSWrapper::getVersionName(){

    NSDictionary *infoDict = [[NSBundle mainBundle] infoDictionary];
    NSString *appVersion = [infoDict objectForKey:@"CFBundleShortVersionString"];
    return appVersion.UTF8String;
}

bool iOSWrapper::isScreenPortrait(){

    UIInterfaceOrientation currentOrient = [UIApplication sharedApplication].statusBarOrientation;
    bool flag = currentOrient == UIInterfaceOrientationPortrait;
    return flag;
}

void iOSWrapper::showShareUI(){
    
    NSArray *items = @[LocalizedNSString(@"TID_UI_SHARE"), [[NSBundle mainBundle] URLForResource:@"share_pic" withExtension:@"jpg"], [NSURL URLWithString:[HLAnalyst stringValue:@"KL_ShareURL" defaultValue:@"http://45.113.69.21/ao2/share_content.php"]]];
    UIActivityViewController *controller = [[UIActivityViewController alloc] initWithActivityItems:items applicationActivities:nil];
    controller.completionHandler = ^(UIActivityType __nullable activityType, BOOL completed){
        if(completed)__NotificationCenter::getInstance()->postNotification("KL_shareFinish");
            
    };
    //!!! ios8 pad上崩溃修复
    if ( [controller respondsToSelector:@selector(popoverPresentationController)] ) {
        controller.popoverPresentationController.sourceView =
        getRootViewController().view;
        CGRect frame = getRootViewController().view.bounds;
        frame.size.height /= 2;
        controller.popoverPresentationController.sourceRect = frame;
    }
    [getRootViewController() presentViewController:controller animated:YES completion:nil];
}

float iOSWrapper::getBattery(){
    float ret = [[UIDevice currentDevice] batteryLevel];
    if (ret == -1) {
        ret = 0.5;
    }
    return ret;
}

std::string iOSWrapper::getRateURL(){
    std::string ret = [HLInterface sharedInstance].comment_download_link.UTF8String;
    return ret;
}

std::string iOSWrapper::getLocalTimeZone(){
    return [NSTimeZone localTimeZone].name.UTF8String;
}

 std::string iOSWrapper::getUserID(){

    NSString *id = [HLADDeviceHelper uniqueIdentifier];
    if (id.length > 0) {
        return [HLADDeviceHelper uniqueIdentifier].UTF8String;
    }
    return "";
}

std::string iOSWrapper::getCurrentLanguage(){
    NSArray *languages = [[NSUserDefaults standardUserDefaults] objectForKey:@"AppleLanguages"];
    for (id e in languages) {
        NSLog(@"e = %@", e);
    }
    NSString *currentLanguage = [languages objectAtIndex:0];
    std::string ret = currentLanguage.UTF8String;
    return ret;
}

bool iOSWrapper::isPad(){
    return UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad;
}

void iOSWrapper::toSettingController(){
    [[NSNotificationCenter defaultCenter] postNotificationName:@"toSettingController" object:nil];
}

void iOSWrapper::showNewGameSheet(){
    [[NSNotificationCenter defaultCenter] postNotificationName:@"showNewGameSheet" object:nil];
}

void iOSWrapper::showHUD(std::string title){

    UIViewController *vc = getRootViewController();
    MBProgressHUD *hud = [MBProgressHUD HUDForView:vc.view];
    hud.labelText = [NSString stringWithFormat:@"%s", title.c_str()];
    hud.mode = MBProgressHUDModeText;
    dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, 0.5 * NSEC_PER_SEC);
    dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
        // Do something...
        [MBProgressHUD hideHUDForView:vc.view animated:NO];
    });
}
