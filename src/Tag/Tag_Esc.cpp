// Rocky 2010-05-06 23:19:07
#include "Page.h"
#include "Tag_Esc.h"
namespace TAG_ESC_SPACE
{







// 标明模块
static const string THIS_MODULE = "Esc";





Tag_Esc::Tag_Esc()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_Esc::Tag_Esc(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_Esc::~Tag_Esc()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Tag_Esc::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Tag::DoInit();
}

// 子类对象创建器
Tag *Tag_Esc::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Tag_Esc;
}








/******************************** 业务代码 ********************************/





string Tag_Esc::Get(Page *page)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    const Conf *pack = page->GetCurrentPack();   // 当前key对应数据集
    const string &password = pack->Get("password");
    const string &decryption = page->GetResult("decryption");
    string html;

    LOG_DEBUG("[%s] [%s]", password.c_str(), decryption.c_str());

    if( "" == password || "YES" == decryption )
    {
        html =  "   ShortKey.Unlock();\n"
                "   OverLayer.Close();\n"
                "   ShowToCenter.Close();\n"
                "";
    }
    return html;
}

















// 设置为全局标记；
static Tag_Esc tmp(TAG_GLOBAL, THIS_MODULE);

}// end of TAG_ESC_SPACE

