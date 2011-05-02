// Rocky 2010-05-04 19:06:05
#include "UserData.h"
#include "Encrypt.h"
#include "Page_Edit.h"
namespace PAGE_EDIT_SPACE
{






// ����ģ��
static const string THIS_MODULE = "edit";



Page_Edit::Page_Edit()
{
    FUNCTION_TRACK(); // �����켣����
}

Page_Edit::~Page_Edit()
{
    FUNCTION_TRACK(); // �����켣����
}


int Page_Edit::DoInit()
{
    FUNCTION_TRACK(); // �����켣����
    m_html = GlobalConfig::instance()->TemplateDir() + "Edit.html";
    return Page::DoInit();
}

// ������󴴽���
Page *Page_Edit::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Page_Edit;
}








/******************************** ҵ����� ********************************/


int Page_Edit::OutBody()
{
    FUNCTION_TRACK(); // �����켣����

    assert(NULL != m_request);

    /*
     * �Ƿ�Ϊ�����ύ�¼�
     */
    const string &decryption = m_request->GetField("decryption");
    if("" != decryption)
    {
        Submit::Ptr submit( Submit::New(SUBMIT_GLOBAL, "decryption") );
        submit->Deal(this);
    }

    /*
     * �Ƿ�Ϊ���������ύ�¼�
     */
    const string &set_password = m_request->GetField("set_password");
    if("" != set_password)
    {
        // �������������
        Submit::Ptr submit( Submit::New("edit", "SetPassword") );
        submit->Deal(this);
    }

    /*
     * ���ݱ����ύ�¼�
     */
    const string &save = m_request->GetField("save_text");
    if("" != save)
    {
        Submit::Ptr submit( Submit::New("edit", "SaveText") );
        submit->Deal(this);
        return OK;
    }

    // ��ʾ��ǰҳ
    return Page::OutBody();
}











// ���嶯̬�����
DefinitinoDllEnter(Page_Edit, THIS_MODULE)

}// end of PAGE_EDIT_SPACE

