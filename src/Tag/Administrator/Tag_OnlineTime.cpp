// Rocky 2010-05-30 22:25:07
#include "Tag_OnlineTime.h"
namespace TAG_ONLINETIME_SPACE
{

// 标明模块
//static const string THIS_MODULE = "OnlineTime";


// 程序回载时开如计时
static unsigned long f_boottime = time(NULL);



Tag_OnlineTime::Tag_OnlineTime()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_OnlineTime::Tag_OnlineTime(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_OnlineTime::~Tag_OnlineTime()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Tag_OnlineTime::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Tag::DoInit();
}

// 子类对象创建器
Tag *Tag_OnlineTime::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Tag_OnlineTime;
}








/******************************** 业务代码 ********************************/





// 取标题 [2010-05]
string Tag_OnlineTime::Get(Page *page)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

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

