// Rocky 2010-05-24 13:55:53
#include "Request.h"
#include "Page.h"
#include "User.h"
#include "Encrypt.h"
#include "Submit_GetPassword.h"
namespace SUBMIT_GETPASSWORD_SPACE
{

// ����ģ��
static const string THIS_MODULE = "GetPassword";





Submit_GetPassword::Submit_GetPassword()
{
    FUNCTION_TRACK(); // �����켣����
}

Submit_GetPassword::Submit_GetPassword(const string &page, const string &element)
                        : Submit(page, element)
{
    FUNCTION_TRACK(); // �����켣����
}

Submit_GetPassword::~Submit_GetPassword()
{
    FUNCTION_TRACK(); // �����켣����
}




int Submit_GetPassword::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Submit::DoInit();
}

// ������󴴽���
Submit *Submit_GetPassword::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Submit_GetPassword;
}





/******************************** ҵ����� ********************************/




// Rocky 2010-05-24 12:15:06
int Submit_GetPassword::Deal(Page *page)
{
    const Request *request = page->GetRequest();
    const string &username = request->GetField("username");
    int ret;
    string msg;
    string html;

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

        /*
         * �Կͻ��˴����Ĵ�Ϊkey�����������м�¼�����ļ���
         */
        const string &cfg_answer = user->GetInfo("answer");
        const string &cli_answer = request->GetField("answer");

        LOG_DEBUG("cfg_answer=[%s]", cfg_answer.c_str());
        LOG_DEBUG("cli_answer=[%s]", cli_answer.c_str());

        if(cfg_answer != Crypt(cli_answer, CRYPT_VERSION))
        {
            LOG_ERROR("Answer password, username=[%s]", username.c_str());
            msg = "�𰸴���";
            throw 2;
        }

        /*
         * ����ȷ����������Ϊһ��������û�ȡ�ø�����󣬿ɵ����޸����롱ҳ
         * �ٸ�Ϊ������Ҫ�����룻
         */
        const string &password = IntToString( GetRandomInt(10000000, 99999999) );
        user->SetInfo("password", Crypt(password, CRYPT_VERSION));
        user->SetInfo("modify", NowTime("%Y%m%d%H%M%S"));

        if( user->Save() < 0 )
        {
            LOG_ERROR("Save user cfg error: [%s]", username.c_str());
            msg = "�ڲ�����[����:SR6]";
            throw 6;
        }

        html = HtmlSetParentInputValue("get_password", password);
    }
    catch(int)
    {
        html = HtmlSetParentInputValue("get_password", "[" + msg + "]");
    }

    page->GetRequest()->GetConnect()->Send(html);

    return OK;
}














static Submit_GetPassword tmp("register", THIS_MODULE);

}// end of SUBMIT_GETPASSWORD_SPACE
