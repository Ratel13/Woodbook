//  ScriptParser.h
//  GreTest
//
//  Created by lyy on 13-5-3.
//
//

#ifndef __GreTest__ScriptParser__
#define __GreTest__ScriptParser__

#include "PersonalApi.h"
#include "cocos2d.h"
#include<map>
#include<vector>
#include<string>
#include<cstdio>

using namespace cocos2d;
using namespace std;

class ScriptParser
{
public:
    
    static string readScript(string fileName);
    
    static CCDictionary* readPlist(string fileName);
    
    static map<string, string> paserScript(string className,string mapName);
    
    static vector<string> traversingMap(map<string, string> aMap);
    
    static CCPoint getPositionFromPlist(CCDictionary * plistDictionary,const string& positionName);
    
    static CCSize getSizeFromPlist(CCDictionary * plistDictionary,const string& sizeName);
    
    static float getFontSizeFromPlist(CCDictionary * plistDictionary,const string& key);
    
    
    static const char* getStringFromPlist(CCDictionary * plistDictionary,const string& key);
    
    static map<string,string> getGroupStringFromPlist(CCDictionary * plistDictionary,const string& imageSectionName);
    
    static const char*  getImageFromPlist(CCDictionary * plistDictionary,const string& key);
    
    static map<string,string> getGroupImageFromPlist(CCDictionary * plistDictionary,const string& imageSectionName);
    
};

#endif /* defined(__GreTest__ScriptParser__) */