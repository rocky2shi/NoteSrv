// Rocky 2010-05-24 13:55:53
#include "Request.h"
#include "Page.h"
#include "User.h"
#include "Encrypt.h"
#include "Submit_GetPassword.h"
namespace SUBMIT_GETPASSWORD_SPACE
{

// 标明模块
static const string THIS_MODULE = "GetPassword";





Submit_GetPassword::Submit_GetPassword()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Submit_GetPassword::Submit_GetPassword(const string &page, const string &element)
                        : Submit(page, element)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Submit_GetPassword::~Submit_GetPassword()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Submit_GetPassword::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Submit::DoInit();
}

// 子类对象创建器
Submit *Submit_GetPassword::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Submit_GetPassword;
}





/******************************** 业务代码 ********************************/




// Rocky 2010-05-24 12:15:06
int Submit_GetPassword::Deal(Page *page)
{
    const Request *request = page->GetRequest();
    const string &username = request->GetField("username");
    int ret;
    string msg;
    string html;

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

        /*
         * 以客户端传来的答案为key，解密配置中记录的密文件答案
         */
        const string &cfg_answer = user->GetInfo("answer");
        const string &cli_answer = request->GetField("answer");

        LOG_DEBUG("cfg_answer=[%s]", cfg_answer.c_str());
        LOG_DEBUG("cli_answer=[%s]", cli_answer.c_str());

        if(cfg_answer != Crypt(cli_answer, CRYPT_VERSION))
        {
            LOG_ERROR("Answer password, username=[%s]", username.c_str());
            msg = "答案错误";
            throw 2;
        }

        /*
         * 答案正确，重置密码为一随机串，用户取得该密码后，可到“修改密码”页
         * 再改为自已需要的密码；
         */
        const string &password = IntToString( GetRandomInt(10000000, 99999999) );
        user->SetInfo("password", Crypt(password, CRYPT_VERSION));
        user->SetInfo("modify", NowTime("%Y%m%d%H%M%S"));

        if( user->Save() < 0 )
        {
            LOG_ERROR("Save user cfg error: [%s]", username.c_str());
            msg = "内部错误[代码:SR6]";
            throw 6;
        }

        html = HtmlSetParentInputValue("get_password", password);
    }
    catch(int)
    {
        html = HtmlSetParentInputValue("get_password", "[" + msg + "]");
    }

    page->GetRequest()->GetConnect()->Send(html);

    return OK;
}














static Submit_GetPassword tmp("register", THIS_MODULE);

}// end of SUBMIT_GETPASSWORD_SPACE
