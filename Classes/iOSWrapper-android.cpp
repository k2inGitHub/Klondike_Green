//
//  iOSWrapper-android.cpp
//  Klondike
//
//  Created by 宋扬 on 16/8/8.
//
//

#include "iOSWrapper.hpp"
#include "AppDelegate.h"
#include "Localization.hpp"
#include "HLAnalsytWrapper.hpp"
#include "Global.h"
#include "KLPopupLayer.hpp"
#include "DataManager.h"
using namespace NSDataManage;

using namespace NSGlobal;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>


static float batteryValue = 0;
static std::string mylanguage = "en";
static std::string myTimeZone = "";
static std::string userID = "12345";

extern "C"
{
    
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_copyThemeFolder(JNIEnv *env, jclass, jstring jthemeName) {
    
        CCLOG("copyThemeFolder");
        
        string themeName = iOSWrapper::jstringTostring(env,jthemeName);
        
        auto fu = FileUtils::getInstance();
        string themeDir = fu->getWritablePath() + themeName + "/";
        fu->createDirectory(themeDir);
        
        string faceDir = themeDir + "faces/";
        fu->createDirectory(faceDir);
        
        string cacheDir = StringUtils::format("/sdcard/%s/", themeName.c_str());
        
        string files[] = {"theme_bg.jpg", "theme_preview.png", "theme_set_preview.png", "cardback.png", "data.plist"};
        int num = 5;
        for(int i = 0; i < num; i++) {
            CCLOG("s = %s", (cacheDir + files[i]).c_str());
            CCLOG("t = %s", (themeDir + files[i]).c_str());
            auto data = fu->getDataFromFile(cacheDir + files[i]);
            if(data.getSize() != 0) {
                fu->writeDataToFile(data, themeDir + files[i]);
            }
        }
        auto dataMap = fu->getValueMapFromFile(cacheDir+"data.plist");
        string faceName = dataMap["_faceName"].asString();
        if (faceName == themeName + "_face") {
            for (int i = 0; i < 4; i++) {
                string faceSubDir = StringUtils::format("%s%d/", faceDir.c_str(), i);
                fu->createDirectory(faceSubDir);
                for(int j = 0; j < 13;j++) {
                    string source = StringUtils::format("%s%d/%d.png", cacheDir.c_str(), i, j+1);
                    string target = StringUtils::format("%s%d.png", faceSubDir.c_str(), j+1);
                    CCLOG("s = %s", (source).c_str());
                    CCLOG("t = %s", (target).c_str());
                    auto data = fu->getDataFromFile(source);
                    fu->writeDataToFile(data, target);
                }
            }
        }
    }

    
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_onThemeSetup(JNIEnv *env, jclass, jstring themeName)
    {
        CCLOG("onThemeSetup");
        __NotificationCenter::getInstance()->postNotification(kOnThemeSetup, __String::create(iOSWrapper::jstringTostring(env,themeName)));
        
    }
    
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_onThemeFileCanceled(JNIEnv *env, jclass)
    {
        __NotificationCenter::getInstance()->postNotification(kOnThemeFileCanceled);
        
    }
    
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_onThemeFileChanged(JNIEnv *env, jclass)
    {
        __NotificationCenter::getInstance()->postNotification(kOnThemeFileChanged);
        
    }
    
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_onCardbackFileCanceled(JNIEnv *env, jclass)
    {
        __NotificationCenter::getInstance()->postNotification(kOnCardbackFileCanceled);

    }
    
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_onCardbackFileChanged(JNIEnv *env, jclass)
    {
        __NotificationCenter::getInstance()->postNotification(kOnCardbackFileChanged);

    }
    
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_onSharedResult(JNIEnv *env, jclass, int resultCode)
    {
        if (resultCode == -1){
            __NotificationCenter::getInstance()->postNotification("KL_shareFinish");
        }
    }
    
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_onScreenSizeChanged(JNIEnv *env, jclass, bool value)
    {
        AppDelegate* app = (AppDelegate *) cocos2d::Application::getInstance();
        app->onScreenSizeChanged(value);
    }
    
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_onBatteryChanged(JNIEnv *env, jclass, jfloat value)
    {
        batteryValue = value;
        __NotificationCenter::getInstance()->postNotification(kBatteryChanged);
    }
    
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_DeviceInfo_getLanguage(JNIEnv *env, jclass, jstring str)
    {
        mylanguage = iOSWrapper::jstringTostring(env,str);
    }
    
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_DeviceInfo_getTimeZone(JNIEnv *env, jclass, jstring str)
    {
        myTimeZone = iOSWrapper::jstringTostring(env,str);
    }
    
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_DeviceInfo_getUserID(JNIEnv *env, jclass, jstring str)
    {
        userID = iOSWrapper::jstringTostring(env,str);
    }
    
    JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_AppActivity_getLocalization(JNIEnv *env, jclass, jstring key){
        std::string keyStr = iOSWrapper::jstringTostring(env,key);
        return StringUtils::newStringUTFJNI(env, LocalizedString(keyStr));
    }

    
    //    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxEditBoxHelper_editBoxEditingDidBegin(JNIEnv *env, jclass, jint index) {
    //        editBoxEditingDidBegin(index);
    //    }
    
}

