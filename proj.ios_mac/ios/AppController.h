#import <UIKit/UIKit.h>

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate> {
    UIWindow *window;
}

@property (strong, nonatomic) UIWindow *window;

@property (nonatomic, retain) UINavigationController *rootNavController;

@end

extern "C"{
    
    AppController* getAppController ();
    
    UIViewController *getRootViewController();
    
    RootViewController *getCustomViewController();
    
    NSString *getLocalizedNSString(NSString *key);
}

#define LocalizedNSString(key) getLocalizedNSString(key)
