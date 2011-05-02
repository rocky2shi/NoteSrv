// Rocky 2008-6-13 12:28
#ifndef _IDC_STRINGFILTER_ROCKY20080613_
#define _IDC_STRINGFILTER_ROCKY20080613_
#include <regex.h>

/*
 *
 *  ����������ʽ�����ˣ�ת�����ַ�����
 *
 */







class StringFilter
{
    struct Node {
        regex_t regex;          // ָ�����������ʽ
        char *pTarget;          // ��Ž�ת���ɵ�Ŀ������ʽ
    };
    enum {
        MAX_REGEX_COUNT=100,                // �����ʽ��
        CFLAGS = REG_EXTENDED|REG_NEWLINE   // ʹ�õı��ʽ��־
    };
public:
    StringFilter();
    StringFilter(const char *pFile);
    ~StringFilter();

    // �򿪹��˱��ʽ�������ļ�������ʼ�����ʽ��
    // ���أ��ɹ���0��������0��
    int OpenConfig(const char *pFile);

    // ������ʽ���������飨׷�ӣ�������ƥ�䡣
    // pRegex : ����ʽ��
    // ���أ��ɹ���0��������0��
    int Regcomp(const char *pRegex, const char *pTarget);

    // �����趨��ֵ��ת��pSrc��������ŵ�pDest�У�
    // ���أ��ɹ����ش���0��ֵ��ʧ�ܷ�-1��
    int ChangTo(const char *pSrc, char *pDest);

    // pSrc�д��ڷ���������ָ���Ĵ�
    // ���أ��ɹ����ش���0��ֵ��ʧ�ܷ�-1��
    int IsExist(const char *pSrc);

private:
    void Clear();

private:
    Node m_Node[MAX_REGEX_COUNT];   // ����ʽ����
    int m_nCount;       // ��ǰm_Node��Ԫ�ؼ���
};






#endif  //end of _IDC_STRINGFILTER_ROCKY20080613_
