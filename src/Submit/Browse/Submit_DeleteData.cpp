// Rocky 2010-05-22 22:38:01
#include "Page.h"
#include "Ini.h"
#include "UserData.h"
#include "Submit_DeleteData.h"
namespace SUBMIT_DELETEDATA_SPACE
{




// 标明模块
static const string THIS_MODULE = "DeleteData";





Submit_DeleteData::Submit_DeleteData()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Submit_DeleteData::Submit_DeleteData(const string &page, const string &element)
                        : Submit(page, element)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Submit_DeleteData::~Submit_DeleteData()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Submit_DeleteData::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Submit::DoInit();
}

// 子类对象创建器
Submit *Submit_DeleteData::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Submit_DeleteData;
}





/******************************** 业务代码 ********************************/



// 删除一条数据
int Submit_DeleteData::Deal(Page *page)
{
    const string &key = page->GetCurrentKey();
    Conf *pack = page->GetCurrentPack();

    // 设为删除，并调用处理函数：
    pack->Set("status", "delete");
    pack->Set("text", "");
    pack->Set("modify", NowTime("%Y%m%d%H%M%S"));

    Ini data;
    data.Set(key, *pack);

    const string &username = page->GetRequest()->GetCurrentUser();
    UserData del( username );

    /*
     * 保存（删除数据）
     */
    if(del.Set(data, true) < 0)
    {
        LOG_ERROR("Detete error: [%s]", key.c_str());
        return ERR;
    }

    /*
     * 删除附件
     */
    vector<string> attachs;
    vector<string>::iterator it;
    del.GetAttachList(key, attachs);   // 取附件列表
    for(it = attachs.begin(); attachs.end() != it; it++)
    {
        LOG_INFO("Delete attach: [%s]", it->c_str());
        DeleteFile(*it);
    }

    LOG_INFO("Detete: [%s]", key.c_str());

    return OK;
}













static Submit_DeleteData tmp("browse", THIS_MODULE);

}// end of SUBMIT_DELETEDATA_SPACE

