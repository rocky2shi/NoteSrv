// Rocky 2010-05-02 18:22:17
#include "Submit.h"
#include "Page_Browse.h"
namespace PAGE_BROWSE_SPACE
{





// ����ģ��
static const string THIS_MODULE = "browse";



Page_Browse::Page_Browse()
{
    FUNCTION_TRACK(); // �����켣����
}

Page_Browse::~Page_Browse()
{
    FUNCTION_TRACK(); // �����켣����
}


int Page_Browse::DoInit()
{
    FUNCTION_TRACK(); // �����켣����
    m_html = GlobalConfig::instance()->TemplateDir() + "Browse.html";
    return Page::DoInit();
}

// ������󴴽���
Page *Page_Browse::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Page_Browse;
}








/******************************** ҵ����� ********************************/




int Page_Browse::OutBody()
{
    FUNCTION_TRACK(); // �����켣����

    assert(NULL != m_request);

    /*
     * ������ǩ�ύ�¼�
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
     * �����ѯ�����ύ�¼�
     */
    const string &save_query = m_request->GetField("save_query");
    if("" != save_query)
    {
        Submit::Ptr submit( Submit::New("browse", "BrowseFilter") );
        submit->Deal(this);
    }

    /*
     * ����ɾ���ύ�¼�
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











// ���嶯̬�����
DefinitinoDllEnter(Page_Browse, THIS_MODULE)

}// end of PAGE_BROWSE_SPACE

