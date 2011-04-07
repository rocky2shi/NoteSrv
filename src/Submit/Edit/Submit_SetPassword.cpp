// Rocky 2010-05-13 16:06:55
#include "Page.h"
#include "UserData.h"
#include "Encrypt.h"
#include "Submit_SetPassword.h"
namespace SUBMIT_SETPASSWORD_SPACE
{





// 标明模块
static const string THIS_MODULE = "SetPassword";





Submit_SetPassword::Submit_SetPassword()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Submit_SetPassword::Submit_SetPassword(const string &page, const string &element)
                        : Submit(page, element)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Submit_SetPassword::~Submit_SetPassword()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Submit_SetPassword::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Submit::DoInit();
}

// 子类对象创建器
Submit *Submit_SetPassword::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Submit_SetPassword;
}





/******************************** 业务代码 ********************************/




// 认证通过，返回的["decryption"]为空串，不通过则返回出错信息；
int Submit_SetPassword::Deal(Page *page)
{
    const string &key = page->GetCurrentKey();
    const Conf *pack = page->GetCurrentPack();          // 当前key对应数据集
    const string &text = pack->Get("text");             // 取出正文数据
    const string &password = pack->Get("password");
    const string &modify = NowTime("%Y%m%d%H%M%S");     // 修改时间为当前时间
    const string &sOldPassword = page->GetRequest()->GetField("msg_pasd_old");      // 原密码
    const string &sNewPassword = page->GetRequest()->GetField("msg_pasd_new");      // 新密码
    const string &msg_pasd_prompt = page->GetRequest()->GetField("msg_pasd_prompt");// 密码提示
    string html = "";
    Ini save;

    /*
     * 验证原密码
     */
    if( "" != password && password != Crypt(sOldPassword, CRYPT_VERSION))
    {
        LOG_ERROR("old password error.");
        return ERR;
    }

    /*
     * 数据打包
     *   若当前数据是加密的，则先解密当前数据；
     */
    const string *pText = &text; // 根据是否需是加密，指向密文或明文；
    string txt;
    if("" != password)
    {
        // 注意，是使用客户端传来的修改前的密码，明文密码。
        txt = Encrypt( sOldPassword ).decrypt( text );
        pText = &txt;
    }

    if("" != sNewPassword)
    {
        // 新密码非空，则加密后放入包中（使用新密码）
        save.Set(key, "text", Encrypt( sNewPassword ).encrypt( *pText ));
        save.Set(key, "password", Crypt(sNewPassword, CRYPT_VERSION));
    }
    else
    {
        save.Set(key, "text", *pText);
        save.Set(key, "password", "");
    }
    save.Set(key, "modify", modify);
    save.Set(key, "prompt", msg_pasd_prompt);

    // 保存
    return page->Save( save );
}













// 设置为全局标记；
static Submit_SetPassword tmp("edit", THIS_MODULE);

}// end of SUBMIT_SETPASSWORD_SPACE

