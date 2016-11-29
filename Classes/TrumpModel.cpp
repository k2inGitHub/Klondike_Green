#include "TrumpModel.h"
#include "TrumpData.h"
#include <sstream>
#include <iostream>
#include<string>
using namespace std;

USING_NS_CC;

static int array1[52][2];
static int array2[52][2];

Vector<TrumpData*>* TrumpModel::winnableFull(std::string sequences) {

    // 随机数种子
    std::srand(time(NULL));
    
    int length = 1;
    
    if (!sequences.empty()) {
        //字符串生牌
        int num = 0;
        std::stringstream ss(sequences);
        std::string sub;
        while(getline(ss,sub,',') && (num < 52))// ',' 是切割字符
        {
            std::stringstream eStr(sub);
            std::string e;
            int num2 = 0;
            while(getline(eStr,e,'-') && (num2 < 2)){
                array1[num][num2] = atoi(e.c_str());
                num2++;
            }
            num++;
        }
    } else {
        //随机生牌
    
        
        std::vector<int> a;
        std::vector<int> b;
        std::vector<int> c;
        std::vector<int> d;
        
        std::vector<int> cards;
        
        for (int i = 0 ;i < 13; i++) {
            a.push_back(i);
        }
        for (int i = 0 ;i < 13; i++) {
            b.push_back(i + 13);
        }
        for (int i = 0 ;i < 13; i++) {
            c.push_back(i + 26);
        }
        for (int i = 0 ;i < 13; i++) {
            d.push_back(i + 39);
        }
        
        
        while(cards.size() != 52) {
            int roll = random(0, 3);// Random.Range(0, 4);
            std::vector<int> *target;
            switch (roll) {
                case 0:{
                    target = &a;
                }
                    break;
                case 1:{
                    target = &b;
                }
                    break;
                case 2:{
                    target = &c;
                }
                    break;
                case 3:{
                    target = &d;
                }
                    break;
                default:
                    break;
            }
            if (target->size() != 0 ){
                auto tmp = target->at(target->size() - 1);
                cards.insert(cards.begin(), tmp);//.Insert(0, tmp);
                target->pop_back();
                //            CCLOG("tmp = %d" , tmp);
            }
        }
        
        //从序列中隔牌抽出28张放在头部
        //最后1张K随机放
        std::vector<int> head;
        int idx = 0;
        bool lastProcess = false;
        while (head.size() < 28) {
            auto num = cards.at(idx);
            
            if (lastProcess) {
                int pos = random(0, (int)head.size()-1);
                head.insert(head.begin() + pos, num);
            } else {
                head.push_back(num);
            }
            
            cards.erase(cards.begin() + idx);
            
            idx++;
            if (idx >= cards.size()) {
                idx = (int)cards.size()-1;
                lastProcess = true;
            }
        }
        cards.insert(cards.begin(), head.begin(), head.end());
        
        // for a hundred times
        for (int i = 0; i < 100; i++) {
            // get two random cards
            int IndexA = random(28, (int)cards.size()-1);//Random.Range(28, cards.size());
            int IndexB = random(28, (int)cards.size()-1);//Random.Range(28, cards.size());
            // perform a swap (Dreieckstausch)
            auto temp = cards[IndexA];
            cards[IndexA] = cards[IndexB];
            cards[IndexB] = temp;
        }
        for (int i = 0; i < cards.size(); i++) {
            int num = cards[i]%13 + 1;
            int suit = cards[i]/13 + 1;
            array1[i][0] = num;
            array1[i][1] = suit;
        }
    }
    
    printOut(array1, array2, length);
    
    auto shuffleCards = new Vector<TrumpData*>();
    
    for (int i = 0; i < COUNT_ALL_CARDS; i++) {
        // 生成从1-52的牌
        shuffleCards->pushBack(TrumpData::create(array1[i][1] - 1, array1[i][0], true));
    }
    
    return shuffleCards;
}

int TrumpModel::getRandom(int count) {

    srand((unsigned int) time(NULL)); //seed

    // 0〜count之间声称一个随机数
    return rand() % count;
}



cocos2d::Vector<TrumpData*>* TrumpModel::repeateFull(){

    auto shuffleCards = new Vector<TrumpData*>();
    
    for (int i = 0; i < COUNT_ALL_CARDS; i++) {
        // 生成从1-52的牌
        shuffleCards->pushBack(TrumpData::create(array1[i][1] - 1, array1[i][0], true));
    }
    
    return shuffleCards;
}

