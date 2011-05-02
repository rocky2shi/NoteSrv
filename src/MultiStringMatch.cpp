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


// ���ڲ��ң�strings�Ӵ���
MultiStringMatch::MultiStringMatch(vector<string> &strings)
                    : m_from( strings )
{
    Init();
}

// �����滻����from�е��Ӵ����ɶ�Ӧ��to�еĴ���
MultiStringMatch::MultiStringMatch(vector<string> &from, vector<string> &to)
                    : m_from(from),
                      m_to(to)
{
    Init();
}

// �����滻����key��Ӧ����תΪvalue��Ӧ���ϣ�
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

// ��ʼ��
int MultiStringMatch::Init()
{
    memset(m_CharTable, 0, sizeof(m_CharTable));

    // ����״̬ת����
    MarkStateTable();

    // ����ʧ��ת����
    NotMatchTable();

    // �����״̬
    //PrintStateTable();
}

// ɨ��m_from�и��Ӵ��������״̬ת����
void MultiStringMatch::MarkStateTable()
{
    unsigned int i;
    int current = INIT;     // ��ǰ̬״
    int index = 0;          // m_from����ţ��±꣩
    int nNewState = INIT;   // ��״̬�����ֵ������Ҫһ���µ�״̬����ʾ��ǰ����������

    /*
     * ��������ӷ���
     */
    for(i=0; i<m_from.size(); i++)
    {
        /*
         * ȡ��һ���ַ�����ɨ�赱ǰ�ַ�����
         */

        const string &str = m_from[i]; // ��ǰ�Ӵ�
        unsigned int n;
        for(n=0; n<str.length(); n++)
        {
            int ch = (unsigned char)str[n]; /* ��ǰ�ַ���ע������תΪ��
                                             * �������ͣ����ڴ����к�
                                             * ��ʱ���������⣨��������
                                             * ת������
                                             */

            /*
             * ��ǰ״̬current�������ַ�chʱ��Ӧ��
             *
             *    1) ����״̬��¼�У������״̬��m_StateTable�У�
             *          a. ��״̬������ʾ
             *          b. ����ǰ�ַ��Ǵ������һ�ַ�ʱ����������һ����
             *             ��str[n + 1]Ϊ�գ�����һ���ֵ����ʾƥ��ɹ���
             *             ���������ܲ�����������Բ�������Ϊ��̬��
             *
             *    2) ���ڼ�¼�У���ת����ǰ״̬������ɨ�裻
             */


            if(0 == m_StateTable[ current ][ ch ])
            {
                if('\0' == str[n + 1])
                {
                    // ƥ��ɹ������һ����ֵ��
                    int id = STATE_MAX + index;
                    m_StateTable[ current ][ ch ] = id;

                    // ��¼��ǰ״̬����Դ
                    Back &source = m_BackTable[ id ];
                    source.state = current;
                    source.ch = ch;

                    current = INIT;
                    index++;
                }
                else
                {
                    // �����µ�״̬
                    nNewState++;
                    m_StateTable[ current ][ ch ] = nNewState;

                    // ��¼��ǰ״̬����Դ
                    Back &source = m_BackTable[ nNewState ];
                    source.state = current;
                    source.ch = ch;

                    // ָ���µ�״̬
                    current = nNewState;
                }

                // ��¼���ֵ��ַ�
                m_CharTable[ch]++;
            }
            else
            {
                current = m_StateTable[ current ][ ch ];
            }
        }
    }
}

