// Rocky 2010-05-17 19:27:28
#include "Page.h"
#include "User.h"
#include "Submit_BrowseInsertBookmark.h"
namespace SUBMIT_BROWSEINSERTBOOKMARK_SPACE
{



// ����ģ�飨��Ӧ��ҳ���е��ύ�ֶΣ�
static const string THIS_MODULE = "BrowseInsertBookmark";





Submit_BrowseInsertBookmark::Submit_BrowseInsertBookmark()
{
    FUNCTION_TRACK(); // �����켣����
}

Submit_BrowseInsertBookmark::Submit_BrowseInsertBookmark(const string &page, const string &element)
                        : Submit(page, element)
{
    FUNCTION_TRACK(); // �����켣����
}

Submit_BrowseInsertBookmark::~Submit_BrowseInsertBookmark()
{
    FUNCTION_TRACK(); // �����켣����
}




int Submit_BrowseInsertBookmark::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Submit::DoInit();
}

// ������󴴽���
Submit *Submit_BrowseInsertBookmark::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Submit_BrowseInsertBookmark;
}





/******************************** ҵ����� ********************************/





int Submit_BrowseInsertBookmark::Deal(Page *page)
{
    const string &username = page->GetRequest()->GetCurrentUser();
    User *user = User::Get(username);
    const string &cfg = user->UserDir() + "browse.txt"; // ��ǰҳ������ơ������ļ� [XXX]

    /*
     * ���ݴ��
     */
    Ini data(cfg);
    const string &bookmark = page->GetRequest()->GetField("bookmark_data"); // ��ǩ����
    data.Set("data", "bookmark", bookmark);

    // ����
    return data.Write();
}














static Submit_BrowseInsertBookmark tmp("browse", THIS_MODULE);

}// end of SUBMIT_BROWSEINSERTBOOKMARK_SPACE
