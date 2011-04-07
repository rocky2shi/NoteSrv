// Rocky 2010-05-12 00:10:37
#include "Page.h"
#include "UserData.h"
#include "Submit_Decryption.h"
namespace SUBMIT_DECRYPTION_SPACE
{


// 标明模块（对应于页面中的提交字段）
static const string THIS_MODULE = "decryption";





Submit_Decryption::Submit_Decryption()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Submit_Decryption::Submit_Decryption(const string &page, const string &element)
                        : Submit(page, element)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Submit_Decryption::~Submit_Decryption()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Submit_Decryption::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Submit::DoInit();
}

// 子类对象创建器
Submit *Submit_Decryption::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Submit_Decryption;
}





/******************************** 业务代码 ********************************/




// 认证通过，返回的["decryption"]为空串，不通过则返回出错信息；
int Submit_Decryption::Deal(Page *page)
{
    const string &ERR_MSG = "auth_msg";
    const string &username = page->GetRequest()->GetCurrentUser();
    const string &key = page->GetCurrentKey();
    const Conf *pack = page->GetCurrentPack();

    // 再次确认其提交来源
    const string &decryption = page->GetRequest()->GetField("decryption");
    if("" == decryption)
    {
        return ERR;
    }

    // 比对密码
    const string &passwd1 = page->GetRequest()->GetField("password");  // 客户端传来的密码（明文）
    const string &passwd2 = pack->Get("password");  // 加密后的密码（密文）
    if( ("" != passwd1 || "" != passwd2) && (passwd2 != Crypt(passwd1, CRYPT_VERSION)) )
    {
        page->SetResult(ERR_MSG, "密码错");
        LOG_ERROR("Password error");
        return ERR;
    }

    page->SetResult(ERR_MSG, "OK");
    page->SetResult("decryption", "YES"); // 认证通过，则标记为解密；
    LOG_DEBUG("User %s authenticate ok", username.c_str());

    return OK;
}













// 设置为全局标记；
static Submit_Decryption tmp(SUBMIT_GLOBAL, THIS_MODULE);

}// end of SUBMIT_DECRYPTION_SPACE

