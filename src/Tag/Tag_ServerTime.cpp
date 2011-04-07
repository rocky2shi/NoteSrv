// Rocky 2010-04-25 22:56:43
#include "Tag_ServerTime.h"
namespace TAG_SERVERTIME_SPACE
{


// 标明模块
static const string THIS_MODULE = "ServerTime";





Tag_ServerTime::Tag_ServerTime()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_ServerTime::Tag_ServerTime(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_ServerTime::~Tag_ServerTime()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Tag_ServerTime::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Tag::DoInit();
}

// 子类对象创建器
Tag *Tag_ServerTime::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Tag_ServerTime;
}








/******************************** 业务代码 ********************************/





// 取标记对象的内容
string Tag_ServerTime::Get(Page *page)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    char buf[64] = "";
    sprintf(buf, "%ld", time(NULL));
    return buf;
}

















// 设置为全局标记；
static Tag_ServerTime tmp(TAG_GLOBAL, THIS_MODULE);


}// end of TAG_SERVERTIME_SPACE

