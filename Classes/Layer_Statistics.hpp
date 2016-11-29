//
//  Layer_Statistics.hpp
//  Klondike
//
//  Created by 宋扬 on 16/7/22.
//
//

#ifndef Layer_Statistics_hpp
#define Layer_Statistics_hpp

#include "KTPauseLayer.h"

class Layer_Statistics : public KTPauseLayer {
    
    Sprite *_bg;
    
    Button *_reset;
    
public:
    
    virtual void updateLayout()override;
    
    int _tabIdx;
    ListView *_listView;
    void loadData(int idx);
    
    vector<string> _titles;
    vector<string> _dataArray1;
    
    Vector<Button *> _tabButtons;
    
    virtual bool init()override;
    CREATE_FUNC(Layer_Statistics);
};

#endif /* Layer_Statistics_hpp */
