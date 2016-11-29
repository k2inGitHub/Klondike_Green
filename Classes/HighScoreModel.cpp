//
//  HighScoreModel.cpp
//  Klondike
//
//  Created by 宋扬 on 16/7/21.
//
//

#include "HighScoreModel.hpp"

HighScoreModel::~HighScoreModel(){

    CC_SAFE_RELEASE_NULL(_chanllengeDate);
}

bool HighScoreModel::init(){

    _chanllengeDate = nullptr;
    return true;
}