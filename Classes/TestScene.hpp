//
//  TestScene.hpp
//  Klondike
//
//  Created by 宋扬 on 16/7/20.
//
//

#ifndef TestScene_hpp
#define TestScene_hpp

#include "KTLayer.h"

class TestLayer : public KTLayer {
    
    
public:
    
    bool show;
    
    Vector<Node *> _labels;
    
    static Scene* scene (){
        Scene *sc = Scene::create();
        sc->addChild(TestLayer::create());
        return sc;
    }
    
    virtual bool init();
    
    CREATE_FUNC(TestLayer);
};

#endif /* TestScene_hpp */
