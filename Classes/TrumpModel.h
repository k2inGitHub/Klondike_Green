#include "cocos2d.h"
using namespace cocos2d;

class TrumpData;

class TrumpModel : public Ref {
private:
    int getRandom(int count);

public:
    static const int COUNT_SUIT = 13; // 每个花色的牌的个数

    static const int COUNT_ALL_CARDS = 52; // 52张
    
    static TrumpModel* create(){
        TrumpModel* tm = new TrumpModel();
        tm->autorelease();
        return tm;
    };
    
    //能赢的局
    cocos2d::Vector<TrumpData*>* winnableFull(std::string sequences);
    
    //随机发放的牌
    cocos2d::Vector<TrumpData*>* winFull();
    
    //重开牌局
    cocos2d::Vector<TrumpData*>* repeateFull();
    
    void feedDeck(int A1[52][2]);
    
    int playDeck(int A1[52][2], int A2[52][2]);
    
    void printOut(int A1[52][2], int A2[52][2], int height);
    
    void saveWinData();
    
    std::string getSequenceString();
};