//
//  Layer_Leaderboard.hpp
//  Klondike
//
//  Created by 宋扬 on 16/8/9.
//
//

#ifndef Layer_Leaderboard_hpp
#define Layer_Leaderboard_hpp

#include "KTPauseLayer.h"
#include "KTLayer.h"
#include "Cell_Setting.hpp"

class LeaderboardModel : public Ref {
    
public:
    
    
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _uid, UserID);
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _name, Name);
    
    CC_SYNTHESIZE(int, _score, Score);
    
    static LeaderboardModel* create(){
        LeaderboardModel *m = new LeaderboardModel();
        m->autorelease();
        return m;
    };
};

class Cell_Leaderboard : public Cell_Base{
    
public:
    
    virtual Widget* createCloneInstance() override;
    virtual void updateLayout() override;
    
    CC_SYNTHESIZE(Button *, _rankButton, RankButton);
    
    CC_SYNTHESIZE(Label *, _titleLabel, TitleLabel);
    
    CC_SYNTHESIZE(Label *, _timeLabel, TimeLabel);
    
    virtual bool init()override;
    
    CREATE_FUNC(Cell_Leaderboard);
};

class Layer_Leaderboard : public KTPauseLayer {
    
public:
    
    Sprite *_bg;
    
    Button *_startButton;
    
    Button *_submitButton;
    
    int _score;
    int _useTime;
    int _minUseTime;
    
    int _firstScore;
    int _firstUseTime;
    int _firstMinUseTime;
    
    int _moves;
    
    Node *_detail1;
    Label *_d1_l1;
    Label *_d1_l2;
    Node *_d1_bg;
    
    Node *_detail2;
    Label *_d2_l1;
    Label *_d2_l2;
    Label *_d2_l3;
    Label *_d2_l4;
    Node *_d2_bg;
    
    Vector<Label *> _headLabels;
    Vector<LeaderboardModel *> _dataArray;
    
    ListView *_listView;
    int _tabIdx;
    void loadData(int idx);
    
    void loadDetailData();
    void updateDetailUI();
    
    Vector<Button *> _tabButtons;
    
    time_t _stamp;
    
//    CC_SYNTHESIZE_PASS_BY_REF(string, _dateString, DateString);
    
    virtual void updateLayout() override;
    virtual bool init(time_t stamp);
    
    CREATE_FUNC_PARAM(Layer_Leaderboard, time_t, stamp);
};

#endif /* Layer_Leaderboard_hpp */
