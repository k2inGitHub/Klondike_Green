#ifndef SolitaireKlondikeScene_hpp
#define SolitaireKlondikeScene_hpp

#include "DataManager.h"
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "VisibleRect.h"
#include "SolitaireKlondikeViewModel.h"
#include "KTPauseLayer.h"
#include "BottomMenu.h"
#include "HighScoreModel.hpp"
#include "InterpFloat.hpp"

USING_NS_CC;
using namespace cocos2d::ui;

class BlankCard;

class Trump;

using namespace NSDataManage;

class SolitaireKlondikeScene : public KTLayer, public KTPauseLayerDelegate {
public:
    
    //横竖适配
    bool _isDeal;
    
    SolitaireKlondikeScene();
    
    InterpFloat _adTimer;
    
    InterpFloat _focusAdTimer;
    
    virtual void pauseLayerDidQuit(KTPauseLayer *layer);
    
    SolitaireKlondikeViewModel* getViewModel();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(bool isChanllenge = false);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // 初始化空白牌
    void initBlank();
    
    // 初始化纸牌
    void initCarts();
    
    // 恢复纸牌
    void decodeInitCarts();
    
    void initValues();
    
    void applySetupTheme(string name);
    
    void showAskForSetupTheme(string name);
    
    void updateBadge(Ref *sender);

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);

    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

    void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);
    
    void startGame(int gameType);
    
    void onThemeSetup(cocos2d::Ref* pSender);
    
    // 重开一局
    void NTFNewGameCallback(cocos2d::Ref* pSender);
    //重开本局
    void NTFRepeateGameCallback(cocos2d::Ref* pSender);
    //重开活局
    void NTFNewGameWinnalbeCallback(cocos2d::Ref* pSender);
    
    //每日挑战
    void NTFDailyGameWinnalbeCallback(cocos2d::Ref* pSender);
    
    Button *_quitChanllengeButton;
    
    BottomMenu *_bottomMenu;
    
    Button *_bottomTouchBtn;
    
    float getRowHeight(int x, int y = -1, float dtY = -1);
    
    bool _isDailyChanllenge;
    
    // undo callback
    void NTFUndoCallback(cocos2d::Ref* pSender);
    //下一步 提示
    void OnShowNextMoveCallback(cocos2d::Ref* pSender);
    
    void stopNextMove();
    
    Vector<Sprite *>_tipCards;
    
    void onDailyCallback(cocos2d::Ref* pSender);
    
    int _adNextMoveCounter = 0;
    
    int _adUndoCounter = 0;
    
    //自动收牌
    void autoCollectCardStep();
    //auto open
    void openCardMove(Trump *card);
    
//    void showWinnerAction();
    
    void showWinnerActionCallback();
    //1
    void showWinnerActionNormal(function<void()> cb);
    //2
    void showWinnerActionNewRecord(function<void()> cb);
    
    void stopWinnerAction(int num);
    
    // 设置后刷新
    void NTFSettingRefreshCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(SolitaireKlondikeScene);
    
    CREATE_FUNC_PARAM(SolitaireKlondikeScene, bool, ischallenge);
    
    virtual bool init(bool ischallenge);
    
    void touchEvent(Ref *pSender, Widget::TouchEventType type);
    
    void update(float dt) override;
    
    void onGameStart();
    
    void onGameEnd();
    
    void onGameCancel();
    
    int getTotalScore();
    
    virtual ValueMap encode();
    
    virtual void decode(ValueMap map);
    
    void saveGame(float dt);

    CC_SYNTHESIZE_RETAIN(SolitaireKlondikeViewModel*, _viewModel, ViewModel);
    
    ~SolitaireKlondikeScene();
    
    int getTrumpSize();
    
    CC_SYNTHESIZE_RETAIN(HighScoreModel *, _currentScore, CurrentScore);
    
    CC_SYNTHESIZE(bool, _isVegasOn, IsVegasOn);
    
    CC_SYNTHESIZE(bool, _isVegasScoreAdd, IsVegasScoreAdd);
    
    ValueMap _blankStockModel;
    
    CC_SYNTHESIZE_READONLY(bool,  _isWin;, IsWin);
    
    virtual void updateLocalizedUI()override;
    
    virtual void updateLayout()override;
    
    virtual void onEnter()override;
    
    void onBatteryValueChanged(Ref *sender);
    
    //移牌sequnces
    Vector<Sequence *> actions;
    
    void setBottomVisible(bool value);
    
    void onShareFinish(Ref *sender);
    
    void onApplicationWillEnterForeground(Ref *sender);
    
    void onAdLeaveApp(Ref *sender);
    
    void onApplicationDidEnterBackground(Ref *sender);
    
    float _sessionTimer;
    
