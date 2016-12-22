//
//  HttpManager.cpp
//  Klondike
//
//  Created by 宋扬 on 16/7/15.
//
//

#include "HttpManager.hpp"
#include "KTUtils.h"

static Vector<HttpManager *> _mangers;

HttpManager* HttpManager::create(){
    HttpManager *manager = new HttpManager();
    manager->init();
    manager->autorelease();
    return manager;
}

HttpManager::~HttpManager(){
    CCLOG("~HttpManager");
}

HttpManager::HttpManager(){

}

bool HttpManager::init(){

    //ref ++
//    auto timeout = HttpClient::getInstance()->getTimeoutForConnect();
//    CCLOG("timeout = %d", timeout);
    HttpClient::getInstance()->setTimeoutForConnect(6);
    
    return true;
}

void HttpManager::sendGET(std::string url, std::unordered_map<std::string, std::string> parameters, HttpRequestSuccessCallback onSuccess, HttpRequestFailureCallback onFailure){
    
    _mangers.pushBack(this);
    
    this->onSuccess = onSuccess;
    this->onFailure = onFailure;
    
    std::string originUrl = url;
    
//    std::unordered_map<std::string, std::string> content;
//    content["uid"] = "1";
//    
//    std::unordered_map<std::string, std::string> param;
//    param["content"] = KTUtils::jsonString(content);
//    std::string url = "http://45.113.69.21/ao2/play_cards_awards_list.php";
    bool first = true;
    for(auto pair : parameters){
        
        url += (first ? "?" : "&");
        url += KTUtils::urlEncode(pair.first);
        url += "=";
        url += KTUtils::urlEncode(pair.second);
        
        originUrl += (first ? "?" : "&");
        originUrl += pair.first;
        originUrl += "=";
        originUrl += pair.second;
        
        first = false;
    }
    setOriginUrl(originUrl);
    CCLOG("origin url = %s", originUrl.c_str());
    CCLOG("url = %s", url.c_str());
    HttpRequest* request = new (std::nothrow) HttpRequest();
    request->setUrl(url.c_str());
    request->setRequestType(HttpRequest::Type::GET);
    
//    std::vector<std::string> header;
//    header.push_back("Content-Type:application/x-www-form-urlencoded;");
//    request->setHeaders(header);
    
    request->setResponseCallback([=](HttpClient *sender, HttpResponse *response){
        
        if (!response || !response->isSucceed()) {
            //fail
            CCLOG("fail = %s", response->getErrorBuffer());
            
            if (this->onFailure != nullptr) {
                this->onFailure(response->getErrorBuffer());
            }
            
        } else {
            //            CCLOG("success = %s", );
            std::vector<char> *buffer = response->getResponseData(); //获取内容
            std::string responseString((*buffer).begin(), (*buffer).end());
            CCLOG("responseString = %s", responseString.c_str());
            Document doc;
            doc.Parse(responseString.c_str());
            
            if (doc.IsObject()){
                if (this->onSuccess != nullptr) {
                    this->onSuccess(doc);
                }
            } else {
                if (this->onFailure != nullptr) {
                    this->onFailure("data not json dict");
                }
            }
        }
        
        //ref --
        _mangers.eraseObject(this);
    });
    HttpClient::getInstance()->send(request);
}
