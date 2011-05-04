// Rocky 2010-06-11 17:57:44
#include "Tag_TextTextarea.h"
namespace TAG_TEXTTEXTAREA_SPACE
{



// 标明模块
static const string THIS_MODULE = "Text";





Tag_TextTextarea::Tag_TextTextarea()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_TextTextarea::Tag_TextTextarea(const string &page, const string &tag) : Tag_Text(page, tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_TextTextarea::~Tag_TextTextarea()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Tag_TextTextarea::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Tag::DoInit();
}

// 子类对象创建器
Tag *Tag_TextTextarea::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Tag_TextTextarea;
}








/******************************** 业务代码 ********************************/




string Tag_TextTextarea::Get(Page *page)
{
    return Tag_Text::Get(page);
}


// 转换函数
const string Tag_TextTextarea::ThisTextToWeb(const string &txt) const
{
    return ::TextToWebEdit(txt);
}

















static Tag_TextTextarea tmp("edit", THIS_MODULE);

}// end of TAG_TEXTTEXTAREA_SPACE
