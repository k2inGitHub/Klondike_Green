/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org

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

#import "AppController.h"
#import "platform/ios/CCEAGLView-ios.h"
#import "cocos2d.h"
#import "AppDelegate.h"
#import "RootViewController.h"
#import "HLService.h"
#import "UIImage+LaunchImage.h"
#include "Localization.hpp"
#import "KLNavigationController.h"
#import "AFNetworking.h"
#import "NSString+KT.h"
#include "HLAdManagerWrapper.hpp"
#import <AppsFlyer/AppsFlyer.h>
#import "KLManager.h"
//#import "KLLoadingController.h"

@interface AppController ()

@property(nonatomic, retain) RootViewController* viewController;

@end

@implementation AppController

@synthesize window = window;

#pragma mark -
#pragma mark Application lifecycle

// cocos2d application instance
static AppDelegate s_sharedApplication;

static AppController *_appController;

- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken{

    [[AppsFlyerTracker sharedTracker] registerUninstall:deviceToken];
    
}

//通过其他app打开应用
-(BOOL)application:(UIApplication *)application openURL:(nonnull NSURL *)url sourceApplication:(nullable NSString *)sourceApplication annotation:(nonnull id)annotation{
    NSLog(@"url=====%@ \n  sourceApplication=======%@ \n  annotation======%@", url, sourceApplication, annotation);

    return [[KLManager sharedManager] application:application handleOpenURL:url];
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {

    _appController = self;
    
    cocos2d::Application *app = cocos2d::Application::getInstance();
    app->initGLContextAttrs();
    cocos2d::GLViewImpl::convertAttrs();

    // Override point for customization after application launch.

    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
    self.window.backgroundColor = [UIColor whiteColor];
    
    [self onLoadingFinish];
    
    [window makeKeyAndVisible];

    [[UIApplication sharedApplication] setStatusBarHidden:true];

    //请求接口
    [[HLInterface sharedInstance] startGet];
    //统计分析
    [HLAnalyst start];
    //加载广告
    [HLAdManager sharedInstance];
    
    [[HLLocalNotificationCenter sharedCenter] registerUserNotification];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onAdLeaveApp) name:HLBannerLeaveNotification object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onAdLeaveApp) name:HLInterstitialLeaveNotification object:nil];

    
//    auto str = Localization::get("TID_UI_CHECKING FOR UPDATES");
//    CCLOG("local txt = %s", str.c_str());

    
//    struct tm _time;
//    time_t t;
//    struct tm *p;
//    t=1468678868;//time(NULL);
//    p=gmtime(&t);
//    char s[80];
//    strftime(s, 80, "%Y-%m-%d %H:%M:%S::%Z", p);
//    printf("%d: %s\n", (int)t, s);
//    
//    t+= 28800;
//    p=gmtime(&t);
//    strftime(s, 80, "%Y-%m-%d %H:%M:%S", p);
//    printf("%d: %s\n", (int)t, s);

    
//    NSDictionary *content = @{@"uid" : @"1"};
//    NSDictionary *dic = @{@"content" : [NSString JSONStringFromDict:content]};
//    AFHTTPSessionManager *manager = [AFHTTPSessionManager manager];
//    manager.responseSerializer.acceptableContentTypes = [NSSet setWithObject:@"text/html"];
//    NSURLSessionDataTask *task = [manager GET:@"http://45.113.69.21/ao2/play_cards_awards_list.php" parameters:dic progress:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject) {
//        
//        NSLog(@"success = %@", responseObject);
//        
//    } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error) {
//        NSLog(@"failure = %@", error);
//    }];
//    
//    NSLog(@"url = %@", task.originalRequest.URL);
    
    return YES;
}

- (void)onAdLeaveApp{
    __NotificationCenter::getInstance()->postNotification(kOnAdLeaveApplication);
}

- (void)application:(UIApplication *)application didReceiveLocalNotification:(UILocalNotification *)notification{
    //仿unity通知 免去unity打包多余代码
    [[NSNotificationCenter defaultCenter] postNotificationName:@"kUnityDidReceiveLocalNotification" object:self userInfo:(id)notification];
}

//创建loading视图与根视图
- (void)createLoadingView{
    
    //loading
//    UIViewController *loadingController = [[[KLLoadingController alloc] init] autorelease];
    
    
    
    
//    [self performSelector:@selector(onLoadingFinish) withObject:nil afterDelay:0];
    
}

