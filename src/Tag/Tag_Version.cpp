// Rocky 2010-06-03 09:52:30
#include "Version.h"
#include "Tag_Version.h"
namespace TAG_VERSION_SPACE
{






// ����ģ��
static const string THIS_MODULE = "Version";





Tag_Version::Tag_Version()
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_Version::Tag_Version(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_Version::~Tag_Version()
{
    FUNCTION_TRACK(); // �����켣����
}




int Tag_Version::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Tag::DoInit();
}

// ������󴴽���
Tag *Tag_Version::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Tag_Version;
}








/******************************** ҵ����� ********************************/





// ȡ��Ƕ��������
string Tag_Version::Get(Page *page)
{
    FUNCTION_TRACK(); // �����켣����
    return VV;
}

















// ����Ϊȫ�ֱ�ǣ�
static Tag_Version tmp(TAG_GLOBAL, THIS_MODULE);

}// end of TAG_VERSION_SPACE