// ȡ������״̬state��ƥ�䵱ǰ�ַ�����ʱ�Ĵ���
const string MultiStringMatch::GetBackStr(int state)
{
    if(m_from.size() != m_to.size())
    {
        Debug("err");
        return "";
    }

    string str = "";

    /*
     * ���ҵ�ǰ״̬����Դ��ȡ�����״̬ʱ�������������ַ�����ɴ��󷵻أ�
     * ���������������������Ҫ��ת�ַ�����
     */
    while(state > INIT)
    {
        Back &cur = m_BackTable[ state ];
        // �ҵ�һ�Ƚ϶̵�ƥ���Ӵ�
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

// ���㲻ƥ��ʱ����ǰ״̬��Ӧ�Ĵ���
void MultiStringMatch::NotMatchTable()
{
    map<int, Back>::iterator itBack;
    for(itBack = m_BackTable.begin(); m_BackTable.end() != itBack; itBack++)
    {
        int cur = itBack->first;
        m_NotMatchTable[ cur ] = GetBackStr(cur);
    }
}

// ���״̬��
void MultiStringMatch::PrintStateTable()
{
    Debug("-----------״̬ת����------------");
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

    Debug("-----------״̬��Դ��------------");
    map<int, Back>::iterator itBack;
    for(itBack = m_BackTable.begin(); m_BackTable.end() != itBack; itBack++)
    {
        int cur = itBack->first;
        Back &src = itBack->second;
        Debug("[%d] <-- [%d, '%c']", cur, src.state, src.ch);
    }

    Debug("-----------ʧ��ת����------------");
    map<int, string>::iterator itNotMatch;
    for(itNotMatch = m_NotMatchTable.begin(); m_NotMatchTable.end() != itNotMatch; itNotMatch++)
    {
        int state = itNotMatch->first;
        const string &str = itNotMatch->second;
        Debug("[%d] --> [%s]", state, str.c_str());
    }

    Debug("-----------------------\n");
}

// �滻����str�е�ƥ���Ӵ���m_from���滻Ϊ��һ��Ӧ�Ӵ���m_to��
const string MultiStringMatch::Replace(const string &str)
{
    // ����������ת���ؼ���ʱ������ԭ����[Rocky 2010-06-01 12:16:33]
    if(m_from.size() != m_to.size()
       || m_from.size() == 0)
    {
        Debug("error");
        return str;
    }

    /*
     * ִ�д�ת��
     */
    string result;
    unsigned int i;
    int state = INIT;
    for(i=0; i<str.length(); i++)
    {
        unsigned char ch = str[i];

        /*
         * ����ǰ���ڳ�ʼ״̬�����ַ�ch���ڲ�����ĸ���У������ڳ�ʼ״̬
         * ��ʧ�䣬��ֱ��׷�ӵ�Ŀ�괮�� [Rocky 2010-06-12 10:52:15]
         */
        if( INIT == state
            && (0 == m_CharTable[ ch ] || 0 == m_StateTable[ state ][ ch ])
          )
        {
            result += ch;
            continue;
        }

        // ����״̬ת��
        if(0 != m_StateTable[ state ][ ch ])
        {
            // ״̬ת��
            state = m_StateTable[ state ][ ch ];
            // ״̬��һ����ֵ������STATE_MAX������ʾƥ����һ�Ӵ���
            if(state >= STATE_MAX)
            {
                // ƥ����һ���Ӵ������ܲ�����Ӵ�������������������
                // Debug("match..., [%d] [%c]", state, ch);
            }
        }
        else
        {
            // ʧ��ʱ��ȡ����ǰ״̬��Ӧ���ַ���ֵ��
            result += m_NotMatchTable[ state ];
            if(INIT != state)
            {
                // �������ƣ����ֵĵ�ǰ�ַ�λ�ã�
                i--;
            }
            state = INIT;
        }
    }

    result += m_NotMatchTable[ state ];

    return result;
}

/*
 * once==true : ���ң�ֻҪƥ����һ���ؼ��֣�����true
 * once==false: ���ң�����ƥ�����и����ؼ��ֲŷ���true
 */
bool MultiStringMatch::MatchKey(const string &str, bool once/*=true*/)
{
    /*
     * ��û������ؼ���ʱ������Ϊ�κδ�����ƥ�䣻
     */
    if(m_from.size() == 0)
    {
        Debug("empty");
        return true; // [Rocky 2010-06-01 12:26:52]
    }

    /*
     * �����봮Ϊ��ʱ������Ϊ�κιؼ��������ǲ�ƥ��ģ�
     */
    if("" == str)
    {
        Debug("empty");
        return false;
    }

    unsigned int i;
    int state = INIT;
    map<string, int> hit; // ��¼�����еĹؼ���
    for(i=0; i<str.length(); i++)
    {
        unsigned char ch = str[i];
        if(0 == m_CharTable[ ch ])
        {
            state = INIT;
            continue;
        }
        else if(0 != m_StateTable[ state ][ ch ]) // ����״̬ת��
        {
            // ״̬ת��
            state = m_StateTable[ state ][ ch ];
            // ״̬��һ����ֵ������STATE_MAX������ʾƥ����һ�Ӵ���
            if(state >= STATE_MAX)
            {
                Debug("match..., [%d] [%c]", state, ch);

                // �Ƿ��ڵ�һ��ƥ����ʱ������true
                if( once )
                {
                    return true;
                }

                const string &key = m_from[ state - STATE_MAX ];
                hit[ key ] ++;

                // �����йؼ��ִ����ѱ�ƥ���������true
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

// ���ң�ֻҪƥ����һ���ؼ��֣�����true
bool MultiStringMatch::MatchOneKey(const string &str)
{
    return MatchKey(str, true);
}

// ���ң�����ƥ�����и����ؼ��ֲŷ���true
bool MultiStringMatch::MatchAllKey(const string &str)
{
    return MatchKey(str, false);
}






}// end of MULTISTRINGMATCH_SPACE
