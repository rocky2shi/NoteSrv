// Rocky 2010-05-04 19:26:12
#include "Page.h"
#include "Tag_Key.h"
namespace TAG_KEY_SPACE
{










// 标明模块
static const string THIS_MODULE = "Key";





Tag_Key::Tag_Key()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_Key::Tag_Key(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_Key::~Tag_Key()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Tag_Key::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Tag::DoInit();
}

// 子类对象创建器
Tag *Tag_Key::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Tag_Key;
}








/******************************** 业务代码 ********************************/





// 取标记对象的内容
string Tag_Key::Get(Page *page)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return page->GetCurrentKey();
}

















// 设置为全局标记；
static Tag_Key tmp(TAG_GLOBAL, THIS_MODULE);

}// end of TAG_KEY_SPACE
