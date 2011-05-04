// Rocky 2010-05-21 10:01:26
#include <string.h>
#include "MultiStringMatch.h"
namespace MULTISTRINGMATCH_SPACE
{

//#define _DEBUG 1


#if _DEBUG
#define Debug(format, args...)   printf("%s:%d : " format "\n", __FILE__, __LINE__, ##args)
#else
#define Debug
#endif


// 用于查找，strings子串集
MultiStringMatch::MultiStringMatch(vector<string> &strings)
                    : m_from( strings )
{
    Init();
}

// 用于替换，把from中的子串换成对应的to中的串；
MultiStringMatch::MultiStringMatch(vector<string> &from, vector<string> &to)
                    : m_from(from),
                      m_to(to)
{
    Init();
}

// 用于替换，把key对应集合转为value对应集合；
MultiStringMatch::MultiStringMatch(map<string, string> &key2value)
{
    map<string, string>::iterator it;
    for(it = key2value.begin(); key2value.end() != it; it++)
    {
        m_from.push_back( it->first );
        m_to.push_back( it->second );
    }

    Init();
}

// 初始化
int MultiStringMatch::Init()
{
    memset(m_CharTable, 0, sizeof(m_CharTable));

    // 构造状态转换表
    MarkStateTable();

    // 构造失配转换表
    NotMatchTable();

    // 输出各状态
    //PrintStateTable();
}

// 扫描m_from中各子串，构造各状态转换表；
void MultiStringMatch::MarkStateTable()
{
    unsigned int i;
    int current = INIT;     // 当前态状
    int index = 0;          // m_from串编号（下标）
    int nNewState = INIT;   // 当状态的最大值（即需要一个新的状态来表示当前量，递增）

    /*
     * 处理各个子符串
     */
    for(i=0; i<m_from.size(); i++)
    {
        /*
         * 取出一个字符串，扫描当前字符串；
         */

        const string &str = m_from[i]; // 当前子串
        unsigned int n;
        for(n=0; n<str.length(); n++)
        {
            int ch = (unsigned char)str[n]; /* 当前字符（注：若不转为无
                                             * 符号类型，则在处理有汉
                                             * 字时将出现问题（正负数的
                                             * 转换）；
                                             */

            /*
             * 当前状态current，遇到字符ch时，应：
             *
             *    1) 不在状态记录中，则记入状态表m_StateTable中；
             *          a. 新状态递增表示
             *          b. 若当前字符是串的最后一字符时（即它的下一个字
             *             符str[n + 1]为空），做一标记值，表示匹配成功，
             *             但因它可能不是最长串，所以不把它做为终态；
             *
             *    2) 已在记录中，则转换当前状态，继续扫描；
             */


            if(0 == m_StateTable[ current ][ ch ])
            {
                if('\0' == str[n + 1])
                {
                    // 匹配成功，标记一特殊值；
                    int id = STATE_MAX + index;
                    m_StateTable[ current ][ ch ] = id;

                    // 记录当前状态的来源
                    Back &source = m_BackTable[ id ];
                    source.state = current;
                    source.ch = ch;

                    current = INIT;
                    index++;
                }
                else
                {
                    // 出现新的状态
                    nNewState++;
                    m_StateTable[ current ][ ch ] = nNewState;

                    // 记录当前状态的来源
                    Back &source = m_BackTable[ nNewState ];
                    source.state = current;
                    source.ch = ch;

                    // 指向新的状态
                    current = nNewState;
                }

                // 记录出现的字符
                m_CharTable[ch]++;
            }
            else
            {
                current = m_StateTable[ current ][ ch ];
            }
        }
    }
}

// 取当处于状态state、匹配当前字符出错时的串；
const string MultiStringMatch::GetBackStr(int state)
{
    if(m_from.size() != m_to.size())
    {
        Debug("err");
        return "";
    }

    string str = "";

    /*
     * 查找当前状态的来源，取出达此状态时，它所经过的字符，组成串后返回；
     * （因它是逆序操作，所以要反转字符串）
     */
    while(state > INIT)
    {
        Back &cur = m_BackTable[ state ];
        // 找到一比较短的匹配子串
        if(state >= STATE_MAX)
        {
            string s = m_to[ state - STATE_MAX ];
            reverse(s.begin(), s.end());
            str += s;
            break;
        }
        str += cur.ch;
        state = cur.state;
    }
    reverse(str.begin(), str.end());
    return str;
}

// 计算不匹配时，当前状态对应的串。
void MultiStringMatch::NotMatchTable()
{
    map<int, Back>::iterator itBack;
    for(itBack = m_BackTable.begin(); m_BackTable.end() != itBack; itBack++)
    {
        int cur = itBack->first;
        m_NotMatchTable[ cur ] = GetBackStr(cur);
    }
}

// 输出状态表
void MultiStringMatch::PrintStateTable()
{
    Debug("-----------状态转换表------------");
    map< int, map<int, int> >::iterator itFirst;
    for(itFirst = m_StateTable.begin(); m_StateTable.end() != itFirst; itFirst++)
    {
        int state = itFirst->first;
        map<int, int> &CharSet = itFirst->second;
        map<int, int>::iterator itCharSet;
        for(itCharSet = CharSet.begin(); CharSet.end() != itCharSet; itCharSet++)
        {
            int ch = itCharSet->first;
            int to = itCharSet->second;
            Debug("[%d, '%c'] --> [%d]", state, ch, to);
        }
    }

    Debug("-----------状态来源表------------");
    map<int, Back>::iterator itBack;
    for(itBack = m_BackTable.begin(); m_BackTable.end() != itBack; itBack++)
    {
        int cur = itBack->first;
        Back &src = itBack->second;
        Debug("[%d] <-- [%d, '%c']", cur, src.state, src.ch);
    }

    Debug("-----------失配转换表------------");
    map<int, string>::iterator itNotMatch;
    for(itNotMatch = m_NotMatchTable.begin(); m_NotMatchTable.end() != itNotMatch; itNotMatch++)
    {
        int state = itNotMatch->first;
        const string &str = itNotMatch->second;
        Debug("[%d] --> [%s]", state, str.c_str());
    }

    Debug("-----------------------\n");
}

// 替换（把str中的匹配子串（m_from）替换为另一对应子串（m_to）
const string MultiStringMatch::Replace(const string &str)
{
    // 输入出错或无转换关键字时，保持原串。[Rocky 2010-06-01 12:16:33]
    if(m_from.size() != m_to.size()
       || m_from.size() == 0)
    {
        Debug("error");
        return str;
    }

    /*
     * 执行串转换
     */
    string result;
    unsigned int i;
    int state = INIT;
    for(i=0; i<str.length(); i++)
    {
        unsigned char ch = str[i];

        /*
         * 若当前处于初始状态，且字符ch不在查找字母表中，或者在初始状态
         * 就失配，则直接追加到目标串； [Rocky 2010-06-12 10:52:15]
         */
        if( INIT == state
            && (0 == m_CharTable[ ch ] || 0 == m_StateTable[ state ][ ch ])
          )
        {
            result += ch;
            continue;
        }

        // 查找状态转换
        if(0 != m_StateTable[ state ][ ch ])
        {
            // 状态转跳
            state = m_StateTable[ state ][ ch ];
            // 状态是一特殊值（大于STATE_MAX），表示匹配中一子串；
            if(state >= STATE_MAX)
            {
                // 匹配着一个子串（可能不是最长子串，所以继续。。。）
                // Debug("match..., [%d] [%c]", state, ch);
            }
        }
        else
        {
            // 失配时，取出当前状态对应的字符串值；
            result += m_NotMatchTable[ state ];
            if(INIT != state)
            {
                // 不往下移（保持的当前字符位置）
                i--;
            }
            state = INIT;
        }
    }

    result += m_NotMatchTable[ state ];

    return result;
}

/*
 * once==true : 查找：只要匹配中一个关键字，返回true
 * once==false: 查找：必须匹配所有给定关键字才返回true
 */
bool MultiStringMatch::MatchKey(const string &str, bool once/*=true*/)
{
    /*
     * 当没有输入关键字时，则认为任何串都可匹配；
     */
    if(m_from.size() == 0)
    {
        Debug("empty");
        return true; // [Rocky 2010-06-01 12:26:52]
    }

    /*
     * 当输入串为空时，则认为任何关键对它都是不匹配的；
     */
    if("" == str)
    {
        Debug("empty");
        return false;
    }

    unsigned int i;
    int state = INIT;
    map<string, int> hit; // 记录已命中的关键字
    for(i=0; i<str.length(); i++)
    {
        unsigned char ch = str[i];
        if(0 == m_CharTable[ ch ])
        {
            state = INIT;
            continue;
        }
        else if(0 != m_StateTable[ state ][ ch ]) // 查找状态转换
        {
            // 状态转跳
            state = m_StateTable[ state ][ ch ];
            // 状态是一特殊值（大于STATE_MAX），表示匹配中一子串；
            if(state >= STATE_MAX)
            {
                Debug("match..., [%d] [%c]", state, ch);

                // 是否在第一次匹配着时即返回true
                if( once )
                {
                    return true;
                }

                const string &key = m_from[ state - STATE_MAX ];
                hit[ key ] ++;

                // 若所有关键字串都已被匹配过，返回true
                if(hit.size() == m_from.size())
                {
                    return true;
                }
            }
        }
        else
        {
            if(INIT != state)
            {
                i--;
            }
            state = INIT;
        }
    }

    return false;
}

// 查找：只要匹配中一个关键字，返回true
bool MultiStringMatch::MatchOneKey(const string &str)
{
    return MatchKey(str, true);
}

// 查找：必须匹配所有给定关键字才返回true
bool MultiStringMatch::MatchAllKey(const string &str)
{
    return MatchKey(str, false);
}






}// end of MULTISTRINGMATCH_SPACE
