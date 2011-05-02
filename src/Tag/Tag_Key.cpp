// Rocky 2010-05-04 19:26:12
#include "Page.h"
#include "Tag_Key.h"
namespace TAG_KEY_SPACE
{










// ����ģ��
static const string THIS_MODULE = "Key";





Tag_Key::Tag_Key()
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_Key::Tag_Key(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_Key::~Tag_Key()
{
    FUNCTION_TRACK(); // �����켣����
}




int Tag_Key::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Tag::DoInit();
}

// ������󴴽���
Tag *Tag_Key::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Tag_Key;
}








/******************************** ҵ����� ********************************/





// ȡ��Ƕ��������
string Tag_Key::Get(Page *page)
{
    FUNCTION_TRACK(); // �����켣����
    return page->GetCurrentKey();
}

















// ����Ϊȫ�ֱ�ǣ�
static Tag_Key tmp(TAG_GLOBAL, THIS_MODULE);

}// end of TAG_KEY_SPACE
