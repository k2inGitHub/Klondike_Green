//
//  Layer_NavigationBar.hpp
//  Klondike
//
//  Created by 宋扬 on 16/7/14.
//
//

#ifndef Layer_NavigationBar_hpp
#define Layer_NavigationBar_hpp

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
//#include "KTLayer.h"
using namespace cocos2d;

//class 

class Layer_NavigationBar : public Layer {
    
public:
    
    Sprite *_bg;
    
    ~Layer_NavigationBar();
    
    void onScreenSizeChanged(Ref *sender);
    
    void setLeftBarItems(Vector<MenuItem *>& bartItems);
    
    void setRightBarItems(Vector<MenuItem *>& bartItems);
    
    virtual bool init();
    
    virtual bool init(std::string title, Vector<MenuItem *> lefttItems, Vector<MenuItem *> righttItems);
    
    CREATE_FUNC(Layer_NavigationBar);
    
    static Layer_NavigationBar*create(std::string title, Vector<MenuItem *> lefttItems, Vector<MenuItem *> righttItems){
    
        Layer_NavigationBar *instance = new Layer_NavigationBar();
        instance->init(title, lefttItems, righttItems);
        instance->autorelease();
        return instance;
    }
    
    virtual void onEnter();
    
    CC_SYNTHESIZE(std::string, _title, Title);
    
    Layer_NavigationBar();
    
    void touchEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
private:
    
    bool _itemDirty;
    
    void createMenu();
    
    cocos2d::Size _barSize;
    
    Node *_root;
    
    void updateItems();
    
    Label *_titleLabel;
    
    Menu *_menu;
    
    Vector<MenuItem *> _leftItems;
    
    Vector<MenuItem *> _rightItems;
};

#endif /* Layer_NavigationBar_hpp */
