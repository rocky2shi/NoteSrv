// Rocky 2010-04-12 10:24:45
#include "Module_WebServer.h"
#include "Connect.h"
#include "ConnectQueue.h"
namespace MODULE_WEBSERVER_SPACE
{








// ����ģ��
static const string THIS_MODULE = "WebServer";




Module_WebServer::Module_WebServer() : m_server(NULL)
{
    FUNCTION_TRACK(); // �����켣����
}

// ע���õĹ��캯��
Module_WebServer::Module_WebServer(const string &id): Module(id), m_server(NULL)
{
    FUNCTION_TRACK(); // �����켣����
}

Module_WebServer::~Module_WebServer()
{
    FUNCTION_TRACK(); // �����켣����
    delete m_server;
}


int Module_WebServer::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

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









/******************************** ҵ����� ********************************/



// ����web����
int Module_WebServer::Run()
{
    FUNCTION_TRACK(); // �����켣����

    int ret;

    while( !SignalDeal::exit() )
    {
        Connect *pConnect = m_server->Accept();
        if(NULL == pConnect)
        {
            LOG_ERROR("���ӳ���");
            sleep(1); // �����������ѭ��
            continue;
        }
        /*
         * ע�����µĵ��ù�ϵ��ȡip����ʱ��connect��������Ч�ģ�
         */
        LOG_DEBUG("Connect[%p] comes from [%s], push to queue.",
                             pConnect, pConnect->GetPeerAddr().c_str());

        // �����Ӵ�����У���ִ���̴߳������첽������
        ConnectQueue::instance()->push(pConnect);
    }

    return OK;
}













// ע��
static Module_WebServer tmp(THIS_MODULE);




}// end of MODULE_WEBSERVER_SPACE
