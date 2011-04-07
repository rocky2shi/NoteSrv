// Rocky 2010-05-13 11:46:20
#include "Page.h"
#include "Tag_CurrentPassword.h"
namespace TAG_CURRENTPASSWORD_SPACE
{





// 标明模块
static const string THIS_MODULE = "CurrentPassword";





Tag_CurrentPassword::Tag_CurrentPassword()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_CurrentPassword::Tag_CurrentPassword(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_CurrentPassword::~Tag_CurrentPassword()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Tag_CurrentPassword::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Tag::DoInit();
}

// 子类对象创建器
Tag *Tag_CurrentPassword::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Tag_CurrentPassword;
}








/******************************** 业务代码 ********************************/





//
string Tag_CurrentPassword::Get(Page *page)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    // password字段来自“此条信息已加密，查看请输入密码”窗口
    const string &password = page->GetRequest()->GetField("password");
    const string &decryption = page->GetRequest()->GetField("decryption");

    LOG_DEBUG("[%s] [%s]", decryption.c_str(), password.c_str());

    if("" != decryption)
    {
        return password;
    }

    return "";
}

















// 设置为全局标记；
static Tag_CurrentPassword tmp(TAG_GLOBAL, THIS_MODULE);

}// end of TAG_CURRENTPASSWORD_SPACE
