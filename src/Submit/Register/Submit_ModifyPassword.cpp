// Rocky 2010-05-24 12:05:18
#include "Request.h"
#include "Page.h"
#include "User.h"
#include "Encrypt.h"
#include "Submit_ModifyPassword.h"
namespace SUBMIT_MODIFYPASSWORD_SPACE
{

// 标明模块
static const string THIS_MODULE = "ModifyPassword";





Submit_ModifyPassword::Submit_ModifyPassword()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Submit_ModifyPassword::Submit_ModifyPassword(const string &page, const string &element)
                        : Submit(page, element)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Submit_ModifyPassword::~Submit_ModifyPassword()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Submit_ModifyPassword::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Submit::DoInit();
}

// 子类对象创建器
Submit *Submit_ModifyPassword::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Submit_ModifyPassword;
}





/******************************** 业务代码 ********************************/




// Rocky 2010-05-24 12:15:06
int Submit_ModifyPassword::Deal(Page *page)
{
    const Request *request = page->GetRequest();
    const string &username = request->GetField("username");
    int ret;
    string msg;

    try
    {
        /*
         * 验证用户输入
         */
        User *user = User::Get(username);
        if( ! user->isValid() )
        {
            LOG_ERROR("Username exist: [%s]", username.c_str());
            msg = "该用户不存在";
            throw 1;
        }

        // 当需要修改用户信息时，需加锁；
        UNIQUE_LOCK( user->GetLock() );

        const string &new_password1 = request->GetField("new_password1");
        const string &new_password2 = request->GetField("new_password2");
        if("" != new_password1 && new_password1 != new_password2)
        {
            LOG_ERROR("Password no match on twice input.");
            msg = "两次输入的密码不一样";
            throw 2;
        }

        /*
         * 把用户配置文件和客户端传来的做比较
         */
        const string &cfg_password = user->GetInfo("password");         // 密文
        const string &old_password = request->GetField("old_password"); // 明文
        if("" != cfg_password && cfg_password != Crypt(old_password, CRYPT_VERSION))
        {
            LOG_ERROR("Old password, username=[%s]", username.c_str());
            msg = "原密码出错";
            throw 3;
        }

        /*
         * 修改用户信息
         */
        user->SetInfo("password", ("" == new_password1 ? "" : Crypt(new_password1, CRYPT_VERSION)));
        user->SetInfo("modify", NowTime("%Y%m%d%H%M%S"));

        if( user->Save() < 0 )
        {
            LOG_ERROR("Save user cfg error: [%s]", username.c_str());
            msg = "内部错误[代码:SM6]";
            throw 6;
        }

        msg = "修改成功";
    }
    catch(int)
    {
    }

    const string html = HtmlAlert(msg);
    page->GetRequest()->GetConnect()->Send(html);

    return OK;
}














static Submit_ModifyPassword tmp("register", THIS_MODULE);

}// end of SUBMIT_MODIFYPASSWORD_SPACE
