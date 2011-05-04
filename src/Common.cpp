// Rocky 2010-04-09 15:17:23
#include "MultiStringMatch.h"
#include "Common.h"
#include <dirent.h>     // 跨系统问题 [XXX]
#include <fstream>











// 包结构转为字符串
const string map2string(const map<string, string> &pack)
{
    ostringstream msg;

    map<string, string>::const_iterator it = pack.begin();
    for (; it != pack.end(); ++it)
    {
        msg << " [" << it->first << ":" << it->second << "]";
    }

    return msg.str();
}

// 串转为全小字
void toLower(char *str)
{
    while( NULL != str && '\0' != *str)
    {
        // Debug("[%c]", *str);
        *str = tolower(*str);
        str++;
    }
}
void toLower(string &str)
{
    unsigned int i;
    for(i=0; i<str.length(); i++)
    {
        // Debug("[%c]", str[i]);
        str[i] = tolower(str[i]);
    }
}

// 串转为全大字
void toUpper(char *str)
{
    while( NULL != str && '\0' != *str)
    {
        // Debug("[%c]", *str);
        *str = toupper(*str);
        str++;
    }
}

// 整数转为串
const string IntToString(int num)
{
    char buf[16] = "";
    sprintf(buf, "%d", num);
    return buf;
}

/*
 * 功能：字符的ASCII（0～255）转为串表示的十六进制数
 * 编写：Rocky 2010-05-12 10:51:22
 */
const char *AsciiToString(int num, char buf[3])
{
    int c1 = (num >> 4) & 0xF;
    int c2 = num & 0xF;
    buf[0] = c1 > 9 ? 'A' + (c1 - 10) : '0' + c1;
    buf[1] = c2 > 9 ? 'A' + (c2 - 10) : '0' + c2;
    buf[2] = '\0';
    return buf;
}

// 通过文件名取文件字节数
// 成功返回文件字节数，失败返0。
int GetSizeOfFile(const string &fullname)
{
    struct stat stFile;
    if( stat(fullname.c_str(), &stFile) == -1 )
    {
        return 0;
    }
    return stFile.st_size;
}


/*
 * 功能：字符（两字符）转整数
 * 编写：Rocky 2010-04-20
 */
int toInt(char c1, char c2)
{
    int n1 = 0; // 十位
    int n2 = 0; // 个位

    // 计算十位数
    if('A' <= c1 && c1 <= 'F')
    {
        n1 = (c1 - 'A' + 10) << 4; // 乘上16
    }
    else if('a' <= c1 && c1 <= 'f')
    {
        n1 = (c1 - 'a' + 10) << 4;
    }
    else // 数字
    {
        n1 = (c1 - '0') << 4;
    }

    // 计算个位数
    if('A' <= c2 && c2 <= 'F')
    {
        n2 = (c2 - 'A' + 10);
    }
    else if('a' <= c2 && c2 <= 'f')
    {
        n2 = (c2 - 'a' + 10);
    }
    else // 数字
    {
        n2 = (c2 - '0');
    }

    // Debug("[%d] [%d] [%d] [%d]", c1, c2, n1, n2);
    return n1 + n2;
}


/*
 * 功能：解码url（%xx => 字符）
 * 编写：Rocky 2010-04-20
 * 注意：str必须是可写内存，转换操作在原串中进行；
 */
char *UrlDecode(char * const str)
{
    assert(NULL != str);

    char *pOld = str;
    char *pNew = str;

    while('\0' != *pOld && '\0' != *(pOld + 1) && '\0' != *(pOld + 2))
    {
        if('%' == *pOld) // 是%XX形式，需解码；
        {
            *pNew = toInt(*(pOld + 1), *(pOld + 2));
            pOld += 3; // 前进三位
        }
        else if('+' == *pOld)
        {
            *pNew = ' ';    // +转为空格
            pOld++;
        }
        else
        {
            *pNew = *pOld; // 不需解码
            pOld++; // 前进一位
        }
        pNew++; // 前进一位
    }
    // 未转换部分
    while('\0' != *pOld)
    {
        *pNew = ('+' == *pOld) ? ' ' : *pOld; // Rocky 2010-05-19 18:54:45
        pOld++;
        pNew++;
    }
    *pNew = '\0';

    return str;
}
const string UrlDecode(const string &str)
{
    static const int LEN_MAX = 512;

    /*
     * 这里分两种处理情况：当字符串很长时，动态分配
     * 内存，因此有有多次copy操作。[XXX]
     */
    if(str.length() >= LEN_MAX)
    {
        char *p = new char[ str.length() + 10 ]; // 其实不需要多加10
        if(NULL == p)
        {
            return "";
        }
        string s = UrlDecode(p);
        delete [] p;
        return s;
    }
    else
    {
        char buf[LEN_MAX];
        strncpy(buf, str.c_str(), LEN_MAX - 1);
        return UrlDecode(buf);
    }
}

/*
 * 功能：把中文等字符转化为百分号形式的编码
 * 编写：Rocky 2010-05-08
 * 参数：from--待转换串
 *       to--存放转换后的串
 *       max--to串的最大长度
 */
