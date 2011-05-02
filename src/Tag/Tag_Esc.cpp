// Rocky 2010-05-06 23:19:07
#include "Page.h"
#include "Tag_Esc.h"
namespace TAG_ESC_SPACE
{







// ����ģ��
static const string THIS_MODULE = "Esc";





Tag_Esc::Tag_Esc()
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_Esc::Tag_Esc(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_Esc::~Tag_Esc()
{
    FUNCTION_TRACK(); // �����켣����
}




int Tag_Esc::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Tag::DoInit();
}

// ������󴴽���
Tag *Tag_Esc::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Tag_Esc;
}








/******************************** ҵ����� ********************************/





string Tag_Esc::Get(Page *page)
{
    FUNCTION_TRACK(); // �����켣����

    const Conf *pack = page->GetCurrentPack();   // ��ǰkey��Ӧ���ݼ�
    const string &password = pack->Get("password");
    const string &decryption = page->GetResult("decryption");
    string html;

    LOG_DEBUG("[%s] [%s]", password.c_str(), decryption.c_str());

    if( "" == password || "YES" == decryption )
    {
        html =  "   ShortKey.Unlock();\n"
                "   OverLayer.Close();\n"
                "   ShowToCenter.Close();\n"
                "";
    }
    return html;
}

















// ����Ϊȫ�ֱ�ǣ�
static Tag_Esc tmp(TAG_GLOBAL, THIS_MODULE);

}// end of TAG_ESC_SPACE

