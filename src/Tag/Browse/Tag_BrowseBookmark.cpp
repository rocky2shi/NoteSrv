// Rocky 2010-05-17 19:47:13
#include "Page.h"
#include "User.h"
#include "Tag_BrowseBookmark.h"
namespace TAG_BROWSEBOOKMARK_SPACE
{






// 标明模块
static const string THIS_MODULE = "BrowseBookmark";





Tag_BrowseBookmark::Tag_BrowseBookmark()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_BrowseBookmark::Tag_BrowseBookmark(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_BrowseBookmark::~Tag_BrowseBookmark()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Tag_BrowseBookmark::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Tag::DoInit();
}

// 子类对象创建器
Tag *Tag_BrowseBookmark::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Tag_BrowseBookmark;
}








/******************************** 业务代码 ********************************/





// 浏览数据（表列）
string Tag_BrowseBookmark::Get(Page *page)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    const string &username = page->GetRequest()->GetCurrentUser();
    User *user = User::Get(username);
    const string &cfg = user->UserDir() + "browse.txt"; // 当前页面的配制、数据文件

    // 取出数据
    Ini data(cfg);

    return data.Get("data", "bookmark");
}

















static Tag_BrowseBookmark tmp("browse", THIS_MODULE);

}// end of TAG_BROWSEBOOKMARK_SPACE
