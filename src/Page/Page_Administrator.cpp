// Rocky 2010-05-28 11:59:37
#include "Page_Administrator.h"
namespace PAGE_ADMINISTRATOR_SPACE
{




// ����ģ��
static const string THIS_MODULE = "administrator";




Page_Administrator::Page_Administrator()
{
    FUNCTION_TRACK(); // �����켣����
}

Page_Administrator::~Page_Administrator()
{
    FUNCTION_TRACK(); // �����켣����
}


int Page_Administrator::DoInit()
{
    FUNCTION_TRACK(); // �����켣����
    m_html = GlobalConfig::instance()->TemplateDir() + "Administrator.html";
    return Page::DoInit();
}

// ������󴴽���
Page *Page_Administrator::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Page_Administrator;
}








/******************************** ҵ����� ********************************/




int Page_Administrator::OutBody()
{
    FUNCTION_TRACK(); // �����켣����

    assert(NULL != m_request);

    // ��ҳ��ֻ�������Ա����������й���ԱȨ�� [XXX]��
    if("administrator" != m_request->GetCurrentUser())
    {
        const string &html = "[<a href='javascript:history.back();'>����</a>]"
                           + HtmlAlert("Ȩ�޲��㣬ֻ�й���Ա�ܷ��ʴ�ҳ�档");
        m_request->GetConnect()->Send(html);
        return ERR;
    }

    /*
     * ��־�����ύ�¼�
     */
    const string &log_save = m_request->GetField("log_save");
    if("" != log_save)
    {
        Submit::Ptr submit( Submit::New("administrator", "LogConfigItems") );
        submit->Deal(this);
        m_request->GetConnect()->Send( HtmlAlert("OK") );
        return OK;
    }

    /*
     * ɾ���û��ύ�¼�
     */
    const string &delete_user = m_request->GetField("delete_user");
    if("" != delete_user)
    {
        Submit::Ptr submit( Submit::New("administrator", "DeleteUser") );
        int ret = submit->Deal(this);
        if(ret < 0)
        {
            const string msg = GetResult("msg");
            m_request->GetConnect()->Send(msg);
        }
        else
        {
            m_request->GetConnect()->Send("OK");
        }
        return OK;
    }

    return Page::OutBody();
}











// ���嶯̬�����
DefinitinoDllEnter(Page_Administrator, THIS_MODULE)

}// end of PAGE_ADMINISTRATOR_SPACE
