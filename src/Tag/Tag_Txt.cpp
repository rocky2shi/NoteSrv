// Rocky 2010-04-22 14:54:28
#include "Tag_Txt.h"
namespace TAG_TXT_SPACE
{




// ����ģ��
static const string THIS_MODULE = "";





Tag_Txt::Tag_Txt()
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_Txt::Tag_Txt(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_Txt::~Tag_Txt()
{
    FUNCTION_TRACK(); // �����켣����
}




int Tag_Txt::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Tag::DoInit();
}

// ������󴴽���
Tag *Tag_Txt::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Tag_Txt;
}








/******************************** ҵ����� ********************************/





// ȡ��Ƕ��������
string Tag_Txt::Get(Page *page)
{
    FUNCTION_TRACK(); // �����켣����
    return m_tag;
}

















// �ı�ֱ�������
static Tag_Txt tmp(THIS_MODULE, "TXT");

}// end of TAG_TXT_SPACE
