// Rocky 2010-05-07 18:21:57
#include "Page_Error.h"
namespace PAGE_ERROR_SPACE
{










// 标明模块
static const string THIS_MODULE = "error";



Page_Error::Page_Error()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Page_Error::~Page_Error()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}


int Page_Error::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return Page::DoInit();
}

// 子类对象创建器
Page *Page_Error::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Page_Error;
}








/******************************** 业务代码 ********************************/

int Page_Error::Deal()
{
    assert(NULL != m_request);

    // 出错时直接转登录页
    ChangeTo("login");
    return OK;
}






// 定义动态库入口
DefinitinoDllEnter(Page_Error, THIS_MODULE)

}// end of PAGE_ERROR_SPACE
