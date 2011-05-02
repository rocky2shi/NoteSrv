// Rocky 2009-07-09 12:06:38
#ifndef __DEBUG_H_20090725__
#define __DEBUG_H_20090725__


#include <assert.h>
#include <stdio.h>
#include <unistd.h>



//#define _DEBUG



// ��ɫ����
#define COLOR_RED           "\033[31;1m"
#define COLOR_GREEN         "\033[32;1m"
#define COLOR_YELLOW        "\033[33;1m"
#define COLOR_FLASH         "\033[05m"
#define COLOR_NONE          "\033[00m"
static const char *COLOR[] = {
    "\033[30;1m",
    "\033[31;1m",
    "\033[32;1m",
    "\033[33;1m",
    "\033[34;1m",
    "\033[35;1m",
    "\033[36;1m"
};




// ���ڵ��ԣ���¼�������ù켣��
// �������롢�˳�����ʱ�����ù��캯������ӡ�����ã���Ϣ����
// ��Ϻ궨��FUNCTION_TRACK()��ʹ�ã�����:
//      void Init()
//      {
//          FUNCTION_TRACK(); // �����켣����
//          ......
//          ......
//      }
//
class CODE_TRACK
{
public:
    CODE_TRACK(){}
    CODE_TRACK( const char *pFunName,
                const char *pFileName, int nLine, int &nCallCount,
                const char *s="")
                              : pFile( pFileName ),
                                pFun( pFunName ),
                                str( s )
    {
        assert(NULL != pFunName && NULL != pFileName);
        color = nCount % (sizeof(COLOR)/sizeof(COLOR[0])); // ˳��ȡ��ɫ���е�Ԫ��
        nL = nLine;
        nIndex = nCount++;  // ���к����������
        nCallCount++;       // ��ǰ�������ü���(�ܴ���)
        printf("%s:%d: +++into %s%s()%s... [%d]<%d> %s\n",
                pFile, nL, COLOR[color], pFun, COLOR_NONE, nIndex, nCallCount, str);
        // Print7(" +++�ѽ���%s()... [%d]<%d>", pFun, nIndex, nCallCount);
        fflush(stdout);
    }
    ~CODE_TRACK()
    {
        printf("%s:%d: ---out  %s%s()%s... [%d] %s\n",
            pFile, nL, COLOR[color], pFun, COLOR_NONE, nIndex, str);
        // Print7(" ---�˳�  %s()... [%d]", pFun, nIndex);
        fflush(stdout);
    }
    const char *pFile;  // �ļ���
    int nL;             // �к�
    const char *pFun;   // ������
    int nIndex;         // ����
    int color;          // �������ɫ��������
    const char *str;    // ��ע��Ϣ
    static int nCount;  // ���е��ü���
};
// �����켣����
#ifdef _DEBUG
#define FUNCTION_TRACK()                                                       \
        static int nFunctionCallCount;                                         \
        CODE_TRACK __FUNCTION__##_CALL( __FUNCTION__, __FILE__, __LINE__,      \
                                        nFunctionCallCount)
#else
#define FUNCTION_TRACK();
#endif



#ifdef _DEBUG
#define Debug(format, args...)                                                 \
{                                                                              \
    printf("%s:%d : " format "\n", __FILE__, __LINE__, ##args);                \
}
#else
#define Debug(format, args...)
#endif









#endif // end of __DEBUG_H_20090725__
