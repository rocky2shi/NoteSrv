// Rocky 2010-05-18 10:07:56
#include "PageCfg.h"
#include "Page.h"
#include "Tag_BrowseFilterItems.h"
namespace TAG_BROWSEFILTERITEMS_SPACE
{



// 标明模块
//static const string THIS_MODULE = "BrowseFilterItems";




Tag_BrowseFilterItems::Tag_BrowseFilterItems()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_BrowseFilterItems::Tag_BrowseFilterItems(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_BrowseFilterItems::~Tag_BrowseFilterItems()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Tag_BrowseFilterItems::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Tag::DoInit();
}

// 子类对象创建器
Tag *Tag_BrowseFilterItems::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Tag_BrowseFilterItems;
}








/******************************** 业务代码 ********************************/





// 取标题 [2010-05]
string Tag_BrowseFilterItems::Get(Page *page)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    const string &filter = "filter";
    PageCfg * const cfg = page->GetCurrentPageCfg();

    /*
     * 时间过滤项
     */
    if("timebeginchk" == m_id)
    {
        return "" == cfg->Get(filter, "timebeginchk") ? "" : "checked";
    }
    else if("timeendchk" == m_id)
    {
        return "" == cfg->Get(filter, "timeendchk") ? "" : "checked";
    }
    else if("qtbegin" == m_id)
    {
        return cfg->Get(filter, "qtbegin");
    }
    else if("qtend" == m_id)
    {
        return cfg->Get(filter, "qtend");
    }
    else if("chk_time_type_create" == m_id)
    {
        const string &value = cfg->Get(filter, "time_type");
        return "create" == value ? "checked" : "";
    }
    else if("chk_time_type_modify" == m_id)
    {
        const string &value = cfg->Get(filter, "time_type");
        return "modify" == value ? "checked" : "";
    }

    /*
     * 关键字过滤项
     */
    else if("query_keyword" == m_id)
    {
        return cfg->Get(filter, "query_keyword");
    }
    else if("chk_title" == m_id)
    {
        const string &query_scope = cfg->Get(filter, "query_scope");
        return "title" == query_scope ? "checked" : "";
    }
    else if("chk_content" == m_id)
    {
        const string &query_scope = cfg->Get(filter, "query_scope");
        return "content" == query_scope ? "checked" : "";
    }
    else if("chk_case_ignore" == m_id) // 当0时，为区分大小写（注意，页面的命名有点易乱）
    {
        const string &value = cfg->Get(filter, "query_case_ignore");
        return "0" == value ? "checked" : "";
    }
    else if("chk_relation_and" == m_id)
    {
        const string &value = cfg->Get(filter, "query_relation");
        return "and" == value ? "checked" : "";
    }
    else if("chk_relation_or" == m_id)
    {
        const string &value = cfg->Get(filter, "query_relation");
        return "or" == value ? "checked" : "";
    }

    /*
     * 类型
     */
    // 由菜单类处理。。。
    //

    /*
     * 结果显示窗口
     *   _blank: 过滤结果在新窗口打开
     *   _self : 过滤结果在当前窗口打开
     */
    else if("chk_blank" == m_id)
    {
        const string &result_win = cfg->Get(filter, "result_win");
        return "_blank" == result_win ? "checked" : "";
    }
    else if("chk_self" == m_id)
    {
        const string &result_win = cfg->Get(filter, "result_win");
        return "_self" == result_win ? "checked" : "";
    }
    else if("result_win" == m_id)
    {
        return cfg->Get(filter, "result_win");
    }

    return "";
}

















// 处理多个项
static Tag_BrowseFilterItems tmp1("browse", "timebeginchk");
static Tag_BrowseFilterItems tmp2("browse", "timeendchk");
static Tag_BrowseFilterItems tmp3("browse", "qtbegin");
static Tag_BrowseFilterItems tmp4("browse", "qtend");
static Tag_BrowseFilterItems tmp5("browse", "query_keyword");
static Tag_BrowseFilterItems tmp6("browse", "chk_title");
static Tag_BrowseFilterItems tmp7("browse", "chk_content");
static Tag_BrowseFilterItems tmp9("browse", "chk_blank");
static Tag_BrowseFilterItems tmp10("browse", "chk_self");
static Tag_BrowseFilterItems tmp11("browse", "result_win");
static Tag_BrowseFilterItems tmp12("browse", "chk_case_ignore");
static Tag_BrowseFilterItems tmp13("browse", "chk_relation_and");
static Tag_BrowseFilterItems tmp14("browse", "chk_relation_or");
static Tag_BrowseFilterItems tmp15("browse", "chk_time_type_create");
static Tag_BrowseFilterItems tmp16("browse", "chk_time_type_modify");

}// end of TAG_BROWSEFILTERITEMS_SPACE
