// Rocky 2010-05-06 23:29:00
#include "Page.h"
#include "User.h"
#include "Tag_EncryptSetting.h"
namespace TAG_ENCRYPTSETTING_SPACE
{








// 标明模块
static const string THIS_MODULE = "EncryptSetting";





Tag_EncryptSetting::Tag_EncryptSetting()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_EncryptSetting::Tag_EncryptSetting(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_EncryptSetting::~Tag_EncryptSetting()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Tag_EncryptSetting::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Tag::DoInit();
}

// 子类对象创建器
Tag *Tag_EncryptSetting::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Tag_EncryptSetting;
}








/******************************** 业务代码 ********************************/





// 取标题 [2010-05]
string Tag_EncryptSetting::Get(Page *page)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return "";
}

















// 设置为全局标记；
static Tag_EncryptSetting tmp(TAG_GLOBAL, THIS_MODULE);

}// end of TAG_ENCRYPTSETTING_SPACE

