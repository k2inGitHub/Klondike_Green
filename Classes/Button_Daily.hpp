//
//  Button_Daily.hpp
//  Klondike
//
//  Created by 宋扬 on 16/7/14.
//
//

#ifndef Button_Daily_hpp
#define Button_Daily_hpp

#include "KTLayer.h"
#include "KTDate.hpp"

class Button_Daily : public Button {
    
private:
    
    bool _unlock;
    
    int _day;
    
    ImageView *_crown;
    
    bool _future;
    
    bool _empty;
    
    bool _selected;
    
    bool _isGetToday;
    
protected:
    virtual void onPressStateChangedToNormal() override;
    
public:
//    CC_SYNTHESIZE_PASS_BY_REF(std::string, selectedImage, <#funName#>)
    
    void setTitleText(const std::string &text);
    
    void setSelected(bool value);
    
    bool isSelected();
    
    Button_Daily();
    
    ~Button_Daily();
    
    CC_SYNTHESIZE_RETAIN(KTDate *, _date, Date);
    
    void reset();
    
    void setEmpty(bool value);
    
    bool isEmpty();
    
    bool isFuture();
    
    void setFuture(bool value);
    
    void updateUI();
    
    bool isUnlock();
    
    void setUnlock(bool value);
    
    int getDay();
    
    void setDay(int value);
    
    void setGetToday(bool value);
    
    bool isGetToday();
    
    virtual bool init() override;
    
    CREATE_FUNC(Button_Daily);
};

#endif /* Button_Daily_hpp */
