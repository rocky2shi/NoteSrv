// Rocky 2010-05-28 14:17:38
#include "Tag_LogConfigItems.h"
namespace TAG_LOGCONFIGITEMS_SPACE
{

// ����ģ��
//static const string THIS_MODULE = "";




Tag_LogConfigItems::Tag_LogConfigItems()
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_LogConfigItems::Tag_LogConfigItems(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_LogConfigItems::~Tag_LogConfigItems()
{
    FUNCTION_TRACK(); // �����켣����
}




int Tag_LogConfigItems::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Tag::DoInit();
}

// ������󴴽���
Tag *Tag_LogConfigItems::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Tag_LogConfigItems;
}








/******************************** ҵ����� ********************************/





// ȡ���� [2010-05]
string Tag_LogConfigItems::Get(Page *page)
{
    FUNCTION_TRACK(); // �����켣����

    // ������־
    if("debug_open" == m_id)
    {
        return Log::instance()->GetLevel(Log::DEBUG) ? "checked" : "";
    }
    else if("debug_close" == m_id)
    {
        return Log::instance()->GetLevel(Log::DEBUG) ? "" : "checked";
    }
    // һ����־
    else if("info_open" == m_id)
    {
        return Log::instance()->GetLevel(Log::INFO) ? "checked" : "";
    }
    else if("info_close" == m_id)
    {
        return Log::instance()->GetLevel(Log::INFO) ? "" : "checked";
    }
    // ������־
    else if("error_open" == m_id)
    {
        return Log::instance()->GetLevel(Log::ERROR) ? "checked" : "";
    }
    else if("error_close" == m_id)
    {
        return Log::instance()->GetLevel(Log::ERROR) ? "" : "checked";
    }

    return "";
}








// ��������
static Tag_LogConfigItems tmp1("administrator", "debug_open");
static Tag_LogConfigItems tmp2("administrator", "debug_close");
static Tag_LogConfigItems tmp3("administrator", "info_open");
static Tag_LogConfigItems tmp4("administrator", "info_close");
static Tag_LogConfigItems tmp5("administrator", "error_open");
static Tag_LogConfigItems tmp6("administrator", "error_close");

}// end of TAG_LOGCONFIGITEMS_SPACE
