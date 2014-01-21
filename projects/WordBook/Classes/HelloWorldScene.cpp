#include "HelloWorldScene.h"
#include "cocos-ext.h"
#include "PersonalApi.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;


USING_NS_CC;
using namespace cocos2d::extension;
using namespace std;


CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    nowPlayNum = 0;
    
    wordListVect.push_back("hello");
    wordListVect.push_back("to");
    wordListVect.push_back("my");
    wordListVect.push_back("world");
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    
    CCSize labelSize = CCSizeMake(100.0f, 0.0f);
    pLabel = CCLabelTTF::create("Loading Word..", "Arial", 40, labelSize, kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
    pLabel->setColor(ccc3(255.0f, 0.0f, 156.0f));
    
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height/2));
    
    this->addChild(pLabel, 1);
    
    
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(pLabel->getPositionX(),
                                pLabel->getPositionY()+pLabel->getContentSize().width/2));


    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    
    CCMenuItemFont * translate =CCMenuItemFont::create("Chinese Mean", this, menu_selector(HelloWorld::Translate));
    translate->setPosition(ccp(origin.x + visibleSize.width - translate->getContentSize().width,
                            origin.y + translate->getContentSize().height));
    this->addChild(translate,1);
    
    
    CCMenuItemFont * next =CCMenuItemFont::create("Next", this, menu_selector(HelloWorld::Next));
    next->setPosition(ccp(origin.x + visibleSize.width - next->getContentSize().width,
                               origin.y + visibleSize.height - next->getContentSize().height));
    this->addChild(next,1);
    
    
    CCMenuItemFont * preview =CCMenuItemFont::create("Preview", this, menu_selector(HelloWorld::Preview));
    preview->setPosition(ccp(origin.x + translate->getContentSize().width,
                               origin.y + visibleSize.height - translate->getContentSize().height));
    this->addChild(preview,1);
    
    CCMenuItemFont * autoPlay =CCMenuItemFont::create("AutoPlay", this, menu_selector(HelloWorld::AutoPlay));
    autoPlay->setPosition(ccp(origin.x + autoPlay->getContentSize().width,
                             origin.y + autoPlay->getContentSize().height));
    this->addChild(autoPlay,1);

    CCSprite* pSprite = CCSprite::create("HelloWorld.png");

    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    //this->addChild(pSprite, 0);


    string phoneticStr = CCUserDefault::sharedUserDefault()->getStringForKey((wordListVect[nowPlayNum]+"Phonetic").c_str());
    string chineseStr = CCUserDefault::sharedUserDefault()->getStringForKey((wordListVect[nowPlayNum]+"Chinese").c_str());
    
    if (0 == phoneticStr.length() || 0 == chineseStr.length())// || 0 == sentenceStr.length())
    {
        goChineseAndPhoneticRequest();
    }
    else
    {
        pLabel->setString((wordListVect[nowPlayNum]+phoneticStr).c_str());
    }
    
    if (0 == chineseStr.length())
    {
        goSoundRequest();
    }
    else
    {
        pLabel->setString((wordListVect[nowPlayNum]+phoneticStr+phoneticStr).c_str());
    }
    
    
    
    return true;
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    
    
}
void HelloWorld::goChineseAndPhoneticRequest()
{
    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl("http://dict.youdao.com/search?q=personality&keyfrom=fanyi.smartResult");//识别单词意思，英标
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, httpresponse_selector(HelloWorld::onHttpRequestCompleted1));
    request->setTag("GET test1");
    CCHttpClient::getInstance()->send(request);
    request->release();
    
}
void HelloWorld::goSoundRequest()
{
    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl("http://fanyi.baidu.com/#en/zh/hello");//获取音频
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, httpresponse_selector(HelloWorld::onHttpRequestCompleted2));
    request->setTag("GET test1");
    CCHttpClient::getInstance()->send(request);
    request->release();
}
void HelloWorld::goSentenceRequest()
{
    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl("http://www.iciba.com/good");//获取例句
    //"http://dict.youdao.com/search?q=personality&keyfrom=fanyi.smartResult");//识别单词意思，英标
    //"http://fanyi.baidu.com/#en/zh/hello");//获取音频
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, httpresponse_selector(HelloWorld::onHttpRequestCompleted3));
    request->setTag("GET test1");
    CCHttpClient::getInstance()->send(request);
    request->release();
}
void HelloWorld::onHttpRequestCompleted1(CCHttpClient *sender, CCHttpResponse *response)
{
    if (!response)
    {
        return;
    }
    
    // You can get original request type from: response->request->reqType
    if (0 != strlen(response->getHttpRequest()->getTag()))
    {
        CCLog("%s completed", response->getHttpRequest()->getTag());
    }
    
    int statusCode = response->getResponseCode();
    char statusString[64] = {};
    sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
    CCLog("statusString: %s", statusString);
    CCLog("response code: %d", statusCode);
    
    if (!response->isSucceed())
    {
        CCLog("response failed");
        CCLog("error buffer: %s", response->getErrorBuffer());
        return;
    }
    
 
    // dump data
    
    vector<char> *buffer = response->getResponseData();
    string resultStr;
    printf("Http Test, dump data: ");
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        //printf("%c", (*buffer)[i]);
        resultStr = resultStr+(*buffer)[i];
    }
    //printf("\n");
    //CCLog("%s",resultStr.c_str());
    
    pLabel->setString((wordListVect[nowPlayNum]+
                       CCUserDefault::sharedUserDefault()->getStringForKey((wordListVect[nowPlayNum]+"Phonetic").c_str())).c_str());
    

    
    //;
    //getChinese(resultStr);
    //getSentence(resultStr);
}
void HelloWorld::onHttpRequestCompleted2(CCHttpClient *sender, CCHttpResponse *response)
{
    if (!response)
    {
        return;
    }
    
    // You can get original request type from: response->request->reqType
    if (0 != strlen(response->getHttpRequest()->getTag()))
    {
        CCLog("%s completed", response->getHttpRequest()->getTag());
    }
    
    int statusCode = response->getResponseCode();
    char statusString[64] = {};
    sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
    CCLog("statusString: %s", statusString);
    CCLog("response code: %d", statusCode);
    
    if (!response->isSucceed())
    {
        CCLog("response failed");
        CCLog("error buffer: %s", response->getErrorBuffer());
        return;
    }
    
    
    // dump data
    
    vector<char> *buffer = response->getResponseData();
    string resultStr;
    printf("Http Test, dump data: ");
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        //printf("%c", (*buffer)[i]);
        resultStr = resultStr+(*buffer)[i];
    }
    
    getChinese(resultStr);
    
    pLabel->setString((wordListVect[nowPlayNum]+
                       CCUserDefault::sharedUserDefault()->getStringForKey((wordListVect[nowPlayNum]+"Phonetic").c_str())+
                       CCUserDefault::sharedUserDefault()->getStringForKey((wordListVect[nowPlayNum]+"Chinese").c_str())
                       ).c_str());
    
   
}
void HelloWorld::onHttpRequestCompleted3(CCHttpClient *sender, CCHttpResponse *response)
{
    if (!response)
    {
        return;
    }
    
    // You can get original request type from: response->request->reqType
    if (0 != strlen(response->getHttpRequest()->getTag()))
    {
        CCLog("%s completed", response->getHttpRequest()->getTag());
    }
    
    int statusCode = response->getResponseCode();
    char statusString[64] = {};
    sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
    CCLog("statusString: %s", statusString);
    CCLog("response code: %d", statusCode);
    
    if (!response->isSucceed())
    {
        CCLog("response failed");
        CCLog("error buffer: %s", response->getErrorBuffer());
        return;
    }
    
    
    // dump data
    
    vector<char> *buffer = response->getResponseData();
    string resultStr;
    printf("Http Test, dump data: ");
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        //printf("%c", (*buffer)[i]);
        resultStr = resultStr+(*buffer)[i];
    }
    getSentence(resultStr);
    pLabel->setString((
                       CCUserDefault::sharedUserDefault()->getStringForKey((wordListVect[nowPlayNum]+"Sentence").c_str())
                       ).c_str());
    /*
    pLabel->setString((wordListVect[nowPlayNum]+
                       CCUserDefault::sharedUserDefault()->getStringForKey((wordListVect[nowPlayNum]+"Phonetic").c_str())+
                       CCUserDefault::sharedUserDefault()->getStringForKey((wordListVect[nowPlayNum]+"Chinese").c_str())+
                       CCUserDefault::sharedUserDefault()->getStringForKey((wordListVect[nowPlayNum]+"Sentence").c_str())
                       ).c_str());
     */
}
vector<std::string> HelloWorld::getPhonetic(string wordName)
{
    vector<string> phoneticVect;
    
    //get english
    
    string patternHead1 = "<span class=\"phonetic\">";
    
    vector<string> vect1;
    
    vect1 = PersonalApi::split(wordName,patternHead1);
    
    string patternTail1 = "</span>";
    
    vector<string> vect2;
    
    vect2 = PersonalApi::split(vect1[1],patternTail1);
    
    phoneticVect.push_back(vect2[0]);
    
    
    //get american
    
    vector<string> vect3;
    
    vect3 = PersonalApi::split(vect1[2],patternTail1);
    
    phoneticVect.push_back(vect3[0]);
    
    CCLog("\nen=%s\nAn=%s",vect2[0].c_str(),vect3[0].c_str());
    
    string key = wordListVect[nowPlayNum]+"Phonetic";
    string  combineStr = vect2[0]+vect3[0];
    CCUserDefault::sharedUserDefault()->setStringForKey(key.c_str(),combineStr);
    
    return phoneticVect;
}
vector<string> HelloWorld::getChinese(string wordName)
{
    vector<string> chinese;
    
    string patternHead1 = "<ul>";

    vector<string> vect1;
    
    vect1 = PersonalApi::split(wordName,patternHead1);
    
    string patternTail1 = "</ul>";
    
    vector<string> vect2;
    
    vect2 = PersonalApi::split(vect1[1],patternTail1);
    
    
    string patternHead2 = "<li>";
    
    vector<string> vect3;
    
    vect3 = PersonalApi::split(vect1[1],patternHead2);
    
    string patternTail2 = "</li>";
    
    string  combineStr;
    for (int i = 1; i<vect3.size(); i++)
    {
        chinese.push_back(PersonalApi::split(vect3[i],patternTail2)[0]+"\n");
        combineStr += PersonalApi::split(vect3[i],patternTail2)[0]+"\n";
        CCLog("%s",chinese[i-1].c_str());
    }
    
    string key = wordListVect[nowPlayNum]+"Chinese";
    
    CCUserDefault::sharedUserDefault()->setStringForKey(key.c_str(),combineStr);
    
    return chinese;
}

