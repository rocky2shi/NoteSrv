// Rocky 2010-04-17 19:11:05
#include "ChineseCoding.h"
#include "User.h"
#include "Session.h"
#include "Page_Login.h"
namespace PAGE_LOGIN_SPACE
{





// 标明模块
static const string THIS_MODULE = "login";



Page_Login::Page_Login()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Page_Login::~Page_Login()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}


int Page_Login::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    m_html = GlobalConfig::instance()->TemplateDir() + "Login.html";
    return Page::DoInit();
}

// 子类对象创建器
Page *Page_Login::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Page_Login;
}








/******************************** 业务代码 ********************************/



int Page_Login::Deal()
{
    /*
     * 处理注销事件
     */
    const string &logout = m_request->GetField("logout");
    if("" != logout)
    {
        Submit::Ptr submit( Submit::New("login", "Logout") );
        submit->Deal(this);
        // 输出登录页面
        return Page::Deal();
    }

    /*
     * 若已登录过，直接跳转，不需再验证；
     */
    if( Session::Get(m_request)->CheckLogin(m_request) )
    {
        LOG_DEBUG("User had logined, goto browse page...");

        const string &login = m_request->GetField("login");
        if("" != login)
        {
            /*
             * 注，若页面已登录，用户在登录页点击登
             * 录按钮，将返回不正确的提示。所以这要
             * 返"OK"以使客户端执行跳转；[XXX]
             */
            Page::OutHead();
            m_request->GetConnect()->Send("OK");
            return OK;
        }
        else
        {
            // 不是登录页面，直接转跳；
            const string &username = m_request->GetCurrentUser();
            string page;
            if("administrator" == username)
            {
                page = "administrator";
            }
            else
            {
                page = "browse";
            }
            ChangeTo(page);
        }
        return OK;
    }

    /*
     * 是否为用户提交的登录数据（用户密码等）
     */
    if( m_request->GetField("login") != "")
    {
        string msg = "";

        // 使用try来做跳转，当验证步骤中间有错时，跳过其它操作；
        try
        {
            // 取用户名（不能使用Request::GetCurrentUser()接口，可能这里还未登录，则Session中无记录）
            const string &username = m_request->GetField("username");
            User *user = User::Get(username);
            if( ! user->isValid() )
            {
                LOG_ERROR("Invalid user: [%s]", username.c_str());
                msg = "用户不存在";
                throw 1;
            }

            // 比对密码
            const string &passwd1 = m_request->GetField("password");  // 客户端传来的密码（明文）
            const string &passwd2 = user->GetInfo("password");        // 加后的密码（密文）
            if( ("" != passwd1 || "" != passwd2) && (passwd2 != Crypt(passwd1, CRYPT_VERSION)) )
            {
                LOG_DEBUG("[%s] [%s] [%s]",
                            passwd1.c_str(), passwd2.c_str(), Crypt(passwd1, CRYPT_VERSION).c_str());
                LOG_ERROR("Password error");
                msg = "密码错";
                throw 2;
            }

            const string &ip = m_request->GetConnect()->GetPeerAddr();
            LOG_INFO("Login ok, user:%s, ip:%s", username.c_str(), ip.c_str());

            /*
             * OK，登录成功，记录登录信息；
             */
            const string &id = GetRandomString();
            LOG_DEBUG("Cookie=[%s]", id.c_str());

            {// 当有多人同时登录时，需加锁

                UNIQUE_LOCK( user->GetLock() );

                // 使前个登录session无效，即后登录者比前登录者优先；
                const string &oldid = user->GetInfo("login", "session");
                LOG_DEBUG("old Cookie=[%s]", oldid.c_str());
                Session::Get( oldid )->SetInvalid();

                // 保存当前登录信息
                const string &counter = user->GetInfo("login", "counter");
                user->SetInfo("login", "counter", Inc(counter)); // 增加登录计数
                user->SetInfo("login", "session", id);  /* 记录session的id，后登录
                                                         * 用户将根据此id来使前登
                                                         * 录用户session无效。若需
                                                         * 要登录时间、ip等信息，也
                                                         * 可由此取出，如：
                                                         *   id --> session --> ip
                                                         */
                user->SetInfo("login", "lasttime", NowTime("%Y%m%d%H%M%S")); // 最后登录时间

                // 保存到文件（高性程序不应立即保存，而应由定时器来做 [XXX]）
                if(user->Save() < 0)
                {
                    LOG_ERROR("Save user info error: [%s]", username.c_str());
                    msg = "未知错误[代码:PD20]"; // 此类信息不应给用户看，只需一模糊提示；
                    throw 20;
                }
            }

            // 构造Cookie和跳转头
            const string html = "HTTP/1.1 200 OK\n"
                                "Date: " + GmgTime() + "\n"
                                "Server: NoteSrv v0.6\n"
                                "Set-Cookie: id=" + id + "; path=/;\n"
                                "Keep-Alive: timeout=15, max=100\n"
                                "Content-Type: text/html; charset=GB2312\n"
                                "\n"
                                "OK";

            // 认证通过，记到缓存；
            Session::Set(id, m_request);

            m_request->GetConnect()->Send(html);
            return OK;
        }
        catch(...)
        {
        }

        /*
         * 发送出错提示信息
         */
        ChineseCoding("gb18030", "utf-8").Converter(msg, msg);
        m_request->GetConnect()->Send( msg );
        return OK;
    }

    return Page::Deal();
}





// 定义动态库入口
DefinitinoDllEnter(Page_Login, THIS_MODULE)

}// end of PAGE_DOWNLOAD_SPACE