private:
    
    bool _checkUpdate;
    
    Vector<Trump *> _movingCards;
    
    bool _needReorderZ;
    
    //Node *_winnerActionParent;
    
    //牌底洗牌 撤销洗牌flag
    bool _isRefreshCard;
    
    bool _isBottomVisible;
    
    //每日挑战 排行榜使用
    string _gameID;
    
    float _autoTipTimer;

    const float kAutoTipTime = 15;
    
    void showStarFx(Trump *card);
    
    Vector<Sprite *> _fxStars;
    
    void preloadFxStar();
    
    Sprite *spawnFxStar();
    
    void despwanFxStar(Sprite *star);
    
    bool _isLastHighScore;
    
    int _sfx_score11_ID;
    
    vector<Trump *> _winnerAdditionTrumps;
    vector<vector<Trump *>> _winnerTrumps;
    
    void playSuitSfx();
    
    int _suitSfxIdx;
    
    float _suitSfxTimer;
    
    const float kSuitSfxTimeMax = 10;
    
    ImageView *_themeBg;
    
    void quitDailyChallenge();
    
    bool _isInitCardFinish;
    
    bool _showNoMoveEnable;
    
    //自动提示无可以移牌操作
    bool _showNoMoveStart;
    
    bool checkNoMove();
    
    int _showNoMoveOpenNum;
    
    bool _gameStartChou3;
    
    bool _gameStart;
    
    float _timer;
    
    int _score;
    
    LayerColor *_tipMask;
    
    ImageView *_statusBar;
    
    Node *_infoParent;
    
    Label *_timeLabel;
    
    Label *_scoreLabel;
    
    Label *_moveLabel;
    
    const int kSysUpdateInteval = 10;

    float _sysTimer;
    
    Label *_sysTimeLabel;
    
    ImageView *_batteryBg;
    
    LoadingBar *_batteryBar;
    
    Label *_winLabel;
    
    Button *_autoButton;
    
//    SolitaireKlondikeViewModel* _viewModel;
    
    bool _isTouchCard = false;

    Trump* _touchingCard;

    cocos2d::Point _touchStartPos;

    cocos2d::Point _touchPos;
    
    cocos2d::Point _beginTouchPos;

    // 是否正在拖拽牌
    bool _isMovingCard = false;
    // 是否正在撤销
    bool _isUndoingCard = false;
    
    // 动画速度
    float  m_tweem_card_move_speed;
    
    // 左手模式=false  右手模式 = true
    bool  m_l_r_mode;
    
    // 设置后刷新
    void refresh();

    //-1 返回refX
    int canTouchCardMove(bool isLast, bool isTip = false);
    
    // 点击的都是但张牌，就是各个x值的最上面的那张牌
    bool onSingleTapOpenCard(bool isLast);

    void onSingleTapClosedStockCard();

    void onSingleTapBlankStockCard();

    void onCompleteDragTween(Ref *sender);

    void flipFieldCard();
    
    // open库中的牌，回撤操作时，需要翻转过去
    void flipOpenStock();

    void endTween();

    cocos2d::Vector<Trump*> _draggedTrumps;

    cocos2d::Vector<Trump*>* getDraggingGroup();

    void moveCard(Trump* card, int toX, float dtY = -1, int ignoreLastNum = 0, function<void()> cb = nullptr);
    
    float getTipMoveTime(Trump* card, int toX);
    
    void tipMoveCard(Trump* card, int toX, int groupIdx);
    
    bool _isTipMove;
    
    bool _isAutoCollect;
    
    bool _isAutoStarted;
    
    // 第三个参数为isDelay 为延迟标记，如果值为true,则表示这个纸牌延迟一下，执行动作
    void undoMove(Trump* card, int toX, bool isDelay = false, float dtY = -1, int ignoreLastNum = 0);
    
    // 把拖动的纸牌轨迹 push 到 步骤记录容器中
    void pushTrumpRoutes(Trump* dragtrump, int fromX, int toX);
    
    // 获得要撤销的上一步拖动的纸牌集合
    cocos2d::Vector<Trump*>* getUndoGroup(Trump* movedTrump);
    
    /*
    调整open库牌的移动，
    isAdd = true 表示新翻开一张牌
    isAdd = false 表示从open库中移除一张牌
     */
    void ajustopenStockCard(bool isAdd, bool animate = true);
    
    void adjustRowHeight(int rowX, int ignoreLastNum = 0, float dtY = -1);
    
    float getDeltaY(int rowX, int addNum);
    
    // 暂时不用这个
    void hideOpenStockCard();

    void hideCard(Trump* card);
    
    float POS_DROP_START_X_NOW();

    float POS_STOCK_START_X_NOW();
    
    float POS_STOCK_OFFSET_X_NOW();
    
    float POS_FIELD_START_X_NOW();
    
    float POS_DROP_OFFSET_X_NOW();
    
    float POS_FIELD_OFFSET_X_NOW();
};


extern "C"{
    
    SolitaireKlondikeScene* getGameScene ();
    
}


#endif
