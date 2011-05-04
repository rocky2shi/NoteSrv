// Rocky 2010-05-17 19:27:28
#include "Page.h"
#include "User.h"
#include "Submit_BrowseInsertBookmark.h"
namespace SUBMIT_BROWSEINSERTBOOKMARK_SPACE
{



// 标明模块（对应于页面中的提交字段）
static const string THIS_MODULE = "BrowseInsertBookmark";





Submit_BrowseInsertBookmark::Submit_BrowseInsertBookmark()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Submit_BrowseInsertBookmark::Submit_BrowseInsertBookmark(const string &page, const string &element)
                        : Submit(page, element)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Submit_BrowseInsertBookmark::~Submit_BrowseInsertBookmark()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Submit_BrowseInsertBookmark::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Submit::DoInit();
}

// 子类对象创建器
Submit *Submit_BrowseInsertBookmark::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Submit_BrowseInsertBookmark;
}





/******************************** 业务代码 ********************************/





int Submit_BrowseInsertBookmark::Deal(Page *page)
{
    const string &username = page->GetRequest()->GetCurrentUser();
    User *user = User::Get(username);
    const string &cfg = user->UserDir() + "browse.txt"; // 当前页面的配制、数据文件 [XXX]

    /*
     * 数据打包
     */
    Ini data(cfg);
    const string &bookmark = page->GetRequest()->GetField("bookmark_data"); // 书签数据
    data.Set("data", "bookmark", bookmark);

    // 保存
    return data.Write();
}














static Submit_BrowseInsertBookmark tmp("browse", THIS_MODULE);

}// end of SUBMIT_BROWSEINSERTBOOKMARK_SPACE
