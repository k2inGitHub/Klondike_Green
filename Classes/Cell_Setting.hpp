//
//  Cell_Setting.hpp
//  Klondike
//
//  Created by 宋扬 on 16/7/25.
//
//

#ifndef Cell_Setting_hpp
#define Cell_Setting_hpp


#include "KTLayer.h"
#include "Button_Highlight.hpp"
#include "Item_Setting.hpp"

class Cell_Base : public Layout {

public:
    
    void onScreenSizeChanged(Ref *sender);
    
    virtual void updateLayout();
    
    virtual Widget* createCloneInstance() override;
    
    virtual void copyClonedWidgetChildren(Widget* model) override;
    
    virtual bool init() override;
    
    virtual void onEnter()override;
    
    ~Cell_Base();
    
    CREATE_FUNC(Cell_Base);
};

class Cell_CustomThemeSubView : public Cell_Base {
    
public:
    
    
    CC_SYNTHESIZE(Button_Highlight *, _iconView, IconView);
    
    virtual Widget* createCloneInstance() override;
    
    virtual void updateLayout()override;
    
    virtual bool init()override;
    
    CREATE_FUNC(Cell_CustomThemeSubView);
};

class Cell_CustomTheme : public Cell_Base{
public:
    
    void refreshBtns();
    
    Node *_layer;
    
    Button *_leftArrow;
    
    Button *_rightArrow;
    
    void reloadData(Vector<PreviewItem *> dataArray);
    
    CC_SYNTHESIZE(ListView *, _listView, ListView);
    
    virtual Widget* createCloneInstance() override;
    
    virtual void updateLayout()override;
    
    CC_SYNTHESIZE(Label *, _titleLabel, TitleLabel);
    
//    CC_SYNTHESIZE_PASS_BY_REF(Vector<Button_Highlight *>, _iconViews, IconViews);
    
    virtual bool init()override;
    
    CREATE_FUNC(Cell_CustomTheme);
};

class Cell_ThemeSet : public Cell_Base{
public:
    HBox *_box;
    virtual Widget* createCloneInstance() override;
    
    virtual void updateLayout()override;
    
    CC_SYNTHESIZE_PASS_BY_REF(Vector<Button_Highlight *>, _iconViews, IconViews);
    
    virtual bool init()override;
    
    CREATE_FUNC(Cell_ThemeSet);
};

class Cell_CardBack : public Cell_Base{

public:
    
    
    
    HBox *_box;
    virtual Widget* createCloneInstance() override;
    virtual void updateLayout()override;
    
    CC_SYNTHESIZE_PASS_BY_REF(Vector<Button_Highlight *>, _iconViews, IconViews);
    
    virtual bool init() override;
    
    CREATE_FUNC(Cell_CardBack);
};

class Cell_Faces : public Cell_Base{
    
public:
    virtual Widget* createCloneInstance() override;
    
    virtual void updateLayout()override;
    
    CC_SYNTHESIZE(Button_Highlight *, _iconView, IconView);
    
    virtual bool init()override;
    
    CREATE_FUNC(Cell_Faces);
};

class Cell_Setting;

typedef std::function<void(Cell_Setting *cell)> SettingCellBlock;

class Cell_Setting : public Cell_Base {
    
    bool _forceSwitchOff;
    
    ImageView *_lock;
    
    CheckBox *_sw;
    
    Sprite *_badge;
public:
    
    void setBadgeEnabled(bool value);
    
    void setForceSwitchOff(bool value);
    
    CC_SYNTHESIZE(SettingCellBlock, _onSwitch, OnSwitch);
        
    virtual Widget* createCloneInstance() override;
    
    virtual void updateLayout()override;
    
    //    CC_SYNTHESIZE(Button *, _rankButton, RankButton);
    
    CC_SYNTHESIZE(Text *, _titleLabel, TitleLabel);
    
    void setSwitchSelected(bool value);
    bool isSwitchSelected();
    void setSwitchVisible(bool value) {
        _sw->setVisible(value);
    }
//    CC_SYNTHESIZE(CheckBox *, _sw, Switch);
    
    CC_SYNTHESIZE(ImageView *, _arrow, Arrow);
    
    CC_SYNTHESIZE(ImageView *, _bg, Bg);
    
    virtual bool init()override;
    
    CREATE_FUNC(Cell_Setting);
    
    Cell_Setting();
};

#endif /* Cell_Setting_hpp */
