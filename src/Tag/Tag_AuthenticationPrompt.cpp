// Rocky 2010-05-11 17:53:24
#include "Page.h"
#include "UserData.h"
#include "Tag_AuthenticationPrompt.h"
namespace TAG_AUTHENTICATIONPROMPT_SPACE
{





// 标明模块
static const string THIS_MODULE = "AuthenticationPrompt";





Tag_AuthenticationPrompt::Tag_AuthenticationPrompt()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_AuthenticationPrompt::Tag_AuthenticationPrompt(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_AuthenticationPrompt::~Tag_AuthenticationPrompt()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Tag_AuthenticationPrompt::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Tag::DoInit();
}

// 子类对象创建器
Tag *Tag_AuthenticationPrompt::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Tag_AuthenticationPrompt;
}








/******************************** 业务代码 ********************************/





// 取标题 [2010-05]
string Tag_AuthenticationPrompt::Get(Page *page)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    const string &key = page->GetCurrentKey();
    const Conf *pack = page->GetCurrentPack();  // 当前key对应数据集
    const string &text = pack->Get("text");     // 取出正文数据
    const string &password = pack->Get("password");
    const string &prompt = pack->Get("prompt");
    const string &decryption = page->GetResult("decryption");
    const string &auth_msg = page->GetResult("auth_msg");
    string html = "";

    LOG_DEBUG("[%s] [%s]", password.c_str(), decryption.c_str());

    if( "" != password && "YES" != decryption )
    {
        html =  ""
                "<div id='div_pasd' style='display:none;position:absolute;z-index:50;'>\n"
                "<table width=100% align=center>\n"
                "<form method='POST'>\n"
                "<input type='hidden' name='key' value='" + key + "'>\n"
                "    <tr>\n"
                "        <td align='center'>\n"
                "        <table width='311' height='100' border='0' bgcolor='#FFFFFF'>\n"
                "            <tr>\n"
                "                <td height=10 colspan='2'></td>\n"
                "            </tr>\n"
                "            <tr align='center'>\n"
                "                <td colspan='2'>此条信息已加密，查看请输入密码：</td>\n"
                "            </tr>\n"
                "            <tr align='center'>\n"
                "                <td colspan='2'><input name='password' id='password' size='25' class='FOCUS' type='password' autocomplete='off'></td>\n"
                "            </tr>\n"
                "            <tr>\n"
                "                <td height=5 colspan='2'></td>\n"
                "            </tr>\n"
                "            <tr align='center'>\n"
                "                <td align=left>　<span class='font12 red'>" + auth_msg + "</span></td>\n"
                "                <td align=right>\n"
                "                    <input type=submit name=decryption class=button value='确定'>\n"
                "                    <input type=button onClick=alert('" + prompt + "') value='提示'>\n"
                "                    <input type=button class='TABTO_password' onClick=OpenPage('browse') value='返回'>\n"
                "                </td>\n"
                "            </tr>\n"
                "        </table>\n"
                "        </td>\n"
                "    </tr>\n"
                "</form>\n"
                "</table>\n"
                "</div>\n"
                "\n";
    }

    return html;
}

















// 设置为全局标记；
static Tag_AuthenticationPrompt tmp(TAG_GLOBAL, THIS_MODULE);

}// end of TAG_AUTHENTICATIONPROMPT_SPACE
