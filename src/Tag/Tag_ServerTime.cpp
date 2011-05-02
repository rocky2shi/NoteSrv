// Rocky 2010-04-25 22:56:43
#include "Tag_ServerTime.h"
namespace TAG_SERVERTIME_SPACE
{


// ����ģ��
static const string THIS_MODULE = "ServerTime";





Tag_ServerTime::Tag_ServerTime()
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_ServerTime::Tag_ServerTime(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_ServerTime::~Tag_ServerTime()
{
    FUNCTION_TRACK(); // �����켣����
}




int Tag_ServerTime::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Tag::DoInit();
}

// ������󴴽���
Tag *Tag_ServerTime::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Tag_ServerTime;
}








/******************************** ҵ����� ********************************/





// ȡ��Ƕ��������
string Tag_ServerTime::Get(Page *page)
{
    FUNCTION_TRACK(); // �����켣����
    char buf[64] = "";
    sprintf(buf, "%ld", time(NULL));
    return buf;
}

















// ����Ϊȫ�ֱ�ǣ�
static Tag_ServerTime tmp(TAG_GLOBAL, THIS_MODULE);


}// end of TAG_SERVERTIME_SPACE

