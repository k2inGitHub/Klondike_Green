#ifndef TrumpData_hpp
#define TrumpData_hpp


#include "cocos2d.h"
using namespace cocos2d;

class TrumpData : public cocos2d::Ref {
public:
    
    int zOrder;
    
    virtual bool init(int suit, int value, bool isReverse){
    
        _suit = suit;
        _value = value;
        _isReverse = isReverse;
        zOrder = 0;
        _fieldX = 0;
        _fieldY = 0;
        
        return true;
    }
    
    static TrumpData*create(){
        TrumpData *td = new TrumpData();
        td->init(0,0,false);
        td->autorelease();
        return td;
    }
    
    static TrumpData* create(int suit, int value, bool isReverse){
        TrumpData *td = new TrumpData();
        td->init(suit, value, isReverse);
        td->autorelease();
        return td;
    }

    int getSuit();

    int getValue();

    int getFieldX();

    void setFieldX(int x);

    int getFieldY();

    void setFieldY(int y);

    bool isReverse();

    void setReverse(bool isReverse);
    
    std::string getString();
    
    ValueMap encode(){
        ValueMap root;
        root["_suit"] = Value(_suit);
        root["_value"] = Value(_value);
        root["_fieldX"] = Value(_fieldX);
        root["_fieldY"] = Value(_fieldY);
        root["_isReverse"] = Value(_isReverse);
        root["zOrder"] = Value(zOrder);
        return root;
    }
    
    void decode(ValueMap map){
        
        _suit = map["_suit"].asInt();
        _value = map["_value"].asInt();
        _fieldX = map["_fieldX"].asInt();
        _fieldY = map["_fieldY"].asInt();
        _isReverse = map["_isReverse"].asBool();
        zOrder = map["zOrder"].asInt();
    }

private:
    int _suit;

    int _fieldX;

    int _fieldY;

    int _value;

    bool _isReverse;

};

#endif
