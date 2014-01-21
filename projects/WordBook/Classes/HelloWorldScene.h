#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
class HelloWorld : public cocos2d::CCLayer
{
public:
    
    std::vector<std::string> wordListVect;
    
    int nowPlayNum;
    
    
    cocos2d::CCLabelTTF* pLabel;
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Http Response Callback
    void goChineseAndPhoneticRequest();
    
    void goSoundRequest();
    
    void goSentenceRequest();
    
    void onHttpRequestCompleted1(cocos2d::extension::CCHttpClient *sender, cocos2d::extension::CCHttpResponse *response);
    
    void onHttpRequestCompleted2(cocos2d::extension::CCHttpClient *sender, cocos2d::extension::CCHttpResponse *response);
    
    void onHttpRequestCompleted3(cocos2d::extension::CCHttpClient *sender, cocos2d::extension::CCHttpResponse *response);
    
    std::vector<std::string> getPhonetic(std::string wordName);
    
    std::vector<std::string> getChinese(std::string wordName);
    
    std::vector<std::vector<std::string> > getSentence(std::string wordName);
    
    void downloadVoice(cocos2d::extension::CCHttpResponse * response);
    
    void Translate(CCObject* pSender);
    void Next(CCObject* pSender);
    void Preview(CCObject* pSender);
    void AutoPlay(CCObject* pSender);
    
    static std::string cutHeadAndTail(std::string str,std::string headPattern,std::string tailPattern);
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
