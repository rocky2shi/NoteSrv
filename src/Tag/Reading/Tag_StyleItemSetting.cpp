// Rocky 2010-05-14 15:39:34
#include "UserData.h"
#include "Page.h"
#include "Tag_StyleItemSetting.h"
namespace TAG_STYLEITEMSETTING_SPACE
{


// ����ģ��
//static const string THIS_MODULE = "Tag_StyleItemSetting";





Tag_StyleItemSetting::Tag_StyleItemSetting()
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_StyleItemSetting::Tag_StyleItemSetting(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_StyleItemSetting::~Tag_StyleItemSetting()
{
    FUNCTION_TRACK(); // �����켣����
}




int Tag_StyleItemSetting::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Tag::DoInit();
}

// ������󴴽���
Tag *Tag_StyleItemSetting::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Tag_StyleItemSetting;
}








/******************************** ҵ����� ********************************/





// ȡ���� [2010-05]
string Tag_StyleItemSetting::Get(Page *page)
{
    FUNCTION_TRACK(); // �����켣����

    const Conf *pack = page->GetCurrentPack();          // ��ǰkey��Ӧ���ݼ�
    const string &text = pack->Get("text");             // ȡ����������

    // �Ƿ�Ϊ���ѡ��
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

    // �Ƿ�Ϊ�ֺ�
    else if("font_size" == m_id)
    {
        const string &font_size = EmptyStringToDefault(pack->Get("font_size"), "18");
        return font_size;
    }

    // �Ƿ�Ϊ��ɫ
    else if("font_color" == m_id)
    {
        const string &font_color = EmptyStringToDefault(pack->Get("font_color"), "#FFFFFF");
        return font_color;
    }

    // �Ƿ�Ϊ����
    else if("font_bgcolor" == m_id)
    {
        const string &font_bgcolor = EmptyStringToDefault(pack->Get("font_bgcolor"), "#000000");
        return font_bgcolor;
    }

    // �Ƿ�Ϊ����
    else if("font_family" == m_id)
    {
        const string &font_family = EmptyStringToDefault(pack->Get("font_family"), "����_GB2312");
        return font_family;
    }

    return "";
}

















// ��������
static Tag_StyleItemSetting tmp1("reading", "chk_font_weight");
static Tag_StyleItemSetting tmp2("reading", "chk_font_style");
static Tag_StyleItemSetting tmp3("reading", "chk_font_text_decoration");
static Tag_StyleItemSetting tmp4("reading", "font_size");
static Tag_StyleItemSetting tmp5("reading", "font_color");
static Tag_StyleItemSetting tmp6("reading", "font_bgcolor");
static Tag_StyleItemSetting tmp7("reading", "font_family");

}// end of TAG_STYLEITEMSETTING_SPACE

