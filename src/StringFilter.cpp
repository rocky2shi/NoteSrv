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

// ���
void StringFilter::Clear()
{
    memset(m_Node, 0, sizeof(m_Node));
    m_nCount = 0;
}

// �򿪹��˱��ʽ�������ļ�������ʼ�����ʽ��
// ���أ��ɹ���0��������0��
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
     *  �����ļ���ȡ������ʽ�ԣ��磺
     *
     *      REGEXP1=([0-9]+).([0-9]+).([0-9]+).([0-9]+)
     *      TARGET1=$1.$2.$3.$4
     *
     *  ��ȡ���õ�ֵ�Ժ󣬵���Regcomp()�����������顣
     */
    while( fgets(szBuf, sizeof(szBuf), fp) )
    {
        // ����ע�ͼ�����
        if('#' == szBuf[0] || '\r' == szBuf[0] || '\n' == szBuf[0]  )
        {
            continue;
        }
        // ���ƣ���������β��������
        if(sscanf(szBuf, "REGEXP%[0-9]=%[^'\r''\n']", s[0], szLine[0]) != 2
           && sscanf(szBuf, "TARGET%[0-9]=%[^'\r''\n']", s[1], szLine[1]) != 2)
        {
            continue;
        }
        // ��ȡ������
        if(nLine> 0 && strcmp(s[0], s[1]) == 0)
        {
            // printf("[%s]\n[%s]\n\n", szLine[0], szLine[1]);

            // ����
            Regcomp(szLine[0], szLine[1]);
            memset(s, 0, sizeof(s));
            memset(szLine, 0, sizeof(szLine));
            nLine = 0;
        }
        nLine++;
    }
    if(0 == m_nCount) // ������Regcomp()ʱ��m_nCount���ӣ�
    {
        return -13;
    }
    return 0;
}

// ������ʽ���������飨׷�ӣ�������ƥ�䡣
// pRegex : ����ʽ��
// pTarget: ��Ҫת���ɵ�Ŀ����ʽ���ʽ
// ���أ��ɹ���0��������0��
int StringFilter::Regcomp(const char *pRegex, const char *pTarget)
{
    assert(NULL != pRegex && NULL != pTarget);

    if(m_nCount >= MAX_REGEX_COUNT)
    {
        return -2;
    }

    // ����
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

// �����趨��ֵ��ת��pSrc��������ŵ�pDest�У�
// ע�⣬Ҫȷ��pDest�㹻����
// ���أ��ɹ����ش���0��ֵ��ʧ�ܷ�-1��
int StringFilter::ChangTo(const char *pSrc, char *pDest)
{
    assert(NULL != pSrc);

    int i;
    regmatch_t pm[10];
    const size_t nmatch = 10;

    /*
     *  ��������ʽ����m_Node������һ��ƥ����ʱ����������أ��Թ�������
     */
    for(i=0; i<m_nCount; i++)
    {
        if(regexec(&m_Node[i].regex, pSrc, nmatch, pm, 0) == 0)
        {
            int j, x, nLen;
            char *p = m_Node[i].pTarget;
            char *q = pDest;

            // ����ַ����������$1,$2,$3,...����ת����
            while('\0' != *p)
            {
                if('$' == *p && isdigit(*(p+1)))
                {
                    x = *(p+1) - '0'; // ��ĸתΪ����
                    nLen = pm[x].rm_eo-pm[x].rm_so;
                    strncpy(q, pSrc+pm[x].rm_so, nLen);
                    q += nLen;
                    p += 2; // ����'$x'
                }
                else
                {
                    *q++ = *p++;
                }
            }
            return i+1; // �������е��ǵڼ�������ʽ����1��ʱ�㣩
        }
    }
    return -1;
}

// pSrc�д��ڷ���������ָ���Ĵ�
// ���أ��ɹ����ش���0��ֵ��ʧ�ܷ�-1��
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
