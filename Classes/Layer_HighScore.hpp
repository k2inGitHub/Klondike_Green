//
//  Layer_HighScore.hpp
//  Klondike
//
//  Created by 宋扬 on 16/7/21.
//
//

#ifndef Layer_HighScore_hpp
#define Layer_HighScore_hpp

#include "KTPauseLayer.h"

class Layer_HighScore : public KTLayer {
    
    Sprite* _bg;
    ListView* _listView;
public:
    
    virtual void updateLayout() override;
    
    void selectedItemEventScrollView(Ref* pSender, ui::ScrollView::EventType type);
    void selectedItemEvent(Ref *pSender, ListView::EventType type);
    virtual bool init() override;
    CREATE_FUNC(Layer_HighScore);
};

#endif /* Layer_HighScore_hpp */
