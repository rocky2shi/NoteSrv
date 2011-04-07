// Rocky 2010-05-30 12:28:18
#include "User.h"
#include "Page.h"
#include "Submit_DeleteUser.h"
namespace SUBMIT_DELETEUSER_SPACE
{


// 标明模块（对应于页面中的提交字段）
static const string THIS_MODULE = "DeleteUser";





Submit_DeleteUser::Submit_DeleteUser()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Submit_DeleteUser::Submit_DeleteUser(const string &page, const string &element)
                        : Submit(page, element)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Submit_DeleteUser::~Submit_DeleteUser()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Submit_DeleteUser::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Submit::DoInit();
}

// 子类对象创建器
Submit *Submit_DeleteUser::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Submit_DeleteUser;
}





/******************************** 业务代码 ********************************/



// [Rocky 2010-05-28 15:04:36]
int Submit_DeleteUser::Deal(Page *page)
{
    const string username = page->GetRequest()->GetField("delete_user");
    User *user = User::Get(username);
    int ret;

    if( ! user->isValid() )
    {
        LOG_ERROR("Anvalid user: [%s]", username.c_str());
        page->SetResult("msg", "用户不存在");
        return ERR;
    }

    if("administrator" == username)
    {
        LOG_ERROR("Can't delete administrator user");
        page->SetResult("msg", "未知错误[代码:SD02]");
        return ERR;
    }

    // 锁定
    UNIQUE_LOCK( user->GetLock() );

    // 打上删除标记（稍后将由定时器执行直正的清理操作，见Other.cpp中）
    user->SetInfo("status", "delete");
    user->SetInfo("modify", NowTime("%Y%m%d%H%M%S"));

    // 保存
    ret = user->Save();
    if(ret < 0)
    {
        LOG_ERROR("Save user delete mark error: [%s]", username.c_str());
        page->SetResult("msg", "未知错误[代码:SD01]");
        return ERR;
    }
    LOG_INFO("User had been maked delete mark: [%s]", username.c_str());

    return OK;
}













static Submit_DeleteUser tmp("administrator", THIS_MODULE);

}// end of SUBMIT_DELETEUSER_SPACE

