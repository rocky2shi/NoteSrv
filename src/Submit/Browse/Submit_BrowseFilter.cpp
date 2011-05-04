// Rocky 2010-05-17 23:32:03
#include "Page.h"
#include "User.h"
#include "PageCfg.h"
#include "Submit_BrowseFilter.h"
namespace SUBMIT_BROWSEFILTER_SPACE
{






// 标明模块（对应于页面中的提交字段）
static const string THIS_MODULE = "BrowseFilter";





Submit_BrowseFilter::Submit_BrowseFilter()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Submit_BrowseFilter::Submit_BrowseFilter(const string &page, const string &element)
                        : Submit(page, element)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Submit_BrowseFilter::~Submit_BrowseFilter()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Submit_BrowseFilter::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Submit::DoInit();
}

// 子类对象创建器
Submit *Submit_BrowseFilter::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Submit_BrowseFilter;
}





/******************************** 业务代码 ********************************/




// 认证通过，返回的["decryption"]为空串，不通过则返回出错信息；
int Submit_BrowseFilter::Deal(Page *page)
{
    const string &filter = "filter";
    PageCfg * const cfg = page->GetCurrentPageCfg();


    // 取出客户端传来的值
    const string &timebeginchk       = page->GetRequest()->GetField("timebeginchk");
    const string &timeendchk         = page->GetRequest()->GetField("timeendchk");
    const string &query_keyword      = page->GetRequest()->GetField("query_keyword");
    const string &query_scope        = page->GetRequest()->GetField("query_scope");
    const string &query_type         = page->GetRequest()->GetField("query_type");
    const string &result_win         = page->GetRequest()->GetField("result_win");
    const string &save_query_setting = page->GetRequest()->GetField("save_query_setting");

    if("1" == page->GetRequest()->GetField("timebeginchk"))
    {
        const string &qtbegin = ""
                              + page->GetRequest()->GetField("qtbegin1")
                              + page->GetRequest()->GetField("qtbegin2")
                              + page->GetRequest()->GetField("qtbegin3")
                              + page->GetRequest()->GetField("qtbegin4")
                              + page->GetRequest()->GetField("qtbegin5")
                              + "00";
        cfg->Set(filter, "qtbegin", qtbegin);
    }

    if("1" == page->GetRequest()->GetField("timeendchk"))
    {
        const string &qtend = ""
                              + page->GetRequest()->GetField("qtend1")
                              + page->GetRequest()->GetField("qtend2")
                              + page->GetRequest()->GetField("qtend3")
                              + page->GetRequest()->GetField("qtend4")
                              + page->GetRequest()->GetField("qtend5")
                              + "00";
        cfg->Set(filter, "qtend", qtend);
    }

    cfg->Set(filter, "timebeginchk",  timebeginchk);
    cfg->Set(filter, "timeendchk",    timeendchk);
    cfg->Set(filter, "query_keyword", query_keyword);
    cfg->Set(filter, "query_scope",   query_scope);
    cfg->Set(filter, "query_type",    query_type);
    cfg->Set(filter, "result_win",    result_win);

    // 是否需要保存
    if("" != save_query_setting)
    {
        return cfg->Save();
    }

    return OK;
}













// 设置为全局标记；
static Submit_BrowseFilter tmp("browse", THIS_MODULE);

}// end of SUBMIT_BROWSEFILTER_SPACE
