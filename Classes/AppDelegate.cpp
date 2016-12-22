#include "AppDelegate.h"
#include "SolitaireKlondikeScene.h"
#include "SolitaireKlondikeViewModel.h"
#include "Global.h"
#include "HLAdManagerWrapper.hpp"
#include "Localization.hpp"
#include "SimpleAudioEngine.h"
#include "TestScene.hpp"
#include "HLInterfaceAndroid.hpp"
#include "KLPopupLayer.hpp"
#include "KTLayer.h"
using namespace CocosDenshion;
USING_NS_CC;
using namespace NSGlobal;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

extern "C"
{
    
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_onActivityDestroy(JNIEnv *env, jclass)
    {
        __NotificationCenter::getInstance()->postNotification(kOnApplicationTerminal);
    }
    
}

#endif


AppDelegate::AppDelegate()
{


}

void AppDelegate::onScreenSizeChanged(bool isP){

    HLAdManagerWrapper::repositionBanner();
    
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    
    auto frameSize = glview->getFrameSize();
    Size s;
    if (!isP) {
        s = Size(std::max(frameSize.width, frameSize.height),
                       std::min(frameSize.width, frameSize.height));
    } else {
        s = Size(std::min(frameSize.width, frameSize.height),
                       std::max(frameSize.width, frameSize.height));
    }
    glview->setFrameSize(s.width, s.height);
    
    if (isP){
        Size frameSize = glview->getFrameSize();
        
        Size winSize = designResolutionSize;
        
        //将宽和高做一个比，通过这个比，来具体的调整逻辑分辨率的大小
        float widthRate = frameSize.width/winSize.width;
        float heightRate = frameSize.height/winSize.height;
        auto size = NSGlobal::designResolutionSize;
        //如果是if中的语句，说明逻辑的高度有点大了，就把逻辑的高缩小到和宽度一样的比率
        if (widthRate > heightRate)
        {
            //pad
            glview->setDesignResolutionSize(640, 960, ResolutionPolicy::SHOW_ALL);
            
        }
        else
        {
            //phone
            glview->setDesignResolutionSize(size.width, size.height,
                                            ResolutionPolicy::FIXED_WIDTH);
        }
    
    } else {
        auto size = NSGlobal::designResolutionSize;
        glview->setDesignResolutionSize(960, 640,
                                        ResolutionPolicy::FIXED_HEIGHT);
    }
    
    __NotificationCenter::getInstance()->postNotification(kScreenSizeChangedNotification);
}

AppDelegate::~AppDelegate()
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("Solitaire", Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("Solitaire");
#endif
        director->setOpenGLView(glview);
    }
    
    if(Localization::getLanguage().empty()){
        Localization::setSystemCurrentLanguage();
    }
    
    
    // turn on display FPS
    director->setDisplayStats(false);
    
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    Size frameSize = glview->getFrameSize();
    
    Size winSize = designResolutionSize;
    
    if (iOSWrapper::isScreenPortrait())
    {
        //将宽和高做一个比，通过这个比，来具体的调整逻辑分辨率的大小
        float widthRate = frameSize.width/winSize.width;
        float heightRate = frameSize.height/winSize.height;
        auto size = NSGlobal::designResolutionSize;
        //如果是if中的语句，说明逻辑的高度有点大了，就把逻辑的高缩小到和宽度一样的比率
        if (widthRate > heightRate)
        {
            
            glview->setDesignResolutionSize(640, 960, ResolutionPolicy::SHOW_ALL);
            
        }
        else
        {
            glview->setDesignResolutionSize(size.width, size.height,
                                            ResolutionPolicy::FIXED_WIDTH);
        }
    }
    else {
        auto size = NSGlobal::designResolutionSize;
        glview->setDesignResolutionSize(960, 640,
                                        ResolutionPolicy::FIXED_HEIGHT);
    }
    
    FileUtils::getInstance()->addSearchPath("ui");
    FileUtils::getInstance()->setPopupNotify(false);
    
    auto str = FileUtils::getInstance()->getWritablePath();
    CCLOG("getWritablePath = %s", str.c_str());
