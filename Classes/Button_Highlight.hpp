//
//  Button_Highlight.hpp
//  Klondike
//
//  Created by 宋扬 on 16/7/26.
//
//

#ifndef Button_Highlight_hpp
#define Button_Highlight_hpp

#include "KTLayer.h"

class Button_Highlight : public Layout {
    
private:
    float _bgOutlineWidth;
    
    ImageView *_badge;
public:
    
    Button_Highlight();
    
    enum class BadgeType{
        Simple,
        New,
    };
    
    void lazyInitBadge();
    
    void setBadgeType(BadgeType badgeType);
    
    void setBadgeEnabled(bool value);
    
    CC_SYNTHESIZE(ImageView *, _addButton, AddIcon);
    
    void setBackgroundOutlineWidth(float value);
    
    std::function<void(Button_Highlight *ref)> onClick;
    
    void setSelected(bool value);
    
    CC_SYNTHESIZE(ImageView *, _icon, Icon);
    
    CC_SYNTHESIZE(Button*, _customButton, CustomButton);
    
    ImageView *_selectIcon;
    
    ImageView *_bgIcon;
    
    virtual bool init(const Size& contentSize);
    
    CREATE_FUNC_PARAM(Button_Highlight, Size&, contentSize);
};

#endif /* Button_Highlight_hpp */