vector<vector<string> > HelloWorld::getSentence(string wordName)
{
    string  combineStr;
    vector<vector<string> > sentenceVect;
    string patternHead1 = "<dt>1.";
    string patternTail1 = "<!--词典例句 End-->";
    string str1 = HelloWorld::cutHeadAndTail(wordName, patternHead1,patternTail1);
    
    vector<string> vect1;
    vect1 = PersonalApi::split(str1,"</p>");
    
    for (int i = 0; i<vect1.size()-1; i++)
    {
        //get englishSentence
        vector<string> oneSetSentenceVect;
        string originStr1 = vect1[i];
        
        string str2;
        if (0 == i)
        {
            string patternTail2 = "<span";
            str2 = PersonalApi::split(originStr1,patternTail2)[0];
        }
        else
        {
            string patternHead2 = "<dt>";
            string patternTail2 = "<span";
            str2 = HelloWorld::cutHeadAndTail(originStr1, patternHead2,patternTail2);
        }
        
        string patternHead3 = "<b>";
        string patternTail3 = "</b></span>";
        string str3 = HelloWorld::cutHeadAndTail(originStr1, patternHead3,patternTail3);
        
        string patternHead4 = "</span> ";
        string patternTail4 = "<a ";
        string str4 = HelloWorld::cutHeadAndTail(originStr1, patternHead4,patternTail4);
        
        string englishSentence = str2+str3+str4;
        if (0 == i)
        {
            englishSentence = "1. "+englishSentence;
        }
        oneSetSentenceVect.push_back(englishSentence);
        CCLog("%s",englishSentence.c_str());
        
        //get chinesSentence
        
        string patternHead5 = "<dd>";
        string patternTail5 = "</dd>";
        string chineseSentence = HelloWorld::cutHeadAndTail(originStr1, patternHead5,patternTail5);
        oneSetSentenceVect.push_back(chineseSentence);
        CCLog("%s",chineseSentence.c_str());
        //get origin
        
        string patternHead6 = "<p>";
        string origin = PersonalApi::split(originStr1,patternHead6)[1];
        oneSetSentenceVect.push_back(origin);
        CCLog("%s\n",origin.c_str());
        sentenceVect.push_back(oneSetSentenceVect);
        combineStr += englishSentence+"\n"+chineseSentence+"\n"+origin+"\n";
    }
    
    string key = wordListVect[nowPlayNum]+"Sentence";
    CCUserDefault::sharedUserDefault()->setStringForKey(key.c_str(),combineStr);
    //CCLog("%s",str1.c_str());
    
    return sentenceVect;
}

