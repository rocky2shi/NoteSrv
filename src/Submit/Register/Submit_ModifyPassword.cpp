// Rocky 2010-05-24 12:05:18
#include "Request.h"
#include "Page.h"
#include "User.h"
#include "Encrypt.h"
#include "Submit_ModifyPassword.h"
namespace SUBMIT_MODIFYPASSWORD_SPACE
{

// ����ģ��
static const string THIS_MODULE = "ModifyPassword";





Submit_ModifyPassword::Submit_ModifyPassword()
{
    FUNCTION_TRACK(); // �����켣����
}

Submit_ModifyPassword::Submit_ModifyPassword(const string &page, const string &element)
                        : Submit(page, element)
{
    FUNCTION_TRACK(); // �����켣����
}

Submit_ModifyPassword::~Submit_ModifyPassword()
{
    FUNCTION_TRACK(); // �����켣����
}




int Submit_ModifyPassword::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Submit::DoInit();
}

// ������󴴽���
Submit *Submit_ModifyPassword::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Submit_ModifyPassword;
}





/******************************** ҵ����� ********************************/




// Rocky 2010-05-24 12:15:06
int Submit_ModifyPassword::Deal(Page *page)
{
    const Request *request = page->GetRequest();
    const string &username = request->GetField("username");
    int ret;
    string msg;

    try
    {
        /*
         * ��֤�û�����
         */
        User *user = User::Get(username);
        if( ! user->isValid() )
        {
            LOG_ERROR("Username exist: [%s]", username.c_str());
            msg = "���û�������";
            throw 1;
        }

        // ����Ҫ�޸��û���Ϣʱ���������
        UNIQUE_LOCK( user->GetLock() );

        const string &new_password1 = request->GetField("new_password1");
        const string &new_password2 = request->GetField("new_password2");
        if("" != new_password1 && new_password1 != new_password2)
        {
            LOG_ERROR("Password no match on twice input.");
            msg = "������������벻һ��";
            throw 2;
        }

        /*
         * ���û������ļ��Ϳͻ��˴��������Ƚ�
         */
        const string &cfg_password = user->GetInfo("password");         // ����
        const string &old_password = request->GetField("old_password"); // ����
        if("" != cfg_password && cfg_password != Crypt(old_password, CRYPT_VERSION))
        {
            LOG_ERROR("Old password, username=[%s]", username.c_str());
            msg = "ԭ�������";
            throw 3;
        }

        /*
         * �޸��û���Ϣ
         */
        user->SetInfo("password", ("" == new_password1 ? "" : Crypt(new_password1, CRYPT_VERSION)));
        user->SetInfo("modify", NowTime("%Y%m%d%H%M%S"));

        if( user->Save() < 0 )
        {
            LOG_ERROR("Save user cfg error: [%s]", username.c_str());
            msg = "�ڲ�����[����:SM6]";
            throw 6;
        }

        msg = "�޸ĳɹ�";
    }
    catch(int)
    {
    }

    const string html = HtmlAlert(msg);
    page->GetRequest()->GetConnect()->Send(html);

    return OK;
}














static Submit_ModifyPassword tmp("register", THIS_MODULE);

}// end of SUBMIT_MODIFYPASSWORD_SPACE
