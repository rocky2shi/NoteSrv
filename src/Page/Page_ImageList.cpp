// Rocky 2010-05-09 01:47:16
#include "User.h"
#include "GetFileList.h"
#include "Page_ImageList.h"
namespace PAGE_IMAGELIST_SPACE
{









// 标明模块
static const string THIS_MODULE = "ImageList";



Page_ImageList::Page_ImageList()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Page_ImageList::~Page_ImageList()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}


int Page_ImageList::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    m_html = GlobalConfig::instance()->TemplateDir() + "ImageList.html";
    return Page::DoInit();
}

// 子类对象创建器
Page *Page_ImageList::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Page_ImageList;
}








/******************************** 业务代码 ********************************/


// 输出数据体
int Page_ImageList::Deal()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    assert(NULL != m_request);

    return Page::Deal();
}








// 定义动态库入口
DefinitinoDllEnter(Page_ImageList, THIS_MODULE)

}// end of PAGE_IMAGELIST_SPACE

