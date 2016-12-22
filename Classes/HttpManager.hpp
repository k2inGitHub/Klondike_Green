//
//  HttpManager.hpp
//  Klondike
//
//  Created by 宋扬 on 16/7/15.
//
//

#ifndef HttpManager_hpp
#define HttpManager_hpp

#include "cocos2d.h"
using namespace cocos2d;
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "network/HttpClient.h"
using namespace cocos2d::network;
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
using namespace  rapidjson;

typedef std::function<void(const rapidjson::Document &json)> HttpRequestSuccessCallback;

typedef std::function<void(std::string error)> HttpRequestFailureCallback;


/**
 AFNetworking式cpp封装
 */
class HttpManager : public Ref{
    
    
    
public:
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _originUrl, OriginUrl);
    
    HttpRequestSuccessCallback onSuccess;
    HttpRequestFailureCallback onFailure;
    ~HttpManager();
    HttpManager();
    virtual bool init();
    
    static HttpManager* create();
    
    void sendGET(std::string url, std::unordered_map<std::string, std::string> parameters, HttpRequestSuccessCallback onSuccess, HttpRequestFailureCallback onFailure);
    
};

#endif /* HttpManager_hpp */
