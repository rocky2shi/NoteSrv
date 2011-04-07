// Rocky 2010-05-23 13:10:46
#include "Page_Register.h"
namespace PAGE_REGISTER_SPACE
{




// 标明模块
static const string THIS_MODULE = "register";



Page_Register::Page_Register()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Page_Register::~Page_Register()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}


int Page_Register::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    m_html = GlobalConfig::instance()->TemplateDir() + "Register.html";
    return Page::DoInit();
}

// 子类对象创建器
Page *Page_Register::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Page_Register;
}








/******************************** 业务代码 ********************************/


// 输出数据体
int Page_Register::OutBody()
{
    const string &tag = m_request->GetField("tag");

    /*
     * 注册用户提交事件
     */
    if("register" == tag)
    {
        Submit::Ptr submit( Submit::New("register", "RegisterUser") );
        submit->Deal(this);
        return OK;
    }

    /*
     * 修改密码提交事件
     */
    if("modify" == tag)
    {
        Submit::Ptr submit( Submit::New("register", "ModifyPassword") );
        submit->Deal(this);
        return OK;
    }

    /*
     * 找回密码提交事件
     */
    if("retrieve" == tag)
    {
        Submit::Ptr submit( Submit::New("register", "GetPassword") );
        submit->Deal(this);
        return OK;
    }

    LOG_DEBUG("tag=[%s]", tag.c_str());

    return Page::OutBody();
}






// 定义动态库入口
DefinitinoDllEnter(Page_Register, THIS_MODULE)

}// end of PAGE_REGISTER_SPACE

