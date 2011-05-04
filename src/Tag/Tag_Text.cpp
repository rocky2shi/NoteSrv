// Rocky 2010-05-03 17:36:28
#include "Tag_Text.h"
#include "Encrypt.h"
#include "Page.h"
#include "UserData.h"
namespace TAG_TEXT_SPACE
{




// 标明模块
static const string THIS_MODULE = "Text";





Tag_Text::Tag_Text()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_Text::Tag_Text(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_Text::~Tag_Text()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Tag_Text::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Tag::DoInit();
}

// 子类对象创建器
Tag *Tag_Text::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Tag_Text;
}








/******************************** 业务代码 ********************************/



// 转换函数
const string Tag_Text::ThisTextToWeb(const string &txt) const
{
    return ::TextToWebHtml(txt);
}


string Tag_Text::Get(Page *page)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    const Conf *pack = page->GetCurrentPack();   // 当前key对应数据集
    const string &text = pack->Get("text");     // 取出正文数据
    const string &password = pack->Get("password");
    const string &decryption = page->GetResult("decryption");

    LOG_DEBUG("[%s] [%s]", password.c_str(), decryption.c_str());

    /* 加密数据是否允许显示（一般情况下，此字段为空，只有的数
     * 据是加密的、且解秘成功时，decryption才被置为"YES"）
     * （注：该值在Submit_Decryption::Deal()类中处理；
     */
    if( "" != password && "YES" != decryption )
    {
        LOG_DEBUG("Data have been encrypted, forbidding them to display.");
        return "";
    }

    // 是加密数据，解密后输出；
    if("" != password)
    {
        // 该key对应数据的密码（注意，是客户端传来的明文密码）
        const string &paswd = page->GetRequest()->GetField("password");
        const string &str = Encrypt(paswd).decrypt( text );
        LOG_DEBUG("Decrypt...");
        return ThisTextToWeb( str );
    }

    LOG_DEBUG("Not need to decrypt...");

    // 未加密数据
    return ThisTextToWeb( text );
}

















// 设置为全局标记；
static Tag_Text tmp(TAG_GLOBAL, THIS_MODULE);

}// end of TAG_TEXT_SPACE

