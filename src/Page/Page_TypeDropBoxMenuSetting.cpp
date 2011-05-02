// Rocky 2010-05-24 19:00:18
#include "Page_TypeDropBoxMenuSetting.h"
namespace PAGE_TYPEDROPBOXMENUSETTING_SPACE
{


// ����ģ��
static const string THIS_MODULE = "TypeDropBoxMenuSetting";



Page_TypeDropBoxMenuSetting::Page_TypeDropBoxMenuSetting()
{
    FUNCTION_TRACK(); // �����켣����
}

Page_TypeDropBoxMenuSetting::~Page_TypeDropBoxMenuSetting()
{
    FUNCTION_TRACK(); // �����켣����
}


int Page_TypeDropBoxMenuSetting::DoInit()
{
    FUNCTION_TRACK(); // �����켣����
    m_html = GlobalConfig::instance()->TemplateDir() + "TypeDropBoxMenuSetting.html";
    return Page::DoInit();
}

// ������󴴽���
Page *Page_TypeDropBoxMenuSetting::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Page_TypeDropBoxMenuSetting;
}








/******************************** ҵ����� ********************************/


// Rocky 2010-05-24 20:42:42
int Page_TypeDropBoxMenuSetting::OutBody()
{
    /*
     * �޸����������˵����ύ�¼�
     */
    const string &submit_modify = m_request->GetField("submit_modify");
    if("" != submit_modify)
    {
        Submit::Ptr submit( Submit::New("TypeDropBoxMenuSetting", "ModifyTypeDropBoxMenu") );
        submit->Deal(this);
    }

    /*
     * �������������˵����ύ�¼�
     */
    const string &submit_new = m_request->GetField("submit_new");
    if("" != submit_new)
    {
        Submit::Ptr submit( Submit::New("TypeDropBoxMenuSetting", "AddTypeDropBoxMenu") );
        submit->Deal(this);
    }
    LOG_DEBUG("submit_new=[%s]", submit_new.c_str());

    return Page::OutBody();
}






// ���嶯̬�����
DefinitinoDllEnter(Page_TypeDropBoxMenuSetting, THIS_MODULE)

}// end of PAGE_TYPEDROPBOXMENUSETTING_SPACE
