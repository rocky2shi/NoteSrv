// Rocky 2010-05-29 18:11:31
#include "Session.h"
#include "User.h"
#include "Page_Ping.h"
namespace PAGE_PING_SPACE
{




// 标明模块
static const string THIS_MODULE = "ping";



Page_Ping::Page_Ping()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Page_Ping::~Page_Ping()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}


int Page_Ping::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    m_html = "";
    return Page::DoInit();
}

// 子类对象创建器
Page *Page_Ping::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Page_Ping;
}








/******************************** 业务代码 ********************************/


// 输出数据体
int Page_Ping::OutBody()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    assert(NULL != m_request);

    const string &page = m_request->GetPageName();

    // 对登录页面，不做测试（返回OK则不显示验证输入框)；
    if("login" == page)
    {
        m_request->GetConnect()->Send("OK");
    }
    else if( Session::Get(m_request)->isValid() )
    {
        m_request->GetConnect()->Send("OK");
    }
    else
    {
        /*
         * 查看是否有其他人使用此账号在别处理登录
         *
         *   注意，若已有用户在别处登录，则不能再使
         *   用m_request->GetCurrentUser()来获取用
         *   户名。
         */
        const string &username = m_request->GetField("username");
        User *user = User::Get(username);
        const string &id = user->GetInfo("login", "session");
        Session *session = Session::Get( id );
        string msg;

        if( session->isValid() )
        {
            // 取对方登录ip
            const string &ip = Session::Get( id )->GetIp();
            msg = "此账号在别处登录，对方IP: " + ip;
        }
        else
        {
            msg = "未知错误（可能已超时），请重新登录。";
        }

        m_request->GetConnect()->Send(msg);
    }

    return Page::OutBody();
}








// 定义动态库入口
DefinitinoDllEnter(Page_Ping, THIS_MODULE)


}// end of PAGE_PING_SPACE

