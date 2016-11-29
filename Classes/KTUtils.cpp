//
//  KTUtils.cpp
//  colorful
//
//  Created by SongYang on 13-10-27.
//
//

#include "KTUtils.h"
//#import <Foundation/Foundation.h>
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "network/HttpClient.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "json/prettywriter.h"
//#include <iconv.h>
#include "SpritePool.hpp"
using namespace KTCocos;
using namespace  rapidjson;


vector<string> split(string str, char splitChar){

    std::string arr = str;
    stringstream ss(arr);
    string sub;
    vector<string> v;
    while(getline(ss,sub,splitChar))// ',' 是切割字符
    {
        if (sub.empty()) {
            continue;
        }
        v.push_back(sub);
    }
    return v;
}

Vec2 KTUtils::getRandomPoint(const cocos2d::Rect& rect){
    Vec2 p = Vec2::ZERO;
    switch (random(0, 3)) {
        case 0:
        {
            p = rect.origin + Vec2(random(0.f, rect.size.width), 0);
        }
            break;
        case 1:
        {
            p = rect.origin + Vec2(random(0.f, rect.size.width), rect.size.height);
        }
            break;
        case 2:
        {
            p = rect.origin + Vec2(0, random(0.f, rect.size.height));
        }
            break;
        case 3:
        {
            p = rect.origin + Vec2(rect.size.width, random(0.f, rect.size.height));
        }
            break;
            
        default:
            break;
    }
    
    return p;
}

Size KTUtils::calculateFontSize(std::string text, float chartWidth, std::string fontName, float fontSize)
{
    std::string tempString = text;
    log("tempString = %s",tempString.c_str());
    size_t computeCount = tempString.size();       //如果字符串很长每次抽取100个字符来计算size；
    Size size = Size(0,0);
    for (int i = 0; i<computeCount ;)
    {
        std::string substring =  tempString.substr(i,1);
        if ((substring.c_str()[0] & 0x80 )!=0) //是汉字
        {
            substring = tempString.substr(i , 3);
            i += 3;
        }
        else
        {
            i++;
        }
        //CCLog("subString  = %s ",substring.c_str());
        auto tempLabel = LabelTTF::create(substring.c_str(),fontName,fontSize);
        tempLabel->setHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
        Size tmpLsize = tempLabel->getContentSize();
        size.width+=tmpLsize.width;
    }
    
    float fHeight= 0;
    if( size.width > chartWidth)//大于容器的宽度
    {
        fHeight = (size.width / 200 );//计算需要多少行
    }
    int nHeight =  ceil(fHeight);
    
    if (nHeight == 0)
    {
        nHeight = 1;
    }
    
    Size labelSize ;
    if (size.width < chartWidth)
    {
        labelSize = Size(size.width,nHeight*32);//计算容器的Size
    }
    else
    {
        labelSize = Size(chartWidth,nHeight*28);
    }
    
    //CCLog("labelSize = (%f, %f)",labelSize.width ,labelSize.height);
    //CCLog("fHeight = %f  nHeight = %d " ,fHeight ,nHeight);
    return labelSize;
}


unsigned char ToHex(unsigned char x)
{
    return  x > 9 ? x + 55 : x + 48;
}

std::string KTUtils::urlEncode(std::string str){
    std::string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++)
    {
        if (isalnum((unsigned char)str[i]) ||
            (str[i] == '-') ||
            (str[i] == '_') ||
            (str[i] == '.') ||
            (str[i] == '~'))
            strTemp += str[i];
        else if (str[i] == ' ')
            strTemp += "+";
        else
        {
            strTemp += '%';
            strTemp += ToHex((unsigned char)str[i] >> 4);
            strTemp += ToHex((unsigned char)str[i] % 16);
        }
    }
    return strTemp;
}

//int KTUtils::code_convert(const char *from_charset,const char *to_charset,char *inbuf,int inlen,char *outbuf,int outlen)
//{
//    iconv_t cd;
//    int rc;
//    char **pin = &inbuf;
//    char **pout = &outbuf;
//    
//    cd = iconv_open(to_charset,from_charset);
//    if (cd==0) return -1;
//    memset(outbuf,0,outlen);
//    iconv_open("", "");
//    
////    size_t iconv (iconv_t /*cd*/,
////                  char ** __restrict /*inbuf*/,  size_t * __restrict /*inbytesleft*/,
////                  char ** __restrict /*outbuf*/, size_t * __restrict /*outbytesleft*/);
//    size_t inlenT = inlen;
//    size_t outlenT = outlen;
////    size_t * inb = &(inlenT);
//    iconv(cd, pin, &inlenT,pout, &outlenT);
////    if (iconv(cd,pin,&inlen,pout,&outlen)==-1) return -1;
//    iconv_close(cd);
//    return 0;
//}
//
///*UTF8 To GB2312*/
//std::string KTUtils::u2a( const char *inbuf )
//{
//    size_t inlen = strlen(inbuf);
//    char * outbuf = new char[inlen * 2 + 2];
//    std::string strRet;
//    if(code_convert("utf-8", "gb2312", inbuf, inlen, outbuf, inlen * 2 + 2) == 0)
//    {
//        strRet = outbuf;
//    }
//    delete [] outbuf;
//    return strRet;
//}

/*GB2312 To UTF8*/
//std::string KTUtils::a2u(const char *inbuf )
//{
//    char *buf = const_cast<char*>(inbuf);
//    
//    size_t inlen = strlen(inbuf);
//    char * outbuf = new char[inlen * 2 + 2];
//    std::string strRet;
//    if(code_convert("gb2312", "utf-8", buf, inlen, outbuf, inlen * 2 + 2) == 0)
//    {
//        strRet = outbuf;
//    }
//    delete [] outbuf;
//    return strRet;
//}

std::string KTUtils::jsonString(std::unordered_map<std::string, std::string> dict){
    
    Document document;
    Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value root(kObjectType);
    for (auto pair : dict) {
        rapidjson::Value key(pair.first.c_str(), allocator);
        rapidjson::Value value(pair.second.c_str(), allocator);
        root.AddMember(key, value, allocator);
        
        CCLOG("paire first = %s second = %s", pair.first.c_str(), pair.second.c_str());
    }
    StringBuffer buffer;
    PrettyWriter<StringBuffer> writer(buffer);
    root.Accept(writer);
    
    std::string ret = buffer.GetString();
    CCLOG("ret = %s", ret.c_str());
    return ret;
}

//const char* KTUtils::localString(const char *key)
//{
//    NSString *str = NSLocalizedString([NSString stringWithUTF8String:key], nil);
//    return str.UTF8String;
//}

//CCSprite *createSpriteFromWriteablePath(const char* name)
//{
//    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath();
//    path+=name;
//    FILE* fp = fopen(path.c_str(),"rb");
//    if (!fp)
//    {
//        return NULL;
//    }
//    fseek(fp,0,SEEK_END);
//    int len = ftell(fp);
//    fseek(fp,0,SEEK_SET);
//    char* buf = (char*)malloc(len);
//    fread(buf,len,1,fp);
//    fclose(fp);
//    CCImage* img = new CCImage;
//    img->initWithImageData(buf,len);
//    free(buf);
//    cocos2d::CCTexture2D* texture = new cocos2d::CCTexture2D();
//    bool isImg = texture->initWithImage(img);
//    img->release();
//    if (!isImg)
//    {
//        delete texture;
//        return CCSprite::create("50black.png");//加载资源并非图片时返回的默认图
//    }
//    CCSprite* sprite = CCSprite::createWithTexture(texture);
//    texture->release();
//    return sprite;
//}
