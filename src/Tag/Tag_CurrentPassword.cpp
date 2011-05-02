// Rocky 2010-05-13 11:46:20
#include "Page.h"
#include "Tag_CurrentPassword.h"
namespace TAG_CURRENTPASSWORD_SPACE
{





// ����ģ��
static const string THIS_MODULE = "CurrentPassword";





Tag_CurrentPassword::Tag_CurrentPassword()
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_CurrentPassword::Tag_CurrentPassword(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_CurrentPassword::~Tag_CurrentPassword()
{
    FUNCTION_TRACK(); // �����켣����
}




int Tag_CurrentPassword::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Tag::DoInit();
}

// ������󴴽���
Tag *Tag_CurrentPassword::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Tag_CurrentPassword;
}








/******************************** ҵ����� ********************************/





//
string Tag_CurrentPassword::Get(Page *page)
{
    FUNCTION_TRACK(); // �����켣����

    // password�ֶ����ԡ�������Ϣ�Ѽ��ܣ��鿴���������롱����
    const string &password = page->GetRequest()->GetField("password");
    const string &decryption = page->GetRequest()->GetField("decryption");

    LOG_DEBUG("[%s] [%s]", decryption.c_str(), password.c_str());

    if("" != decryption)
    {
        return password;
    }

    return "";
}

















// ����Ϊȫ�ֱ�ǣ�
static Tag_CurrentPassword tmp(TAG_GLOBAL, THIS_MODULE);

}// end of TAG_CURRENTPASSWORD_SPACE
