//
//  Layer_Setting.hpp
//  Klondike
//
//  Created by 宋扬 on 16/7/25.
//
//

#ifndef Layer_Setting_hpp
#define Layer_Setting_hpp

#include "KTPauseLayer.h"



class Layer_HowToPlay : public KTPauseLayer{
    
    Sprite *_bg;
    
public:
    ui::ScrollView *_scrollView;
    Label *_label;
    int _tabIdx;
    void loadData(int idx);
    Vector<Button *> _tabButtons;
    virtual bool init()override;
    CREATE_FUNC(Layer_HowToPlay);
    virtual void updateLayout()override;
};


class Layer_Language : public KTPauseLayer{

//    vector<string> languages;
    string languages[16] = {"en","chs","cht","fr","de","it","kr","nl","pt","ru","es","tr","jp","in","ar","hi"};
    
    LayerColor *_bg;
    ImageView *_panel;
    Label *_titleLbl;
    Button *_btn;
    Layout *_box;
public:
    virtual bool init()override;
    CREATE_FUNC(Layer_Language);
    
    virtual void updateLayout()override;
};


class Layer_CustomTheme : public KTPauseLayer{
    
protected:
    ListView *_listView;
    Sprite *_bg;
    RenderTexture *_rt;
public:
    
//    CC_SYNTHESIZE_RETAIN(Node *, _clipContainer, ClipContainer);
    
    ~Layer_CustomTheme();
    virtual void updateLayout()override;
    virtual bool init()override;
    virtual Layout *cellForDisplay();
    virtual void reloadData();
    CREATE_FUNC(Layer_CustomTheme);
    
    void updateBadge(Ref *sender);
    void onCardbackFileCanceled(Ref *sender);
    void onCardbackFileChanged(Ref *sender);
    void onThemeFileChanged(Ref *sender);
    void onThemeFileCanceled(Ref *sender);
};

class Layer_Select : public KTPauseLayer{
    
protected:
    ListView *_listView;
    Sprite *_bg;
    Button *_cancelBtn;
    Button *_submitBtn;
public:
    
    virtual void updateLayout()override;
    virtual bool init()override;
    virtual Layout *cellForDisplay();
    virtual void reloadData();
    virtual void onConfirm();
    virtual void onCancel();
};

class Layer_ThemeSet : public Layer_Select {
public:
    bool _inited;
    int _lastIdx;
    int _idx;
    virtual Layout *cellForDisplay()override;
    virtual void reloadData()override;
    virtual void onConfirm()override;
    virtual bool init()override;
    CREATE_FUNC(Layer_ThemeSet);
    ~Layer_ThemeSet();
    virtual void updateLayout()override;
};

/*
class Layer_Themes : public Layer_Select {
    
public:
    
    int _idx;
    virtual Layout *cellForDisplay()override;
    virtual void reloadData()override;
    virtual void onConfirm() override;
    virtual void onCancel() override;
    virtual bool init()override;
    CREATE_FUNC(Layer_Themes);
    ~Layer_Themes();
    
    void onThemeFileChanged(Ref *sender);
    void onThemeFileCanceled(Ref *sender);

    
};

class Layer_Faces : public Layer_Select {
    
public:
    
    int _idx;
//    vector<string> dataArray;
    
    virtual Layout *cellForDisplay()override;
    virtual void reloadData()override;
    virtual void onConfirm() override;
    virtual bool init()override;
    CREATE_FUNC(Layer_Faces);
};

class Layer_CardBack : public Layer_Select{
    
public:
    ~Layer_CardBack();
    
    int _idx;
    vector<vector<string>> dataArray;
    
    virtual Layout *cellForDisplay()override;
    virtual void reloadData()override;
    virtual void onConfirm() override;
    virtual bool init()override;
    CREATE_FUNC(Layer_CardBack);
    
    void onCardbackFileCanceled(Ref *sender);
    void onCardbackFileChanged(Ref *sender);
};
*/

class Layer_Setting : public KTPauseLayer {

    
    
    
    Button *_sharedBtn;
    Text *_titleLabel;
    Button *_closeBtn;
    ImageView *_panel;
    LayerColor *_bg;
public:
    
    ~Layer_Setting();
    
    void updateBadge(Ref *sender);
    
    virtual void updateLayout()override;
    
    virtual void updateLocalizedUI()override;
    
    void loadData();
    
    virtual void onEnter()override;
    
    ListView *_listView;
    
    ValueVector dataArray;
    
    virtual bool init()override;
    
    CREATE_FUNC(Layer_Setting);
};

#endif /* Layer_Setting_hpp */
