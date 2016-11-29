#include "SolitaireKlondikeViewModel.h"
#include "BlankCard.h"
#include "Trump.h"
#include "TrumpData.h"
#include "TrumpModel.h"
#include "SolitaireKlondikeScene.h"

USING_NS_CC;

/*
*
*
*   基础堆(4列)                   open库              close库        隐藏牌库(暂时没用)
* x:7〜10 y:0〜12               x:11 y:0〜           x:12 y:0〜      x:13 y:0〜
*
*  牌堆(7列)
* x:0〜6 y:0〜
*
*
*
*/



bool isSameSuit(Trump *a, Trump *b){
    int num = a->getData()->getSuit() - b->getData()->getSuit();
    if (abs(num) == 1) {
        return true;
    }
    return false;
}

void TrumpRoute::decode(ValueMap map){
//    SolitaireKlondikeScene * gameScene = (SolitaireKlondikeScene *)Director::getInstance()->getRunningScene();
//        if (gameScene) {
    
    TrumpData *data = TrumpData::create();
    data->decode(map["dragedTrump"].asValueMap());
    dragedTrump = SolitaireKlondikeViewModel::getInstance()->getFieldRow(data->getFieldX()).at(data->getFieldY());
    fromX = map["fromX"].asInt();
    isCauseFlip = map["isCauseFlip"].asBool();
    score = map["score"].asInt();
    
}

static SolitaireKlondikeViewModel *_instance = nullptr;


ValueMap TrumpRoute::encode(){
    ValueMap root;
    root["fromX"] = Value(fromX);
    root["isCauseFlip"] = Value(isCauseFlip);
    root["score"] = Value(score);
    root["dragedTrump"] = Value(this->dragedTrump->getData()->encode());
    return root;
}

SolitaireKlondikeViewModel::SolitaireKlondikeViewModel():
_stockBlank(nullptr)
{
    
    
}

SolitaireKlondikeViewModel::~SolitaireKlondikeViewModel(){
    CC_SAFE_RELEASE_NULL(_stockBlank);
}

SolitaireKlondikeViewModel* SolitaireKlondikeViewModel::create(int gameType, std::string sequence){
    SolitaireKlondikeViewModel* model = new SolitaireKlondikeViewModel();
    _instance = model;
    model->init(gameType,sequence);
    model->autorelease();
    return model;
};

//SolitaireKlondikeViewModel::SolitaireKlondikeViewModel(int gameType) {
//
//    _instance = this;
//    // TODO 存档
//    
//    //初始化
//    init(gameType);
//}

int SolitaireKlondikeViewModel::getTrumpSize(){

    return _trumpRoutes.size();
}

TrumpRoute* SolitaireKlondikeViewModel::getTrumRoute(int idx){

    return _trumpRoutes.at(idx);
}

void SolitaireKlondikeViewModel::removeTrumpRoute(int idx){
    _trumpRoutes.erase(idx);
}

void SolitaireKlondikeViewModel::addTrumRoute(TrumpRoute* tr){
    _trumpRoutes.pushBack(tr);
}

ValueMap SolitaireKlondikeViewModel::encode(){
    ValueMap root;
    ValueVector cards;
    for (int i = 0; i < 14; i++) {
        ValueVector rows;
        Vector<Trump *> cardRows = _cards.at(i);
        for (int i = 0; i < cardRows.size(); i++) {
            auto card = cardRows.at(i);
            auto data = card->getData();
            rows.push_back(Value(data->encode()));
        }
        cards.push_back(Value(rows));
    }
    root["cards"] = cards;
    
    
    ValueVector trumpRoutesVector;
    for (auto route : _trumpRoutes) {
        auto map = route->encode();
        trumpRoutesVector.push_back(Value(map));
    }
    root["trumpRoutes"] = trumpRoutesVector;
    
    return root;
}


SolitaireKlondikeViewModel *SolitaireKlondikeViewModel::getInstance(){

    if (_instance) {
        return _instance;
    }
    return nullptr;
}

void SolitaireKlondikeViewModel::decode(ValueMap map){
    ValueVector cards = map["cards"].asValueVector();
    for (int i = 0; i < 14; i++) {
        Vector<Trump *> rows;
        for (auto cardValue : cards[i].asValueVector()) {
            auto data = TrumpData::create();
            data->decode(cardValue.asValueMap());
            Trump *card = Trump::create(data);
            card->setLocalZOrder(data->zOrder);
            rows.pushBack(card);
        }
        _cards[i] = rows;
    }
    Vector<TrumpRoute *> rows;
    ValueVector trumpRoutesVector = map["trumpRoutes"].asValueVector();
    for (auto trumpRouteMap : trumpRoutesVector) {
        TrumpRoute *r = TrumpRoute::create();
        r->decode(trumpRouteMap.asValueMap());
        rows.pushBack(r);
    }
    _trumpRoutes = rows;
   
}