- (void)onLoadingFinish{
    
    
    
    cocos2d::Application *app = cocos2d::Application::getInstance();
    // Init the CCEAGLView
    CCEAGLView *eaglView = [CCEAGLView viewWithFrame: [window bounds]
                                         pixelFormat: (NSString*)cocos2d::GLViewImpl::_pixelFormat
                                         depthFormat: cocos2d::GLViewImpl::_depthFormat
                                  preserveBackbuffer: NO
                                          sharegroup: nil
                                       multiSampling: NO
                                     numberOfSamples: 0 ];
    
    // Enable or disable multiple touches
    [eaglView setMultipleTouchEnabled:NO];
    
    // Use RootViewController manage CCEAGLView
    _viewController = [[[RootViewController alloc] initWithNibName:nil bundle:nil] autorelease];
    _viewController.wantsFullScreenLayout = YES;
    _viewController.view = eaglView;
    
//    [_rootNavController setViewControllers:@[_viewController]];
    
    // IMPORTANT: Setting the GLView should be done after creating the RootViewController
    cocos2d::GLView *glview = cocos2d::GLViewImpl::createWithEAGLView(eaglView);
    cocos2d::Director::getInstance()->setOpenGLView(glview);
    
    app->run();
    
//    [[HLAdManager sharedInstance] showBanner];
//    UIView *bannerView = [[HLAdManager sharedInstance] bannerView];
//    [bannerView addObserver:self forKeyPath:@"frame" options:NSKeyValueObservingOptionNew|NSKeyValueObservingOptionOld context:NULL];
//    [bannerView addObserver:self forKeyPath:@"center" options:NSKeyValueObservingOptionNew|NSKeyValueObservingOptionOld context:NULL];
//    bannerView.center = [self bannerCenter];
    
    _rootNavController = [[[KLNavigationController alloc] initWithRootViewController:_viewController]autorelease];
    _rootNavController.navigationBarHidden = YES;
    NSDictionary * dict = [NSDictionary dictionaryWithObject:[UIColor whiteColor] forKey:NSForegroundColorAttributeName];
    _rootNavController.navigationBar.titleTextAttributes = dict;
    
    self.window.rootViewController  = _rootNavController;
    
}



//- (CGPoint) bannerCenter{
//    UIView *bannerView = [[HLAdManager sharedInstance] bannerView];
//    return CGPointMake([window bounds].size.width/2, bannerView.frame.size.height/2);
//}
//
//-(void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context
//{
//    if([keyPath isEqualToString:@"frame"] || [keyPath isEqualToString:@"center"])
//    {
//        UIView *bannerView = [[HLAdManager sharedInstance] bannerView];
//        if (!CGPointEqualToPoint(bannerView.center, [self bannerCenter])) {
//            bannerView.center = [self bannerCenter];
//        }
//    }
//}


- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
     //We don't need to call this method any more. It will interupt user defined game pause&resume logic
    /* cocos2d::Director::getInstance()->pause(); */
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
     //We don't need to call this method any more. It will interupt user defined game pause&resume logic
    /* cocos2d::Director::getInstance()->resume(); */
    
    [UIApplication sharedApplication].applicationIconBadgeNumber = 0;
    CCLOG("applicationDidBecomeActive");
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    [[HLLocalNotificationCenter sharedCenter] schedeuleAll];

    cocos2d::Application::getInstance()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    cocos2d::Application::getInstance()->applicationWillEnterForeground();
    CCLOG("applicationWillEnterForeground");
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
    CCLOG("applicationWillTerminate");
    auto app = (AppDelegate *)cocos2d::Application::getInstance();
    app->applicationTerminal();
    
   
}


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}


- (void)dealloc {
    [window release];
    [super dealloc];
}


@end


extern "C"{
    
    NSString *getLocalizedNSString(NSString *key){
        
        return [NSString stringWithCString:Localization::get(key.UTF8String).c_str() encoding:NSUTF8StringEncoding];
    }

    AppController* getAppController () {
        return _appController;
    }
    
    UIViewController *getRootViewController() {
        return getAppController().rootNavController;
    }
    RootViewController *getCustomViewController(){
        return getAppController().viewController;
    }
}
