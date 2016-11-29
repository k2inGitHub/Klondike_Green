//
//  KTScene.h
//  HelloCpp
//
//  Created by Men on 13-9-27.
//
//



#ifndef HelloCpp_KTSceneBase_h
#define HelloCpp_KTSceneBase_h

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "VisibleRect.h"
#include "KTUtils.h"
#include "KTFactory.h"
//#include "MIRDynamicData.h"
//#include "MIRConfig.h"

//class Layer_NavigationBar;
#include "Layer_NavigationBar.hpp"
#include "Global.h"
#include "iOSWrapper.hpp"
#include "HLAdManagerWrapper.hpp"
#include "HLAnalsytWrapper.hpp"
#include "KLColor.hpp"

#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "network/HttpClient.h"
//#include "cocostudio/CocoStudio.h"
#include "Localization.hpp"
#include "KLUtils.hpp"
#include "md5.h"


using namespace cocos2d::network;

using namespace NSGlobal;

USING_NS_CC;
using namespace CocosDenshion;
using namespace cocos2d::ui;
using namespace std;

static const char* kScreenSizeChangedNotification = "KTScreenSizeChangedNotification";

/**
 Layer基类
 */
class KTLayer : public Layer{
    
protected:
    void onScreenSizeChanged(Ref *sender);
    
    void onLanguageChanged(Ref *sender);
public:

    float _scale;
    
//    MIRDynamicData *data;
    
    cocos2d::Size winSize ;
    
    virtual void updateLayout();
    
    virtual void updateLocalizedUI();
    
    virtual bool init() override;
    
    virtual void onEnter()override;
    
    virtual void onExit()override;
    
    CREATE_FUNC(KTLayer);
    
    
//    virtual void keyBackClicked();
//    virtual void keyMenuClicked();
    
    KTLayer();
    
    ~KTLayer();
    
    //MIR custom
    void showTipLayer(std::string file, std::string content);
    
    void playEffect(const char*file);
    
    void playBackgroundMusic(const char* file);
    
    
    //KL custom
    void setNavigationBarEnable(bool value);
    void addNavigationBar(std::string title, Vector<MenuItem *> lefttItems, Vector<MenuItem *> righttItems);
    
    bool isNavigationBarEnable();
    
    Layer_NavigationBar *_navigationBar;
    
    Layer_NavigationBar * getNavigationBar();
    
    void horizonAliament(Vector<Node *> nodes, float padding, float y = 0, bool useCenter = false);
    
    void verticalAliament(Vector<Node *> nodes, float padding, Vec2 center = Vec2::ZERO);
};


#endif
