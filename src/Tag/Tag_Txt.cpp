// Rocky 2010-04-22 14:54:28
#include "Tag_Txt.h"
namespace TAG_TXT_SPACE
{




// 标明模块
static const string THIS_MODULE = "";





Tag_Txt::Tag_Txt()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_Txt::Tag_Txt(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_Txt::~Tag_Txt()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Tag_Txt::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Tag::DoInit();
}

// 子类对象创建器
Tag *Tag_Txt::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Tag_Txt;
}








/******************************** 业务代码 ********************************/





// 取标记对象的内容
string Tag_Txt::Get(Page *page)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return m_tag;
}

















// 文本直接输出；
static Tag_Txt tmp(THIS_MODULE, "TXT");

}// end of TAG_TXT_SPACE
