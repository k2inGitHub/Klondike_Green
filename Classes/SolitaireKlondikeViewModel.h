
#ifndef SolitaireKlondikeViewModel_hpp
#define SolitaireKlondikeViewModel_hpp

#include "cocos2d.h"
//#include "SolitaireKlondikeScene.h"
using namespace cocos2d;

class Trump;

class BlankCard;

//  每一步，拖动纪录
class  TrumpRoute : public cocos2d::Ref{
public:
//    TrumpRoute(){
//    
//    };
//    
//    TrumpRoute(Trump * dragedTrump ,  int fromX, bool isCauseFlip, int score = 0)
//    {
//        this->dragedTrump = dragedTrump;
//        this->fromX = fromX;
//        this->isCauseFlip = isCauseFlip;
//        this->score = score;
//    };
    
    // 每走一步，拖动的那个纸牌
    Trump * dragedTrump;
    // 起始点，
    int fromX;
    // 拖动之后，是不是引起牌库的翻转
    bool isCauseFlip;
    //该步得分
    int score;
    
    virtual bool init(Trump * dragedTrump ,  int fromX, bool isCauseFlip, int score = 0){
        
        
        
        this->dragedTrump = dragedTrump;
        this->fromX = fromX;
        this->isCauseFlip = isCauseFlip;
        this->score = score;
        return true;
    }
    virtual bool init(){
        return true;
    }
    
    static TrumpRoute *create(Trump * dragedTrump ,  int fromX, bool isCauseFlip, int score = 0){
        TrumpRoute *tr = new TrumpRoute();
        tr->init(dragedTrump, fromX, isCauseFlip, score);
        tr->autorelease();
        return tr;
    };
    
    static TrumpRoute *create(){
        TrumpRoute *tr = new TrumpRoute();
        tr->init();
        tr->autorelease();
        return tr;
    };
    
    virtual ValueMap encode();

    virtual void decode(ValueMap map);
};

class SolitaireKlondikeViewModel : public cocos2d::Ref {
public:
    ~SolitaireKlondikeViewModel();
    SolitaireKlondikeViewModel();
    
    /**
    * 牌堆列数
    */
    static const int COUNT_FIELD_MAX_ROWS = 7;

    /**
    * 基础碓列数
    */
    static const int COUNT_DROP_MAX_ROWS = 4;

    /**
    *牌库的最大列数
    */
    static const int COUNT_STOCK_MAX_ROWS = 3;

    /**
    * open牌库x的索引
    */
    static const int INDEX_STOCK_OPEN_X = 11;

    /**
    * close牌库x的索引
    */
    static const int INDEX_STOCK_CLOSE_X = 12;

    /**
    * 隐藏牌库x的索引
    */
    static const int INDEX_STOCK_HIDDEN_X = 13;

    /**
    * 游戏视图model
    */
//    SolitaireKlondikeViewModel(int gameType = 0);
    
    static SolitaireKlondikeViewModel* create(int gameType = 0,std::string sequence="");
    
    //0 随机 1重来 2活局 3 每日挑战
    int gameType;
    
    /**
     * 获得所有纸牌
     */
    cocos2d::Vector<Trump*> getAllCards();
    
    /**
    * 获得牌堆纸牌
    */
    cocos2d::Vector<Trump*> getFieldCards();

    /**
    * 获得指定列的纸牌
    */
    cocos2d::Vector<Trump*> getFieldRow(int row);
    
    int getFieldRowSize(int row);
    
    Trump *getCard(int x, int y);

    /**
    * x,y位置的纸牌是否在最上面
    */
    bool isLast(int x, int y);
    
    /**
     * 获得所有空白牌
     */
    cocos2d::Vector<BlankCard*> getAllBlanks();

    /**
    * 获得指定列的空白牌
    */
    BlankCard* getFieldBlank(int x);

    /**
    * 获得牌库的空白牌集合
    */
    cocos2d::Vector<BlankCard*> getFieldBlanks();

    /**
    * 获得基础堆里的牌集合
    */
    cocos2d::Vector<Trump*> getDropCards();

    /**
    * 获得基础堆某个列的牌集合
    */
    
    Vector<Trump *> getSuitRow(int x);
    
    cocos2d::Vector<Trump*> getDropRow(int x);

    /**
    * 获得基础堆某个列的空白牌
    */
    BlankCard* getDropBlank(int x);

    /**
    * 获得基础堆得空白牌集合
    */
    cocos2d::Vector<BlankCard*> getDropBlanks();

    /**
    * 获得open库中可以拖动的牌
    */
    Trump* getDraggableStockRow();

    /**
    * 获得牌库中Close牌合集
    */
    cocos2d::Vector<Trump*> getCloseStockCards();
    
    /**
     * 获得牌库中Open牌合集
     */
    cocos2d::Vector<Trump*> getOpenStockCards();
    
    /**
     * 获得牌库中Hiden牌合集
     */
    cocos2d::Vector<Trump*> getCloseHidenCards();

    /**
    * 获得牌库的空白牌
    */
    BlankCard* getStockBlank();

    /**
    *  移动纸牌
    */
    Trump* moveCard(Trump* card, int toX);
    
    /**
     * 撤销上一步的移动
     */
    Trump* undoMove(Trump* card, int toX);

    /**
    * 获得指定模式，翻一张  还是  翻3张  模式
    */
    int getDropCount();

    /*
    * 每次翻牌的个数
    */
    static int _dropCount;

    /**
    * back-下面的牌、front上面的牌
    * 判断牌堆中上下两个牌是否可以连接
    */
    bool isKlondikeSequence(Trump* back, Trump* front);

    /**
    * back-下面的牌、front上面的牌
    * 判断基础堆中上下两个牌是否可以连接
    */
    bool isKlondikeDrop(Trump* back, Trump* front);
    
    bool isSameSuit(Trump *a, Trump *b);

    void saveData();
    
    // 获取记录容器
//    cocos2d::Vector<TrumpRoute*> getTrumpRoutes();
    
    virtual ValueMap encode();

    virtual void decode(ValueMap map);
    
    static SolitaireKlondikeViewModel *getInstance();
    
    static const int INDEX_DROP_X = 7; // 基础碓的x起始位置
    
    int getTrumpSize();
    
    TrumpRoute* getTrumRoute(int idx);
    
    void removeTrumpRoute(int idx);
    
    void addTrumRoute(TrumpRoute* tr);
    
private:
    /**
    * 初始化
    */
    void init(int gameType, std::string sequence);

    static const int COUNT_DATA_X = 16; // cards整个列数

    std::vector<Vector<Trump*>> _cards;

    //牌堆
    Vector<BlankCard*> _fieldBlank;

    //回收
    Vector<BlankCard*> _dropBlank;

    //牌库
    BlankCard* _stockBlank;
    
    // 步骤记录容器
    cocos2d::Vector<TrumpRoute*> _trumpRoutes;

};

#endif
