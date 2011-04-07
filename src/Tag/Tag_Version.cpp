// Rocky 2010-06-03 09:52:30
#include "Version.h"
#include "Tag_Version.h"
namespace TAG_VERSION_SPACE
{






// 标明模块
static const string THIS_MODULE = "Version";





Tag_Version::Tag_Version()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_Version::Tag_Version(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_Version::~Tag_Version()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Tag_Version::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Tag::DoInit();
}

// 子类对象创建器
Tag *Tag_Version::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Tag_Version;
}








/******************************** 业务代码 ********************************/





// 取标记对象的内容
string Tag_Version::Get(Page *page)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return VV;
}

















// 设置为全局标记；
static Tag_Version tmp(TAG_GLOBAL, THIS_MODULE);

}// end of TAG_VERSION_SPACE
