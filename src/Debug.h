// Rocky 2009-07-09 12:06:38
#ifndef __DEBUG_H_20090725__
#define __DEBUG_H_20090725__


#include <assert.h>
#include <stdio.h>
#include <unistd.h>



//#define _DEBUG



// 颜色定义
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




// 用于调试，记录函数调用轨迹。
// （当进入、退出函数时，调用构造函数来打印（调用）信息。）
// 结合宏定义FUNCTION_TRACK()来使用，例如:
//      void Init()
//      {
//          FUNCTION_TRACK(); // 函数轨迹跟综
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
        color = nCount % (sizeof(COLOR)/sizeof(COLOR[0])); // 顺序取颜色组中的元素
        nL = nLine;
        nIndex = nCount++;  // 所有函数调用序号
        nCallCount++;       // 当前函数调用计数(总次数)
        printf("%s:%d: +++into %s%s()%s... [%d]<%d> %s\n",
                pFile, nL, COLOR[color], pFun, COLOR_NONE, nIndex, nCallCount, str);
        // Print7(" +++已进入%s()... [%d]<%d>", pFun, nIndex, nCallCount);
        fflush(stdout);
    }
    ~CODE_TRACK()
    {
        printf("%s:%d: ---out  %s%s()%s... [%d] %s\n",
            pFile, nL, COLOR[color], pFun, COLOR_NONE, nIndex, str);
        // Print7(" ---退出  %s()... [%d]", pFun, nIndex);
        fflush(stdout);
    }
    const char *pFile;  // 文件名
    int nL;             // 行号
    const char *pFun;   // 函数名
    int nIndex;         // 计数
    int color;          // 随机的颜色数组索引
    const char *str;    // 备注信息
    static int nCount;  // 所有调用计数
};
// 函数轨迹跟综
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
