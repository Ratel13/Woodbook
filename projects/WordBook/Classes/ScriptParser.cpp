//  ScriptParser.cpp
//  GreTest
//
//  Created by lyy on 13-5-3.
//
//

#include "ScriptParser.h"

#define rate1 1.0
#define rate2 1.0



string ScriptParser::readScript(string fileName)
{
    string path = CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName.c_str());
    FILE *fp =fopen(path.c_str(),"r");//根据路径打开文件
    
    char *pchBuf = NULL;//将要取得的字符串
    int  nLen = 0;//将要取得的字符串长度
    fseek(fp,0,SEEK_END);//文件指针移到文件尾
    nLen =ftell(fp); //得到当前指针位置,即是文件的长度
    rewind(fp);   //文件指针恢复到文件头位置
    
    //动态申请空间,为保存字符串结尾标志\0,多申请一个字符的空间
    pchBuf = (char*)malloc(sizeof(char)*nLen+1);
    if(!pchBuf)
    {
        // perror("内存不够!\n");
        exit(0);
    }
    
    //读取文件内容//读取的长度和源文件长度有可能有出入，这里自动调整 nLen
    nLen =fread(pchBuf,sizeof(char), nLen, fp);
    
    pchBuf[nLen] ='\0';//添加字符串结尾标志
    
    //printf("%s\n", pchBuf);//把读取的内容输出到屏幕看看
    string detailStr = pchBuf;
    fclose(fp); //关闭文件
    free(pchBuf);//释放空间
    
    return detailStr;
}
//TODO just demo coding
CCDictionary* ScriptParser::readPlist(string fileName)
{
    //const char* testPlistPath = "Psetting.plist";//"BSPlistDatas\\Psetting.plist";
    //const char* fullPath = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile("Psetting.plist", testPlistPath);
    CCDictionary* plistDic = CCDictionary::createWithContentsOfFile(fileName.c_str());
    
    CCDictionary* levelDic = dynamic_cast<CCDictionary*>(plistDic->objectForKey("particle"));
    CCDictionary* farScene = dynamic_cast<CCDictionary*>(levelDic->objectForKey("bg_far_scene"));
    CCString* farScenePath = dynamic_cast<CCString*>(farScene->objectForKey("particle1"));
    CCPoint point = CCPointFromString(farScene->valueForKey("pos")->getCString());
    
    CCLog("path = %s", farScenePath->getCString());
    CCLog("pos = %f, %f", point.x, point.y);
    return plistDic;
}

map<string, string> ScriptParser::paserScript(string className,string mapName)
{
    className += ".xml";
    string detailStr = ScriptParser::readScript(className);
    map<string, string> aMap;
    
    string pattern1("<title>");
    
    vector<string>ivec1 =  PersonalApi::split(detailStr,pattern1);//取出所有单词套组
    
    for (int j = 1;j<ivec1.size(); j++)
    {
        vector<string> tempIvec;
        
        string sourceStr = ivec1[j];
        
        string patternStr = mapName;
        
        
        if (PersonalApi::isContantString(sourceStr,patternStr))//判断是否可以直接取，或者是要通过字符串合并
        {
            string pattern2("<key>");
            vector<string>ivec2 =  PersonalApi::split(ivec1[j],pattern2);
            for (int i = 1;i<ivec2.size(); i++)
            {
                string pattern3("</key>");//去尾
                vector<string>ivec3 =  PersonalApi::split(ivec2[i],pattern3);
                
                string pattern4("<string>");//去头
                vector<string>ivec4 =  PersonalApi::split(ivec3[1],pattern4);
                
                string pattern5("</string>");//去尾
                vector<string>ivec5 =  PersonalApi::split(ivec4[1],pattern5);
                
                aMap[ivec3[0]]=ivec5[0];
            }
        }
    }
    
    //ScriptParser::traversingMap(aMap);
    return aMap;
}
vector<string> ScriptParser::traversingMap(map<string, string> aMap)
{
    vector<string> aVect;
    map<string, string>::iterator iter;
    
    for (iter = aMap.begin(); iter != aMap.end(); ++iter)
    {
        CCLOG("%s",iter->second.c_str());
    }
    return aVect;
}


