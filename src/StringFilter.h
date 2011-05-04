// Rocky 2008-6-13 12:28
#ifndef _IDC_STRINGFILTER_ROCKY20080613_
#define _IDC_STRINGFILTER_ROCKY20080613_
#include <regex.h>

/*
 *
 *  利用正则表达式，过滤（转换）字符串；
 *
 */







class StringFilter
{
    struct Node {
        regex_t regex;          // 指向编译后的正则式
        char *pTarget;          // 存放将转换成的目标表达形式
    };
    enum {
        MAX_REGEX_COUNT=100,                // 最多表达式数
        CFLAGS = REG_EXTENDED|REG_NEWLINE   // 使用的表达式标志
    };
public:
    StringFilter();
    StringFilter(const char *pFile);
    ~StringFilter();

    // 打开过滤表达式的配置文件，并初始化表达式；
    // 返回：成功返0，出错返非0；
    int OpenConfig(const char *pFile);

    // 编译表达式，存入数组（追加），用于匹配。
    // pRegex : 正则式串
    // 返回：成功返0，出错返非0；
    int Regcomp(const char *pRegex, const char *pTarget);

    // 根据设定的值，转换pSrc串，结果放到pDest中；
    // 返回：成功返回大于0的值，失败返-1；
    int ChangTo(const char *pSrc, char *pDest);

    // pSrc中存在符合配置中指定的串
    // 返回：成功返回大于0的值，失败返-1；
    int IsExist(const char *pSrc);

private:
    void Clear();

private:
    Node m_Node[MAX_REGEX_COUNT];   // 正则式数组
    int m_nCount;       // 当前m_Node中元素计器
};






#endif  //end of _IDC_STRINGFILTER_ROCKY20080613_
