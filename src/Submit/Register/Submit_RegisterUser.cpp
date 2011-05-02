// Rocky 2010-05-23 22:22:28
#include "Request.h"
#include "Page.h"
#include "User.h"
#include "Encrypt.h"
#include "Submit_RegisterUser.h"
namespace SUBMIT_REGISTERUSER_SPACE
{



// ����ģ��
static const string THIS_MODULE = "RegisterUser";





Submit_RegisterUser::Submit_RegisterUser()
{
    FUNCTION_TRACK(); // �����켣����
}

Submit_RegisterUser::Submit_RegisterUser(const string &page, const string &element)
                        : Submit(page, element)
{
    FUNCTION_TRACK(); // �����켣����
}

Submit_RegisterUser::~Submit_RegisterUser()
{
    FUNCTION_TRACK(); // �����켣����
}




int Submit_RegisterUser::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Submit::DoInit();
}

// ������󴴽���
Submit *Submit_RegisterUser::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Submit_RegisterUser;
}





/******************************** ҵ����� ********************************/




// Rocky 2010-05-24 00:10:13
int Submit_RegisterUser::Deal(Page *page)
{
    const Request *request = page->GetRequest();
    const string &username = request->GetField("username");
    int ret;
    string msg;

    try
    {
        /*
         * ����֤�����Ƿ�Ϸ�
         *   ��� ��\ / : * ? " < > | '��
         */
        if( username.find_first_of("\\/:*?\"<>|'") != -1 )
        {
            LOG_ERROR("Username error: [%s]", username.c_str());
            msg = "�û������зǷ��ַ���";
            throw 1;
        }
        if("" == username)
        {
            LOG_ERROR("Username can't empty.");
            msg = "�û�������Ϊ�գ�";
            throw 2;
        }

        const string &password = request->GetField("password");
        const string &password1 = request->GetField("password1");
        if("" != password && password != password1)
        {
            LOG_ERROR("Password no match on twice input.");
            msg = "������������벻һ����";
            throw 4;
        }

        // �Ƿ�Ϊ�Ѵ��ڵ��û�
        if( User::Get(username)->isValid() )
        {
            LOG_ERROR("Username exist: [%s]", username.c_str());
            msg = "�û�[" + username + "]�ѱ�ע�ᣡ";
            throw 3;
        }

        /*
         * ��ʼ���������û�����Ŀ¼�ȣ�
         */
        User *user = User::Create(username);
        if(NULL == user)
        {
            LOG_ERROR("Create user error: [%s]", username.c_str());
            msg = "�ڲ�����[����:SR5]";
            throw 5;
        }

        // ����Ҫ�޸��û���Ϣʱ���������
        UNIQUE_LOCK( user->GetLock() );

        /*
         * �����û���Ϣ��˵����
         *
         *  1. ������ܴ�ţ������棨����ʧ�󲻿��һأ�
         *  2. ��Ҳ�����ܴ�ţ���
         */
        const string &question = request->GetField("question");
        const string &answer = request->GetField("answer");
        const string &comment = request->GetField("comment");
        const string &regtime = request->GetField("regtime");

        user->SetInfo("password", ("" == password ? "" : Crypt(password, CRYPT_VERSION)));
        user->SetInfo("question", question);
        user->SetInfo("answer", Crypt(answer, CRYPT_VERSION));
        user->SetInfo("comment", comment);
        user->SetInfo("regtime", NowTime("%Y%m%d%H%M%S"));
        user->SetInfo("status", "enable");

        if( user->Save() < 0 )
        {
            LOG_ERROR("Save user cfg error: [%s]", username.c_str());
            msg = "�ڲ�����[����:SR6]";
            throw 6;
        }

        msg = "ע��ɹ�";
    }
    catch(int)
    {
    }

    const string html = HtmlAlert(msg);
    page->GetRequest()->GetConnect()->Send(html);

    return OK;
}














static Submit_RegisterUser tmp("register", THIS_MODULE);

}// end of SUBMIT_REGISTERUSER_SPACE

