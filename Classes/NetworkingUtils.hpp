//
//  NetworkingUtils.hpp
//  Klondike
//
//  Created by 宋扬 on 16/12/21.
//
//

#ifndef NetworkingUtils_hpp
#define NetworkingUtils_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "HttpManager.hpp"
using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;

class DownloaderWrapper : public Ref {
public:
    std::unique_ptr<network::Downloader> downloader;
    DownloaderWrapper(){
        downloader.reset(new network::Downloader());
    }
};

typedef std::function<void(const DownloadTask& task)> ImageRequestSuccessCallback;

typedef std::function<void(const DownloadTask& task,
int errorCode,
int errorCodeInternal,
const std::string& errorStr)> ImageRequestFailureCallback;

class NetworkingUtils {
    
public:
    
    static void initDB();
    //带缓存的网络图片请求
    static void loadImage(ImageView *imageView,const string& url, const string& placeHolder = "", ImageRequestSuccessCallback onSuccess = nullptr, ImageRequestFailureCallback onFailure = nullptr);
    
    //带缓存的网络数据请求
    static void sendGet(std::string url, std::unordered_map<std::string, std::string> parameters, HttpRequestSuccessCallback onSuccess, HttpRequestFailureCallback onFailure);
};

#endif /* NetworkingUtils_hpp */
