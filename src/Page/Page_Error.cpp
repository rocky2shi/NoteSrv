// Rocky 2010-05-07 18:21:57
#include "Page_Error.h"
namespace PAGE_ERROR_SPACE
{










// ����ģ��
static const string THIS_MODULE = "error";



Page_Error::Page_Error()
{
    FUNCTION_TRACK(); // �����켣����
}

Page_Error::~Page_Error()
{
    FUNCTION_TRACK(); // �����켣����
}


int Page_Error::DoInit()
{
    FUNCTION_TRACK(); // �����켣����
    return Page::DoInit();
}

// ������󴴽���
Page *Page_Error::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Page_Error;
}








/******************************** ҵ����� ********************************/

int Page_Error::Deal()
{
    assert(NULL != m_request);

    // ����ʱֱ��ת��¼ҳ
    ChangeTo("login");
    return OK;
}






// ���嶯̬�����
DefinitinoDllEnter(Page_Error, THIS_MODULE)

}// end of PAGE_ERROR_SPACE
