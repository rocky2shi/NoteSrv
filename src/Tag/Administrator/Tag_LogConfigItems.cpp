// Rocky 2010-05-28 14:17:38
#include "Tag_LogConfigItems.h"
namespace TAG_LOGCONFIGITEMS_SPACE
{

// 标明模块
//static const string THIS_MODULE = "";




Tag_LogConfigItems::Tag_LogConfigItems()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_LogConfigItems::Tag_LogConfigItems(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_LogConfigItems::~Tag_LogConfigItems()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Tag_LogConfigItems::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Tag::DoInit();
}

// 子类对象创建器
Tag *Tag_LogConfigItems::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Tag_LogConfigItems;
}








/******************************** 业务代码 ********************************/





// 取标题 [2010-05]
string Tag_LogConfigItems::Get(Page *page)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    // 调试日志
    if("debug_open" == m_id)
    {
        return Log::instance()->GetLevel(Log::DEBUG) ? "checked" : "";
    }
    else if("debug_close" == m_id)
    {
        return Log::instance()->GetLevel(Log::DEBUG) ? "" : "checked";
    }
    // 一般日志
    else if("info_open" == m_id)
    {
        return Log::instance()->GetLevel(Log::INFO) ? "checked" : "";
    }
    else if("info_close" == m_id)
    {
        return Log::instance()->GetLevel(Log::INFO) ? "" : "checked";
    }
    // 错误日志
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








// 处理多个项
static Tag_LogConfigItems tmp1("administrator", "debug_open");
static Tag_LogConfigItems tmp2("administrator", "debug_close");
static Tag_LogConfigItems tmp3("administrator", "info_open");
static Tag_LogConfigItems tmp4("administrator", "info_close");
static Tag_LogConfigItems tmp5("administrator", "error_open");
static Tag_LogConfigItems tmp6("administrator", "error_close");

}// end of TAG_LOGCONFIGITEMS_SPACE
