// Rocky 2010-05-06 23:29:00
#include "Page.h"
#include "User.h"
#include "Tag_EncryptSetting.h"
namespace TAG_ENCRYPTSETTING_SPACE
{








// ����ģ��
static const string THIS_MODULE = "EncryptSetting";





Tag_EncryptSetting::Tag_EncryptSetting()
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_EncryptSetting::Tag_EncryptSetting(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_EncryptSetting::~Tag_EncryptSetting()
{
    FUNCTION_TRACK(); // �����켣����
}




int Tag_EncryptSetting::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Tag::DoInit();
}

// ������󴴽���
Tag *Tag_EncryptSetting::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Tag_EncryptSetting;
}








/******************************** ҵ����� ********************************/





// ȡ���� [2010-05]
string Tag_EncryptSetting::Get(Page *page)
{
    FUNCTION_TRACK(); // �����켣����

    return "";
}

















// ����Ϊȫ�ֱ�ǣ�
static Tag_EncryptSetting tmp(TAG_GLOBAL, THIS_MODULE);

}// end of TAG_ENCRYPTSETTING_SPACE

