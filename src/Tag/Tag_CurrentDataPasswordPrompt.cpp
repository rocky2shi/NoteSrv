// Rocky 2010-05-13 16:22:07
#include "UserData.h"
#include "Page.h"
#include "Tag_CurrentDataPasswordPrompt.h"
namespace TAG_CURRENTDATAPASSWORDPROMPT_SPACE
{




// 标明模块
static const string THIS_MODULE = "CurrentDataPasswordPrompt";





Tag_CurrentDataPasswordPrompt::Tag_CurrentDataPasswordPrompt()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_CurrentDataPasswordPrompt::Tag_CurrentDataPasswordPrompt(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_CurrentDataPasswordPrompt::~Tag_CurrentDataPasswordPrompt()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Tag_CurrentDataPasswordPrompt::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Tag::DoInit();
}

// 子类对象创建器
Tag *Tag_CurrentDataPasswordPrompt::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Tag_CurrentDataPasswordPrompt;
}








/******************************** 业务代码 ********************************/





// 取标题 [2010-05]
string Tag_CurrentDataPasswordPrompt::Get(Page *page)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    const Conf *pack = page->GetCurrentPack();          // 当前key对应数据集
    return pack->Get("prompt");
}

















// 设置为全局标记；
static Tag_CurrentDataPasswordPrompt tmp(TAG_GLOBAL, THIS_MODULE);

}// end of TAG_CURRENTDATAPASSWORDPROMPT_SPACE
