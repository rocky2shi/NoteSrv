// Rocky 2010-05-04 19:06:05
#include "UserData.h"
#include "Encrypt.h"
#include "Page_Edit.h"
namespace PAGE_EDIT_SPACE
{






// 标明模块
static const string THIS_MODULE = "edit";



Page_Edit::Page_Edit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Page_Edit::~Page_Edit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}


int Page_Edit::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    m_html = GlobalConfig::instance()->TemplateDir() + "Edit.html";
    return Page::DoInit();
}

// 子类对象创建器
Page *Page_Edit::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Page_Edit;
}








/******************************** 业务代码 ********************************/


int Page_Edit::OutBody()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    assert(NULL != m_request);

    /*
     * 是否为解密提交事件
     */
    const string &decryption = m_request->GetField("decryption");
    if("" != decryption)
    {
        Submit::Ptr submit( Submit::New(SUBMIT_GLOBAL, "decryption") );
        submit->Deal(this);
    }

    /*
     * 是否为设置密码提交事件
     */
    const string &set_password = m_request->GetField("set_password");
    if("" != set_password)
    {
        // 调设置密码代码
        Submit::Ptr submit( Submit::New("edit", "SetPassword") );
        submit->Deal(this);
    }

    /*
     * 内容保存提交事件
     */
    const string &save = m_request->GetField("save_text");
    if("" != save)
    {
        Submit::Ptr submit( Submit::New("edit", "SaveText") );
        submit->Deal(this);
        return OK;
    }

    // 显示当前页
    return Page::OutBody();
}











// 定义动态库入口
DefinitinoDllEnter(Page_Edit, THIS_MODULE)

}// end of PAGE_EDIT_SPACE

