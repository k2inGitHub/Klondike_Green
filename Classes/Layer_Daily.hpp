//
//  Layer_Daily.hpp
//  Klondike
//
//  Created by 宋扬 on 16/7/14.
//
//

#ifndef Layer_Daily_hpp
#define Layer_Daily_hpp

#include "KTPauseLayer.h"
#include "Button_Daily.hpp"
#include "MonthModel.hpp"
#include "KTDate.hpp"
#include "SolitaireKlondikeScene.h"


class Layer_Daily : public KTPauseLayer {
    
    Sprite *_bg;
    
    Button *_leftBtn;
    
    Button *_rightButton;
    
    Node* _rankBg;
    
    Button *_rankButton;
    
    Vector<Node *> _labels;
    
    Node *_white1;
    
    Node *_white2;
    
    vector<Vector<Node *>> _dailyButtons;
    
    Node *_greenBg;
    
    Node *_greenCrown;
    
    Node *_greenProgressBg;
    
    Node *_badgeRoot;
    
    Node *_dayCrown;
    
    Node *_dayBgGreen;
    
public:
    
    void updateLayout() override;
    
    CC_SYNTHESIZE(SolitaireKlondikeScene *, _gameScene, GameScene);
    
    CC_SYNTHESIZE(bool, _fromGameMenu, FromGameMenu);
    
    int _historyTotal;
    
    void startCardGame();
    
    Vector<Button *> _badges;
    
    void onSelectButton(Button_Daily *btn);
    
    int _selectedDayIdx;
    
    void update(float dt);
    
    virtual void onEnter();
    
    Layer_Daily();
    
    ~Layer_Daily();
    
//    CC_SYNTHESIZE_RETAIN(KTDate *, _requestDate, RequestDate);
    
    int _monthIdx;
        
    MonthModel* getCurrentMonth();
    
    Vector<MonthModel *>_dataArray;
    
    void updateUI();
    
    void requestData();
    
    LoadingBar *_progressBar;
    
    Button *_startButton;
    
    ImageView *_unlockImg;
    
    Label *_unlockLabel;
    
    Label *_historyLabel;
    
    Label *_dayLabel;
    
    Label *_monthLabel;
    
    ImageView *_badgeIcon;
    
    Vector<Button_Daily *> _buttons;
    
    Button_Daily *_selectedButton;
    
    void popToLeaderBoard();
    
    void uploadUserName(string name, function<void()> onFinish);
    
    virtual bool init(KTPauseLayerDelegate *delegate);
    
    CREATE_FUNC_PARAM(Layer_Daily, KTPauseLayerDelegate *, delegate);
    
    void touchEvent(Ref *pSender, Widget::TouchEventType type);
    
    int getDayOffset(MonthModel* model, Button_Daily* btn);
};

#endif /* Layer_Daily_hpp */