void iOSWrapper::showToSetupThemeAppPage(bool directly){
    
    function<void()> func = [=]()
    {
        JniMethodInfo minfo;
        bool isHave = JniHelper::getStaticMethodInfo(minfo,"org.cocos2dx.cpp.AppActivity","showToSetupThemeAppPage","(Ljava/lang/String;)V");
        if(!isHave){
        }else{
            string devName = HLAnalsytWrapper::stringValue("KL_SetupThemeURL", "HiColor");
            
            jstring p1 = minfo.env->NewStringUTF(devName.c_str());
            
            minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID, p1);
        }
    };
    
    if(directly) {
        func();
        return;
    }
    
    //KT SYN
    vector<string> btns;
    btns.push_back(LocalizedString("TID_UI_CONFIRM"));
    KLAlertLayer::show("", LocalizedString("TID_UI_GOOGOLE_TO_DOWNLOAD_THEME"), LocalizedString("TID_UI_CANCEL"), btns, [=](KLAlertLayer *layer, int buttonIdx){
        if(buttonIdx == 1) {
            func();
        }
    });
}

void iOSWrapper::setStart(bool value){
}

char* iOSWrapper::jstringTostring(JNIEnv* env, jstring jstr)
{
    char* rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("utf-8");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr= (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0)
    {
        rtn = (char*)malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    return rtn;
}

void iOSWrapper::showMoreTheme(){
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo,"org.cocos2dx.cpp.AppActivity","showMoreTheme","()V");
    if(!isHave){
    }else{
        minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID);
    }
}

void iOSWrapper::showImagePicker(){
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo,"org.cocos2dx.cpp.AppActivity","showImagePicker","()V");
    if(!isHave){
    }else{
        minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID);
    }
}

void iOSWrapper::showThemePicker(){
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo,"org.cocos2dx.cpp.AppActivity","showThemePicker","()V");
    if(!isHave){
    }else{
        minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID);
    }
}

std::string iOSWrapper::updateUrl(){
    
    std::string ret = UserDefault::getInstance()->getStringForKey("itunes_updated_url");
    return ret;
}

bool iOSWrapper::isForceUpdate(){
//    return [HLInterface sharedInstance].itunes_is_forced == 1;
    std::string ret = UserDefault::getInstance()->getStringForKey("itunes_is_forced");
    return ret == "1";
}

std::string iOSWrapper::updateVersion(){
    std::string ret = UserDefault::getInstance()->getStringForKey("latest_version");
    return ret;
}

bool iOSWrapper::isUpdateEnable(){
    std::string ret = UserDefault::getInstance()->getStringForKey("itunes_update_ctrl_switch");
    return ret == "1";
}

std::string iOSWrapper::updateContent(){
    std::string ret = UserDefault::getInstance()->getStringForKey("itunes_update_content");
    return ret;
}

float iOSWrapper::getBattery(){
    return batteryValue;
}

std::string iOSWrapper::getRateURL(){
    std::string ret = UserDefault::getInstance()->getStringForKey("comment_download_link");
    return ret;
}

std::string iOSWrapper::getLocalTimeZone(){
    return myTimeZone;
}

std::string iOSWrapper::getUserID(){
    return userID;
}

std::string iOSWrapper::getCurrentLanguage(){
    std::string ret = mylanguage;
    return ret;
}

std::string iOSWrapper::getAppName(){
    std::string appName = "";
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo,
                                                 "org.cocos2dx.cpp.DeviceInfo","getAppName","()Ljava/lang/Object;");
    if(!isHave){
    }else{
        jstring js_pkn = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID,minfo.methodID);
        appName = JniHelper::jstring2string(js_pkn);
    }
    return appName;
}

std::string iOSWrapper::getVersionName(){
    std::string versionName = "";
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo,
                                                 "org.cocos2dx.cpp.DeviceInfo","getVersionName","()Ljava/lang/Object;");
    if(!isHave){
    }else{
        jstring js_pkn = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID,minfo.methodID);
        versionName = JniHelper::jstring2string(js_pkn);
    }
    return versionName;
}

bool iOSWrapper::isPad(){
    //    return UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad;
    return false;
}

void iOSWrapper::toSettingController(){
    //    [[NSNotificationCenter defaultCenter] postNotificationName:@"toSettingController" object:nil];
}

void iOSWrapper::showNewGameSheet(){
    //    [[NSNotificationCenter defaultCenter] postNotificationName:@"showNewGameSheet" object:nil];
}

void iOSWrapper::showHUD(std::string title){
    
    //    UIViewController *vc = getRootViewController();
    //    MBProgressHUD *hud = [MBProgressHUD HUDForView:vc.view];
    //    hud.labelText = [NSString stringWithFormat:@"%s", title.c_str()];
    //    hud.mode = MBProgressHUDModeText;
    //    dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, 0.5 * NSEC_PER_SEC);
    //    dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
    //        // Do something...
    //        [MBProgressHUD hideHUDForView:vc.view animated:NO];
    //    });
}

void iOSWrapper::showShareUI(){

    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo,
                                                 "org.cocos2dx.cpp.AppActivity","shareUI","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    if(!isHave){
    }else{
        auto title = LocalizedString("TID_UI_SHARE");
        auto content = LocalizedString("TID_UI_SHARE");
        content += "\n";
        content += HLAnalsytWrapper::stringValue("KL_ShareURL", "http://45.113.69.21/ao2/share_content.php");
        
        jstring p1 = minfo.env->NewStringUTF(title.c_str());
        jstring p2 = minfo.env->NewStringUTF(content.c_str());
        jstring p3 = minfo.env->NewStringUTF(HLAnalsytWrapper::stringValue("KL_ShareFB_URL", "http://45.113.69.21/ao2/share_content.php").c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID, p1, p2, p3);
    }
    
}

bool iOSWrapper::isScreenPortrait(){

    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo,
                                                 "org.cocos2dx.cpp.AppActivity","isScreenPortrait","()Z");
    if(!isHave){
        return true;
    }else{
        
        bool ret = (bool)minfo.env->CallStaticBooleanMethod(minfo.classID,minfo.methodID);
        return ret;
    }
}

#endif
