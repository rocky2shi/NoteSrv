// Rocky 2010-04-23 18:44:43
#include "DllLoad_Linux.h"
#include <dlfcn.h>
#include <unistd.h>
namespace DLLLOAD_LINUX_SPACE
{



DllLoad_Linux::DllLoad_Linux() : m_handle(NULL)
{
    FUNCTION_TRACK(); // �����켣����
}

DllLoad_Linux::~DllLoad_Linux()
{
    FUNCTION_TRACK(); // �����켣����
}


// �򿪶�̬���ļ�
int DllLoad_Linux::Open(const string &file)
{
    FUNCTION_TRACK(); // �����켣����

    m_handle = dlopen(file.c_str(), RTLD_LAZY);
    if (NULL == m_handle)
    {
         LOG_ERROR("dlopen error:%s", dlerror());
         return ERR;
    }

    return OK;
}

// �رն�̬��
int DllLoad_Linux::Close()
{
    FUNCTION_TRACK(); // �����켣����
    dlclose(m_handle);
    return OK;
}

// ȡִ�ж���
void *DllLoad_Linux::GetSymbol(const string &sym)
{
    FUNCTION_TRACK(); // �����켣����

    char *error;
    void *exe;

    // ȡ��ִ�з���
    exe = dlsym(m_handle, sym.c_str());
    if ((error = dlerror()) != NULL)
    {
        Debug("dlsym error: %s", error);
        return NULL;
    }

    return exe;
}







}// end of DLLLOAD_LINUX_SPACE