//    glview->setDesignResolutionSize(size.width, size.height,
//                                    ResolutionPolicy::FIXED_WIDTH);


    register_all_packages();
    
    Size visibleSize = director->getVisibleSize();
    
    //  初始化可视区域的缩放因子
    ScaleFactor::initScale(visibleSize.width, visibleSize.height);
    
    auto thisVersion = iOSWrapper::getVersionName();
    auto updateKey = thisVersion + "_hasLauched";
    
    if (!UserDefault::getInstance()->getBoolForKey(updateKey.c_str())) {
        auto mgr = DataManager::getInstance();
        
        //产品更新后
        //重置好评弹出
        KLRateLayer::reset();
        
        //!!!
        //主题 牌背 id纠错
        //将id改为key 主键由int变为key
        
        int faceid = mgr->getFaceID();
        if (faceid > mgr->getFaces().size()) {
            faceid = 1;
        }
//        mgr->setFaceID(faceid);
        mgr->setCustomSelectFaceName(mgr->getFaces().at(faceid-1)->getName());
        
        
        int cbid = mgr->getCardbackID();
        cbid = clampf(cbid, 2, mgr->getCardbacks().size());
//        mgr->setCardbackID(cbid);
        mgr->setCustomSelectCardbackName(mgr->getCardbacks().at(cbid-1)->getName());

        auto thid = mgr->getThemeID();
        thid = clampf(thid, 2, mgr->getThemes().size());
//        mgr->setThemeID(thid);
        mgr->setCustomSelectThemeName(mgr->getThemes().at(thid-1)->getName());

        //KT SYN
        mgr->setHappyChrismtasShowed(false);
        
        UserDefault::getInstance()->setBoolForKey(updateKey.c_str(), true);
        UserDefault::getInstance()->flush();
    }
    
//    HLAdManagerWrapper::showBanner();
    

    // 进入游戏场景
    auto scene = KLLoadingLayer::scene([=](){
        auto sc = SolitaireKlondikeScene::createScene();
        director->replaceScene(sc);
        KLUtils::showInstertial24Later();
        iOSWrapper::setStart(true);
    });//
    //SolitaireKlondikeScene::createScene();
    //TestLayer::scene();
//    KLLoadingLayer* la = scene->getChildByTag<KLLoadingLayer *>(0);
//    la->onFinish = ;
    // run
    director->runWithScene(scene);
    
    HLInterfaceAndroid::getInstance()->startGet();
    
    
    //date
    KTDate *date = KTDate::date(1468999936);
    KTDate *localDate = date->localDate();
    
    auto date2 = localDate->utcDate();
    CCLOG("date = %ld", date->stamp());
    CCLOG("localDate = %ld", localDate->stamp());
    CCLOG("date2 = %ld", date2->stamp());
    return true;
}


// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    
    
    __NotificationCenter::getInstance()->postNotification(kOnApplicationDidEnterBackground);

}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    if (DataManager::getInstance()->isSoundOn()) {
        SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    }
    
    __NotificationCenter::getInstance()->postNotification(kOnApplicationWillEnterForeground);
    
//    //fix Admob弹出立刻消失bug
//    Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(0.2f), CallFunc::create([=](){
//        auto stamp = UserDefault::getInstance()->getFloatForKey("KL_LastShowActiveAd", 0);
//        auto last = KTDate::date(stamp);
//        auto now = KTDate::date();
//        auto since = now->timeIntervalSinceLastDate(last);
//        auto interval = HLAnalsytWrapper::intValue("KL_ShowActiveAdInterval", 120);
//        if (stamp == 0 || since > interval) {
//            
//            HLAdManagerWrapper::showUnsafeInterstitial();
//            
//            UserDefault::getInstance()->setFloatForKey("KL_LastShowActiveAd", now->stamp());
//            UserDefault::getInstance()->flush();
//        }
//    }), NULL));
    
    CCLOG("EnterForeground>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    
//    auto date = KTDate::date();
//    KTDate::date
    
}

void AppDelegate::applicationTerminal(){

    __NotificationCenter::getInstance()->postNotification(kOnApplicationTerminal);
    
}
