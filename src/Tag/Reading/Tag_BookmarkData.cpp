// Rocky 2010-05-14 15:06:42
#include "UserData.h"
#include "Page.h"
#include "Tag_BookmarkData.h"
namespace TAG_BOOKMARKDATA_SPACE
{



// ����ģ��
static const string THIS_MODULE = "BookmarkData";





Tag_BookmarkData::Tag_BookmarkData()
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_BookmarkData::Tag_BookmarkData(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_BookmarkData::~Tag_BookmarkData()
{
    FUNCTION_TRACK(); // �����켣����
}




int Tag_BookmarkData::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Tag::DoInit();
}

// ������󴴽���
Tag *Tag_BookmarkData::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Tag_BookmarkData;
}








/******************************** ҵ����� ********************************/





// ȡ���� [2010-05]
string Tag_BookmarkData::Get(Page *page)
{
    FUNCTION_TRACK(); // �����켣����

    const Conf *pack = page->GetCurrentPack();          // ��ǰkey��Ӧ���ݼ�
    const string &bookmark = pack->Get("bookmark");
    return TextToWeb( bookmark );
}

















// ����Ϊȫ�ֱ�ǣ�
static Tag_BookmarkData tmp("reading", THIS_MODULE);

}// end of TAG_BOOKMARKDATA_SPACE
