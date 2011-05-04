#ifndef __MODULE_CGI_H_20100415__
#define __MODULE_CGI_H_20100415__
#include "Module.h"
#include "ThreadPool.h"
#include "Common.h"
namespace MODULE_CGI_SPACE
{






// 处理web存入队列的请求
class Module_Cgi : public Module
{
protected:
    virtual int Run();
    virtual int Deal();


public:
    Module_Cgi(const string &id);
    virtual ~Module_Cgi();

protected:
    Module_Cgi();
    virtual int DoInit();

private:
    ThreadPool *m_ThreadPool;
};





}// end of MODULE_CGI_SPACE
using namespace MODULE_CGI_SPACE;
#endif // end of __MODULE_CGI_H_20100415__
