//
//  PersonalApi.cpp
//  GreTest
//
//  Created by lyy on 13-6-14.
//
//

#include "PersonalApi.h"
#include "cocos2d.h"
#include "ScriptParser.h"

using namespace cocos2d;

vector<string> PersonalApi::split(string str,string pattern)
{
    string::size_type pos;
    vector<string> result;
    str+=pattern;
    int size=str.size();
    
    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}


bool PersonalApi::isContantString(string sourceStr,string patternStr)
{
    const char *show;
    
    show=strstr(sourceStr.c_str(),patternStr.c_str());
    
    bool isContant;
    if (show == NULL)
    {
        isContant = false;
    }
    else
    {
        isContant = true;
    }
    
    return isContant;
}

size_t PersonalApi::SafeStringCopy(void* dest,size_t destLen,const char* src)
{
    size_t stringLen = strlen(src);
    size_t size=min(destLen-1,stringLen);
    memcpy(dest,src,size);
    ((char*)dest)[size]='\0';
    return size;
}

string PersonalApi::string_replace(string & strBig, const string & strsrc, const string &strdst)
{
    string::size_type pos=0;
    string::size_type srclen=strsrc.size();
    string::size_type dstlen=strdst.size();
    
    while( (pos=strBig.find(strsrc, pos)) != string::npos)
    {
        strBig.replace(pos, srclen, strdst);
        pos += dstlen;
    }
    
    return strBig;
}

string PersonalApi::convertIntToString (int aNum) //int ?? string
{
    ostringstream oss;
    oss<<aNum;
    return oss.str();
}
string PersonalApi::convertfloatToString (float aNum) //int ?? string
{
    ostringstream oss;
    oss<<aNum;
    return oss.str();
}
int PersonalApi::convertStringToInt (string aStr) //string ?? int
{
    return  atoi( aStr.c_str());
}

string PersonalApi::getRealImageName (const char * imageName,const char * defaultName)
{
    CCFileUtils* aCCFileUtils = CCFileUtils::sharedFileUtils();
    string  str=(aCCFileUtils->getWritablePath()+imageName).c_str();
    
    FILE *fp =fopen((aCCFileUtils->getWritablePath()+imageName).c_str(),"r");//根据路径打开文件
    
    if (fp)
    {
        fclose(fp);
        return str;
    }
    else
    {
        return defaultName;
    }
    
}

void PersonalApi::wirteVecToXml(map<string, string> aMap,string fileName)
{
    string xmlStr;
    for (int i =0; i<aMap.size(); i++)
    {
        xmlStr += aMap[PersonalApi::convertIntToString(i+1)]+"\n";
    }
    
    const char * aChar = xmlStr.c_str();
    
    //取得文件储存的位置
    CCFileUtils* aCCFileUtils = CCFileUtils::sharedFileUtils();
    std::string documentPath = aCCFileUtils->getWritablePath();
    
    
    string filePath = documentPath + fileName;
    
    //保存字符串为xml文件
    string path = filePath;
    FILE *fp =fopen(path.c_str(),"w");
    
    
    fputs(aChar, fp);
    fclose(fp);
    
}

map<string, string> PersonalApi::readXmlToVec(const char * key)
{
    CCFileUtils* aCCFileUtils = CCFileUtils::sharedFileUtils();
    std::string documentPath = aCCFileUtils->getWritablePath();
    string fileName = key;
    
    string filePath = documentPath + fileName;
    //2,读取xml文件为string
    
    string path = filePath;
    FILE *fp =fopen(path.c_str(),"r");//根据路径打开文件
    
    map<string, string> resultMap;
    if (!fp)
    {
        return resultMap;
    }
    
    
    char *pchBuf = NULL;//将要取得的字符串
    int  nLen = 0;//将要取得的字符串长度
    fseek(fp,0,SEEK_END);//文件指针移到文件尾
    nLen =ftell(fp); //得到当前指针位置,即是文件的长度
    rewind(fp);   //文件指针恢复到文件头位置
    
    //动态申请空间,为保存字符串结尾标志\0,多申请一个字符的空间
    pchBuf = (char*)malloc(sizeof(char)*nLen+1);
    if(!pchBuf)
    {
        //perror("内存不够!\n");
        exit(0);
    }
    
    //读取文件内容//读取的长度和源文件长度有可能有出入，这里自动调整 nLen
    nLen =fread(pchBuf,sizeof(char), nLen, fp);
    
    pchBuf[nLen] ='\0';//添加字符串结尾标志
    
    //printf("sss==%s\n", pchBuf);//把读取的内容输出到屏幕看看
    string detailStr = pchBuf;
    fclose(fp); //关闭文件
    free(pchBuf);//释放空间
    
    vector<string> resultVec = PersonalApi::split(detailStr,"\n");
    for (int i = 0; i<resultVec.size(); i++)
    {
        resultMap[PersonalApi::convertIntToString(i+1)] = resultVec[i];
    }
    
    return resultMap;
}
map<string, string> PersonalApi::getRealGroupImageMap(CCDictionary * plistDictionary,const char * mapName,const char * preMapName)
{
    map<string, string> aMap;
    if (PersonalApi::readXmlToVec(mapName).size()>0)
    {
        aMap = PersonalApi::readXmlToVec(mapName);
    }
    else
    {
        
        aMap =  ScriptParser::getGroupImageFromPlist(plistDictionary,preMapName);
    }
    
    return aMap;
}
map<string, string> PersonalApi::getRealGroupStringMap(CCDictionary * plistDictionary,const char * mapName,const char * preMapName)
{
    map<string, string> aMap;
    if (PersonalApi::readXmlToVec(mapName).size()>0)
    {
        aMap = PersonalApi::readXmlToVec(mapName);
    }
    else
    {
        aMap =  ScriptParser::getGroupStringFromPlist(plistDictionary,preMapName);
    }
    
    return aMap;
}