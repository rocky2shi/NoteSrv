// Rocky 2010-05-23 01:43:19
#include "UserData.h"
#include "Page.h"
#include "User.h"
#include "Submit_DeleteAttach.h"
namespace SUBMIT_DELETEATTACH_SPACE
{



// 标明模块
static const string THIS_MODULE = "DeleteAttach";





Submit_DeleteAttach::Submit_DeleteAttach()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Submit_DeleteAttach::Submit_DeleteAttach(const string &page, const string &element)
                        : Submit(page, element)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Submit_DeleteAttach::~Submit_DeleteAttach()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Submit_DeleteAttach::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Submit::DoInit();
}

// 子类对象创建器
Submit *Submit_DeleteAttach::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Submit_DeleteAttach;
}





/******************************** 业务代码 ********************************/



int Submit_DeleteAttach::Deal(Page *page)
{
    int ret;
    Request *request = page->GetRequest();
    const string &username = request->GetCurrentUser();
    const string &key = page->GetCurrentKey();
    User *user = User::Get( username );
    const string &filename = request->GetField("file");
    const string &fullpath = user->AttachDir() + key + "." + filename;

    LOG_DEBUG("file=[%s]", fullpath.c_str());

    ret = DeleteFile( fullpath.c_str() );
    if( ret < 0 )
    {
        LOG_ERROR("Can't delete file: [%s]", fullpath.c_str());
        return ERR;
    }

    /*
     * 更新附件数字段
     */
    Ini data;
    vector<string> attachs;
    UserData(username).GetAttachList(key, attachs);     // 附件列表

    data.Set(key, "modify", NowTime("%Y%m%d%H%M%S"));
    data.Set(key, "attach_num", IntToString(attachs.size()));

    ret = page->Save(data);
    if(ret < 0)
    {
        LOG_ERROR("Update attach counte error, key=[%s]", key.c_str());
        return ERR;
    }

    LOG_INFO("File had been deleted: [%s]", fullpath.c_str());

    return OK;
}













static Submit_DeleteAttach tmp("attach", THIS_MODULE);

}// end of SUBMIT_DELETEATTACH_SPACE

