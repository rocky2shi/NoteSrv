#ifndef __DLLLOAD_LINUX_H_20100423__
#define __DLLLOAD_LINUX_H_20100423__
#include "DllLoad.h"
namespace DLLLOAD_LINUX_SPACE
{



// linux上动态库加载类
class DllLoad_Linux : public DllLoad
{
public:
    DllLoad_Linux();
    virtual ~DllLoad_Linux();

    virtual int Open(const string &file);
    virtual int Close();
    virtual void *GetSymbol(const string &sym);


private:
    void *m_handle;
};






}// end of DLLLOAD_LINUX_SPACE
using namespace DLLLOAD_LINUX_SPACE;
#endif // end of __DLLLOAD_LINUX_H_20100423__
