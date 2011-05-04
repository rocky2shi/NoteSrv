// Rocky 2010-04-12 10:24:45
#include "Module_WebServer.h"
#include "Connect.h"
#include "ConnectQueue.h"
namespace MODULE_WEBSERVER_SPACE
{








// 标明模块
static const string THIS_MODULE = "WebServer";




Module_WebServer::Module_WebServer() : m_server(NULL)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

// 注册用的构造函数
Module_WebServer::Module_WebServer(const string &id): Module(id), m_server(NULL)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Module_WebServer::~Module_WebServer()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    delete m_server;
}


int Module_WebServer::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    int ret;
    const int port = atoi( GlobalConfig::instance()->GetOption("::ServicePort", "80").c_str() );

    m_server = Connect::New();
    if(NULL == m_server)
    {
        LOG_ERROR("new Connect obj error");
        return ERR;
    }

    ret = m_server->InitAsServer(port);
    if(ret < 0)
    {
        LOG_ERROR("init WebServer error, port=[%d]", port);
        return ERR;
    }

    LOG_INFO("Start WebServer, listen on port: %d", port);

    return Module::DoInit();;
}









/******************************** 业务代码 ********************************/



// 启动web服务
int Module_WebServer::Run()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    int ret;

    while( !SignalDeal::exit() )
    {
        Connect *pConnect = m_server->Accept();
        if(NULL == pConnect)
        {
            LOG_ERROR("连接出错");
            sleep(1); // 避免无意的死循环
            continue;
        }
        /*
         * 注意以下的调用关系，取ip操作时，connect必须是有效的；
         */
        LOG_DEBUG("Connect[%p] comes from [%s], push to queue.",
                             pConnect, pConnect->GetPeerAddr().c_str());

        // 把连接存入队列，由执行线程处理。（异步操作）
        ConnectQueue::instance()->push(pConnect);
    }

    return OK;
}













// 注册
static Module_WebServer tmp(THIS_MODULE);




}// end of MODULE_WEBSERVER_SPACE
