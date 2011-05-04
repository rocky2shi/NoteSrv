// Rocky 2010-04-23 18:44:43
#include "DllLoad_Linux.h"
#include <dlfcn.h>
#include <unistd.h>
namespace DLLLOAD_LINUX_SPACE
{



DllLoad_Linux::DllLoad_Linux() : m_handle(NULL)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

DllLoad_Linux::~DllLoad_Linux()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}


// 打开动态库文件
int DllLoad_Linux::Open(const string &file)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    m_handle = dlopen(file.c_str(), RTLD_LAZY);
    if (NULL == m_handle)
    {
         LOG_ERROR("dlopen error:%s", dlerror());
         return ERR;
    }

    return OK;
}

// 关闭动态库
int DllLoad_Linux::Close()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    dlclose(m_handle);
    return OK;
}

// 取执行对象
void *DllLoad_Linux::GetSymbol(const string &sym)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    char *error;
    void *exe;

    // 取出执行符号
    exe = dlsym(m_handle, sym.c_str());
    if ((error = dlerror()) != NULL)
    {
        Debug("dlsym error: %s", error);
        return NULL;
    }

    return exe;
}







}// end of DLLLOAD_LINUX_SPACE
