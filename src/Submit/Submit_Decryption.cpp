// Rocky 2010-05-12 00:10:37
#include "Page.h"
#include "UserData.h"
#include "Submit_Decryption.h"
namespace SUBMIT_DECRYPTION_SPACE
{


// ����ģ�飨��Ӧ��ҳ���е��ύ�ֶΣ�
static const string THIS_MODULE = "decryption";





Submit_Decryption::Submit_Decryption()
{
    FUNCTION_TRACK(); // �����켣����
}

Submit_Decryption::Submit_Decryption(const string &page, const string &element)
                        : Submit(page, element)
{
    FUNCTION_TRACK(); // �����켣����
}

Submit_Decryption::~Submit_Decryption()
{
    FUNCTION_TRACK(); // �����켣����
}




int Submit_Decryption::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Submit::DoInit();
}

// ������󴴽���
Submit *Submit_Decryption::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Submit_Decryption;
}





/******************************** ҵ����� ********************************/




// ��֤ͨ�������ص�["decryption"]Ϊ�մ�����ͨ���򷵻س�����Ϣ��
int Submit_Decryption::Deal(Page *page)
{
    const string &ERR_MSG = "auth_msg";
    const string &username = page->GetRequest()->GetCurrentUser();
    const string &key = page->GetCurrentKey();
    const Conf *pack = page->GetCurrentPack();

    // �ٴ�ȷ�����ύ��Դ
    const string &decryption = page->GetRequest()->GetField("decryption");
    if("" == decryption)
    {
        return ERR;
    }

    // �ȶ�����
    const string &passwd1 = page->GetRequest()->GetField("password");  // �ͻ��˴��������루���ģ�
    const string &passwd2 = pack->Get("password");  // ���ܺ�����루���ģ�
    if( ("" != passwd1 || "" != passwd2) && (passwd2 != Crypt(passwd1, CRYPT_VERSION)) )
    {
        page->SetResult(ERR_MSG, "�����");
        LOG_ERROR("Password error");
        return ERR;
    }

    page->SetResult(ERR_MSG, "OK");
    page->SetResult("decryption", "YES"); // ��֤ͨ��������Ϊ���ܣ�
    LOG_DEBUG("User %s authenticate ok", username.c_str());

    return OK;
}













// ����Ϊȫ�ֱ�ǣ�
static Submit_Decryption tmp(SUBMIT_GLOBAL, THIS_MODULE);

}// end of SUBMIT_DECRYPTION_SPACE

