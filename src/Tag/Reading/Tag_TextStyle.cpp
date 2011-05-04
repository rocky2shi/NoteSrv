// Rocky 2010-05-14 14:25:03
#include "UserData.h"
#include "Page.h"
#include "Tag_TextStyle.h"
namespace TAG_TEXTSTYLE_SPACE
{


// 标明模块
static const string THIS_MODULE = "TextStyle";





Tag_TextStyle::Tag_TextStyle()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_TextStyle::Tag_TextStyle(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_TextStyle::~Tag_TextStyle()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Tag_TextStyle::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Tag::DoInit();
}

// 子类对象创建器
Tag *Tag_TextStyle::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Tag_TextStyle;
}








/******************************** 业务代码 ********************************/





// 取标题 [2010-05]
string Tag_TextStyle::Get(Page *page)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    const Conf *pack = page->GetCurrentPack();          // 当前key对应数据集
    const string &font_weight = pack->Get("font_weight");
    const string &font_style = pack->Get("font_style");
    const string &font_text_decoration = pack->Get("font_text_decoration");
    const string &font_size = EmptyStringToDefault(pack->Get("font_size"), "18");
    const string &font_color = EmptyStringToDefault(pack->Get("font_color"), "#FFFFFF");
    const string &font_family = EmptyStringToDefault(pack->Get("font_family"), "楷体_GB2312");
    const string &font_bgcolor = EmptyStringToDefault(pack->Get("font_bgcolor"), "#000000");

    string html = "margin: 13px 20px; padding:6px; "
                  "font-weight:" + font_weight + "; "
                  "font-style:" + font_style + "; "
                  "text-decoration:" + font_text_decoration + "; "
                  "font-size:" + font_size + "pt; "
                  "color:" + font_color + "; "
                  "font-family:" + font_family + "; "
                  "background-color:" + font_bgcolor + "; "
                  "";

    return html;
}

















static Tag_TextStyle tmp("reading", THIS_MODULE);

}// end of TAG_TEXTSTYLE_SPACE
