// Rocky 2010-05-23 22:22:28
#include "Request.h"
#include "Page.h"
#include "User.h"
#include "Encrypt.h"
#include "Submit_RegisterUser.h"
namespace SUBMIT_REGISTERUSER_SPACE
{



// 标明模块
static const string THIS_MODULE = "RegisterUser";





Submit_RegisterUser::Submit_RegisterUser()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Submit_RegisterUser::Submit_RegisterUser(const string &page, const string &element)
                        : Submit(page, element)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Submit_RegisterUser::~Submit_RegisterUser()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Submit_RegisterUser::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Submit::DoInit();
}

// 子类对象创建器
Submit *Submit_RegisterUser::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Submit_RegisterUser;
}





/******************************** 业务代码 ********************************/




// Rocky 2010-05-24 00:10:13
int Submit_RegisterUser::Deal(Page *page)
{
    const Request *request = page->GetRequest();
    const string &username = request->GetField("username");
    int ret;
    string msg;

    try
    {
        /*
         * 简单验证输入是否合法
         *   检查 “\ / : * ? " < > | '”
         */
        if( username.find_first_of("\\/:*?\"<>|'") != -1 )
        {
            LOG_ERROR("Username error: [%s]", username.c_str());
            msg = "用户名含有非法字符！";
            throw 1;
        }
        if("" == username)
        {
            LOG_ERROR("Username can't empty.");
            msg = "用户名不能为空！";
            throw 2;
        }

        const string &password = request->GetField("password");
        const string &password1 = request->GetField("password1");
        if("" != password && password != password1)
        {
            LOG_ERROR("Password no match on twice input.");
            msg = "两次输入的密码不一样！";
            throw 4;
        }

        // 是否为已存在的用户
        if( User::Get(username)->isValid() )
        {
            LOG_ERROR("Username exist: [%s]", username.c_str());
            msg = "用户[" + username + "]已被注册！";
            throw 3;
        }

        /*
         * 初始化，创建用户必需目录等；
         */
        User *user = User::Create(username);
        if(NULL == user)
        {
            LOG_ERROR("Create user error: [%s]", username.c_str());
            msg = "内部错误[代码:SR5]";
            throw 5;
        }

        // 当需要修改用户信息时，需加锁；
        UNIQUE_LOCK( user->GetLock() );

        /*
         * 设置用户信息，说明：
         *
         *  1. 密码加密存放，不可逆（即丢失后不可找回）
         *  2. 答案也被加密存放，以
         */
        const string &question = request->GetField("question");
        const string &answer = request->GetField("answer");
        const string &comment = request->GetField("comment");
        const string &regtime = request->GetField("regtime");

        user->SetInfo("password", ("" == password ? "" : Crypt(password, CRYPT_VERSION)));
        user->SetInfo("question", question);
        user->SetInfo("answer", Crypt(answer, CRYPT_VERSION));
        user->SetInfo("comment", comment);
        user->SetInfo("regtime", NowTime("%Y%m%d%H%M%S"));
        user->SetInfo("status", "enable");

        if( user->Save() < 0 )
        {
            LOG_ERROR("Save user cfg error: [%s]", username.c_str());
            msg = "内部错误[代码:SR6]";
            throw 6;
        }

        msg = "注册成功";
    }
    catch(int)
    {
    }

    const string html = HtmlAlert(msg);
    page->GetRequest()->GetConnect()->Send(html);

    return OK;
}














static Submit_RegisterUser tmp("register", THIS_MODULE);

}// end of SUBMIT_REGISTERUSER_SPACE

