//
//  NetworkingUtils.cpp
//  Klondike
//
//  Created by 宋扬 on 16/12/21.
//
//

#include "NetworkingUtils.hpp"
#include "storage/local-storage/LocalStorage.h"
#include "base/base64.h"

static Vector<DownloaderWrapper *> downloaders;
static string _storagePath;
static bool _inited = false;

void NetworkingUtils::initDB(){

    if (!_inited) {
        
        auto path = FileUtils::getInstance()->getWritablePath() + "NetworkingCache.db";
        localStorageInit(path);
        _storagePath = FileUtils::getInstance()->getWritablePath() + "NetworkingCacheFolder/";
        FileUtils::getInstance()->createDirectory(_storagePath);
        _inited = true;
    }
}

void NetworkingUtils::loadImage(ImageView *imageView,const string& url, const string& placeHolder, ImageRequestSuccessCallback onSuccess, ImageRequestFailureCallback onFailure){
    initDB();
    const char* str = url.c_str();
    char * encodeName;
    base64Encode((unsigned char*)str, (unsigned int)strlen(str), &encodeName);
    std::string name(reinterpret_cast<char*>(encodeName));
    auto originSize = imageView->getContentSize();
    
    string texPath;
    localStorageGetItem(name, &texPath);
    if (!texPath.empty()){
    
        imageView->loadTexture(_storagePath+name);
        auto tex = Director::getInstance()->getTextureCache()->getTextureForKey(_storagePath+name);
        auto rect = Rect(0,0,tex->getContentSize().width, tex->getContentSize().height);
        imageView->setScale9Enabled(true);
        imageView->setCapInsets(rect);
        imageView->setContentSize(originSize);
        if (onSuccess) {
            onSuccess(DownloadTask());
        }
        return;
    }
    
    DownloaderWrapper *dw = new DownloaderWrapper();
    
    dw->downloader->createDownloadFileTask(url, _storagePath+name);
    
    if (!placeHolder.empty()) {
        CCLOG("placeHolder = %s", placeHolder.c_str());
        imageView->loadTexture(placeHolder);
        auto tex = Director::getInstance()->getTextureCache()->getTextureForKey(placeHolder);
        auto rect = Rect(0,0,tex->getContentSize().width, tex->getContentSize().height);
        imageView->setScale9Enabled(true);
        imageView->setCapInsets(rect);
        imageView->setContentSize(originSize);
        {
            auto indicator = ImageView::create("_indicator.png");
            auto tex = Director::getInstance()->getTextureCache()->getTextureForKey("_indicator.png");
            auto rect = Rect(0,0,tex->getContentSize().width, tex->getContentSize().height);
            indicator->setScale9Enabled(true);
            indicator->setCapInsets(rect);
            indicator->setContentSize(originSize * 0.6);
            indicator->setPosition(imageView->getContentSize()/2);
            imageView->addChild(indicator, 64, 1136);
            indicator->runAction(RepeatForever::create(RotateBy::create(0.75, 360)));
        }
    }
    dw->downloader->onFileTaskSuccess = [=](const DownloadTask& task){
        localStorageSetItem(name, _storagePath+name);
        imageView->loadTexture(_storagePath+name);
        auto tex = Director::getInstance()->getTextureCache()->getTextureForKey(_storagePath+name);
        auto rect = Rect(0,0,tex->getContentSize().width, tex->getContentSize().height);
        imageView->setScale9Enabled(true);
        imageView->setCapInsets(rect);
        imageView->setContentSize(originSize);
        auto indicator = imageView->getChildByTag(1136);
        if (indicator) {
            indicator->removeFromParent();
        }
        if (onSuccess) {
            onSuccess(task);
        }
        
        downloaders.eraseObject(dw);
    };
    dw->downloader->onTaskError = [=](const DownloadTask& task,
                                 int errorCode,
                                 int errorCodeInternal,
                                 const std::string& errorStr){
        auto indicator = imageView->getChildByTag(1136);
        if (indicator) {
            indicator->removeFromParent();
        }
        if (onFailure) {
            onFailure(task, errorCode, errorCodeInternal, errorStr);
        }
        downloaders.eraseObject(dw);
    };
    dw->downloader->onTaskProgress = [=](const DownloadTask& task,
                                    int64_t bytesReceived,
                                    int64_t totalBytesReceived,
                                    int64_t totalBytesExpected){
    
    };
    
    downloaders.pushBack(dw);
    dw->release();
}

void NetworkingUtils::sendGet(std::string url, std::unordered_map<std::string, std::string> parameters, HttpRequestSuccessCallback onSuccess, HttpRequestFailureCallback onFailure){
    
    initDB();
    
    std::string originUrl = url;
    
    bool first = true;
    for(auto pair : parameters){
        originUrl += (first ? "?" : "&");
        originUrl += pair.first;
        originUrl += "=";
        originUrl += pair.second;
        
        first = false;
    }
    
    HttpRequestSuccessCallback successFunc = [=](const rapidjson::Document &json){
        if (onSuccess) {
            onSuccess(json);
        }
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        json.Accept(writer);
        string value = buffer.GetString();
        CCLOG("success value = %s", value.c_str());
        localStorageSetItem(originUrl, value);
    };
    
    HttpRequestFailureCallback failureFunc = [=](std::string error){
        string value;
        localStorageGetItem(originUrl, &value);
        Document doc;
        doc.Parse(value.c_str());
        if (!value.empty() && doc.IsObject()) {
            if (onSuccess) {
                onSuccess(doc);
            }
        } else {
            if(onFailure){
                onFailure(error);
            }
        }
    };
    auto http = HttpManager::create();
    http->sendGET(url, parameters, successFunc, failureFunc);
}
