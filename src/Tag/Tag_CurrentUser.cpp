// Rocky 2010-05-02 18:58:00
#include "Page.h"
#include "Tag_CurrentUser.h"
namespace TAG_CURRENTUSER_SPACE
{



// 标明模块
static const string THIS_MODULE = "CurrentUser";





Tag_CurrentUser::Tag_CurrentUser()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_CurrentUser::Tag_CurrentUser(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_CurrentUser::~Tag_CurrentUser()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Tag_CurrentUser::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Tag::DoInit();
}

// 子类对象创建器
Tag *Tag_CurrentUser::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Tag_CurrentUser;
}








/******************************** 业务代码 ********************************/





// 取当前登录用户名
string Tag_CurrentUser::Get(Page *page)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return page->GetRequest()->GetCurrentUser();
}

















// 设置为全局标记；
static Tag_CurrentUser tmp(TAG_GLOBAL, THIS_MODULE);


}// end of TAG_CURRENTUSER_SPACE

