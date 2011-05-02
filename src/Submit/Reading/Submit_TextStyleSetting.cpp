// Rocky 2010-05-14 16:06:25
#include "Page.h"
#include "UserData.h"
#include "Submit_TextStyleSetting.h"
namespace SUBMIT_TEXTSTYLESETTING_SPACE
{




// ����ģ�飨��Ӧ��ҳ���е��ύ�ֶΣ�
static const string THIS_MODULE = "TextStyleSetting";





Submit_TextStyleSetting::Submit_TextStyleSetting()
{
    FUNCTION_TRACK(); // �����켣����
}

Submit_TextStyleSetting::Submit_TextStyleSetting(const string &page, const string &element)
                        : Submit(page, element)
{
    FUNCTION_TRACK(); // �����켣����
}

Submit_TextStyleSetting::~Submit_TextStyleSetting()
{
    FUNCTION_TRACK(); // �����켣����
}




int Submit_TextStyleSetting::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Submit::DoInit();
}

// ������󴴽���
Submit *Submit_TextStyleSetting::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Submit_TextStyleSetting;
}





/******************************** ҵ����� ********************************/




// ��֤ͨ�������ص�["decryption"]Ϊ�մ�����ͨ���򷵻س�����Ϣ��
int Submit_TextStyleSetting::Deal(Page *page)
{
    const string &key = page->GetCurrentKey();

    // ȡ���ͻ��˴�����ֵ
    const string &font_weight = page->GetRequest()->GetField("font_weight");
    const string &font_style = page->GetRequest()->GetField("font_style");
    const string &font_text_decoration = page->GetRequest()->GetField("font_text_decoration");
    const string &font_size = page->GetRequest()->GetField("font_size");
    const string &font_color = page->GetRequest()->GetField("font_color");
    const string &font_family = page->GetRequest()->GetField("font_family");
    const string &font_bgcolor = page->GetRequest()->GetField("font_bgcolor");

    // д���
    Ini item;
    item.Set(key, "modify", NowTime("%Y%m%d%H%M%S"));
    item.Set(key, "font_weight", font_weight);
    item.Set(key, "font_style", font_style);
    item.Set(key, "font_text_decoration", font_text_decoration);
    item.Set(key, "font_size", font_size);
    item.Set(key, "font_color", font_color);
    item.Set(key, "font_family", font_family);
    item.Set(key, "font_bgcolor", font_bgcolor);
    // ���Ӽ��� [XXX]
    // ...

    // ����
    return page->Save( item );
}













static Submit_TextStyleSetting tmp("reading", THIS_MODULE);

}// end of SUBMIT_TEXTSTYLESETTING_SPACE

