#ifndef __MULTISTRINGMATCH_H_20100521__
#define __MULTISTRINGMATCH_H_20100521__
#include <string>
#include <map>
#include <vector>
#include <algorithm>
namespace MULTISTRINGMATCH_SPACE
{

using namespace std;






/*
 * 功能：多模字符串匹配（查找、替换）
 * 编写：Rocky 2010-05-21 10:46:59
 * 版本：v0.3
 */
class MultiStringMatch
{
    static const int INIT = 1;              // 初态
    static const int STATE_MAX = 100000;    // 特殊标记，假设所有状态都小于此值；
    struct Back
    {
        int state;
        int ch;
    };

public:
    // 用于查找，strings子串集
    MultiStringMatch(vector<string> &strings);

    // 用于替换，把from中的子串换成对应的to中的串；
    MultiStringMatch(vector<string> &from, vector<string> &to);

    // 用于替换，把key对应集合转为value对应集合；
    MultiStringMatch(map<string, string> &key2value);

    // 替换：把str中的匹配子串（m_from）替换为另一对应子串（m_to）
    const string Replace(const string &str);

    // 查找：只要匹配中一个关键字，返回true
    bool MatchOneKey(const string &str);

    // 查找：必须匹配所有给定关键字才返回true
    bool MatchAllKey(const string &str);


private:
    // 初始化
    int Init();

    // 扫描m_from中各子串，构造各状态转换表；
    void MarkStateTable();

    // 取当处于状态state、匹配当前字符出错时的串；
    const string GetBackStr(int state);

    // 计算不匹配时，当前状态对应的串。
    void NotMatchTable();

    // 输出状态表
    void PrintStateTable();

    // 关键字匹配执行函数
    bool MatchKey(const string &str, bool once=true);

private:
    map<int, Back> m_BackTable;             // 回退跟踪表（即记录当前状来的来源）
    map< int, map<int, int> > m_StateTable; // 状态表
    unsigned int m_CharTable[256];          // 字符表
    vector<string> m_from;                  // 源子串集
    vector<string> m_to;                    // 目的子串集
    map<int, string> m_NotMatchTable;       // 当不匹配时，当前状态应转为的串；
};












}// end of MULTISTRINGMATCH_SPACE
using namespace MULTISTRINGMATCH_SPACE;
#endif // end of __MULTISTRINGMATCH_H_20100521__




/*******************************************************************************

history:
 2010-05-21: v0.1
 2010-05-21: v0.2，增加MatchOneKey()、MatchAllKey()接口；
 2010-05-21: v0.3，修改MultiStringMatch::MatchKey()中的int ch改为unsigned char ch；


***/
