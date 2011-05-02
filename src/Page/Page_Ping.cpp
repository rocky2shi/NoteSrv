// Rocky 2010-05-29 18:11:31
#include "Session.h"
#include "User.h"
#include "Page_Ping.h"
namespace PAGE_PING_SPACE
{




// ����ģ��
static const string THIS_MODULE = "ping";



Page_Ping::Page_Ping()
{
    FUNCTION_TRACK(); // �����켣����
}

Page_Ping::~Page_Ping()
{
    FUNCTION_TRACK(); // �����켣����
}


int Page_Ping::DoInit()
{
    FUNCTION_TRACK(); // �����켣����
    m_html = "";
    return Page::DoInit();
}

// ������󴴽���
Page *Page_Ping::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Page_Ping;
}








/******************************** ҵ����� ********************************/


// ���������
int Page_Ping::OutBody()
{
    FUNCTION_TRACK(); // �����켣����

    assert(NULL != m_request);

    const string &page = m_request->GetPageName();

    // �Ե�¼ҳ�棬�������ԣ�����OK����ʾ��֤�����)��
    if("login" == page)
    {
        m_request->GetConnect()->Send("OK");
    }
    else if( Session::Get(m_request)->isValid() )
    {
        m_request->GetConnect()->Send("OK");
    }
    else
    {
        /*
         * �鿴�Ƿ���������ʹ�ô��˺��ڱ����¼
         *
         *   ע�⣬�������û��ڱ𴦵�¼��������ʹ
         *   ��m_request->GetCurrentUser()����ȡ��
         *   ������
         */
        const string &username = m_request->GetField("username");
        User *user = User::Get(username);
        const string &id = user->GetInfo("login", "session");
        Session *session = Session::Get( id );
        string msg;

        if( session->isValid() )
        {
            // ȡ�Է���¼ip
            const string &ip = Session::Get( id )->GetIp();
            msg = "���˺��ڱ𴦵�¼���Է�IP: " + ip;
        }
        else
        {
            msg = "δ֪���󣨿����ѳ�ʱ���������µ�¼��";
        }

        m_request->GetConnect()->Send(msg);
    }

    return Page::OutBody();
}








// ���嶯̬�����
DefinitinoDllEnter(Page_Ping, THIS_MODULE)


}// end of PAGE_PING_SPACE

