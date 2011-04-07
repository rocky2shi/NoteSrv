// Rocky 2010-04-15 17:30:33
#include "Module_Cgi.h"
#include "Connect.h"
#include "ConnectQueue.h"
#include "Page.h"
#include "Request.h"
namespace MODULE_CGI_SPACE
{






// 线程池回调函数
static void *CgiDealBackCall(void *param)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    Connect *pConnect = (Connect *)param;

    Request request( pConnect );

    // 取页面处理对象（根据页面名，调对应处理对象）
    const string &name = request.GetPageName();

    LOG_DEBUG("page=[%s]", name.c_str());

    Page *page = NULL;

    /*
     * 根据类型（情况），调用不同处理模块；
     */
    if( request.isText() )
    {
        // 调文本处理类
        page = Page::New("text");
    }
    else if( request.isImage() )
    {
        // 调图片处理类
        page = Page::New("image");
    }
    else
    {
        /*
         * 对于cgi处理等，需要用户登录才能操作；
         */
        if( ! request.ok() )
        {
            // 调出错处理页
            page = Page::New("error");
        }
        else if("" == name)
        {
            // 页面空时，进入登录页；（注：当用户已登录时，将再由登录页跳到预览页）
            page = Page::New("login");
        }
        else
        {
            // 进入求请的页面
            page = Page::New( name );
        }
    }

    if(NULL != page)
    {
        // 设置连接请求
        page->SetRequest( &request );

        // 执行处理
        page->Deal();

        // 处理完，释放内存；
        delete page;
    }

    // 处理完成，关闭连接；
    pConnect->Close();

    return NULL;
}













// 标明模块
static const string THIS_MODULE = "CgiDeal";




Module_Cgi::Module_Cgi() : m_ThreadPool( NULL )
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

// 注册用的构造函数
Module_Cgi::Module_Cgi(const string &id): Module(id), m_ThreadPool( NULL )
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Module_Cgi::~Module_Cgi()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}


int Module_Cgi::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

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







/******************************** 业务代码 ********************************/




// 循环调用处理函数
int Module_Cgi::Run()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    // 处理每个彩种
    while( !SignalDeal::exit() )
    {
        // 调用处理函数，每次调用都扫描一次连接队列（直到队列空才返回）
        Deal();

    }// end of while( ...

    return OK;
}


// 启动web服务
int Module_Cgi::Deal()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    Connect *pConnect;
    int ret;

    // 取出各个连接对象，放到线程池中。（无数据，则等待）
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













// 注册
static Module_Cgi tmp(THIS_MODULE);






}// end of MODULE_CGI_SPACE
