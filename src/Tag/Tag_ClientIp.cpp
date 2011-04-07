// Rocky 2010-05-02 01:12:07
#include "Page.h"
#include "Tag_ClientIp.h"
namespace TAG_CLIENTIP_SPACE
{




// 标明模块
static const string THIS_MODULE = "ClientIp";





Tag_ClientIp::Tag_ClientIp()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_ClientIp::Tag_ClientIp(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_ClientIp::~Tag_ClientIp()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Tag_ClientIp::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Tag::DoInit();
}

// 子类对象创建器
Tag *Tag_ClientIp::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Tag_ClientIp;
}








/******************************** 业务代码 ********************************/





// 取标记对象的内容
string Tag_ClientIp::Get(Page *page)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    Connect * const connect = page->GetRequest()->GetConnect();
    return connect->GetPeerAddr(); // 取出客户端连接ip
}

















// 设置为全局标记；
static Tag_ClientIp tmp(TAG_GLOBAL, THIS_MODULE);

}// end of TAG_CLIENTIP_SPACE