CCPoint ScriptParser::getPositionFromPlist(CCDictionary * plistDictionary,const string& positionName)
{
    
    CCDictionary* positionDic = dynamic_cast<CCDictionary*>(plistDictionary->objectForKey("position"));
    CCDictionary* positionNameDic = dynamic_cast<CCDictionary*>(positionDic->objectForKey(positionName));
    CCString* x = dynamic_cast<CCString*>(positionNameDic->objectForKey("x"));
    CCString* y = dynamic_cast<CCString*>(positionNameDic->objectForKey("y"));
    CCPoint point = CCPointMake(x->floatValue()/rate1, y->floatValue()/rate2);
    
    return point;
}

CCSize ScriptParser::getSizeFromPlist(CCDictionary * plistDictionary,const string& sizeName)
{
    CCDictionary* sizeDic = dynamic_cast<CCDictionary*>(plistDictionary->objectForKey("size"));
    CCDictionary* sizeNameDic = dynamic_cast<CCDictionary*>(sizeDic->objectForKey(sizeName));
    CCString* width = dynamic_cast<CCString*>(sizeNameDic->objectForKey("width"));
    CCString* height = dynamic_cast<CCString*>(sizeNameDic->objectForKey("height"));
    CCSize size = CCSizeMake(width->floatValue()/rate1, height->floatValue()/rate2);
    
    return size;
}

float ScriptParser::getFontSizeFromPlist(CCDictionary * plistDictionary,const string& key)
{
    CCDictionary* sizeDic = dynamic_cast<CCDictionary*>(plistDictionary->objectForKey("fontSize"));
    CCString* keyStr = dynamic_cast<CCString*>(sizeDic->objectForKey(key));
    return keyStr->floatValue();
}





const char* ScriptParser::getStringFromPlist(CCDictionary * plistDictionary,const string& key)
{
    CCDictionary* sizeDic = dynamic_cast<CCDictionary*>(plistDictionary->objectForKey("string"));
    CCString* keyStr = dynamic_cast<CCString*>(sizeDic->objectForKey(key));
    
    return keyStr->getCString();
}

map<string,string> ScriptParser::getGroupStringFromPlist(CCDictionary * plistDictionary,const string& stringSectionName)
{
    CCDictionary* sizeDic = dynamic_cast<CCDictionary*>(plistDictionary->objectForKey("groupString"));
    
    CCDictionary* sizeNameDic = dynamic_cast<CCDictionary*>(sizeDic->objectForKey(stringSectionName));
    
    map<string,string>stringMap;
    for (int i = 0; i<sizeNameDic->count(); i++)
    {
        const string str = PersonalApi::convertIntToString(i+1);
        CCString * key = (CCString * )sizeNameDic->objectForKey(str);
        stringMap[PersonalApi::convertIntToString(i+1)]=key->getCString();
    }
    return stringMap;
}

const char* ScriptParser::getImageFromPlist(CCDictionary * plistDictionary,const string& imageName)
{
    CCDictionary* sizeDic = dynamic_cast<CCDictionary*>(plistDictionary->objectForKey("image"));
    CCString* imageNameStr = dynamic_cast<CCString*>(sizeDic->objectForKey(imageName));
    
    return imageNameStr->getCString();
}

map<string,string> ScriptParser::getGroupImageFromPlist(CCDictionary * plistDictionary,const string& imageSectionName)
{
    CCDictionary* sizeDic = dynamic_cast<CCDictionary*>(plistDictionary->objectForKey("groupImage"));
    CCDictionary* sizeNameDic = dynamic_cast<CCDictionary*>(sizeDic->objectForKey(imageSectionName));
    map<string,string>imageMap;
    for (int i = 0; i<sizeNameDic->count(); i++)
    {
        const string str = PersonalApi::convertIntToString(i+1);
        CCString * key = dynamic_cast<CCString*>(sizeNameDic->objectForKey(str));
        imageMap[PersonalApi::convertIntToString(i+1)]=key->getCString();
    }
    
    return imageMap;
}