//
//  KTFactory.h
//  LeiTing
//
//  Created by SongYang on 13-9-27.
//
//



#ifndef LeiTing_KTFactory_h
#define LeiTing_KTFactory_h

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace cocos2d::ui;

/*
 CC元素工厂
 **/
class KTFactory {
    
public:
    static void addLabel(const char* string, cocos2d::Point pos, Node *node);
    static MenuItemImage* menuItemImage(const char *img, const char *imgSel, Ref *target, SEL_MenuHandler sel, cocos2d::Point position, int tag);
    static Animation* createAnimation(const char* file, int count, int startIndex, float delayPerUnit, int loops);
    static Animation* createAnimationFromSpriteFrameCache(const char* file, int count, int startIndex, float delayPerUnit, int loops);
    static void addSpriteFrameToCache(const char*filename);
    static SpriteFrame* spriteFrameByName(const char*filename);
    static LabelAtlas* createLabelAtlas(const char* string, const char *filename, unsigned int startItem, int itemCount, cocos2d::Point anchorPoint, cocos2d::Point position);
    static Button *createButton(std::string normal, std::string select, std::string disable, std::string title, int fontSize, std::string titleFont = "Arial", cocos2d::Size contentSize = cocos2d::Size::ZERO);
    
//    static CCSprite* createSpriteFromWriteablePath(const char* name);
};


#endif
