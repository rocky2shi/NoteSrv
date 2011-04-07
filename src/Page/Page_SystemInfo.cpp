// Rocky 2010-06-04 19:43:32
#include "Page_SystemInfo.h"
namespace PAGE_SYSTEMINFO_SPACE
{





// 标明模块
static const string THIS_MODULE = "SystemInfo";



Page_SystemInfo::Page_SystemInfo()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Page_SystemInfo::~Page_SystemInfo()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}


int Page_SystemInfo::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return Page::DoInit();
}

// 子类对象创建器
Page *Page_SystemInfo::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Page_SystemInfo;
}








/******************************** 业务代码 ********************************/


// 输出数据体
int Page_SystemInfo::Deal()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    assert(NULL != m_request);

    Connect * const connect = m_request->GetConnect();
    const string &field = m_request->GetField("field");

    // 返回cpu使用率
    if("CpuUsed" == field)
    {
        const string html = IntToString( GetCpuUsed() );
        connect->Send(html);
    }

    return OK;
}













// 定义动态库入口
DefinitinoDllEnter(Page_SystemInfo, THIS_MODULE)

}// end of PAGE_SYSTEMINFO_SPACE
