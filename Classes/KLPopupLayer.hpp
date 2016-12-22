//
//  KLPopupLayer.hpp
//  Klondike
//
//  Created by 宋扬 on 16/7/17.
//
//

#ifndef KLPopupLayer_hpp
#define KLPopupLayer_hpp

#include "KTPauseLayer.h"
#include "HighScoreManager.hpp"

class KLThanksGivingLayer : public KTPauseLayer {
    
public:
    Node *_bg;
    Node *_panel;
    Node *_titleLbl;
    Node *_shareBtn;
    Node *_clostBtn;
    Node *_sp;
    Node *_label;
    virtual bool init()override;
    virtual void updateLayout() override;
    CREATE_FUNC(KLThanksGivingLayer);
};

class KLUpdateLayer : public KTPauseLayer{

public:
    
    static bool show();
    virtual bool init()override;
    virtual void updateLayout() override;
    CREATE_FUNC(KLUpdateLayer);
};

class KLShareSuccessLayer : public KTPauseLayer{
    Node *_bg;
    Node *_panel;
    Node *_titleLbl;
    Node *_shareBtn;
    Node *_clostBtn;
    Node *_sp;
    Node *_label;
public:
    function<void()> onClick;
    virtual bool init()override;
    virtual void updateLayout() override;
    CREATE_FUNC(KLShareSuccessLayer);
    
};

class KLShareInfoLayer : public KTPauseLayer{
    
    Node *_bg;
    Node *_panel;
    Node *_titleLbl;
    Node *_shareBtn;
    Node *_clostBtn;
    Node *_sp;
    Node *_label;
    
public:
    virtual void updateLayout() override;
    virtual bool init()override;
    CREATE_FUNC(KLShareInfoLayer);
    
};

class KLLoadingLayer : public KTLayer{

    Node *_bg;
    Node *_icon;
    Node *_tipBg;
    Node *_label;
    Node *_progressBg;
    
public:
    
    LoadingBar *_progressBar;
    
    float _timer;
    
    void update(float dt) override;
    
    virtual void onEnter() override;
    
    function<void()> onFinish;
    
    virtual void updateLayout() override;
    
    virtual bool init() override;
    
    CREATE_FUNC(KLLoadingLayer);
    
    static Scene* scene(function<void()> onFinish){
        auto sc = Scene::create();
        auto la = KLLoadingLayer::create();
        la->onFinish = onFinish;
        sc->addChild(la, 0);
        return sc;
    };
};

class KLProgressLayer : public KTPauseLayer {
    
    Node * _bg;
    Node *_icon;
    
public:
    ~KLProgressLayer();
    static void hide(Layer *parent);
    static void show(Layer *parent);
    
    virtual void updateLayout() override;
    virtual bool init() override;
    
    CREATE_FUNC(KLProgressLayer);
};

class KLAlertLayer;

typedef std::function<void(KLAlertLayer *layer, int buttonIdx)> KLAlertLayerTapCallback;

class KLAlertLayer : public KTPauseLayer {
    
    Node *_bg;
    Node *_panel;
    
    Vector<Node *> buttons;
    
public:
    
    CC_SYNTHESIZE(Text*, _contentLbl, ContentLabel);
    
    std::string _title;
    
    std::string _content;
    
    std::string _cancelButton;
    
    std::vector<std::string> _otherButtons;
    
    KLAlertLayerTapCallback onTap;
    
    CREATE_FUNC(KLAlertLayer);
    
    virtual void onEnter()override;
    
    virtual void updateLayout() override;

    virtual bool init()override;
    
    static KLAlertLayer* show(std::string title, std::string content, std::string cancelButton, std::vector<std::string>otherButtons, KLAlertLayerTapCallback onTap);
    
};

class KLDailyOpenLayer;

typedef std::function<void(KLDailyOpenLayer *layer, int buttonIdx)> KLDailyOpenLayerTapCallback;

class KLDailyOpenLayer : public KTPauseLayer {
    
    Node *_bg;
    Node *_panel;
    Node *_titleLbl;
    Node *_helpBtn;
    Node *_contentLbl;
    Node *_crown;
    Node *_playBtn;
    
public:
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event) override;
    
    virtual bool init() override;
    
    virtual void updateLayout() override;
    
    CREATE_FUNC(KLDailyOpenLayer);
    
    KLDailyOpenLayerTapCallback onTap;
};

class KLNoMoveLayer;

typedef std::function<void(KLNoMoveLayer *layer, int buttonIdx)> KLNoMoveLayerTapCallback;

class KLNoMoveLayer : public KTPauseLayer {
    
    Node *_bg;
    Node *_panel;
    Node *_panel2;
    Node *_titleLbl;
    Node *_titleLbl2;
    Node *_titleLbl3;
    Node *_layout;
    
public:
    
