// Rocky 2010-05-02 18:58:00
#include "Page.h"
#include "Tag_CurrentUser.h"
namespace TAG_CURRENTUSER_SPACE
{



// ����ģ��
static const string THIS_MODULE = "CurrentUser";





Tag_CurrentUser::Tag_CurrentUser()
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_CurrentUser::Tag_CurrentUser(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_CurrentUser::~Tag_CurrentUser()
{
    FUNCTION_TRACK(); // �����켣����
}




int Tag_CurrentUser::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Tag::DoInit();
}

// ������󴴽���
Tag *Tag_CurrentUser::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Tag_CurrentUser;
}








/******************************** ҵ����� ********************************/





// ȡ��ǰ��¼�û���
string Tag_CurrentUser::Get(Page *page)
{
    FUNCTION_TRACK(); // �����켣����

    return page->GetRequest()->GetCurrentUser();
}

















// ����Ϊȫ�ֱ�ǣ�
static Tag_CurrentUser tmp(TAG_GLOBAL, THIS_MODULE);


}// end of TAG_CURRENTUSER_SPACE

