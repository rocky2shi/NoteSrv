// Rocky 2010-04-23 18:44:43
#include "DllLoad_Linux.h"
#include <dlfcn.h>
#include <unistd.h>
namespace DLLLOAD_LINUX_SPACE
{



DllLoad_Linux::DllLoad_Linux() : m_handle(NULL)
{
    FUNCTION_TRACK(); // º¯Êý¹ì¼£¸ú×Û
}

DllLoad_Linux::~DllLoad_Linux()
{
    FUNCTION_TRACK(); // º¯Êý¹ì¼£¸ú×Û
}


// ´ò¿ª¶¯Ì¬¿âÎÄ¼þ
int DllLoad_Linux::Open(const string &file)
{
    FUNCTION_TRACK(); // º¯Êý¹ì¼£¸ú×Û

    m_handle = dlopen(file.c_str(), RTLD_LAZY);
    if (NULL == m_handle)
    {
         LOG_ERROR("dlopen error:%s", dlerror());
         return ERR;
    }

    return OK;
}

// ¹Ø±Õ¶¯Ì¬¿â
int DllLoad_Linux::Close()
{
    FUNCTION_TRACK(); // º¯Êý¹ì¼£¸ú×Û
    dlclose(m_handle);
    return OK;
}

// È¡Ö´ÐÐ¶ÔÏó
void *DllLoad_Linux::GetSymbol(const string &sym)
{
    FUNCTION_TRACK(); // º¯Êý¹ì¼£¸ú×Û

    char *error;
    void *exe;

    // È¡³öÖ´ÐÐ·ûºÅ
    exe = dlsym(m_handle, sym.c_str());
    if ((error = dlerror()) != NULL)
    {
        Debug("dlsym error: %s", error);
        return NULL;
    }

    return exe;
}







}// end of DLLLOAD_LINUX_SPACE
