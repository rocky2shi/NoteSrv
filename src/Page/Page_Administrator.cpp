// Rocky 2010-05-28 11:59:37
#include "Page_Administrator.h"
namespace PAGE_ADMINISTRATOR_SPACE
{




// 标明模块
static const string THIS_MODULE = "administrator";




Page_Administrator::Page_Administrator()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Page_Administrator::~Page_Administrator()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}


int Page_Administrator::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    m_html = GlobalConfig::instance()->TemplateDir() + "Administrator.html";
    return Page::DoInit();
}

// 子类对象创建器
Page *Page_Administrator::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Page_Administrator;
}








/******************************** 业务代码 ********************************/




int Page_Administrator::OutBody()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    assert(NULL != m_request);

    // 本页面只允许管理员操作（或具有管理员权限 [XXX]）
    if("administrator" != m_request->GetCurrentUser())
    {
        const string &html = "[<a href='javascript:history.back();'>返回</a>]"
                           + HtmlAlert("权限不足，只有管理员能访问此页面。");
        m_request->GetConnect()->Send(html);
        return ERR;
    }

    /*
     * 日志配制提交事件
     */
    const string &log_save = m_request->GetField("log_save");
    if("" != log_save)
    {
        Submit::Ptr submit( Submit::New("administrator", "LogConfigItems") );
        submit->Deal(this);
        m_request->GetConnect()->Send( HtmlAlert("OK") );
        return OK;
    }

    /*
     * 删除用户提交事件
     */
    const string &delete_user = m_request->GetField("delete_user");
    if("" != delete_user)
    {
        Submit::Ptr submit( Submit::New("administrator", "DeleteUser") );
        int ret = submit->Deal(this);
        if(ret < 0)
        {
            const string msg = GetResult("msg");
            m_request->GetConnect()->Send(msg);
        }
        else
        {
            m_request->GetConnect()->Send("OK");
        }
        return OK;
    }

    return Page::OutBody();
}











// 定义动态库入口
DefinitinoDllEnter(Page_Administrator, THIS_MODULE)

}// end of PAGE_ADMINISTRATOR_SPACE
