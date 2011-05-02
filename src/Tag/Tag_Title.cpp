// Rocky 2010-05-04 22:12:48
#include "Page.h"
#include "UserData.h"
#include "Tag_Title.h"
namespace TAG_TITLE_SPACE
{


// ����ģ��
static const string THIS_MODULE = "Title";





Tag_Title::Tag_Title()
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_Title::Tag_Title(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_Title::~Tag_Title()
{
    FUNCTION_TRACK(); // �����켣����
}




int Tag_Title::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Tag::DoInit();
}

// ������󴴽���
Tag *Tag_Title::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Tag_Title;
}








/******************************** ҵ����� ********************************/





// ȡ���� [2010-05]
string Tag_Title::Get(Page *page)
{
    FUNCTION_TRACK(); // �����켣����

    const Conf *pack = page->GetCurrentPack();          // ��ǰkey��Ӧ���ݼ�
    const string &title = pack->Get("title");

    return TextToWeb( title );
}

















// ����Ϊȫ�ֱ�ǣ�
static Tag_Title tmp(TAG_GLOBAL, THIS_MODULE);

}// end of TAG_TITLE_SPACE

