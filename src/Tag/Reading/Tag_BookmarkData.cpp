// Rocky 2010-05-14 15:06:42
#include "UserData.h"
#include "Page.h"
#include "Tag_BookmarkData.h"
namespace TAG_BOOKMARKDATA_SPACE
{



// 标明模块
static const string THIS_MODULE = "BookmarkData";





Tag_BookmarkData::Tag_BookmarkData()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_BookmarkData::Tag_BookmarkData(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_BookmarkData::~Tag_BookmarkData()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Tag_BookmarkData::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Tag::DoInit();
}

// 子类对象创建器
Tag *Tag_BookmarkData::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Tag_BookmarkData;
}








/******************************** 业务代码 ********************************/





// 取标题 [2010-05]
string Tag_BookmarkData::Get(Page *page)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    const Conf *pack = page->GetCurrentPack();          // 当前key对应数据集
    const string &bookmark = pack->Get("bookmark");
    return TextToWeb( bookmark );
}

















// 设置为全局标记；
static Tag_BookmarkData tmp("reading", THIS_MODULE);

}// end of TAG_BOOKMARKDATA_SPACE
