// Rocky 2010-05-17 19:47:13
#include "Page.h"
#include "User.h"
#include "Tag_BrowseBookmark.h"
namespace TAG_BROWSEBOOKMARK_SPACE
{






// ����ģ��
static const string THIS_MODULE = "BrowseBookmark";





Tag_BrowseBookmark::Tag_BrowseBookmark()
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_BrowseBookmark::Tag_BrowseBookmark(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_BrowseBookmark::~Tag_BrowseBookmark()
{
    FUNCTION_TRACK(); // �����켣����
}




int Tag_BrowseBookmark::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Tag::DoInit();
}

// ������󴴽���
Tag *Tag_BrowseBookmark::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Tag_BrowseBookmark;
}








/******************************** ҵ����� ********************************/





// ������ݣ����У�
string Tag_BrowseBookmark::Get(Page *page)
{
    FUNCTION_TRACK(); // �����켣����

    const string &username = page->GetRequest()->GetCurrentUser();
    User *user = User::Get(username);
    const string &cfg = user->UserDir() + "browse.txt"; // ��ǰҳ������ơ������ļ�

    // ȡ������
    Ini data(cfg);

    return data.Get("data", "bookmark");
}

















static Tag_BrowseBookmark tmp("browse", THIS_MODULE);

}// end of TAG_BROWSEBOOKMARK_SPACE
