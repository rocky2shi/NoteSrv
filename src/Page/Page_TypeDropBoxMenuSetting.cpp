// Rocky 2010-05-24 19:00:18
#include "Page_TypeDropBoxMenuSetting.h"
namespace PAGE_TYPEDROPBOXMENUSETTING_SPACE
{


// 标明模块
static const string THIS_MODULE = "TypeDropBoxMenuSetting";



Page_TypeDropBoxMenuSetting::Page_TypeDropBoxMenuSetting()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Page_TypeDropBoxMenuSetting::~Page_TypeDropBoxMenuSetting()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}


int Page_TypeDropBoxMenuSetting::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    m_html = GlobalConfig::instance()->TemplateDir() + "TypeDropBoxMenuSetting.html";
    return Page::DoInit();
}

// 子类对象创建器
Page *Page_TypeDropBoxMenuSetting::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Page_TypeDropBoxMenuSetting;
}








/******************************** 业务代码 ********************************/


// Rocky 2010-05-24 20:42:42
int Page_TypeDropBoxMenuSetting::OutBody()
{
    /*
     * 修改类型下拉菜单项提交事件
     */
    const string &submit_modify = m_request->GetField("submit_modify");
    if("" != submit_modify)
    {
        Submit::Ptr submit( Submit::New("TypeDropBoxMenuSetting", "ModifyTypeDropBoxMenu") );
        int ret = submit->Deal(this);
        if(ret < 0)
        {
            LOG_ERROR("Modify deal error, ret=[%d]", ret);
        }
    }

    /*
     * 新增类型下拉菜单项提交事件
     */
    const string &submit_new = m_request->GetField("submit_new");
    if("" != submit_new)
    {
        Submit::Ptr submit( Submit::New("TypeDropBoxMenuSetting", "AddTypeDropBoxMenu") );
        int ret = submit->Deal(this);
        if(ret < 0)
        {
            LOG_ERROR("Create deal error, ret=[%d]", ret);
        }
    }

    LOG_DEBUG("submit_new=[%s]", submit_new.c_str());

    return Page::OutBody();
}






// 定义动态库入口
DefinitinoDllEnter(Page_TypeDropBoxMenuSetting, THIS_MODULE)

}// end of PAGE_TYPEDROPBOXMENUSETTING_SPACE
