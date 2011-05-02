// Rocky 2010-05-13 16:06:55
#include "Page.h"
#include "UserData.h"
#include "Encrypt.h"
#include "Submit_SetPassword.h"
namespace SUBMIT_SETPASSWORD_SPACE
{





// ����ģ��
static const string THIS_MODULE = "SetPassword";





Submit_SetPassword::Submit_SetPassword()
{
    FUNCTION_TRACK(); // �����켣����
}

Submit_SetPassword::Submit_SetPassword(const string &page, const string &element)
                        : Submit(page, element)
{
    FUNCTION_TRACK(); // �����켣����
}

Submit_SetPassword::~Submit_SetPassword()
{
    FUNCTION_TRACK(); // �����켣����
}




int Submit_SetPassword::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Submit::DoInit();
}

// ������󴴽���
Submit *Submit_SetPassword::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Submit_SetPassword;
}





/******************************** ҵ����� ********************************/




// ��֤ͨ�������ص�["decryption"]Ϊ�մ�����ͨ���򷵻س�����Ϣ��
int Submit_SetPassword::Deal(Page *page)
{
    const string &key = page->GetCurrentKey();
    const Conf *pack = page->GetCurrentPack();          // ��ǰkey��Ӧ���ݼ�
    const string &text = pack->Get("text");             // ȡ����������
    const string &password = pack->Get("password");
    const string &modify = NowTime("%Y%m%d%H%M%S");     // �޸�ʱ��Ϊ��ǰʱ��
    const string &sOldPassword = page->GetRequest()->GetField("msg_pasd_old");      // ԭ����
    const string &sNewPassword = page->GetRequest()->GetField("msg_pasd_new");      // ������
    const string &msg_pasd_prompt = page->GetRequest()->GetField("msg_pasd_prompt");// ������ʾ
    string html = "";
    Ini save;

    /*
     * ��֤ԭ����
     */
    if( "" != password && password != Crypt(sOldPassword, CRYPT_VERSION))
    {
        LOG_ERROR("old password error.");
        return ERR;
    }

    /*
     * ���ݴ��
     *   ����ǰ�����Ǽ��ܵģ����Ƚ��ܵ�ǰ���ݣ�
     */
    const string *pText = &text; // �����Ƿ����Ǽ��ܣ�ָ�����Ļ����ģ�
    string txt;
    if("" != password)
    {
        // ע�⣬��ʹ�ÿͻ��˴������޸�ǰ�����룬�������롣
        txt = Encrypt( sOldPassword ).decrypt( text );
        pText = &txt;
    }

    if("" != sNewPassword)
    {
        // ������ǿգ�����ܺ������У�ʹ�������룩
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

    // ����
    return page->Save( save );
}













// ����Ϊȫ�ֱ�ǣ�
static Submit_SetPassword tmp("edit", THIS_MODULE);

}// end of SUBMIT_SETPASSWORD_SPACE

