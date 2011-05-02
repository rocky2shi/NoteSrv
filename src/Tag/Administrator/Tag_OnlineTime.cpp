// Rocky 2010-05-30 22:25:07
#include "Tag_OnlineTime.h"
namespace TAG_ONLINETIME_SPACE
{

// ����ģ��
//static const string THIS_MODULE = "OnlineTime";


// �������ʱ�����ʱ
static unsigned long f_boottime = time(NULL);



Tag_OnlineTime::Tag_OnlineTime()
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_OnlineTime::Tag_OnlineTime(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_OnlineTime::~Tag_OnlineTime()
{
    FUNCTION_TRACK(); // �����켣����
}




int Tag_OnlineTime::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Tag::DoInit();
}

// ������󴴽���
Tag *Tag_OnlineTime::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Tag_OnlineTime;
}








/******************************** ҵ����� ********************************/





// ȡ���� [2010-05]
string Tag_OnlineTime::Get(Page *page)
{
    FUNCTION_TRACK(); // �����켣����

    if("OnlineTime" == m_id)
    {
        return IntToString(time(NULL) - f_boottime);
    }
    else if("BootTime" == m_id)
    {
        return TimeTo(f_boottime, "%Y-%m-%d %H:%M:%S");
    }

    return "";
}








static Tag_OnlineTime tmp1("administrator", "OnlineTime");
static Tag_OnlineTime tmp2("administrator", "BootTime");

}// end of TAG_ONLINETIME_SPACE