void SolitaireKlondikeViewModel::init(int gameType, std::string sequence) {

    this->gameType = gameType;
    // 生成牌序的model
    TrumpModel* model = TrumpModel::create();
    
    // 随机牌面的初始化
    // Vector<TrumpData*> trumpData = model->dealFull();
    // 避免死局的初始化
    Vector<TrumpData*> trumpData;
    
    if (gameType == 0) {
        trumpData = *model->winFull();
    } else if (gameType == 1 ) {
        trumpData = *model->repeateFull();
    } else {
        trumpData = *model->winnableFull(sequence);
    }
    
    for (int x = 0; x < COUNT_FIELD_MAX_ROWS; x++) {
        Vector<Trump*> rows;
        _cards.push_back(rows);
//        _fieldBlank[x] = BlankCard::create(true, x, 0);
        _fieldBlank.pushBack(BlankCard::create(true, x, 0));
    }
    // 牌堆初始化 (x:0〜6)
    int index = 27;
    int row = 7;
    int x = row-1,y=0;
    for (int i = row - 1; i >= 0; i--) {
        x = row - 1;
        y = x - i;
        while( (x >= 0) && (y >= 0)){
            //            card = deckPile.getCard();
            //            tablePile[x].addCard(card);
            
//            auto trump = _viewModel->getFieldRow(x)->at(y);
//            // 初期位置在牌库的位置
//            trump->setPosition(Point(POS_STOCK_START_X * ScaleFactor::scale_x, POS_STOCK_START_Y * ScaleFactor::scale_y));
//            this->addChild(trump, y);
            
            trumpData.at(index)->setFieldX(x);
            trumpData.at(index)->setFieldY(y);
            Trump* card = Trump::create(trumpData.at(index));
            _cards.at(x).pushBack(card);
            
            x--;
            y = x - i;
            index--;
        }
    }
    
//    for (int x = 0; x < COUNT_FIELD_MAX_ROWS; x++) {
//
//        _cards[x] = new Vector<Trump*>();
//        _fieldBlank[x] = new BlankCard(true, x, 0);
//
//        int y = 0;
//        while (y <= x) {
//            trumpData->at(index)->setFieldX(x);
//            trumpData->at(index)->setFieldY(y);
//
//            Trump* card = Trump::create(trumpData->at(index));//new Trump(trumpData->at(index));
//            _cards[x]->pushBack(card);
//
//            y++;
//            index++;
//        }
//    }

    // 基础碓的初始化 (x:7〜10)
    for (int x = INDEX_DROP_X; x < INDEX_DROP_X + COUNT_DROP_MAX_ROWS; x++) {
//        _cards[x] = new Vector<Trump*>();
        Vector<Trump*> v;
//        _cards.replace(x, v);
        _cards.push_back(v);
    }
    for (int i = 0; i < COUNT_DROP_MAX_ROWS; i++) {
        //ace
//        _dropBlank[i] = BlankCard::create(false, INDEX_DROP_X + i, 0, 1);
        _dropBlank.pushBack(BlankCard::create(false, INDEX_DROP_X + i, 0, 1));
    }

    // 牌库的初始化 (x:11)
//    _cards.at(INDEX_STOCK_OPEN_X) = new Vector<Trump*>();
    Vector<Trump*> v1;
    _cards.push_back(v1);
    // 山札(裏向き)データを作る(x:12)
    Vector<Trump*> v2;
//    _cards.at(INDEX_STOCK_CLOSE_X) = new Vector<Trump*>();
    _cards.push_back(v2);

    index = 28;
    y = 0;
    while (index < TrumpModel::COUNT_ALL_CARDS) {

        TrumpData* data = trumpData.at(index);
        data->setFieldX(INDEX_STOCK_CLOSE_X);
        data->setFieldY(y);

        Trump* card = Trump::create(data);//new Trump(data);

        _cards.at(INDEX_STOCK_CLOSE_X).pushBack(card);

        index++;
        y++;
    }
    _stockBlank = BlankCard::create(false, INDEX_STOCK_CLOSE_X, 0, 2);
    _stockBlank->retain();

    // 隐藏的牌库的初始化
    Vector<Trump*> v3;
    _cards.push_back(v3);//.at(INDEX_STOCK_HIDDEN_X).clear();
    // 步骤记录容器的初始化
    _trumpRoutes.clear();
}

