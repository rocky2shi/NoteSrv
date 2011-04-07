#ifndef __MODULE_WEBSERVER_H_20100412__
#define __MODULE_WEBSERVER_H_20100412__
#include "Module.h"
#include "Common.h"
namespace MODULE_WEBSERVER_SPACE
{





// 创建web服务器类
class Module_WebServer : public Module
{
protected:
    virtual int Run();


public:
    Module_WebServer(const string &id);
    virtual ~Module_WebServer();

protected:
    Module_WebServer();
    virtual int DoInit();

private:
   Connect *m_server;
};









}// end of MODULE_WEBSERVER_SPACE
using namespace MODULE_WEBSERVER_SPACE;
#endif // end of __MODULE_WEBSERVER_H_20100412__
