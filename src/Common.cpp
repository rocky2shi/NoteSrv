// Rocky 2010-04-09 15:17:23
#include "MultiStringMatch.h"
#include "Common.h"
#include <dirent.h>     // ��ϵͳ���� [XXX]
#include <fstream>











// ���ṹתΪ�ַ���
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

// ��תΪȫС��
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

// ��תΪȫ����
void toUpper(char *str)
{
    while( NULL != str && '\0' != *str)
    {
        // Debug("[%c]", *str);
        *str = toupper(*str);
        str++;
    }
}

// ����תΪ��
const string IntToString(int num)
{
    char buf[16] = "";
    sprintf(buf, "%d", num);
    return buf;
}

/*
 * ���ܣ��ַ���ASCII��0��255��תΪ����ʾ��ʮ��������
 * ��д��Rocky 2010-05-12 10:51:22
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

// ͨ���ļ���ȡ�ļ��ֽ���
// �ɹ������ļ��ֽ�����ʧ�ܷ�0��
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
 * ���ܣ��ַ������ַ���ת����
 * ��д��Rocky 2010-04-20
 */
int toInt(char c1, char c2)
{
    int n1 = 0; // ʮλ
    int n2 = 0; // ��λ

    // ����ʮλ��
    if('A' <= c1 && c1 <= 'F')
    {
        n1 = (c1 - 'A' + 10) << 4; // ����16
    }
    else if('a' <= c1 && c1 <= 'f')
    {
        n1 = (c1 - 'a' + 10) << 4;
    }
    else // ����
    {
        n1 = (c1 - '0') << 4;
    }

    // �����λ��
    if('A' <= c2 && c2 <= 'F')
    {
        n2 = (c2 - 'A' + 10);
    }
    else if('a' <= c2 && c2 <= 'f')
    {
        n2 = (c2 - 'a' + 10);
    }
    else // ����
    {
        n2 = (c2 - '0');
    }

    // Debug("[%d] [%d] [%d] [%d]", c1, c2, n1, n2);
    return n1 + n2;
}


/*
 * ���ܣ�����url��%xx => �ַ���
 * ��д��Rocky 2010-04-20
 * ע�⣺str�����ǿ�д�ڴ棬ת��������ԭ���н��У�
 */