Vector<TrumpData*>* TrumpModel::winFull()
{
    // 随机数种子
    std::srand(time(NULL));
    
    int length = 1;
    
//    auto string = UserDefault::getInstance()->getStringForKey("win_sequence");
    
//    if (!string.empty()) {
//        int num = 0;
//        std::stringstream ss(string);
//        std::string sub;
//        while(getline(ss,sub,',') && (num < 52))// ',' 是切割字符
//        {
//            std::stringstream eStr(sub);
//            std::string e;
//            int num2 = 0;
//            while(getline(eStr,e,'-') && (num2 < 2)){
//                array1[num][num2] = atoi(e.c_str());
//                num2++;
//            }
//            num++;
//        }
//    } else {
        feedDeck(array1);
//    }
    
//    while (length != 0) {
    
//      length = playDeck(array1, array2);
//    }
    printOut(array1, array2, length);
    
    
    auto shuffleCards = new Vector<TrumpData*>();
    
    for (int i = 0; i < COUNT_ALL_CARDS; i++) {
        // 生成从1-52的牌
        shuffleCards->pushBack(TrumpData::create(array1[i][1] - 1, array1[i][0], true));
    }
    
    return shuffleCards;
}

std::string TrumpModel::getSequenceString(){

    string string;
    for (int i = 0; i < 52; i++) {
        char sub[64];
        sprintf(sub, "%d-%d", array1[i][0], array1[i][1]);
        
        string += sub;
        if (i != 51) {
            string += ",";
        }
    }
    return string;
}

void TrumpModel::saveWinData(){

    auto string = getSequenceString();
    
    CCLOG("win seq = %s", string.c_str());
    UserDefault::getInstance()->setStringForKey("win_sequence", string);
    UserDefault::getInstance()->flush();
}

void TrumpModel::feedDeck(int A1[52][2]) {
    for (int i = 0; i < 52; i++) {
        A1[i][0] = 0;
        A1[i][1] = 0;
    }
    for (int i = 1; i <= 4; i++) {
        for (int j = 1; j <= 13; j++) {
            bool repeat = true;
            while (repeat) {
                int random = std::rand() % 52;
                if (A1[random][0] == 0) {
                    A1[random][0] = j;
                    A1[random][1] = i;
                    repeat = false;
                }
            }
        }
    }
}

int TrumpModel::playDeck(int A1[52][2], int A2[52][2]) {
    int height = 0;
    //int maxHeight = 0; //For maxheight addition to game uncomment
    for (int i = 0; i < 52; i++) {
        A2[i][0] = 0;
        A2[i][1] = 0;
    }
    
    for (int i = 51; i >= 0;) {
        if (height < 3) {
            for (; height < 4; height++) {
                if (i >= 0) {
                    A2[height][0] = A1[i][0];
                    A2[height][1] = A1[i][1];
                    i--;
                }
            }
        } else {
            A2[height][0] = A1[i][0];
            A2[height][1] = A1[i][1];
            height++;
            i--;
        }
        bool test = false;
        if (height > 3) {
            while (!test) {
                if (A2[height-1][0] == A2[height-4][0]) {
                    for (int j = 1; j <= 4; j++) {
                        A2[height-j][0] = 0;
                        A2[height-j][1] = 0;
                    }
                    height -= 4;
                } else if (A2[height-1][1] == A2[height-4][1]) {
                    for (int j = 2; j <= 3; j++) {
                        A2[height-j][0] = 0;
                        A2[height-j][1] = 0;
                    }
                    A2[height-3][0] = A2[height-1][0];
                    A2[height-3][1] = A2[height-1][1];
                    A2[height-1][0] = 0;
                    A2[height-1][1] = 0;
                    height -= 2;
                } else {
                    test = true;
                }
            }
        }
        //if (height > maxHeight) //Comment out
        //    maxHeight = height; //Comment out
    }
    //if (maxHeight < 29 && height == 0) //Comment out
    //    height = 1; //Comment out
    return height;
}

void TrumpModel::printOut(int A1[52][2], int A2[52][2], int height) {
    CCLOG("The deck was as follows\n");
    for (int i = 0; i < 52; i++) {
        
        int suite = A1[i][1];
        std::string suit;
        if (suite == 1)
            suit = " ♠️";
        else if (suite == 2)
            suit = " ♥️";
        else if (suite == 3)
            suit = " ♣️";
        else if (suite == 4)
            suit = " ♦️";
        else
            CCLOG("%s suit = %d"," ERROR\n", suite);
        
        int number = A1[i][0];
        if (number < 11 && number != 1)
            CCLOG("%d ...%s %d",i+1, suit.c_str(), A1[i][0]);
        else if (number == 11)
            CCLOG("%d ...%s %s",i+1, suit.c_str(), "J");
        else if (number == 12)
            CCLOG("%d ...%s %s",i+1, suit.c_str(), "Q");
        else if (number == 13)
            CCLOG("%d ...%s %s",i+1, suit.c_str(), "K");
        else if (number == 1)
            CCLOG("%d ...%s %s",i+1, suit.c_str(), "A");
        
    
    }
}
