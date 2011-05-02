// Rocky 2010-05-17 23:32:03
#include "Page.h"
#include "User.h"
#include "PageCfg.h"
#include "Submit_BrowseFilter.h"
namespace SUBMIT_BROWSEFILTER_SPACE
{






// ����ģ�飨��Ӧ��ҳ���е��ύ�ֶΣ�
static const string THIS_MODULE = "BrowseFilter";





Submit_BrowseFilter::Submit_BrowseFilter()
{
    FUNCTION_TRACK(); // �����켣����
}

Submit_BrowseFilter::Submit_BrowseFilter(const string &page, const string &element)
                        : Submit(page, element)
{
    FUNCTION_TRACK(); // �����켣����
}

Submit_BrowseFilter::~Submit_BrowseFilter()
{
    FUNCTION_TRACK(); // �����켣����
}




int Submit_BrowseFilter::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Submit::DoInit();
}

// ������󴴽���
Submit *Submit_BrowseFilter::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Submit_BrowseFilter;
}





/******************************** ҵ����� ********************************/




// ��֤ͨ�������ص�["decryption"]Ϊ�մ�����ͨ���򷵻س�����Ϣ��
int Submit_BrowseFilter::Deal(Page *page)
{
    const string &filter = "filter";
    PageCfg * const cfg = page->GetCurrentPageCfg();


    // ȡ���ͻ��˴�����ֵ
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

    // �Ƿ���Ҫ����
    if("" != save_query_setting)
    {
        return cfg->Save();
    }

    return OK;
}













// ����Ϊȫ�ֱ�ǣ�
static Submit_BrowseFilter tmp("browse", THIS_MODULE);

}// end of SUBMIT_BROWSEFILTER_SPACE
