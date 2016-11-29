//
//  KTFactory.cpp
//  LeiTing
//
//  Created by Men on 13-9-27.
//
//

#include "KTFactory.h"

Button *KTFactory::createButton(std::string normal, std::string select, std::string disable, std::string title, int fontSize, std::string titleFont, Size contentSize){
    Button *button = Button::create(normal, select, disable);
    button->setScale9Enabled(true);
    if (!contentSize.equals(Size::ZERO)) {
        button->setContentSize(contentSize);
    }
    button->setPressedActionEnabled(false);
    button->setTitleAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
    button->setTitleFontName(titleFont);
    button->setTitleFontSize(fontSize);
    button->setTitleText(title);

    if (!contentSize.equals(Size::ZERO)) {
        button->getTitleRenderer()->setDimensions(MAX(0, contentSize.width), MIN(fontSize * 2 - 2, contentSize.height));
    }
    
    
    button->getTitleRenderer()->setVerticalAlignment(TextVAlignment::CENTER);
    
    button->getTitleRenderer()->setOverflow(Label::Overflow::SHRINK);
    
    
    return button;
}

//CCSprite *createSpriteFromWriteablePath(const char* name)
//{
//    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath();
//    path+=name;
//    FILE* fp = fopen(path.c_str(),"rb");
//    if (!fp)
//    {
//        return NULL;
//    }
//    fseek(fp,0,SEEK_END);
//    int len = ftell(fp);
//    fseek(fp,0,SEEK_SET);
//    char* buf = (char*)malloc(len);
//    fread(buf,len,1,fp);
//    fclose(fp);
//    CCImage* img = new CCImage;
//    img->initWithImageData(buf,len);
//    free(buf);
//    cocos2d::CCTexture2D* texture = new cocos2d::CCTexture2D();
//    CCTextureCache::sharedTextureCache()->addUIImage(img, name);
//    bool isImg = texture->initWithImage(img);
//    img->release();
//    if (!isImg)
//    {
//        delete texture;
//        CCAssert(isImg, "img no exit");
//    }
//    CCSpriteFrame *f = CCSpriteFrame::createWithTexture(texture, CCRectMake(0, 0, texture->getContentSize().width, texture->getContentSize().height));
//    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(f, name);
//    CCSprite* sprite = CCSprite::createWithTexture(texture);
//    texture->release();
//    return sprite;
//}


void KTFactory::addLabel(const char* string, cocos2d::Point pos, Node *node)
{
    cocos2d::Size s = Director::getInstance()->getWinSize();
    CCLabelTTF *label = CCLabelTTF::create(string, "CourierNewPS-BoldItalicMT", 64, cocos2d::CCSizeMake(s.width-50,s.height-50), kCCTextAlignmentCenter);
    label->setPosition(pos);
    label->setColor(ccc3(255, 0, 0));
    node->addChild(label, 128);
}

CCLabelAtlas* KTFactory::createLabelAtlas(const char* string, const char *filename, unsigned int startItem, int itemCount, CCPoint anchorPoint, CCPoint position)
{
    CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage(filename);
    cocos2d::Size size = pTexture->getContentSize();
    CCLabelAtlas *pLabelAtlas = CCLabelAtlas::create(string, filename, size.width/itemCount, size.height, startItem);
    pLabelAtlas->setAnchorPoint(anchorPoint);
    pLabelAtlas->setPosition(position);
    return pLabelAtlas;
}

void KTFactory::addSpriteFrameToCache(const char*filename)
{
    CCTexture2D *t = CCTextureCache::sharedTextureCache()->addImage(filename);
    CCSpriteFrame *f = CCSpriteFrame::createWithTexture(t, CCRectMake(0, 0, t->getContentSize().width, t->getContentSize().height));
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(f, filename);
}
 CCSpriteFrame* KTFactory::spriteFrameByName(const char*filename)
{
    CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(filename);
    if (pFrame == NULL) {
        addSpriteFrameToCache(filename);
    }
    return CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(filename);
}

CCMenuItemImage* KTFactory::menuItemImage(const char *img, const char *imgSel, CCObject *target, SEL_MenuHandler sel, CCPoint position, int tag)
{
    CCMenuItemImage *itemImg = CCMenuItemImage::create(img, imgSel, target, sel);
    if (imgSel && !strcmp(img, imgSel)) {
        CCSprite *spSel = (CCSprite *)itemImg->getSelectedImage();
        spSel->setColor(ccc3(128, 128, 128));
    }
    itemImg->setPosition(position);
    itemImg->setTag(tag);
    return itemImg;
}

CCAnimation* KTFactory::createAnimation(const char* file, int count, int startIndex, float delayPerUnit, int loops){
    CCAnimation *animation = CCAnimation::create();
    for (int i = startIndex; i < startIndex + count; i++) {
        const char * name =CCString::createWithFormat("%s%d.png", file, i)->getCString();
        animation->addSpriteFrameWithFileName(name);
    }
    animation->setDelayPerUnit(delayPerUnit);
    animation->setLoops(loops);
    return animation;
}

Animation* KTFactory::createAnimationFromSpriteFrameCache(const char *file, int count, int startIndex, float delayPerUnit, int loops)
{
    Vector<SpriteFrame *> frames;
    auto *sfc = SpriteFrameCache::getInstance();
    for (int i = startIndex; i < startIndex + count; i++) {
        const char * name =CCString::createWithFormat("%s%d.png", file, i)->getCString();
        frames.pushBack(sfc->getSpriteFrameByName(name));
    }
    auto *animation = Animation::createWithSpriteFrames(frames);
    animation->setDelayPerUnit(delayPerUnit);
    animation->setLoops(loops);
    return animation;
}