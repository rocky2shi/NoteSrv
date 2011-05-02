// Rocky 2010-05-30 12:28:18
#include "User.h"
#include "Page.h"
#include "Submit_DeleteUser.h"
namespace SUBMIT_DELETEUSER_SPACE
{


// ����ģ�飨��Ӧ��ҳ���е��ύ�ֶΣ�
static const string THIS_MODULE = "DeleteUser";





Submit_DeleteUser::Submit_DeleteUser()
{
    FUNCTION_TRACK(); // �����켣����
}

Submit_DeleteUser::Submit_DeleteUser(const string &page, const string &element)
                        : Submit(page, element)
{
    FUNCTION_TRACK(); // �����켣����
}

Submit_DeleteUser::~Submit_DeleteUser()
{
    FUNCTION_TRACK(); // �����켣����
}




int Submit_DeleteUser::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Submit::DoInit();
}

// ������󴴽���
Submit *Submit_DeleteUser::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Submit_DeleteUser;
}





/******************************** ҵ����� ********************************/



// [Rocky 2010-05-28 15:04:36]
int Submit_DeleteUser::Deal(Page *page)
{
    const string username = page->GetRequest()->GetField("delete_user");
    User *user = User::Get(username);
    int ret;

    if( ! user->isValid() )
    {
        LOG_ERROR("Anvalid user: [%s]", username.c_str());
        page->SetResult("msg", "�û�������");
        return ERR;
    }

    if("administrator" == username)
    {
        LOG_ERROR("Can't delete administrator user");
        page->SetResult("msg", "δ֪����[����:SD02]");
        return ERR;
    }

    // ����
    UNIQUE_LOCK( user->GetLock() );

    // ����ɾ����ǣ��Ժ��ɶ�ʱ��ִ��ֱ���������������Other.cpp�У�
    user->SetInfo("status", "delete");
    user->SetInfo("modify", NowTime("%Y%m%d%H%M%S"));

    // ����
    ret = user->Save();
    if(ret < 0)
    {
        LOG_ERROR("Save user delete mark error: [%s]", username.c_str());
        page->SetResult("msg", "δ֪����[����:SD01]");
        return ERR;
    }
    LOG_INFO("User had been maked delete mark: [%s]", username.c_str());

    return OK;
}













static Submit_DeleteUser tmp("administrator", THIS_MODULE);

}// end of SUBMIT_DELETEUSER_SPACE