    CC_SYNTHESIZE(CheckBox *, _checkBox, CheckBox);
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    
    virtual bool init();
    
    virtual void updateLayout() override;
    
    CREATE_FUNC(KLNoMoveLayer);
    
    KLNoMoveLayerTapCallback onTap;
};

class KLRestartLayer;

typedef std::function<void(KLRestartLayer *layer, int buttonIdx)> KLRestartLayerTapCallback;

class KLRestartLayer : public KTPauseLayer {
    Node *_bg;
    Node *_panel;
    Node *_layout;
    bool _isDailyChallege;
public:
    
    CC_SYNTHESIZE_PASS_BY_REF(Vector<Button *>, _items, Items);
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event)override;
    virtual void updateLayout() override;
    virtual bool init(bool isDailyChallege);
    
    CREATE_FUNC_PARAM(KLRestartLayer, bool, isDailyChallege);
    
    KLRestartLayerTapCallback onTap;
};

class KLWinLayer;

typedef std::function<void(KLWinLayer *layer, int buttonIdx)> KLWinLayerTapCallback;


class KLWinLayer : public KTPauseLayer{
    
    bool newRecord;
    
    Node *_bg;
    Vector<Node *> _lights;
    Node *_panel;
    
    Node *_titleLbl;
    Node *_titleLbl2;
    Node *_clostBtn;
    Node *againButton;
    Node *_shareBtn;
    Node *_leaderBtn;
    Node *_scoreBtn;
    
    
    Vector<Label *> labels;
    Vector<Label *> detailLabels;
    
public:
    
    ~KLWinLayer();
    
    virtual void updateLayout() override;
    
    virtual bool init(HighScoreModel *m);
    
    CREATE_FUNC_PARAM(KLWinLayer, HighScoreModel *, m);
    
    KLWinLayerTapCallback onTap;
};


class KLDailyInfoLayer : public KTPauseLayer {
    
    Node *_bg;
    Node *_panel;
    Node *_titleLbl;
    Node *_bg2;
    Node *_titleLbl2;
    Node *_box;
    Node *_submitBtn;
    
public:
    
    virtual void updateLayout() override;
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event)override;
    
    virtual bool init()override;
    
    CREATE_FUNC(KLDailyInfoLayer);
};

class KLAllClearLayer : public KTPauseLayer{

    Node *_bg;
    
    Node *_starParent;
    
    Node *_root;
    
    Vector<Sprite *>leftStars;
    
    Vector<Sprite *>rightStars;
public:
    
    function<void()> onRemove;
    
    virtual void updateLayout() override;
    
    virtual bool init() override;
    
    CREATE_FUNC(KLAllClearLayer);
};



class KLRateLayer : public KTPauseLayer {
    
    Node *_bg;
    Node *_panel;
    Node *_titleLbl;
    Node *_submitBtn;
    Node *_closeBtn;
    Node *_titleLbl1;
    Node *_titleLbl2;
    Node *_titleLbl3;
    Vector<Node *> _stars;
    bool _showStar;
public:
    ~KLRateLayer();
    
    virtual void updateLayout() override;
    virtual bool init(bool showStar);
    
    CREATE_FUNC_PARAM(KLRateLayer, bool, showStar);
    
    
    function<void()> onClick;
    static bool show(Node *parent, function<void()> onClick);
    //重置
    static void reset();
};

class KLInputNameLayer : public KTPauseLayer {
    
    Node *_bg;
    
    Node *_panel;
    
    Node *_titleLbl;
    
    Node *_whiteBg;
        
    Node *_submitBtn;
    
public:
    
    CC_SYNTHESIZE(TextField*, _tf, TextField);
    
    virtual void updateLayout() override;
    
    virtual bool init() override;
    
    CREATE_FUNC(KLInputNameLayer);
    
    CC_SYNTHESIZE_PASS_BY_REF(string, _username, UserName);
    
    function<void()> onClick;
};

class KLIntroLayer : public KTPauseLayer {
    Node *_bg;
    Node *_panel;
    Node *_titleLbl;
    Node *_icon;
    Node *_closeBtn;
    Node *_pageView;
public:
    
    std::function<void()> onClose;
    
    KLIntroLayer();
    
    enum class IntroType{
        First = 0,
//        CustomCardbackTheme,
    };
    
    IntroType _introType;
    
    ~KLIntroLayer();
    
    Vector<Sprite *> _stars;
    
    Button *_leftBtn;
    
    Button *_rightBtn;
    
    Button *_startButton;

    virtual void updateLayout() override;
    
    bool init(IntroType introType);
    
//    CREATE_FUNC(KLIntroLayer);
    
    static KLIntroLayer *create(IntroType introType){
        KLIntroLayer *la = new KLIntroLayer();
        la->autorelease();
        la->init(introType);
        return la;
    }
};

#endif /* KLPopupLayer_hpp */