char *UrlDecode(char * const str)
{
    assert(NULL != str);

    char *pOld = str;
    char *pNew = str;

    while('\0' != *pOld && '\0' != *(pOld + 1) && '\0' != *(pOld + 2))
    {
        if('%' == *pOld) // ��%XX��ʽ������룻
        {
            *pNew = toInt(*(pOld + 1), *(pOld + 2));
            pOld += 3; // ǰ����λ
        }
        else if('+' == *pOld)
        {
            *pNew = ' ';    // +תΪ�ո�
            pOld++;
        }
        else
        {
            *pNew = *pOld; // �������
            pOld++; // ǰ��һλ
        }
        pNew++; // ǰ��һλ
    }
    // δת������
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
     * ��������ִ�����������ַ����ܳ�ʱ�����Ƕ�̬����
     * �ڴ棬������ж��copy������[XXX]
     */
    if(str.length() >= LEN_MAX)
    {
        char *p = new char[ str.length() + 10 ];
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
 * ���ܣ������ĵ��ַ�ת��Ϊ�ٷֺ���ʽ�ı���
 * ��д��Rocky 2010-05-08
 * ������from--��ת����
 *       to--���ת����Ĵ�
 *       max--to������󳤶�
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
    max -= 10; // �������²����У�to��һ��ѭ���в�ֹ����1λ��[Rocky 2010-05-17 15:41:30]
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
     * ��������ִ�����������ַ����ܳ�ʱ�����Ƕ�̬����
     * �ڴ棬������ж��copy������[XXX]
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
 * ���ܣ�����ָ��ȫ·���л��������ֵ�ָ��
 * ��д��Rocky 2008-4-7 12:02
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
 * ���ܣ�ȡ�ļ����ͣ���׺������
 * ��д��Rocky 2010-04-27 16:41:39
 */
const char *GetFileType(const char *filename)
{
    // �Ӻ���ǰ�ҡ�.��
    filename = rindex(filename, '.');
    if(NULL == filename)
    {
        return "";
    }
    return filename + 1;
}

/*
 * ���ܣ������ļ�������
 * ��д��Rocky 2010-0427
 * ���أ��ѷ��͵��ֽ���
 */
int SendFile(FileObj &file, Connect *connect)
{
    int ret;
    char buf[256];
    unsigned int count = 0;
    const string &ip = connect->GetPeerAddr(); // ȡ���ͻ�������ip

    // ʹ֮���ļ�ͷ��ʼ��ȡ
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
 * ���ܣ�ȡָ����Χ�������
 * ��д��Rocky 2010-04-28
 * ���أ�min <= ret <= max
 */
int GetRandomInt(int min, int max)
{
    // ��ʼ��
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
 * ���ܣ�ȡ����ַ�������ĸ������ɣ�����������lenָ����
 * ��д��Rocky 2010-05-07 10:01:24
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
 * ���ܣ���key����20060423000003��ȡ��Ӧ�ļ���
 * ��д��Rocky 2010-05-04 11:44:32
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
 * ���ܣ���key����20060423000003��ת��Ϊ����
 * ��д��Rocky 2010-05-05 16:13:39
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
 * ���ܣ��Ѻ�̨�洢�����ݣ�ת��ҳ�����ʾ��ʽ��
 * ��д��Rocky 2010-05-04 11:44:15
 *
 * $web = TextToWeb($txt)
 */
const string TextToWeb(const string &txt)
{
    /*
     *  ��ת����
     *
     *  ��<��=>��&lt;��
     *  ��>��=>��&gt;��
     *  ��&��=>��&amp;��
     *  ��\x1��=>��\n��
     *  ��\x2��=>��<��
     *  ��\x3��=>��>��
     */


    string s;
    const char *p = txt.c_str();
    while('\0' != *p)
    {
        switch( *p )
        {
        /****************************
         * Ϊʹҳ��������ʾ��Ӧ��ת�壺��<����>����&����ҳ�������ַ�
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
         * ��̨�洢ʱ�����ض�ת������������ת������
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
         * ����ת���Ĳ���
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
 * ���ܣ��Ѻ�̨�洢�����ݣ�ת��ҳ�����ʾ��ʽ�����ڱ༭���硰<textarea>����ǩ�У���
 * ��д��Rocky 2010-06-11 17:09:30
 *
 * $web = TextToWebEdit($txt)
 */
const string TextToWebEdit(const string &txt)
{
    /*
     *  ��ת����
     *
     *  ��\x1��=>��\x1��
     */


    /*
     * �滻����س����з��������ַ����䣩��
     * ע�⣬����ļ��ɣ���Ϊ��������ַ��滻ֻ�����ݱ���
     * ��أ���������Ϊ��̬�ģ��������ظ��ĳ�ʼ����
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
 * ���ܣ��Ѻ�̨�洢�����ݣ�ת��ҳ�����ʾ��ʽ��������ʾ����
 * ��д��Rocky 2010-06-11 17:23:55
 *
 * $web = TextToWebHtml($txt)
 */
const string TextToWebHtml(const string &txt)
{
    /*
     *  ��ת����
     *
     *  ��<��=>��&lt;��
     *  ��>��=>��&gt;��
     *  ��&��=>��&amp;��
     *  ��\x1��=>��\n��
     *  ��\x2��=>��<��
     *  ��\x3��=>��>��
     */

    /*
     * �滻����س����з��������ַ����䣩��
     * ע�⣬����ļ��ɣ���Ϊ��������ַ��滻ֻ�����ݱ���
     * ��أ���������Ϊ��̬�ģ��������ظ��ĳ�ʼ����
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
 * ���ܣ���ҳ���Դ������ת���ܴ洢�ķ�ʽ����Ҫ������洢/��ʾ����Ļ������⣩
 * ��д��Rocky 2009-12-04 16:48:17
 *
 * $txt = WebToText($html)
 */
const string WebToText(const string &html)
{
    /*
     *  ��ת����
     *
     *  ��&lt;��=>��<��
     *  ��&gt;��=>��>��
     *  ��&amp��=>��&��
     *  ��\n��  =>��\1��
     *  ��\r��  =>����
     *  ��<"    =>��\x2��
     *  ��>"    =>��\x3��
     */

    int i;
    int c;
    int state = 0; // ��̬
    string str;

    for(i=0; '\0' != (c = html[i]); i++)
    {
        // ����\r����\n��
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


        // �ԡ�&lt;����&gt;����&amp����ʹ��״̬������
        switch( state )
        {
        case 0:     // ��ʼ̬
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
 * ���ܣ���ҳ��ı༭��������ת���ܴ洢�ķ�ʽ����Ҫ������洢/��ʾ����Ļ������⣩
 * ��д��Rocky 2010-06-11 17:37:24
 *
 * $txt = WebEditToText($html)
 */
const string WebEditToText(const string &html)
{
    /*
     *  ��ת����ֻ����س����з���
     *
     *  ��\n��  =>��\x1��
     *  ��\r��  =>����
     */


    /*
     * �滻����س����з��������ַ����䣩��
     * ע�⣬����ļ��ɣ���Ϊ��������ַ��滻ֻ�����ݱ���
     * ��أ���������Ϊ��̬�ģ��������ظ��ĳ�ʼ����
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
 * ���ܣ���ҳ���Դ������ת���ܴ洢�ķ�ʽ����Ҫ������洢/��ʾ����Ļ������⣩
 * ��д��Rocky 2010-06-11 17:44:35
 *
 * $txt = WebHtmlToText($html)
 */
const string WebHtmlToText(const string &html)
{
    /*
     *  ��ת����
     *
     *  ��&lt;��=>��<��
     *  ��&gt;��=>��>��
     *  ��&amp��=>��&��
     *  ��\n��  =>��\1��
     *  ��\r��  =>����
     *  ��<"    =>��\x2��
     *  ��>"    =>��\x3��
     */


    /*
     * �滻����س����з��������ַ����䣩��
     * ע�⣬����ļ��ɣ���Ϊ��������ַ��滻ֻ�����ݱ���
     * ��أ���������Ϊ��̬�ģ��������ظ��ĳ�ʼ����
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
 *  ���ܣ�ת����������ǰϵͳʱ��[��nSecΪ0ʱ]��Ϊָ����ʽ
 *  ������
 *           pTimeBuf : ���ʱ�䴮���ڴ���
 *           mode     : 0=>���磺1204434269
 *                      1=>���磺080302
 *                      2=>���磺08_03_02
 *                      3=>���磺08.03.02
 *                      4=>���磺20080302
 *                      5=>���磺2008_03_02
 *                      6=>���磺2008.03.02
 *                      7=>���磺2009-02-07 18:56:07
 *           nSec     : ��Ҫת����������Ĭ��0��
 *  ��д��Rocky, rocky2shi@126.com, 2008-3-2 12:15:36
 *  �汾��1.1
 *  ���أ���ǰʱ�������
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
            sprintf(pTimeBuf, "%u", (int)t); // ȡ������ʱ��ֵ
        }break;
        //------
        case 1:
        {
            strftime(pTimeBuf, 32, "%y%m%d", localtime(&t) );  // �磺080302
        }break;
        case 2:
        {
            strftime(pTimeBuf, 32, "%y_%m_%d", localtime(&t) );// ��:08_03_02
        }break;
        case 3:
        {
            strftime(pTimeBuf, 32, "%y.%m.%d", localtime(&t) );// ��:08.03.02
        }break;
        //------
        case 4:
        {
            strftime(pTimeBuf, 32, "%Y%m%d", localtime(&t) );  // ��:20080302
        }break;
        case 5:
        {
            strftime(pTimeBuf, 32, "%Y_%m_%d", localtime(&t) );// ��:2008_03_02
        }break;
        case 6:
        {
            strftime(pTimeBuf, 32, "%Y.%m.%d", localtime(&t) );// ��:2008.03.02
        }break;
        case 7:
        {
            strftime(pTimeBuf, 32, "%Y-%m-%d %H:%M:%S", localtime(&t) );// ��:2009-02-07 18:56:07
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
 * ȡformat��ʽָ���ĵ�ǰʱ�䴮
 *
 *      ��ʽ�����磺
 *          %Y: �꣨��λ��
 *          %m: ��
 *          %d: ��
 *          %H: ʱ
 *          %M: ��
 *          %S: ��
 *
 *      ����ȡֵ��ʽ�磺
 *          "%Y-%m-%d %H:%M:%S" => 2009-02-07 18:56:07
 *  ������ʽʱ�䴮���ο�strftime()����
 */
const string NowTime(const string &format/*="%Y-%m-%d %H:%M:%S"*/)
{
    return TimeTo(time(NULL), format);
}

/*
 * ���ܣ�ȡ��ǰ�ģ���ת������Ϊ��GMTʱ��
 * ��д��Rocky 2010-05-24 11:27:18
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
 * ���ܣ��ַ��滻����ԭ������ֱ���޸ģ���
 * ��д��Rocky 2009-1-20 17:11:23
 * ������
 *          buf[in][out] --- ���滻�Ļ���������
 *          len[in] --- ���ݳ��ȣ�
 *          cOld[in] --- ���ַ�
 *          cNew[in] --- ���ַ�
 * ���أ����滻���ַ�����
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
 * ���ܣ���ʽ���ֽ���Ϊ�׶���ʽ
 * ��д��Rocky 2008-10-12 02:35:33
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
 * ���ܣ��ֽ��ַ���Ϊ[key/value]�ԣ�����pack�У�
 * ��д��Rocky 2010-05-07 10:52:08
 * ������str--����
 *       pack--���[key/value]��
 *       first--��һ��ָ���
 *       second--�ڶ���ָ���
 * ע�⣺str�����ǿ�д���ڴ棨�ǳ�����
 */
void ParseStr(char *str, map<string, string> &pack, char first, char second)
{
    char *begin;
    char *end;
    char *name;
    char *value;

    /*
     * ����������ȣ�
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
        str = strchr(str, first); // ��firstΪ';'
        if(NULL != str)
        {
            // �����;��
            *str = '\0';
            // �Ƶ���һ�Σ�Ϊ�´ν�����׼��������;�����ǿո���Ҫ�������ַ���
            str++;
            IGNORE_BLANK(str); // ����ǰ��ǰ��
        }
        end = strchr(begin, second);    // ��secondΪ'='
        if(NULL != end)
        {
            *end = '\0';    // ���������
            end++;
            value = end;
        }
        name = begin;
        if('\0' != *name)
        {
            // �������루ע��UrlDecode()�������ڴ�����ǿ�д�ģ�
            pack[name] = (value && *value) ? UrlDecode(value) : "";
            //Debug("[%s]=[%s]", name, value);
        }
    }// end of while(NULL != str...
}// end of void ParseStr(char *str...

/*
 * ���ܣ�ȡmap<string, string>�е�ֵ���粻���ڼ����򷵻�Ĭ��ֵ��
 * ��д��Rocky 2010-05-07 11:19:08
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
 * ���ܣ��������ĵ����룬��crypt_r()�������а�װ��
 * ��д��Rocky 2010-05-07 16:44:26
 * ���أ����ܺ�����Ĵ�
 */
const string Crypt(const string &key, const string salt)
{
    char *crypt_r(const char *key, const char *salt, char *buf);
    char buf[256] = "";
    crypt_r(key.c_str(), salt.c_str(), buf);
    return buf;
}



// �Ƿ�ΪͼƬ��jpg, jpeg, png, gif, ...
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

// �Ƿ�Ϊ�ı��ļ���txt, js, css, html, htm, ...
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
 * ���ܣ�ɾ�������ļ�
 * ��д��Rocky 2010-05-10 20:32:44
 */
int DeleteFile(const char *fullname)
{
    if("" == fullname || "/" == fullname)
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
// ɾ��pDirPath��ָ�ļ���Ŀ¼������������Ŀ¼��
// ���أ�С��0����
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
        // ��������Ŀ¼���൱��˫��ѭ�����������ڶ��ط��ں�Fun()�У�
        while( (pDirent = readdir(pDir)) != NULL )
        {
            if( strcmp(".", pDirent->d_name) == 0 ||
                strcmp("..", pDirent->d_name) == 0 )
            {
                continue;
            }
            // ��װΪȫ·��
            sprintf(szFullName, "%s/%s", pDirPath, pDirent->d_name);
            // ȡ�ļ�/Ŀ¼��Ϣ
            struct stat stDir;
            if( lstat(szFullName, &stDir) == -1)
            {
                break;
            }
            if( S_ISDIR(stDir.st_mode) )
            {
                // ����Ŀ¼���ݹ鴦��
                DeleteDir(szFullName);
            }
            else
            {
                // ɾ�ļ�
                // printf(" %s\n", szFullName);
                unlink(szFullName);
            }
        }// end of while( (pDirent ...
        closedir(pDir);
    }
    // ɾĿ¼/�ļ���
    return remove(pDirPath) == 0 ? OK : ERR;
}

/*
 * ���ܣ��ƶ������ļ���Ŀ���ļ������ڽ������ǣ�
 * ��д��Rocky 2010-05-11 11:11:53
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
 * ���ܣ��ļ���Ŀ¼�Ѵ��ڣ��򷵻�true
 * ��д��Rocky 2010-05-11 11:11:59
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
 * ���ܣ���һ���ļ�����true
 * ��д��Rocky 2010-05-11 11:11:56
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
 * ���ܣ���Ŀ¼����true
 * ��д��Rocky 2010-05-11 11:11:59
 */
bool isDir(const string &fullname)
{
    /*
     * ע��
     *      stat() �����fullname��һ�����ӣ�������������fullnameָ����Ǹ�����
     *      lstat()����stat()��Ψһ��ͬ�ǣ���fullname�����ӣ�lstat()�����������ӱ���
     */
    struct stat stDir;
    if( stat(fullname.c_str(), &stDir) == -1)
    {
        return false;
    }
    return S_ISDIR(stDir.st_mode) ? true : false;
}


/*
 * ���ܣ���spΪ�ָ������ָ�strΪ��Σ��ŵ�results�У�
 * ��д��Rocky 2010-05-13
 * ���磺
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
 * ���ܣ���ϣ����
 * ��д������������� [Rocky 2010-05-14 11:44:23]
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
 * ���ܣ��ϲ�vectorΪ����Ԫ�ؼ���spΪ�������
 * ��д��Rocky 2010-05-14 12:14:04
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
 * ���ܣ����ַ�����תΪĬ�ϵ�ֵ�����򷵻�ԭֵ��
 * ��д��Rocky 2010-05-14 14:37:56
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
 * ���ܣ�ʹ���ַ�����ʾ����������һ
 * ��д��Rocky 2010-05-15 15:30
 */
const string Inc(const string &num)
{
    return IntToString( atoi( num.c_str() ) + 1 );
}

/*
 * ���ܣ�ɾ��html���
 * ��д��Rocky 2010-05-17 17:12:06
 * �汾��v0.1
 * ˵����δ�ܴ���Ƕ��
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
                // ������һ�� <...>
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
 * ���ܣ�ȡ�ļ�����޸�ʱ��
 * ��д��Rocky 2010-05-19 11:05:06
 * ���أ�����������ʱ����ֵ������0��
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
 * ���ܣ��޸��ļ��Ĵ�ȡʱ��͸���ʱ��
 * ��д��Rocky 2010-05-19 15:14:07
 * ���أ�ʧ�ܷ���С��0ֵ
 */
int SettFileModifyTime(const string &filename, long second)
{
    struct utimbuf t;
    t.modtime = second;
    t.actime  = second;
    return utime(filename.c_str(), &t) == 0 ? OK : ERR;
}

/*
 * ���ܣ�����Ŀ¼
 * ��д��Rocky 2010-05-24
 * ���أ�ʧ�ܷ���С��0ֵ
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
 * ���ܣ�ȡ�����ø�ҳ����input�ؼ�ֵ��html����
 * ��д��Rocky 2010-05-24 15:42:02
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
 * ���ܣ�ȡ�����ø�ҳ���пؼ���innerHTMLֵ��html����
 * ��д��Rocky 2010-05-24 17:25:51
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
 * ���ܣ�ȡAlert()������html����
 * ��д��Rocky 2010-05-24 15:42:02
 */
const string HtmlAlert(const string &value)
{
    return ("<script>alert('" + value + "')</script>");
}

/*
 * ���ܣ�ȡ�µ�KEY��
 * ��д��Rocky 2010-05-25 18:39:57
 */
const string NewKeyStr()
{
    return NowTime("%Y%m%d%H%M%S");
}

/*
 * ���ܣ����ַ���д����ʱ�ļ�
 * ��д��Rocky 2010-05-25 21:14
 * ���أ��ɹ������ļ����������ؿմ���
 */
const string StringToTempFile(const string &str)
{
    // ȡһ��ʱ�ļ������������
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

    // �ɹ������ļ�
    return tmp;
}

/*
 * ���ܣ������ļ����ݴ�
 * ��д��Rocky 2010-05-25 21:59
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
 * ���ܣ��ı䵱ǰ�Ĺ���Ŀ¼
 * ��д��Rocky 2010-05-28 10:46:01
 */
int ChangeDir(const string &path)
{
    // chdir()ִ�гɹ��򷵻�0
    return chdir(path.c_str()) == 0 ? OK : ERR;
}

/*
 * ���ܣ�ȡ��ǰCPUʹ����
 * ��д��Rocky 2009-02-11 16:21:56
 * ���أ��ɹ����ص�ǰCPUʹ���ʣ�-1����
 * ˵����
 *          1. �統ǰʱ�����[cat /dev/shm/.cpustates]�ĵڶ����ֶ�С��30����ȡ
 *             ��һ���ֶ�ֵΪCPU�����ʣ�����
 *          2. ȡ[/apps/bzw/bin/setup/sysinfo]�е�cpuΪ�ֶ�ֵΪ�����ʡ�����ע
 *             ���������ڴ��ֶβ�׼����
 */
int GetCpuUsed()
{
    // cpu��Ϣ�ṹ
    struct Info
    {
        unsigned long nUserTime;
        unsigned long nNiceTime;
        unsigned long nSysTime;
        unsigned long nIdleTime;
        unsigned long nTimeTotal;
        unsigned long uptime;       // �ṹ�����ݵĸ���ʱ��
        Info()
        {
            memset(this, 0, sizeof(*this));
        }
    };

    // ȡ��ǰcpu��Ϣ
    class Get
    {
    public:
        // ȡ��ǰcpu��
        static const void CpuInfo(Info &info)
        {
            string sLine = "";
            ifstream FileStream("/proc/stat");

            if(!FileStream)
            {
                Debug("�򿪶�ȡCPU�����ļ�ʧ��");
                return;
            }

            //��ȡ�ļ���һ�л�ȡ��Ӧcpuֵ
            getline(FileStream, sLine);
            sscanf(sLine.c_str(), "cpu %lu %lu %lu %lu",
                                    &info.nUserTime,
                                    &info.nNiceTime,
                                    &info.nSysTime,
                                    &info.nIdleTime
                  );
            // ��¼���ݸ���ʱ��
            info.uptime = time(NULL);

            info.nTimeTotal = info.nIdleTime
                            + info.nNiceTime
                            + info.nSysTime
                            + info.nUserTime;
        }

        // ȡ�ϴ�cpu�������ں͵�ǰcpu�Ƚϣ���ʹ���ʵȣ�
        static const Info &PrevInfo()
        {
            static Info info[4];
            static unsigned int cur = 0;

            // ���¼�����õ�������ʹ�ö�ʱ�������£�������߳����� [XXX]��
            if(time(NULL) - info[cur].uptime < 3)
            {
                return info[cur];
            }

            /*
             * ��ת������info�е�Ԫ�أ�curָ��ǰʹ�õ�Ԫ�أ�
             *   ע��(n + x) & 0x3�����޶���ֵֻȡ0,1,2,3����infoԪ
             *       ���±ꣻ
             */

            int last = (cur + 3) & 0x3; // ָ����Զ��һ��Ԫ��
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
 * ���ܣ����ļ������루����ͬϵͳ�������⣩
 * ��д��Rocky 2010-06-05 22:36:06
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
 * ���ܣ����루��ԭ�ļ���������Ӧ��FilenameEncode()��
 * ��д��Rocky 2010-06-05 22:50:32
 */
const string FilenameDecode(const string &filename)
{
    // �Ǳ������ݣ�������
    if(filename.length() % 2 != 0)
    {
        return filename;
    }

    const char *p = filename.c_str();
    string result = "";

    while('\0' != *p)
    {
        // ascii��ԭΪ�ַ�
        int n1 = (unsigned char)(*p);
        int n2 = (unsigned char)(*(p + 1));

        /*
         * ��Ϊ��������ʮ���������ģ��������֡���д��ĸ����Ϊ����
         * ������ļ��������жϿ��ܲ�̫׼ȷ��Ҳ���ܴ����Ժ����ݵ�
         * ���������� [XXX] [Rocky 2010-06-07 11:56:55]
         */
        if( ! ( ('A' <= n1 && n1 <= 'F' || '0' <= n1 && n1 <= '9')
                || ('A' <= n2 && n2 <= 'F' || '0' <= n2 && n2 <= '9')
              )
          )
        {
            // ����˵�����Ǳ���Ĵ�������ԭ�ļ�����
            return filename;
        }

        char ch = toInt(n1, n2);
        result += ch;
        p += 2;
    }

    return result;
}

