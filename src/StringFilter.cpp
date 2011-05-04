// Rocky 2008-6-13 12:28
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include "StringFilter.h"




StringFilter::StringFilter()
{
    Clear();
}

StringFilter::StringFilter(const char *pFile)
{
    Clear();
    OpenConfig(pFile);
}

StringFilter::~StringFilter()
{
    int i;

    for(i=0; i<m_nCount; i++)
    {
        regfree(&m_Node[i].regex);
        free(m_Node[i].pTarget);
    }
}

// 清空
void StringFilter::Clear()
{
    memset(m_Node, 0, sizeof(m_Node));
    m_nCount = 0;
}

// 打开过滤表达式的配置文件，并初始化表达式；
// 返回：成功返0，出错返非0；
int StringFilter::OpenConfig(const char *pFile)
{
    assert(NULL != pFile);

    char szBuf[256];
    char szLine[2][256] = {""};
    char s[2][8] = {""};
    int nLine = 0;
    int n;
    FILE *fp;

    fp = fopen(pFile, "r");

    if( NULL == fp )
    {
        printf("Can't open: %s\n", pFile);
        return -11;
    }

    /*
     *  遍历文件，取出正则式对，如：
     *
     *      REGEXP1=([0-9]+).([0-9]+).([0-9]+).([0-9]+)
     *      TARGET1=$1.$2.$3.$4
     *
     *  出取配置的值对后，调用Regcomp()加入配置数组。
     */
    while( fgets(szBuf, sizeof(szBuf), fp) )
    {
        // 跳过注释及空行
        if('#' == szBuf[0] || '\r' == szBuf[0] || '\n' == szBuf[0]  )
        {
            continue;
        }
        // 复制，并跳过行尾结束符。
        if(sscanf(szBuf, "REGEXP%[0-9]=%[^'\r''\n']", s[0], szLine[0]) != 2
           && sscanf(szBuf, "TARGET%[0-9]=%[^'\r''\n']", s[1], szLine[1]) != 2)
        {
            continue;
        }
        // 已取到两行
        if(nLine> 0 && strcmp(s[0], s[1]) == 0)
        {
            // printf("[%s]\n[%s]\n\n", szLine[0], szLine[1]);

            // 加入
            Regcomp(szLine[0], szLine[1]);
            memset(s, 0, sizeof(s));
            memset(szLine, 0, sizeof(szLine));
            nLine = 0;
        }
        nLine++;
    }
    if(0 == m_nCount) // 当调用Regcomp()时，m_nCount增加；
    {
        return -13;
    }
    return 0;
}

// 编译表达式，存入数组（追加），用于匹配。
// pRegex : 正则式串
// pTarget: 将要转换成的目标形式表达式
// 返回：成功返0，出错返非0；
int StringFilter::Regcomp(const char *pRegex, const char *pTarget)
{
    assert(NULL != pRegex && NULL != pTarget);

    if(m_nCount >= MAX_REGEX_COUNT)
    {
        return -2;
    }

    // 编译
    if(regcomp(&m_Node[m_nCount].regex, pRegex, CFLAGS) != 0)
    {
        return -1;
    }
    m_Node[m_nCount].pTarget = strdup(pTarget);
    if(NULL == m_Node[m_nCount].pTarget)
    {
        return -3;
    }
    m_nCount++;
    return 0;
}

// 根据设定的值，转换pSrc串，结果放到pDest中；
// 注意，要确保pDest足够长；
// 返回：成功返回大于0的值，失败返-1；
int StringFilter::ChangTo(const char *pSrc, char *pDest)
{
    assert(NULL != pSrc);

    int i;
    regmatch_t pm[10];
    const size_t nmatch = 10;

    /*
     *  遍历正则式数组m_Node，当有一项匹配中时，输出并返回（略过后继项）；
     */
    for(i=0; i<m_nCount; i++)
    {
        if(regexec(&m_Node[i].regex, pSrc, nmatch, pm, 0) == 0)
        {
            int j, x, nLen;
            char *p = m_Node[i].pTarget;
            char *q = pDest;

            // 逐个字符输出，遇到$1,$2,$3,...，则转换；
            while('\0' != *p)
            {
                if('$' == *p && isdigit(*(p+1)))
                {
                    x = *(p+1) - '0'; // 字母转为数字
                    nLen = pm[x].rm_eo-pm[x].rm_so;
                    strncpy(q, pSrc+pm[x].rm_so, nLen);
                    q += nLen;
                    p += 2; // 跳过'$x'
                }
                else
                {
                    *q++ = *p++;
                }
            }
            return i+1; // 返回命中的是第几项正则式（从1开时算）
        }
    }
    return -1;
}

// pSrc中存在符合配置中指定的串
// 返回：成功返回大于0的值，失败返-1；
int StringFilter::IsExist(const char *pSrc)
{
    assert(NULL != pSrc);

    int i;
    regmatch_t pm[10];
    const size_t nmatch = 10;

    for(i=0; i<m_nCount; i++)
    {
        if(regexec(&m_Node[i].regex, pSrc, nmatch, pm, 0) == 0)
        {
            return i+1;
        }
    }
    return -1;
}
