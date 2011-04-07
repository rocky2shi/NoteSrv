#ifndef __DLLLOAD_H_20100415__
#define __DLLLOAD_H_20100415__
#include "Common.h"
namespace DLLLOAD_SPACE
{



// 平台无关动态库加载类
class DllLoad
{
public:
    // 定义智能指针类型
    typedef AutoPtr<DllLoad> Ptr;

public:
    virtual ~DllLoad();

    // 打开动态库文件
    virtual int Open(const string &file);

    // 关闭动态库
    virtual int Close();

    // 取执行对象
    virtual void *GetSymbol(const string &sym);


    // 跟据传入的id从工厂中产生一个处理对象
    static DllLoad *New(const string &id="linux");

protected:
    DllLoad();
};







}// end of DLLLOAD_SPACE
using namespace DLLLOAD_SPACE;
#endif // end of __DLLLOAD_H_20100415__
