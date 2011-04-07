// Rocky 2010-05-14 15:39:34
#include "UserData.h"
#include "Page.h"
#include "Tag_StyleItemSetting.h"
namespace TAG_STYLEITEMSETTING_SPACE
{


// 标明模块
//static const string THIS_MODULE = "Tag_StyleItemSetting";





Tag_StyleItemSetting::Tag_StyleItemSetting()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_StyleItemSetting::Tag_StyleItemSetting(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_StyleItemSetting::~Tag_StyleItemSetting()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Tag_StyleItemSetting::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Tag::DoInit();
}

// 子类对象创建器
Tag *Tag_StyleItemSetting::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Tag_StyleItemSetting;
}








/******************************** 业务代码 ********************************/





// 取标题 [2010-05]
string Tag_StyleItemSetting::Get(Page *page)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    const Conf *pack = page->GetCurrentPack();          // 当前key对应数据集
    const string &text = pack->Get("text");             // 取出正文数据

    // 是否为风格单选顶
    if("chk_font_weight" == m_id)
    {
        const string &font_weight = pack->Get("font_weight");
        return "" == font_weight ? "" : "checked";
    }
    else if("chk_font_style" == m_id)
    {
        const string &font_style = pack->Get("font_style");
        return "" == font_style ? "" : "checked";
    }
    else if("chk_font_text_decoration" == m_id)
    {
        const string &font_text_decoration = pack->Get("font_text_decoration");
        return "" == font_text_decoration ? "" : "checked";
    }

    // 是否为字号
    else if("font_size" == m_id)
    {
        const string &font_size = EmptyStringToDefault(pack->Get("font_size"), "18");
        return font_size;
    }

    // 是否为颜色
    else if("font_color" == m_id)
    {
        const string &font_color = EmptyStringToDefault(pack->Get("font_color"), "#FFFFFF");
        return font_color;
    }

    // 是否为背景
    else if("font_bgcolor" == m_id)
    {
        const string &font_bgcolor = EmptyStringToDefault(pack->Get("font_bgcolor"), "#000000");
        return font_bgcolor;
    }

    // 是否为字体
    else if("font_family" == m_id)
    {
        const string &font_family = EmptyStringToDefault(pack->Get("font_family"), "楷体_GB2312");
        return font_family;
    }

    return "";
}

















// 处理多个项
static Tag_StyleItemSetting tmp1("reading", "chk_font_weight");
static Tag_StyleItemSetting tmp2("reading", "chk_font_style");
static Tag_StyleItemSetting tmp3("reading", "chk_font_text_decoration");
static Tag_StyleItemSetting tmp4("reading", "font_size");
static Tag_StyleItemSetting tmp5("reading", "font_color");
static Tag_StyleItemSetting tmp6("reading", "font_bgcolor");
static Tag_StyleItemSetting tmp7("reading", "font_family");

}// end of TAG_STYLEITEMSETTING_SPACE

