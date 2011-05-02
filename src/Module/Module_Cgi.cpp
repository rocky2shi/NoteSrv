// Rocky 2010-04-15 17:30:33
#include "Module_Cgi.h"
#include "Connect.h"
#include "ConnectQueue.h"
#include "Page.h"
#include "Request.h"
namespace MODULE_CGI_SPACE
{






// �̳߳ػص�����
static void *CgiDealBackCall(void *param)
{
    FUNCTION_TRACK(); // �����켣����

    Connect *pConnect = (Connect *)param;

    Request request( pConnect );

    // ȡҳ�洦����󣨸���ҳ����������Ӧ�������
    const string &name = request.GetPageName();

    LOG_DEBUG("page=[%s]", name.c_str());

    Page *page = NULL;

    /*
     * �������ͣ�����������ò�ͬ����ģ�飻
     */
    if( request.isText() )
    {
        // ���ı�������
        page = Page::New("text");
    }
    else if( request.isImage() )
    {
        // ��ͼƬ������
        page = Page::New("image");
    }
    else
    {
        /*
         * ����cgi����ȣ���Ҫ�û���¼���ܲ�����
         */
        if( ! request.ok() )
        {
            // ��������ҳ
            page = Page::New("error");
        }
        else if("" == name)
        {
            // ҳ���ʱ�������¼ҳ����ע�����û��ѵ�¼ʱ�������ɵ�¼ҳ����Ԥ��ҳ��
            page = Page::New("login");
        }
        else
        {
            // ���������ҳ��
            page = Page::New( name );
        }
    }

    if(NULL != page)
    {
        // ������������
        page->SetRequest( &request );

        // ִ�д���
        page->Deal();

        // �����꣬�ͷ��ڴ棻
        delete page;
    }

    // ������ɣ��ر����ӣ�
    pConnect->Close();

    return NULL;
}













// ����ģ��
static const string THIS_MODULE = "CgiDeal";




Module_Cgi::Module_Cgi() : m_ThreadPool( NULL )
{
    FUNCTION_TRACK(); // �����켣����
}

// ע���õĹ��캯��
Module_Cgi::Module_Cgi(const string &id): Module(id), m_ThreadPool( NULL )
{
    FUNCTION_TRACK(); // �����켣����
}

Module_Cgi::~Module_Cgi()
{
    FUNCTION_TRACK(); // �����켣����
}


int Module_Cgi::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    int ret;


    m_ThreadPool = new ThreadPool( CgiDealBackCall );
    if(NULL == m_ThreadPool)
    {
        LOG_ERROR("new ThreadPool() error.");
        return ERR;
    }


    ret = m_ThreadPool->Init();
    if(ret < 0)
    {
        LOG_ERROR("m_ThreadPool.Init() error.");
        return ERR;
    }

    return Module::DoInit();;
}







/******************************** ҵ����� ********************************/




// ѭ�����ô�����
int Module_Cgi::Run()
{
    FUNCTION_TRACK(); // �����켣����

    // ����ÿ������
    while( !SignalDeal::exit() )
    {
        // ���ô�������ÿ�ε��ö�ɨ��һ�����Ӷ��У�ֱ�����пղŷ��أ�
        Deal();

    }// end of while( ...

    return OK;
}


// ����web����
int Module_Cgi::Deal()
{
    FUNCTION_TRACK(); // �����켣����

    Connect *pConnect;
    int ret;

    // ȡ���������Ӷ��󣬷ŵ��̳߳��С��������ݣ���ȴ���
    while( (pConnect = ConnectQueue::instance()->pop()) != NULL )
    {
        LOG_DEBUG("get pConnect=[%p]", pConnect);
        while( (ret = m_ThreadPool->push( pConnect )) < 0 )
        {
            LOG_INFO("thread queue is full, sleep 1s ...");
            sleep(1);
        }
    }

    //LOG_DEBUG("connect queue is empty.");

    return OK;
}













// ע��
static Module_Cgi tmp(THIS_MODULE);






}// end of MODULE_CGI_SPACE
