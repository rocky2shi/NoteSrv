// Rocky 2010-05-23 13:10:46
#include "Page_Register.h"
namespace PAGE_REGISTER_SPACE
{




// ����ģ��
static const string THIS_MODULE = "register";



Page_Register::Page_Register()
{
    FUNCTION_TRACK(); // �����켣����
}

Page_Register::~Page_Register()
{
    FUNCTION_TRACK(); // �����켣����
}


int Page_Register::DoInit()
{
    FUNCTION_TRACK(); // �����켣����
    m_html = GlobalConfig::instance()->TemplateDir() + "Register.html";
    return Page::DoInit();
}

// ������󴴽���
Page *Page_Register::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Page_Register;
}








/******************************** ҵ����� ********************************/


// ���������
int Page_Register::OutBody()
{
    const string &tag = m_request->GetField("tag");

    /*
     * ע���û��ύ�¼�
     */
    if("register" == tag)
    {
        Submit::Ptr submit( Submit::New("register", "RegisterUser") );
        submit->Deal(this);
        return OK;
    }

    /*
     * �޸������ύ�¼�
     */
    if("modify" == tag)
    {
        Submit::Ptr submit( Submit::New("register", "ModifyPassword") );
        submit->Deal(this);
        return OK;
    }

    /*
     * �һ������ύ�¼�
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






// ���嶯̬�����
DefinitinoDllEnter(Page_Register, THIS_MODULE)

}// end of PAGE_REGISTER_SPACE

