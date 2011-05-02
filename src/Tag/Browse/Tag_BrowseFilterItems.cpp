// Rocky 2010-05-18 10:07:56
#include "PageCfg.h"
#include "Page.h"
#include "Tag_BrowseFilterItems.h"
namespace TAG_BROWSEFILTERITEMS_SPACE
{



// ����ģ��
//static const string THIS_MODULE = "BrowseFilterItems";




Tag_BrowseFilterItems::Tag_BrowseFilterItems()
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_BrowseFilterItems::Tag_BrowseFilterItems(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_BrowseFilterItems::~Tag_BrowseFilterItems()
{
    FUNCTION_TRACK(); // �����켣����
}




int Tag_BrowseFilterItems::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Tag::DoInit();
}

// ������󴴽���
Tag *Tag_BrowseFilterItems::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Tag_BrowseFilterItems;
}








/******************************** ҵ����� ********************************/





// ȡ���� [2010-05]
string Tag_BrowseFilterItems::Get(Page *page)
{
    FUNCTION_TRACK(); // �����켣����

    const string &filter = "filter";
    PageCfg * const cfg = page->GetCurrentPageCfg();

    /*
     * ʱ�������
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

    /*
     * �ؼ��ֹ�����
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
    else if("chk_all" == m_id)
    {
        const string &query_scope = cfg->Get(filter, "query_scope");
        return "all" == query_scope ? "checked" : "";
    }

    /*
     * ����
     */
    // �ɲ˵��ദ������
    //

    /*
     * �����ʾ����
     *   _blank: ���˽�����´��ڴ�
     *   _self : ���˽���ڵ�ǰ���ڴ�
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

















// ��������
static Tag_BrowseFilterItems tmp1("browse", "timebeginchk");
static Tag_BrowseFilterItems tmp2("browse", "timeendchk");
static Tag_BrowseFilterItems tmp3("browse", "qtbegin");
static Tag_BrowseFilterItems tmp4("browse", "qtend");
static Tag_BrowseFilterItems tmp5("browse", "query_keyword");
static Tag_BrowseFilterItems tmp6("browse", "chk_title");
static Tag_BrowseFilterItems tmp7("browse", "chk_content");
static Tag_BrowseFilterItems tmp8("browse", "chk_all");
static Tag_BrowseFilterItems tmp9("browse", "chk_blank");
static Tag_BrowseFilterItems tmp10("browse", "chk_self");
static Tag_BrowseFilterItems tmp11("browse", "result_win");

}// end of TAG_BROWSEFILTERITEMS_SPACE