Vector<Trump*> SolitaireKlondikeViewModel::getAllCards(){
    Vector<Trump*> cards;
    for (int x = 0; x < COUNT_FIELD_MAX_ROWS; x++) {
        for (Trump* trump : _cards.at(x)) {
            cards.pushBack(trump);
        }
    }
    for (int x = COUNT_FIELD_MAX_ROWS; x < INDEX_STOCK_OPEN_X; x++) {
        for (Trump* trump : _cards.at(x)) {
            cards.pushBack(trump);
        }
    }
    
    for (Trump* trump : _cards.at(INDEX_STOCK_OPEN_X)) {
        cards.pushBack(trump);
    }
    for (Trump* trump : _cards.at(INDEX_STOCK_CLOSE_X)) {
        cards.pushBack(trump);
    }
    for (Trump* trump : _cards.at(INDEX_STOCK_HIDDEN_X)) {
        cards.pushBack(trump);
    }
    
    return cards;
}

Vector<Trump*> SolitaireKlondikeViewModel::getFieldCards() {

    Vector<Trump*> cards;
    for (int x = 0; x < COUNT_FIELD_MAX_ROWS; x++) {
        for (Trump* trump : _cards.at(x)) {
            cards.pushBack(trump);
        }
    }
    return cards;
}

Vector<Trump*> SolitaireKlondikeViewModel::getFieldRow(int x) {
    return _cards.at(x);
}

int SolitaireKlondikeViewModel::getFieldRowSize(int x){
    return _cards.at(x).size();
}

Trump *SolitaireKlondikeViewModel::getCard(int x, int y){
    return _cards.at(x).at(y);
}

bool SolitaireKlondikeViewModel::isLast(int x, int y) {

    Vector<Trump*> row = _cards.at(x);

    return (row.size() == 0) ? true : ((row.size() - 1) == y);
}

Vector<BlankCard*> SolitaireKlondikeViewModel::getAllBlanks(){
    Vector<BlankCard*> blanks;
    for (int x = 0; x < COUNT_FIELD_MAX_ROWS; x++) {
        blanks.pushBack(_fieldBlank.at(x));
    }
    for (int x = 0; x < COUNT_DROP_MAX_ROWS; x++) {
        blanks.pushBack(_dropBlank.at(x));
    }
    blanks.pushBack(_stockBlank);
    return blanks;
}

BlankCard* SolitaireKlondikeViewModel::getFieldBlank(int x) {
    return _fieldBlank.at(x);
}

Vector<BlankCard*> SolitaireKlondikeViewModel::getFieldBlanks() {

    Vector<BlankCard*> blanks;
    for (int x = 0; x < COUNT_FIELD_MAX_ROWS; x++) {
        blanks.pushBack(_fieldBlank.at(x));
    }
    return blanks;
}

Vector<Trump*> SolitaireKlondikeViewModel::getDropCards() {

    Vector<Trump*> cards;
    for (int x = COUNT_FIELD_MAX_ROWS; x < INDEX_STOCK_OPEN_X; x++) {
        for (Trump* trump : _cards.at(x)) {
            cards.pushBack(trump);
        }
    }
    return cards;
}

Vector<Trump *> SolitaireKlondikeViewModel::getSuitRow(int x){
    
//    Vector<Trump*> cards = new Vector<Trump*>();
    return _cards.at(COUNT_FIELD_MAX_ROWS + x);
}

Vector<Trump*> SolitaireKlondikeViewModel::getDropRow(int x) {
    return _cards.at(x);
}

BlankCard* SolitaireKlondikeViewModel::getDropBlank(int x) {
    return _dropBlank.at(x);
}

Vector<BlankCard*> SolitaireKlondikeViewModel::getDropBlanks() {

    Vector<BlankCard*> blanks;
    for (int x = 0; x < COUNT_DROP_MAX_ROWS; x++) {
        blanks.pushBack(_dropBlank.at(x));
    }
    return blanks;
}

Trump* SolitaireKlondikeViewModel::getDraggableStockRow() {

    
    if (_cards.at(INDEX_STOCK_OPEN_X).size() > 0) {
        return _cards.at(INDEX_STOCK_OPEN_X).back();
    }
    return nullptr;
}

