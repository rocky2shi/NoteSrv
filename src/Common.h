#ifndef __COMMON_H_20100409__
#define __COMMON_H_20100409__
#include "Debug.h"
#include "Typedef.h"
#include "AutoPtr.h"
#include "Log.h"
#include "Lock.h"
#include "Connect.h"
#include "FileObj.h"
#include "GlobalConfig.h"
#include "ClassInit.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <utime.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <boost/thread/thread.hpp>
using namespace std;







// �����հ��ַ�
inline static void IGNORE_BLANK(char *&str)
{
    while( ('\0' != *str) && (' ' == *str) )
    {
        str++;
    }
}

// �ǻس����з�����true
inline static bool IS_CR(char ch)
{
    return '\r' == ch || '\n' == ch;
}

// ������β��ָ���з�������\r����\n����\0����
inline static void JUMP_TO_LINE_END(char *&str)
{
    while( ('\r' != *str) && ('\n' != *str) && ('\0' != *str) )
    {
        str++;
    }
}

// ɾ���س���
inline static int CUT_CR(char *str, int len)
{
    int n = 0;
    if( len > 0 && IS_CR(str[len - 1]) )
    {
        n++;
        str[len - 1] = '\0';
    }
    if( len > 1 && IS_CR(str[len - 2]) )
    {
        n++;
        str[len - 2] = '\0';
    }
    return n;
}
inline static int CUT_CR(char *str)
{
    int n = strlen(str);
    return CUT_CR(str, n);
}

// ȥ��˫���ţ� "xxx.txt" > xxx.txt
// ����ʱ�����볤��lenʱ��Ҫ���㡰�����š�������β��λ��
inline static char *CUT_QUOTE(char * const str, int len=0)
{
    if(0 == len)    // ����ʱ�����봮����
    {
        char *quote = strchr(str+1, '"');
        if(NULL == quote)
        {
            return str;
        }
        len = quote - str + 1; // ע�⣬���ﴮ���ȵļ��㣻
    }
    if('"' == str[0] && '"' == str[len-1])
    {
        str[len-1] = '\0';  // �������ÿ�
        return str + 1;     // ����ǰһ����
    }
    return str;
}

// ʹ֮��Ϊһ���������ӽ�����'\0'��
inline static void TO_STR(char *str, int len)
{
    str[len] = '\0';
}

inline int min(int a, int b)
{
    return a > b ? b : a;
}




const string map2string(const map<string, string> &pack);
void toLower(char *str);
void toLower(string &str);
void toUpper(char *str);
int GetSizeOfFile(const string &fullname);
char *UrlDecode(char * const str);
const string UrlDecode(const string &str);
char *UrlCode(const char *from, char *to, int max);
const string UrlCode(const string &str);
const string NowTime(const string &format="%Y-%m-%d %H:%M:%S");
const string GmgTime(long sec=0);
const char *GetBaseName(const char *pFullPath);
const char *GetFileType(const char *filename);
int SendFile(FileObj &file, Connect *connect);
int GetRandomInt(int min, int max);
const string GetRandomString(int len=32);
const string IntToString(int num);
const string KeyToFile(const string &key);
const string TextToWeb(const string &txt);
const string TextToWebHtml(const string &txt);
const string TextToWebEdit(const string &txt);
const string WebToText(const string &html);
const string WebHtmlToText(const string &html);
const string WebEditToText(const string &html);
time_t TimeTo(char *pTimeBuf, int mode=7, int nSec=0);
const string TimeTo(int nSec, int mode=7);
const string TimeTo(long sec, const string &format="%Y-%m-%d %H:%M:%S");
time_t KeyToSecond(const string &key);
int CharReplace(void *buf, int len, char cOld, char cNew);
int CharReplace(string &src, char cOld, char cNew);
const string ByteSizeFormat(int size);
void ParseStr(char *str, map<string, string> &pack, char first, char second);
const string GetMapValue(const map<string, string> &pack, const string &key, const string def="");
const string Crypt(const string &key, const string salt);
bool isText(const string &type);
bool isImage(const string &type);
int DeleteFile(const char *fullname);
int DeleteFile(const string &fullname);
int DeleteDir(const char *pDirPath);
int CreateDir(const string &dir);
int ChangeDir(const string &path);
bool MoveFile(const string &src, const string &dest);
bool isExistFile(const string &fullname);
bool isFile(const string &fullname);
bool isDir(const string &fullname);
int toInt(char c1, char c2);
const char *AsciiToString(int num, char buf[3]);
void Split(const string &str, const string sp, vector<string> &results);
const string Join(const vector<string> &v, const string sp=",");
unsigned int SDBMHash(const char *str);
const string EmptyStringToDefault(const string &str, const string def="");
const string Inc(const string &num);
const string DeleteHtmlLable(const string &html);
time_t GetFileModifyTime(const string &filename);
int SettFileModifyTime(const string &filename, long second);
int ZipCompress(const string &src, const string &dest);   // ������lib/zip.cpp��
int ZipDecompress(const string &src, const string &dest); // ������lib/zip.cpp��
const string HtmlSetParentInputValue(const string &element, const string &value);
const string HtmlSetParentInnerhtmlValue(const string &element, const string &value);
const string HtmlAlert(const string &value);
const string NewKeyStr();
const string StringToTempFile(const string &str);
const string FileToStr(const string &filename);
int GetCpuUsed();
const string FilenameEncode(const string &filename);
const string FilenameDecode(const string &filename);




#endif // end of __COMMON_H_20100409__