char *UrlCode(const char *from, char *to, int max)
{
    char *dest = to;
    struct Init
    {
        static char *doit()
        {
            static char xx[256] = {0};
            int i;

            for(i='a'; i<='z'; i++)
            {
                xx[i] = i;
            }
            for(i='A'; i<='Z'; i++)
            {
                xx[i] = i;
            }
            for(i='0'; i<='9'; i++)
            {
                xx[i] = i;
            }
            xx[' '] = '+';
            return xx;
        }
    };
    static char *xx = Init::doit();

    int i;
    max -= 10; // 因在以下操作中，to在一次循环中不止后移1位；[Rocky 2010-05-17 15:41:30]
    for(i = 0; (i < max) && ('\0' != *from); i++, from++)
    {
        const unsigned char &c = *from;

        if(0 != xx[c])
        {
            *(to++) = xx[c];
        }
        else
        {
            char tmp[3];
            sprintf(tmp, "%02X", c);
            *(to++) = '%';
            *(to++) = tmp[0];
            *(to++) = tmp[1];
        }
    }
    *to = '\0';
    return dest;
}

const string UrlCode(const string &str)
{
    static const int LEN_MAX = 4 * 1024;

    /*
     * 这里分两种处理情况：当字符串很长时，动态分配
     * 内存，因此有有多次copy操作。[XXX]
     * （注：这里的“*2”及“+10”只是一个猜测值；）
     */
    if( (str.length() * 2) >= LEN_MAX )
    {
        const int len = str.length() * 2;
        char *p = new char[ len + 10 ];
        if(NULL == p)
        {
            return "";
        }
        const string s = UrlCode(str.c_str(), p, len);
        delete [] p;
        return s;
    }
    else
    {
        char buf[LEN_MAX];
        return UrlCode(str.c_str(), buf, LEN_MAX - 1);
    }
}

/*
 * 功能：返回指向全路径中基本名部分的指针
 * 编写：Rocky 2008-4-7 12:02
 */
const char *GetBaseName(const char *pFullPath)
{
    const char *pSlash;
    assert(NULL != pFullPath);

    if( (pSlash = strrchr(pFullPath, '/')) != NULL
        || (pSlash = strrchr(pFullPath, '\\')) != NULL
      )
    {
        return pSlash + 1;
    }

    return pFullPath;
}

/*
 * 功能：取文件类型（后缀名）串
 * 编写：Rocky 2010-04-27 16:41:39
 */
const char *GetFileType(const char *filename)
{
    // 从后往前找‘.’
    filename = rindex(filename, '.');
    if(NULL == filename)
    {
        return "";
    }
    return filename + 1;
}

/*
 * 功能：发送文件到连接
 * 编写：Rocky 2010-0427
 * 返回：已发送的字节数
 */
int SendFile(FileObj &file, Connect *connect)
{
    int ret;
    char buf[256];
    unsigned int count = 0;
    const string &ip = connect->GetPeerAddr(); // 取出客户端连接ip

    // 使之从文件头开始读取
    file.SetSeek(FileObj::SET, 0);

    while( (ret = file.Read(buf, sizeof(buf))) > 0 )
    {
        const int n = connect->Send(buf, ret);
        count += n;
        if(n != ret)
        {
            LOG_INFO("File send to [%s] error: [%u/%u]", ip.c_str(), count, file.Size());
            return ERR;
        }
    }

    LOG_DEBUG("File[%s] send to [%s] finish: [%u/%u]",
                file.Fullname().c_str(), ip.c_str(), count, file.Size());

    return count;
}

/*
 * 功能：取指定范围随机整数
 * 编写：Rocky 2010-04-28
 * 返回：min <= ret <= max
 */
int GetRandomInt(int min, int max)
{
    // 初始化
    static struct Init{
        Init()
        {
            srand( time(NULL) );
        }
    }tmp;

    if(min > max)
    {
        return min;
    }

    return min + rand() % (max + 1 - min);
}

/*
 * 功能：取随机字符串（字母数字组成），串长度由len指定；
 * 编写：Rocky 2010-05-07 10:01:24
 */
