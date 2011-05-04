// Rocky 2010-05-23 12:37:58
#include "Session.h"
#include "Page.h"
#include "Request.h"
#include "Submit_Logout.h"
namespace SUBMIT_LOGOUT_SPACE
{



// 标明模块
static const string THIS_MODULE = "Logout";





Submit_Logout::Submit_Logout()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Submit_Logout::Submit_Logout(const string &page, const string &element)
                        : Submit(page, element)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Submit_Logout::~Submit_Logout()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Submit_Logout::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Submit::DoInit();
}

// 子类对象创建器
Submit *Submit_Logout::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Submit_Logout;
}





/******************************** 业务代码 ********************************/



int Submit_Logout::Deal(Page *page)
{
    const Request *request = page->GetRequest();
    const string &username = request->GetCurrentUser();

    Session::Get(request)->SetInvalid();
    LOG_INFO("User logout: [%s]", username.c_str());

    return OK;
}













static Submit_Logout tmp("login", THIS_MODULE);

}// end of SUBMIT_LOGOUT_SPACE