Vector<Trump*> SolitaireKlondikeViewModel::getCloseStockCards() {
    return _cards.at(INDEX_STOCK_CLOSE_X);
}

Vector<Trump*> SolitaireKlondikeViewModel::getOpenStockCards(){
    return _cards.at(INDEX_STOCK_OPEN_X);
}

Vector<Trump*> SolitaireKlondikeViewModel::getCloseHidenCards(){
    return _cards.at(INDEX_STOCK_HIDDEN_X);
}

BlankCard* SolitaireKlondikeViewModel::getStockBlank() {
    return _stockBlank;
}

//Vector<TrumpRoute*> SolitaireKlondikeViewModel::getTrumpRoutes(){
//    return _trumpRoutes;
//}

Trump* SolitaireKlondikeViewModel::moveCard(Trump* card, int toX) {

    int fromX = card->getData()->getFieldX();
    int fromY = card->getData()->getFieldY();

    Trump* moveCard = _cards.at(fromX).at(fromY);

    // 添加到新的牌vector中
    _cards.at(toX).pushBack(moveCard);
    moveCard->getData()->setFieldX(toX);
    
    int toY =_cards.at(toX).size() - 1;
    moveCard->getData()->setFieldY(toY);

    // 之前的牌vector中 移除这个牌
    _cards.at(fromX).erase(fromY);

    // 调整之前牌集合中每个牌的Y值
    for (Trump* card : _cards.at(fromX)) {
        if (card->getData()->getFieldY() > fromY) {
            card->getData()->setFieldY(card->getData()->getFieldY() - 1);
        }
    }
    
    // TODO 移動履歴スタックを積む


    return moveCard;
}

Trump* SolitaireKlondikeViewModel::undoMove(Trump* card, int toX){
    
    int fromX = card->getData()->getFieldX();
    int fromY = card->getData()->getFieldY();
    
    Trump* moveCard = _cards.at(fromX).at(fromY);
    
    // 添加到之前的牌vector中
    if(toX == INDEX_STOCK_CLOSE_X){
      _cards.at(toX).insert(0, moveCard);
        // 调整之前牌集合中每个牌的Y值
        for (Trump* card : _cards.at(toX)) {
                card->getData()->setFieldY(card->getData()->getFieldY() + 1);
        }
        moveCard->getData()->setFieldY(0);
        moveCard->getData()->setReverse(true);
    }
    else{
      _cards.at(toX).pushBack(moveCard);
      int toY = (int)_cards.at(toX).size() - 1;
      moveCard->getData()->setFieldY(toY);
    }
    moveCard->getData()->setFieldX(toX);
    
    // 之前的牌vector中 移除这个牌
    _cards.at(fromX).erase(fromY);
    
    // 调整之前牌集合中每个牌的Y值
    for (Trump* card : _cards.at(fromX)) {
        if (card->getData()->getFieldY() > fromY) {
            card->getData()->setFieldY(card->getData()->getFieldY() - 1);
        }
    }
    
    
    
    return moveCard;
}

int SolitaireKlondikeViewModel::getDropCount() {
    return _dropCount;
}

// back(被覆盖的那张纸牌)、front(覆盖的纸牌)
// 判断两张纸牌是否符合覆盖的规则
bool SolitaireKlondikeViewModel::isKlondikeSequence(Trump* back, Trump* front) {

    // Suit判断
    if (back->getData()->getSuit() == 0 || back->getData()->getSuit() == 2) {

        if (front->getData()->getSuit() == 0 || front->getData()->getSuit() == 2) {
            return false;
        }
    } else {
        if (front->getData()->getSuit() == 1 || front->getData()->getSuit() == 3) {
            return false;
        }
    }

    // 数字大小是否连接
    return ((back->getData()->getValue() - 1) == front->getData()->getValue());
}

// back(被覆盖的那张纸牌)、front(覆盖的纸牌)
// 是否可以放入基础碓
bool SolitaireKlondikeViewModel::isKlondikeDrop(Trump* back, Trump* front) {

    // Suit判断
    if (front->getData()->getSuit() != back->getData()->getSuit()) {
        return false;
    }

    // 数字大小是否衔接
    return ((back->getData()->getValue() + 1) == front->getData()->getValue());
}

void SolitaireKlondikeViewModel::saveData() {


    ValueMap vmap;
    vmap["hoge"] = Value(1);
}

// static
int SolitaireKlondikeViewModel::_dropCount;