const string GetRandomString(int len/*=32*/)
{
    static char str[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string s;
    int i;

    for(i=0; i<len; i++)
    {
        const int &index = GetRandomInt(0, sizeof(str) - 2);
        s += str[ index ];
    }

    return s;
}

/*
 * 功能：由key（如20060423000003）取对应文件名
 * 编写：Rocky 2010-05-04 11:44:32
 */
const string KeyToFile(const string &key)
{
    char year[10] = "";
    char month[10] = "";
    char date[10] = "";

    if( sscanf(key.c_str(), "%4s%2s%2s", year, month, date) != 3 )
    {
        return "";
    }

    int dd = atoi(date);

    if(1 <= dd && dd <= 10)
    {
        return year + string(month) + ".01.10";
    }
    else if(11 <= dd && dd <= 20)
    {
        return year + string(month) + ".11.20";
    }
    else if(21 <= dd && dd <= 31)
    {
        return year + string(month) + ".21.31";
    }

    return "";
}

/*
 * 功能：由key（如20060423000003）转换为秒数
 * 编写：Rocky 2010-05-05 16:13:39
 */
time_t KeyToSecond(const string &key)
{
    if(14 != key.length())
    {
        return 0;
    }
    struct tm tm;
    strptime(key.c_str(), "%Y%m%d%H%M%S", &tm);
    return mktime( &tm );
}

/*
 * 功能：把后台存储的数据，转成页面可显示格式。
 * 编写：Rocky 2010-05-04 11:44:15
 *
 * $web = TextToWeb($txt)
 */
const string TextToWeb(const string &txt)
{
    /*
     *  作转换：
     *
     *  ‘<’=>‘&lt;’
     *  ‘>’=>‘&gt;’
     *  ‘&’=>‘&amp;’
     *  ‘\x1’=>‘\n’
     *  ‘\x2’=>‘<’
     *  ‘\x3’=>‘>’
     */


    string s;
    const char *p = txt.c_str();
    while('\0' != *p)
    {
        switch( *p )
        {
        /****************************
         * 为使页面正常显示，应当转义：‘<’‘>’‘&’等页面特殊字符
         */
        case '<':
            s += "&lt;";
            break;
        case '>':
            s += "&gt;";
            break;
        case '&':
            s += "&amp;";
            break;
        /****************************
         * 后台存储时做了特定转换，所以这里转回来；
         */
        case 1:
            s += "\n";
            break;
        case 2:
            s += "<";
            break;
        case 3:
            s += ">";
            break;
        /****************************
         * 不需转换的部分
         */
        default:
            s += *p;
            break;
        }
        p++;
    }

    return s;
}

/*
 * 功能：把后台存储的数据，转成页面可显示格式（用于编辑，如“<textarea>”标签中）。
 * 编写：Rocky 2010-06-11 17:09:30
 *
 * $web = TextToWebEdit($txt)
 */
const string TextToWebEdit(const string &txt)
{
    /*
     *  作转换：
     *
     *  ‘\x1’=>‘\x1’
     */


    /*
     * 替换处理回车换行符（其它字符不变）。
     * 注意，这里的技巧，因为对这里的字符替换只和内容本身
     * 相关，所以设置为静态的，避免多次重复的初始化；
     */
    struct ToWeb
    {
        static MultiStringMatch & doit()
        {
            vector<string> from;
            vector<string> to;

            from.push_back("\x1");

            to.push_back("\n");

            static MultiStringMatch match(from ,to);
            return match;
        }
    };
    static MultiStringMatch &toWeb = ToWeb::doit();

    return toWeb.Replace(txt);
}

/*
 * 功能：把后台存储的数据，转成页面可显示格式（用于显示）。
 * 编写：Rocky 2010-06-11 17:23:55
 *
 * $web = TextToWebHtml($txt)
 */
const string TextToWebHtml(const string &txt)
{
    /*
     *  作转换：
     *
     *  ‘<’=>‘&lt;’
     *  ‘>’=>‘&gt;’
     *  ‘&’=>‘&amp;’
     *  ‘\x1’=>‘\n’
     *  ‘\x2’=>‘<’
     *  ‘\x3’=>‘>’
     */

    /*
     * 替换处理回车换行符（其它字符不变）。
     * 注意，这里的技巧，因为对这里的字符替换只和内容本身
     * 相关，所以设置为静态的，避免多次重复的初始化；
     */
    struct ToWeb
    {
        static MultiStringMatch & doit()
        {
            vector<string> from;
            vector<string> to;

            from.push_back("<");
            from.push_back(">");
            from.push_back("&");
            from.push_back("\x1");
            from.push_back("\x2");
            from.push_back("\x3");

            to.push_back("&lt;");
            to.push_back("&gt;");
            to.push_back("&amp;");
            to.push_back("\n");
            to.push_back("<");
            to.push_back(">");

            static MultiStringMatch match(from ,to);
            return match;
        }
    };
    static MultiStringMatch &toWeb = ToWeb::doit();

    return toWeb.Replace(txt);
}

/*
 * 功能：把页面的源代码做转换能存储的方式（主要解决『存储/显示』间的互换问题）
 * 编写：Rocky 2009-12-04 16:48:17
 *
 * $txt = WebToText($html)
 */
const string WebToText(const string &html)
{
    /*
     *  作转换：
     *
     *  ‘&lt;’=>‘<’
     *  ‘&gt;’=>‘>’
     *  ‘&amp’=>‘&’
     *  ‘\n’  =>‘\1’
     *  ‘\r’  =>‘’
     *  ‘<"    =>‘\x2’
     *  ‘>"    =>‘\x3’
     */

    int i;
    int c;
    int state = 0; // 初态
    string str;

    for(i=0; '\0' != (c = html[i]); i++)
    {
        // 处理‘\r’‘\n’
        switch( c )
        {
        case '\n':
            str += '\x1';
            continue;
        case '\r':
            continue;
        case '<':
            str += '\x2';
            continue;
        case '>':
            str += '\x3';
            continue;
        }


        // 对‘&lt;’‘&gt;’‘&amp’，使用状态机处理；
        switch( state )
        {
        case 0:     // 起始态
            if('&' == c)
            {
                state = 1;
            }
            else
            {
                str += c;
                state = 0;
            }
            break;
        case 1:
            switch(c)
            {
            case 'l':
                state = 2;
                break;
            case 'g':
                state = 3;
                break;

            case 'a':
                state = 4;
                break;
            default:
                str += "&";
                str += c;
                state = 0;
                break;
            }
            break;
        case 2:
            if('t' == c)
            {
                state = 5;
            }
            else
            {
                str += "&l";
                str += c;
                state = 0;
            }
            break;
        case 3:
            if('t' == c)
            {
                state = 6;
            }
            else
            {
                str += "&g";
                str += c;
                state = 0;
            }
            break;
        case 4:
            if('m' == c)
            {
                state = 7;
            }
            else
            {
                str += "&a";
                str += c;
                state = 0;
            }
            break;
        case 5:
            str += "<";
            if('&' == c)
            {
                state = 1;
            }
            else
            {
                if(';' != c)
                {
                    str += c;
                }
                state = 0;
            }
            break;
        case 6:
            str += ">";
            if('&' == c)
            {
                state = 1;
            }
            else
            {
                if(';' != c)
                {
                    str += c;
                }
                state = 0;
            }
            break;
        case 7:
            if('p' == c)
            {
                state = 8;
            }
            else
            {
                str += "&am";
                str += c;
                state = 0;
            }
            break;
        case 8:
            str += "&";
            if('&' == c)
            {
                state = 1;
            }
            else
            {
                if(';' != c)
                {
                    str += c;
                }
                state = 0;
            }
            break;
        }
    }
    return str;
}

/*
 * 功能：把页面的编辑框内容做转换能存储的方式（主要解决『存储/显示』间的互换问题）
 * 编写：Rocky 2010-06-11 17:37:24
 *
 * $txt = WebEditToText($html)
 */
const string WebEditToText(const string &html)
{
    /*
     *  作转换，只处理回车换行符：
     *
     *  ‘\n’  =>‘\x1’
     *  ‘\r’  =>‘’
     */


    /*
     * 替换处理回车换行符（其它字符不变）。
     * 注意，这里的技巧，因为对这里的字符替换只和内容本身
     * 相关，所以设置为静态的，避免多次重复的初始化；
     */
    struct ToText
    {
        static MultiStringMatch & doit()
        {
            vector<string> from;
            vector<string> to;

            from.push_back("\n");
            from.push_back("\r");

            to.push_back("\x1");
            to.push_back("");

            static MultiStringMatch match(from ,to);
            return match;
        }
    };
    static MultiStringMatch &toText = ToText::doit();

    return toText.Replace(html);
}

/*
 * 功能：把页面的源代码做转换能存储的方式（主要解决『存储/显示』间的互换问题）
 * 编写：Rocky 2010-06-11 17:44:35
 *
 * $txt = WebHtmlToText($html)
 */
const string WebHtmlToText(const string &html)
{
    /*
     *  作转换：
     *
     *  ‘&lt;’=>‘<’
     *  ‘&gt;’=>‘>’
     *  ‘&amp’=>‘&’
     *  ‘\n’  =>‘\1’
     *  ‘\r’  =>‘’
     *  ‘<"    =>‘\x2’
     *  ‘>"    =>‘\x3’
     */


    /*
     * 替换处理回车换行符（其它字符不变）。
     * 注意，这里的技巧，因为对这里的字符替换只和内容本身
     * 相关，所以设置为静态的，避免多次重复的初始化；
     */
    struct ToText
    {
        static MultiStringMatch & doit()
        {
            vector<string> from;
            vector<string> to;

            from.push_back("&lt");
            from.push_back("&lt;");
            from.push_back("&gt");
            from.push_back("&gt;");
            from.push_back("&amp");
            from.push_back("&amp;");
            from.push_back("\n");
            from.push_back("\r");
            from.push_back("<");
            from.push_back(">");

            to.push_back("<");
            to.push_back("<");
            to.push_back(">");
            to.push_back(">");
            to.push_back("&");
            to.push_back("&");
            to.push_back("\x1");
            to.push_back("");
            to.push_back("\x2");
            to.push_back("\x3");

            static MultiStringMatch match(from ,to);
            return match;
        }
    };
    static MultiStringMatch &toText = ToText::doit();

    return toText.Replace(html);
}

/*
 *  功能：转换秒数（或当前系统时间[当nSec为0时]）为指定形式
 *  参数：
 *           pTimeBuf : 存放时间串的内存区
 *           mode     : 0=>形如：1204434269
 *                      1=>形如：080302
 *                      2=>形如：08_03_02
 *                      3=>形如：08.03.02
 *                      4=>形如：20080302
 *                      5=>形如：2008_03_02
 *                      6=>形如：2008.03.02
 *                      7=>形如：2009-02-07 18:56:07
 *           nSec     : 需要转换的秒数（默认0）
 *  编写：Rocky, rocky2shi@126.com, 2008-3-2 12:15:36
 *  版本：1.1
 *  返回：当前时间的秒数
 */
time_t TimeTo(char *pTimeBuf, int mode/*=7*/, int nSec/*=0*/)
{
    assert(NULL != pTimeBuf);
    time_t t;
    t = (0 == nSec) ? time(NULL) : nSec;
    switch(mode)
    {
        case 0:
        {
            sprintf(pTimeBuf, "%u", (int)t); // 取秒数的时间值
        }break;
        //------
        case 1:
        {
            strftime(pTimeBuf, 32, "%y%m%d", localtime(&t) );  // 如：080302
        }break;
        case 2:
        {
            strftime(pTimeBuf, 32, "%y_%m_%d", localtime(&t) );// 如:08_03_02
        }break;
        case 3:
        {
            strftime(pTimeBuf, 32, "%y.%m.%d", localtime(&t) );// 如:08.03.02
        }break;
        //------
        case 4:
        {
            strftime(pTimeBuf, 32, "%Y%m%d", localtime(&t) );  // 如:20080302
        }break;
        case 5:
        {
            strftime(pTimeBuf, 32, "%Y_%m_%d", localtime(&t) );// 如:2008_03_02
        }break;
        case 6:
        {
            strftime(pTimeBuf, 32, "%Y.%m.%d", localtime(&t) );// 如:2008.03.02
        }break;
        case 7:
        {
            strftime(pTimeBuf, 32, "%Y-%m-%d %H:%M:%S", localtime(&t) );// 如:2009-02-07 18:56:07
        }break;
        //------
        default:
        {
            sprintf(pTimeBuf, "%u", (unsigned int)t);
        }break;
    }
    return t;
}
const string TimeTo(int nSec, int mode/*=7*/)
{
    char buf[32] = "";
    TimeTo(buf, mode, nSec);
    return buf;
}
// Rocky 2010-05-24 11:37:43
const string TimeTo(long sec, const string &format/*="%Y-%m-%d %H:%M:%S"*/)
{
    char buf[256] = "";
    if("" == format)
    {
        sprintf(buf, "%lu", sec);
    }
    else
    {
        strftime(buf, 32, format.c_str(), localtime(&sec) );
    }
    return buf;
}

/*
 * 取format格式指定的当前时间串
 *
 *      格式定义如：
 *          %Y: 年（四位）
 *          %m: 月
 *          %d: 日
 *          %H: 时
 *          %M: 分
 *          %S: 秒
 *
 *      以上取值形式如：
 *          "%Y-%m-%d %H:%M:%S" => 2009-02-07 18:56:07
 *  其它形式时间串，参考strftime()函数
 */
const string NowTime(const string &format/*="%Y-%m-%d %H:%M:%S"*/)
{
    return TimeTo(time(NULL), format);
}

/*
 * 功能：取当前的（或转换秒数为）GMT时间
 * 编写：Rocky 2010-05-24 11:27:18
 */
const string GmgTime(long sec/*=0*/)
{
    if(0 == sec)
    {
        sec = time(NULL);
    }
    return TimeTo(sec, "%a, %d %b %Y %H:%M:%S GMT");
}

/*
 * 功能：字符替换（在原数据上直接修改）；
 * 编写：Rocky 2009-1-20 17:11:23
 * 参数：
 *          buf[in][out] --- 需替换的缓冲区数据
 *          len[in] --- 数据长度；
 *          cOld[in] --- 旧字符
 *          cNew[in] --- 新字符
 * 返回：被替换的字符个数
 */
int CharReplace(void *buf, int len, char cOld, char cNew)
{
    char *src = (char *)buf;
    int i;
    int n;

    assert(NULL != buf);

    n = 0;
    for(i=0; i<len; i++)
    {
        if(src[i] == cOld)
        {
            src[i] = cNew;
            n++;
        }
    }
    return n;
}
int CharReplace(string &src, char cOld, char cNew)
{
    unsigned int i;
    int n;

    n = 0;
    for(i=0; i<src.length(); i++)
    {
        if(src[i] == cOld)
        {
            src[i] = cNew;
            n++;
        }
    }
    return n;
}

/*
 * 功能：格式化字节数为易读形式
 * 编写：Rocky 2008-10-12 02:35:33
 */
const string ByteSizeFormat(int size)
{
    char buf[32] = "";

    if(size > 1048576) // 1K
    {
        sprintf(buf, "%.2fMB", (float)size/1048576); // 1M
    }
    else if(size > 1024)
    {
        sprintf(buf, "%.2fKB", (float)size/1024);
    }
    else
    {
        sprintf(buf, "%dB", size);
    }
    return buf;
}

/*
 * 功能：分解字符串为[key/value]对，存入pack中；
 * 编写：Rocky 2010-05-07 10:52:08
 * 参数：str--主串
 *       pack--存放[key/value]对
 *       first--第一层分隔符
 *       second--第二层分隔符
 * 注意：str必须是可写的内存（非常量）
 */
void ParseStr(char *str, map<string, string> &pack, char first, char second)
{
    char *begin;
    char *end;
    char *name;
    char *value;

    /*
     * 处理几种情况等：
     *
     * 1 => [window.screen.width=1024&window.screen.height=&username=abcdefghijklmnopqrstuvwxyz&login=GET\0]
     * 2 => [user\0]
     * 3 => [user&pasd=1234\0]
     * 4 => [\0]
     */
    while(NULL != str)
    {
        name = NULL;
        value = NULL;
        begin = str;
        str = strchr(str, first); // 如first为';'
        if(NULL != str)
        {
            // 清掉‘;’
            *str = '\0';
            // 移到下一段，为下次解析做准备；若‘;’后是空格，则要跳过两字符；
            str++;
            IGNORE_BLANK(str); // 忽略前导前格
        }
        end = strchr(begin, second);    // 如second为'='
        if(NULL != end)
        {
            *end = '\0';    // 清掉‘＝’
            end++;
            value = end;
        }
        name = begin;
        if('\0' != *name)
        {
            // 解码后存入（注意UrlDecode()操作的内存必须是可写的）
            pack[name] = (value && *value) ? UrlDecode(value) : "";
            //Debug("[%s]=[%s]", name, value);
        }
    }// end of while(NULL != str...
}// end of void ParseStr(char *str...

/*
 * 功能：取map<string, string>中的值，如不存在键，则返回默认值；
 * 编写：Rocky 2010-05-07 11:19:08
 */
const string GetMapValue(const map<string, string> &pack, const string &key, const string def/*=""*/)
{
    map<string, string>::const_iterator it = pack.find( key );
    if(pack.end() != it)
    {
        return it->second;
    }
    return def;
}

/*
 * 功能：加密明文的密码，对crypt_r()函数进行包装；
 * 编写：Rocky 2010-05-07 16:44:26
 * 返回：加密后的码文串
 */
const string Crypt(const string &key, const string salt)
{
    char *crypt_r(const char *key, const char *salt, char *buf);
    char buf[256] = "";
    crypt_r(key.c_str(), salt.c_str(), buf);
    return buf;
}



// 是否为图片：jpg, jpeg, png, gif, ...
bool isImage(const string &type)
{
    static const string suffix[] = {
        "bmp",
        "gif",
        "ico",
        "jpeg",
        "jpg",
        "png",
        "tif",
        ""
    };

    if("" == type)
    {
        return false;
    }

    int i;
    for(i=0; "" != suffix[i]; i++)
    {
        if(type == suffix[i])
        {
            return true;
        }
    }
    return false;
}

// 是否为文本文件：txt, js, css, html, htm, ...
bool isText(const string &type)
{
    static const string suffix[] = {
        "htm",
        "js",
        "css",
        "txt",
        "html",
        ""
    };

    if("" == type)
    {
        return false;
    }

    int i;
    for(i=0; "" != suffix[i]; i++)
    {
        if(type == suffix[i])
        {
            return true;
        }
    }
    return false;
}

/*
 * 功能：删除单个文件
 * 编写：Rocky 2010-05-10 20:32:44
 */
int DeleteFile(const char *fullname)
{
    if(NULL == fullname || '\0' == *fullname || '/' == *fullname)
    {
        return ERR;
    }
    return unlink(fullname) == 0 ? OK : ERR;
}
int DeleteFile(const string &fullname)
{
    return DeleteFile(fullname.c_str());
}

// Rocky, 2008-4-5 3:22:47
// 删除pDirPath所指文件或目录（包括所有子目录）
// 返回：小于0出错。
int DeleteDir(const char *pDirPath)
{
    char szFullName[512];
    DIR *pDir;
    struct dirent *pDirent;

    assert(NULL != pDirPath);
    strncpy(szFullName, pDirPath, sizeof(szFullName));

    assert(NULL != pDirPath);
    pDir = opendir(pDirPath);
    if( NULL != pDir )
    {
        // 遍历数据目录，相当于双重循环来遍历（第二重放在函Fun()中）
        while( (pDirent = readdir(pDir)) != NULL )
        {
            if( strcmp(".", pDirent->d_name) == 0 ||
                strcmp("..", pDirent->d_name) == 0 )
            {
                continue;
            }
            // 组装为全路径
            sprintf(szFullName, "%s/%s", pDirPath, pDirent->d_name);
            // 取文件/目录信息
            struct stat stDir;
            if( lstat(szFullName, &stDir) == -1)
            {
                break;
            }
            if( S_ISDIR(stDir.st_mode) )
            {
                // 进入目录（递归处理）
                DeleteDir(szFullName);
            }
            else
            {
                // 删文件
                // printf(" %s\n", szFullName);
                unlink(szFullName);
            }
        }// end of while( (pDirent ...
        closedir(pDir);
    }
    // 删目录/文件，
    return remove(pDirPath) == 0 ? OK : ERR;
}

/*
 * 功能：移动单个文件（目的文件若存在将被覆盖）
 * 编写：Rocky 2010-05-11 11:11:53
 */
bool MoveFile(const string &src, const string &dest)
{
    if("" == src || "" == dest)
    {
        return ERR;
    }
    return rename(src.c_str(), dest.c_str()) == 0 ? true : false;
}

/*
 * 功能：文件、目录已存在，则返回true
 * 编写：Rocky 2010-05-11 11:11:59
 */
bool isExistFile(const string &fullname)
{
    struct stat stDir;
    if( stat(fullname.c_str(), &stDir) == -1)
    {
        return false;
    }
    return true;
}

/*
 * 功能：是一般文件返回true
 * 编写：Rocky 2010-05-11 11:11:56
 */
bool isFile(const string &fullname)
{
    struct stat stDir;
    if( stat(fullname.c_str(), &stDir) == -1)
    {
        return false;
    }
    return S_ISREG(stDir.st_mode) ? true : false;
}

/*
 * 功能：是目录返回true
 * 编写：Rocky 2010-05-11 11:11:59
 */
bool isDir(const string &fullname)
{
    /*
     * 注：
     *      stat() ：如果fullname是一个链接，函数操作的是fullname指向的那个对象；
     *      lstat()：与stat()的唯一不同是，若fullname是链接，lstat()操作的是链接本身；
     */
    struct stat stDir;
    if( stat(fullname.c_str(), &stDir) == -1)
    {
        return false;
    }
    return S_ISDIR(stDir.st_mode) ? true : false;
}


/*
 * 功能：以sp为分隔符，分割str为多段，放到results中；
 * 编写：Rocky 2010-05-13
 * 例如：
 *        [rocky@rocky-desktop today]$ a.out "12,34,,56"
 *        split.cpp:54 : [12]
 *        split.cpp:54 : [34]
 *        split.cpp:54 : []
 *        split.cpp:54 : [56]
 *        [rocky@rocky-desktop today]$
 */
void Split(const string &str, const string sp, vector<string> &results)
{
    unsigned int begin = 0;
    unsigned int end = 0;
    const int len = sp.length();

    while( (end = str.find(sp, begin)) != str.npos )
    {
        results.push_back( str.substr(begin, end - begin) );
        begin = end + len;
    }
    results.push_back(str.substr(begin));
}

/*
 * 功能：哈希函数
 * 编写：网络整理而来 [Rocky 2010-05-14 11:44:23]
 */
unsigned int SDBMHash(const char *str)
{
    unsigned int hash = 0;
    while (*str)
    {
        // equivalent to: hash = 65599*hash + (*str++);
        hash = (*str++) + (hash << 6) + (hash << 16) - hash;
    }
    return (hash & 0x7FFFFFFF);
}

/*
 * 功能：合并vector为串，元素间以sp为间隔符；
 * 编写：Rocky 2010-05-14 12:14:04
 */
const string Join(const vector<string> &v, const string sp/*=","*/)
{
    string result = "";
    vector<string>::const_iterator it = v.begin();
    if(v.end() != it)
    {
        result = *it;
        it++;
    }
    for(; v.end() != it; it++)
    {
        result += (sp + *it);
    }
    return result;
}

/*
 * 功能：空字符串则转为默认的值（否则返回原值）
 * 编写：Rocky 2010-05-14 14:37:56
 */
const string EmptyStringToDefault(const string &str, const string def/*=""*/)
{
    if("" == str)
    {
        return def;
    }
    return str;
}

/*
 * 功能：使由字符串表示的数值增加一
 * 编写：Rocky 2010-05-15 15:30
 */
const string Inc(const string &num)
{
    return IntToString( atoi( num.c_str() ) + 1 );
}

/*
 * 功能：删除html标记
 * 编写：Rocky 2010-05-17 17:12:06
 * 版本：v0.1
 * 说明：未能处理嵌套
 */
const string DeleteHtmlLable(const string &html)
{
    string result = "";
    int begin = 0;
    int left = 0;
    unsigned int i;
    for(i=0; i<html.length(); i++)
    {
        if('<' == html[i])
        {
            left = i;
            i++;
            for(; i<html.length(); i++)
            {
                // 遇到了一对 <...>
                if('>' == html[i])
                {
                    result += html.substr(begin, left - begin);
                    begin = i + 1;
                    left = begin;
                    break;
                }
            }// end of for(; i<html.length()...
        }// end of if('<' == ...
    }// end of for(i=0; i<html.length()...
    result += html.substr(begin);
    return result;
}

/*
 * 功能：取文件最后修改时间
 * 编写：Rocky 2010-05-19 11:05:06
 * 返回：返回整数的时间数值，出错返0；
 */
time_t GetFileModifyTime(const string &filename)
{
    struct stat stFile;
    if( stat(filename.c_str(), &stFile) == -1)
    {
        return 0;
    }
    return stFile.st_mtime;
}

/*
 * 功能：修改文件的存取时间和更改时间
 * 编写：Rocky 2010-05-19 15:14:07
 * 返回：失败返回小于0值
 */
int SettFileModifyTime(const string &filename, long second)
{
    struct utimbuf t;
    t.modtime = second;
    t.actime  = second;
    return utime(filename.c_str(), &t) == 0 ? OK : ERR;
}

/*
 * 功能：创建目录
 * 编写：Rocky 2010-05-24
 * 返回：失败返回小于0值
 */
int CreateDir(const string &dir)
{
    if(mkdir(dir.c_str(), S_IRWXU|S_IRWXO) == -1)
    {
        LOG_ERROR("mkdir() error, [%s] [%s]", dir.c_str(), strerror(errno));
        return ERR;
    }
    return OK;
}

/*
 * 功能：取“设置父页面中input控件值”html代码
 * 编写：Rocky 2010-05-24 15:42:02
 */
const string HtmlSetParentInputValue(const string &element, const string &value)
{
    return (""
            "<script>\n"
            "var o = parent.document.getElementById('" + element + "');\n"
            "if(null != o)\n"
            "{\n"
            "    o.value = '" + value + "';\n"
            "}\n"
            "</script>\n"
           );
}

/*
 * 功能：取“设置父页面中控件的innerHTML值”html代码
 * 编写：Rocky 2010-05-24 17:25:51
 */
const string HtmlSetParentInnerhtmlValue(const string &element, const string &value)
{
    return (""
            "<script>\n"
            "var o = parent.document.getElementById('" + element + "');\n"
            "if(null != o)\n"
            "{\n"
            "    o.innerHTML = '" + value + "';\n"
            "}\n"
            "</script>\n"
           );
}

/*
 * 功能：取Alert()函数的html代码
 * 编写：Rocky 2010-05-24 15:42:02
 */
const string HtmlAlert(const string &value)
{
    return ("<script>alert('" + value + "')</script>");
}

/*
 * 功能：取新的KEY串
 * 编写：Rocky 2010-05-25 18:39:57
 */
const string NewKeyStr()
{
    return NowTime("%Y%m%d%H%M%S");
}

/*
 * 功能：把字符串写入临时文件
 * 编写：Rocky 2010-05-25 21:14
 * 返回：成功返回文件名，出错返回空串；
 */
const string StringToTempFile(const string &str)
{
    // 取一临时文件名（随机串）
    const string &tmp = GlobalConfig::instance()->SystemTmpDir()
                      + GetRandomString();
    FileObj file;

    if(!file.Open(tmp, FileObj::W))
    {
        LOG_ERROR("Can't open file: [%s]", tmp.c_str());
        return "";
    }

    int ret = file.Write(str.c_str(), str.length());
    if(ret != str.length())
    {
        DeleteFile(tmp);
        return "";
    }

    // 成功返回文件
    return tmp;
}

/*
 * 功能：返回文件内容串
 * 编写：Rocky 2010-05-25 21:59
 */
const string FileToStr(const string &filename)
{
    string result = "";
    char buf[512];
    int ret;
    FileObj file(filename);

    while( (ret = file.Read(buf, sizeof(buf) - 1)) > 0 )
    {
        buf[ret] = '\0';
        result += buf;
    }

    return result;
}

/*
 * 功能：改变当前的工作目录
 * 编写：Rocky 2010-05-28 10:46:01
 */
int ChangeDir(const string &path)
{
    // chdir()执行成功则返回0
    return chdir(path.c_str()) == 0 ? OK : ERR;
}

/*
 * 功能：取当前CPU使用率
 * 编写：Rocky 2009-02-11 16:21:56
 * 返回：成功返回当前CPU使用率，-1出错；
 * 说明：
 *          1. 如当前时间戳比[cat /dev/shm/.cpustates]的第二个字段小于30，则取
 *             第一个字段值为CPU空闲率，否则
 *          2. 取[/apps/bzw/bin/setup/sysinfo]中的cpu为字段值为空闲率。（另：注
 *             意该命令的内存字段不准备）
 */
int GetCpuUsed()
{
    // cpu信息结构
    struct Info
    {
        unsigned long nUserTime;
        unsigned long nNiceTime;
        unsigned long nSysTime;
        unsigned long nIdleTime;
        unsigned long nTimeTotal;
        unsigned long uptime;       // 结构体数据的更新时间
        Info()
        {
            memset(this, 0, sizeof(*this));
        }
    };

    // 取当前cpu信息
    class Get
    {
    public:
        // 取当前cpu量
        static const void CpuInfo(Info &info)
        {
            string sLine = "";
            ifstream FileStream("/proc/stat");

            if(!FileStream)
            {
                Debug("打开读取CPU配置文件失败");
                return;
            }

            //获取文件第一行获取响应cpu值
            getline(FileStream, sLine);
            sscanf(sLine.c_str(), "cpu %lu %lu %lu %lu",
                                    &info.nUserTime,
                                    &info.nNiceTime,
                                    &info.nSysTime,
                                    &info.nIdleTime
                  );
            // 记录数据更新时间
            info.uptime = time(NULL);

            info.nTimeTotal = info.nIdleTime
                            + info.nNiceTime
                            + info.nSysTime
                            + info.nUserTime;
        }

        // 取上次cpu量（用于和当前cpu比较，求使用率等）
        static const Info &PrevInfo()
        {
            static Info info[4];
            static unsigned int cur = 0;

            // 更新间隔（好的做法是使用定时器来更新，以免多线程问题 [XXX]）
            if(time(NULL) - info[cur].uptime < 3)
            {
                return info[cur];
            }

            /*
             * 轮转来更新info中的元素，cur指向当前使用的元素；
             *   注：(n + x) & 0x3操作限定其值只取0,1,2,3，即info元
             *       素下标；
             */

            int last = (cur + 3) & 0x3; // 指向最远的一个元素
            CpuInfo( info[last] );
            cur = (cur + 1) & 0x3;
            return info[cur];
        }
    };

    const Info &prev = Get::PrevInfo();
    Info now;
    int nCpuIdle = 0;

    Get::CpuInfo(now);
    // Debug("[%ld]", now.nTimeTotal);

    if(now.nTimeTotal == prev.nTimeTotal)
    {
        return 0;
    }

    nCpuIdle = (now.nIdleTime - prev.nIdleTime) * 100 / (now.nTimeTotal - prev.nTimeTotal);

    return 100 - nCpuIdle;
}

/*
 * 功能：把文件名编码（处理不同系统汉字问题）
 * 编写：Rocky 2010-06-05 22:36:06
 */
const string FilenameEncode(const string &filename)
{
    string result = "";
    const char *p = filename.c_str();
    char buf[3];
    while('\0' != *p)
    {
        int ch = (unsigned char)(*p);
        result += AsciiToString(ch, buf);
        p++;
    }
    return result;
}

/*
 * 功能：解码（还原文件名）（对应于FilenameEncode()）
 * 编写：Rocky 2010-06-05 22:50:32
 */
const string FilenameDecode(const string &filename)
{
    // 非编码数据（或有误）
    if(filename.length() % 2 != 0)
    {
        return filename;
    }

    const char *p = filename.c_str();
    string result = "";

    while('\0' != *p)
    {
        // ascii还原为字符
        int n1 = (unsigned char)(*p);
        int n2 = (unsigned char)(*(p + 1));

        /*
         * 因为编码是用十六进制数的，不在数字、大写字母则认为不是
         * 编码的文件名，此判断可能不太准确，也可能存在以后数据的
         * 兼容性问题 [XXX] [Rocky 2010-06-07 11:56:55]
         */
        if( ! ( ('A' <= n1 && n1 <= 'F' || '0' <= n1 && n1 <= '9')
                || ('A' <= n2 && n2 <= 'F' || '0' <= n2 && n2 <= '9')
              )
          )
        {
            // 否则说明不是编码的串，返回原文件名；
            return filename;
        }

        char ch = toInt(n1, n2);
        result += ch;
        p += 2;
    }

    return result;
}

