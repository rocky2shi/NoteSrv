#ifndef __CLASSINIT_H_20100603__
#define __CLASSINIT_H_20100603__
#include "Typedef.h"
#include <list>
#include <assert.h>
namespace CLASSINIT_SPACE
{

using namespace std;



// 执行类的初始化
class ClassInit
{
public:
    typedef int (*ExeFunc)();
    typedef enum {
        INIT_HIGH,  // 高优先级标志
        INIT_LOW,   // 低优先级标志
    }InitLevel;

public:
    ClassInit(ExeFunc init, InitLevel level=INIT_LOW);

    // 调用名个注册的初始化
    static int init();

    //
    static int RegisterHL(const ExeFunc exector);   // 高优先级
    static int RegisterLL(const ExeFunc exector);   // 低优先级

private:
    static list<ExeFunc> m_HighQueue;
    static list<ExeFunc> m_LowQueue;
};






}// end of CLASSINIT_SPACE
using namespace CLASSINIT_SPACE;
#endif // end of __CLASSINIT_H_20100603__
