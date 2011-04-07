// Rocky 2010-05-02 18:22:17
#include "Submit.h"
#include "Page_Browse.h"
namespace PAGE_BROWSE_SPACE
{





// 标明模块
static const string THIS_MODULE = "browse";



Page_Browse::Page_Browse()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Page_Browse::~Page_Browse()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}


int Page_Browse::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    m_html = GlobalConfig::instance()->TemplateDir() + "Browse.html";
    return Page::DoInit();
}

// 子类对象创建器
Page *Page_Browse::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Page_Browse;
}








/******************************** 业务代码 ********************************/




int Page_Browse::OutBody()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    assert(NULL != m_request);

    /*
     * 处理书签提交事件
     */
    const string &bookmark_save = m_request->GetField("bookmark_save");
    if("" != bookmark_save)
    {
        Submit::Ptr submit( Submit::New("browse", "BrowseInsertBookmark") );
        if(submit->Deal(this) < 0)
        {
            m_request->GetConnect()->Send("[ERROR]");
            return OK;
        }
        m_request->GetConnect()->Send("[OK]");
        return OK;
    }

    /*
     * 处理查询过滤提交事件
     */
    const string &save_query = m_request->GetField("save_query");
    if("" != save_query)
    {
        Submit::Ptr submit( Submit::New("browse", "BrowseFilter") );
        submit->Deal(this);
    }

    /*
     * 处理删除提交事件
     */
    const string &delete_data = m_request->GetField("delete_data");
    if("" != delete_data)
    {
        Submit::Ptr submit( Submit::New("browse", "DeleteData") );
        if(submit->Deal(this) < 0)
        {
            m_request->GetConnect()->Send("ERROR");
            return OK;
        }
        m_request->GetConnect()->Send("OK");
        return OK;
    }

    return Page::OutBody();
}











// 定义动态库入口
DefinitinoDllEnter(Page_Browse, THIS_MODULE)

}// end of PAGE_BROWSE_SPACE

