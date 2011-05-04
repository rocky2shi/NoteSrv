// Rocky 2010-05-04 22:12:48
#include "Page.h"
#include "UserData.h"
#include "Tag_Title.h"
namespace TAG_TITLE_SPACE
{


// 标明模块
static const string THIS_MODULE = "Title";





Tag_Title::Tag_Title()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_Title::Tag_Title(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_Title::~Tag_Title()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Tag_Title::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Tag::DoInit();
}

// 子类对象创建器
Tag *Tag_Title::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Tag_Title;
}








/******************************** 业务代码 ********************************/





// 取标题 [2010-05]
string Tag_Title::Get(Page *page)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    const Conf *pack = page->GetCurrentPack();          // 当前key对应数据集
    const string &title = pack->Get("title");

    return TextToWeb( title );
}

















// 设置为全局标记；
static Tag_Title tmp(TAG_GLOBAL, THIS_MODULE);

}// end of TAG_TITLE_SPACE

