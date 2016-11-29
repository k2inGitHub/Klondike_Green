#include "cocos2d.h"
using namespace cocos2d;

class BlankCard : public cocos2d::Sprite {
private:
    bool _isField; // 是否在牌库中

    int _x;

    int _y;
    
    int _type;
    
    int _flipChance;
    
    Sprite *_icon;
    Sprite *_icon2;
    
    Label *_label1;
    Label *_label2;
    
    Sprite *getIcon();
    
    Sprite *getIcon2();
    
public:
    BlankCard();
    
    
    virtual cocos2d::Rect getBoundingBox() const override;
    virtual bool init(bool isField, int x, int y, int type = 0);
    
    static BlankCard *create(bool isField, int x, int y, int type = 0){
        BlankCard *bc = new BlankCard();
        bc->init(isField, x, y, type);
        bc->autorelease();
        return bc;
    }
    
    ValueMap encode(){
        ValueMap root;
        root["_type"] = Value(_type);
        root["_flipChance"] = Value(_flipChance);
        return root;
    }
    
    void decode(ValueMap map){
        setType(map["_type"].asInt());
        _flipChance = map["_flipChance"].asInt();
    }
    
    void setType(int type);
    int getType(){
        return _type;
    }
    
    void setFlipChance(int num){
        _flipChance = num;
        if (_label2) {
            char str[64];
            sprintf(str, "%d MORE\n\n\n\nFLIPS", _flipChance);
            _label2->setString(str);
        }
    };
    int getFlipchance(){
        return _flipChance;
    };
    bool isOutOfFlips(){
        return _type == 3;
    }
//    BlankCard(bool isField, int x, int y, int type = 0);

    bool isField();

    int getX();

    int getY();

};