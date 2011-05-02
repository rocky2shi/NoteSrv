// Rocky 2010-04-17 19:11:05
#include "ChineseCoding.h"
#include "User.h"
#include "Session.h"
#include "Page_Login.h"
namespace PAGE_LOGIN_SPACE
{





// ����ģ��
static const string THIS_MODULE = "login";



Page_Login::Page_Login()
{
    FUNCTION_TRACK(); // �����켣����
}

Page_Login::~Page_Login()
{
    FUNCTION_TRACK(); // �����켣����
}


int Page_Login::DoInit()
{
    FUNCTION_TRACK(); // �����켣����
    m_html = GlobalConfig::instance()->TemplateDir() + "Login.html";
    return Page::DoInit();
}

// ������󴴽���
Page *Page_Login::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Page_Login;
}








/******************************** ҵ����� ********************************/



int Page_Login::Deal()
{
    /*
     * ����ע���¼�
     */
    const string &logout = m_request->GetField("logout");
    if("" != logout)
    {
        Submit::Ptr submit( Submit::New("login", "Logout") );
        submit->Deal(this);
        // �����¼ҳ��
        return Page::Deal();
    }

    /*
     * ���ѵ�¼����ֱ����ת����������֤��
     */
    if( Session::Get(m_request)->CheckLogin(m_request) )
    {
        LOG_DEBUG("User had logined, goto browse page...");

        const string &login = m_request->GetField("login");
        if("" != login)
        {
            /*
             * ע����ҳ���ѵ�¼���û��ڵ�¼ҳ�����
             * ¼��ť�������ز���ȷ����ʾ��������Ҫ
             * ��"OK"��ʹ�ͻ���ִ����ת��[XXX]
             */
            Page::OutHead();
            m_request->GetConnect()->Send("OK");
            return OK;
        }
        else
        {
            // ���ǵ�¼ҳ�棬ֱ��ת����
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
     * �Ƿ�Ϊ�û��ύ�ĵ�¼���ݣ��û�����ȣ�
     */
    if( m_request->GetField("login") != "")
    {
        string msg = "";

        // ʹ��try������ת������֤�����м��д�ʱ����������������
        try
        {
            // ȡ�û���������ʹ��Request::GetCurrentUser()�ӿڣ��������ﻹδ��¼����Session���޼�¼��
            const string &username = m_request->GetField("username");
            User *user = User::Get(username);
            if( ! user->isValid() )
            {
                LOG_ERROR("Invalid user: [%s]", username.c_str());
                msg = "�û�������";
                throw 1;
            }

            // �ȶ�����
            const string &passwd1 = m_request->GetField("password");  // �ͻ��˴��������루���ģ�
            const string &passwd2 = user->GetInfo("password");        // �Ӻ�����루���ģ�
            if( ("" != passwd1 || "" != passwd2) && (passwd2 != Crypt(passwd1, CRYPT_VERSION)) )
            {
                LOG_DEBUG("[%s] [%s] [%s]",
                            passwd1.c_str(), passwd2.c_str(), Crypt(passwd1, CRYPT_VERSION).c_str());
                LOG_ERROR("Password error");
                msg = "�����";
                throw 2;
            }

            const string &ip = m_request->GetConnect()->GetPeerAddr();
            LOG_INFO("Login ok, user:%s, ip:%s", username.c_str(), ip.c_str());

            /*
             * OK����¼�ɹ�����¼��¼��Ϣ��
             */
            const string &id = GetRandomString();
            LOG_DEBUG("Cookie=[%s]", id.c_str());

            {// ���ж���ͬʱ��¼ʱ�������

                UNIQUE_LOCK( user->GetLock() );

                // ʹǰ����¼session��Ч�������¼�߱�ǰ��¼�����ȣ�
                const string &oldid = user->GetInfo("login", "session");
                LOG_DEBUG("old Cookie=[%s]", oldid.c_str());
                Session::Get( oldid )->SetInvalid();

                // ���浱ǰ��¼��Ϣ
                const string &counter = user->GetInfo("login", "counter");
                user->SetInfo("login", "counter", Inc(counter)); // ���ӵ�¼����
                user->SetInfo("login", "session", id);  /* ��¼session��id�����¼
                                                         * �û������ݴ�id��ʹǰ��
                                                         * ¼�û�session��Ч������
                                                         * Ҫ��¼ʱ�䡢ip����Ϣ��Ҳ
                                                         * ���ɴ�ȡ�����磺
                                                         *   id --> session --> ip
                                                         */
                user->SetInfo("login", "lasttime", NowTime("%Y%m%d%H%M%S")); // ����¼ʱ��

                // ���浽�ļ������Գ���Ӧ�������棬��Ӧ�ɶ�ʱ������ [XXX]��
                if(user->Save() < 0)
                {
                    LOG_ERROR("Save user info error: [%s]", username.c_str());
                    msg = "δ֪����[����:PD20]"; // ������Ϣ��Ӧ���û�����ֻ��һģ����ʾ��
                    throw 20;
                }
            }

            // ����Cookie����תͷ
            const string html = "HTTP/1.1 200 OK\n"
                                "Date: " + GmgTime() + "\n"
                                "Server: NoteSrv v0.6\n"
                                "Set-Cookie: id=" + id + "; path=/;\n"
                                "Keep-Alive: timeout=15, max=100\n"
                                "Content-Type: text/html; charset=GB2312\n"
                                "\n"
                                "OK";

            // ��֤ͨ�����ǵ����棻
            Session::Set(id, m_request);

            m_request->GetConnect()->Send(html);
            return OK;
        }
        catch(...)
        {
        }

        /*
         * ���ͳ�����ʾ��Ϣ
         */
        ChineseCoding("gb18030", "utf-8").Converter(msg, msg);
        m_request->GetConnect()->Send( msg );
        return OK;
    }

    return Page::Deal();
}





// ���嶯̬�����
DefinitinoDllEnter(Page_Login, THIS_MODULE)

}// end of PAGE_DOWNLOAD_SPACE