void HelloWorld::downloadVoice(CCHttpResponse * response)
{
    vector<char> *data = response->getResponseData();
    string recieveData(data->begin(),data->end());
    string filepath= CCFileUtils::sharedFileUtils()->getWritablePath();
    filepath +="hello.mp3";
    FILE *fp = fopen(filepath.c_str(), "wb+");
    fwrite(recieveData.c_str(), 1,data->size(),fp);
    fclose(fp);
    
    SimpleAudioEngine::sharedEngine()->playEffect(filepath.c_str());
}
void HelloWorld::Translate(CCObject* pSender)
{
    string sentenceStr = CCUserDefault::sharedUserDefault()->getStringForKey((wordListVect[nowPlayNum]+"Sentence").c_str());
    if (0 == sentenceStr.length())
    {
        goSentenceRequest();
    }
    else
    {
        pLabel->setString(sentenceStr.c_str());
    }
}
void HelloWorld::Next(CCObject* pSender)
{
    if (wordListVect.size() == nowPlayNum)
    {
        return;
    }
    nowPlayNum++;
}
void HelloWorld::Preview(CCObject* pSender)
{
    if (0 == nowPlayNum)
    {
        return;
    }
    nowPlayNum--;
}
void HelloWorld::AutoPlay(CCObject* pSender)
{
    
}
string HelloWorld::cutHeadAndTail(string str, string headPattern,string tailPattern)
{

    string cutStr;
    vector<string> vect1;
    vect1 = PersonalApi::split(str,headPattern);
    vector<string> vect2;
    
    vect2 = PersonalApi::split(vect1[1],tailPattern);
    
    cutStr = vect2[0];
    
    return cutStr;
}

