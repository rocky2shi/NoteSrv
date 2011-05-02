// Rocky 2010-06-04 19:43:32
#include "Page_SystemInfo.h"
namespace PAGE_SYSTEMINFO_SPACE
{





// ����ģ��
static const string THIS_MODULE = "SystemInfo";



Page_SystemInfo::Page_SystemInfo()
{
    FUNCTION_TRACK(); // �����켣����
}

Page_SystemInfo::~Page_SystemInfo()
{
    FUNCTION_TRACK(); // �����켣����
}


int Page_SystemInfo::DoInit()
{
    FUNCTION_TRACK(); // �����켣����
    return Page::DoInit();
}

// ������󴴽���
Page *Page_SystemInfo::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Page_SystemInfo;
}








/******************************** ҵ����� ********************************/


// ���������
int Page_SystemInfo::Deal()
{
    FUNCTION_TRACK(); // �����켣����

    assert(NULL != m_request);

    Connect * const connect = m_request->GetConnect();
    const string &field = m_request->GetField("field");

    // ����cpuʹ����
    if("CpuUsed" == field)
    {
        const string html = IntToString( GetCpuUsed() );
        connect->Send(html);
    }

    return OK;
}













// ���嶯̬�����
DefinitinoDllEnter(Page_SystemInfo, THIS_MODULE)

}// end of PAGE_SYSTEMINFO_SPACE
