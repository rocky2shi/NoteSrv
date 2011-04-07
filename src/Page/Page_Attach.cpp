// Rocky 2010-05-10 19:24:01
#include "User.h"
#include "UserData.h"
#include "Page_Attach.h"
namespace PAGE_ATTACH_SPACE
{






// 标明模块
static const string THIS_MODULE = "attach";



Page_Attach::Page_Attach()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Page_Attach::~Page_Attach()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}


int Page_Attach::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    m_html = GlobalConfig::instance()->TemplateDir() + "Attach.html";
    return Page::DoInit();
}

// 子类对象创建器
Page *Page_Attach::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Page_Attach;
}








/******************************** 业务代码 ********************************/


// 上传附件
int Page_Attach::Upload()
{
    FileObj *attach = m_request->GetFileObj("attach");
    const string &key = GetCurrentKey();
    const string &username = m_request->GetCurrentUser();
    User *user = User::Get( username );

    const string &basename = m_request->GetField("attach");
    const string &src = attach->Fullname();
    const string &dest = user->AttachDir()
                       + key
                       + "."
                       + FilenameEncode(basename); /* 对文件名编码（汉字问题）
                                                    * [Rocky 2010-06-06 00:54:01]
                                                    */

    LOG_DEBUG("src=[%s], dest=[%s]", src.c_str(), dest.c_str());

    if( isExistFile(dest) )
    {
        LOG_ERROR("File exist: [%s]", basename.c_str());
        const string err = "文件已存在：[" + basename + "]";
        SetResult("UploadError", err);
        return ERR;
    }

    if( ! MoveFile(src, dest) )
    {
        LOG_ERROR("Move file error: src=[%s], dest=[%s]", src.c_str(), dest.c_str());
        const string err = "文件上传出错：[" + basename + "]";
        SetResult("UploadError", err);
        return ERR;
    }

    // 更新记录信息
    const string &modify = NowTime("%Y%m%d%H%M%S");     // 修改时间为当前时间
    vector<string> attachs;
    Ini data;

    UserData(username).GetAttachList(key, attachs);     // 附件列表（用于取个数）

    data.Set(key, "modify", modify);
    data.Set(key, "attach_num", IntToString(attachs.size()));

    LOG_INFO("Upload file ok: [%s]", basename.c_str());

    return Save( data );
}

// 输出数据
int Page_Attach::Deal()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    assert(NULL != m_request);

    int ret;
    const string &upfile = m_request->GetField("upfile");
    if("" != upfile)
    {
        /*
         * 处理上传
         *  注意，当新建条目，而未填写内容（未保存），这时先上传文件也是可以的，
         *  但在保存内容前刷新页面，则上传的文件将丢失（变为垃圾），系统应适时
         *  清理这部分文件。待写。  [Rocky 2010-05-11 16:56:39] [XXX]
         */
        Upload();
    }

    /*
     * 处理删除附件提交事件
     */
    const string &delete_attach = m_request->GetField("delete_attach");
    if("" != delete_attach)
    {
        Submit::Ptr submit( Submit::New("attach", "DeleteAttach") );
        if(submit->Deal(this) < 0)
        {
            m_request->GetConnect()->Send("删除附件出错");
        }
        else
        {
            m_request->GetConnect()->Send("OK");
        }
        return OK;
    }

    return Page::Deal();
}

// 输出数据体
int Page_Attach::OutBody()
{
    const string &err = GetResult("UploadError");
    if("" != err)
    {
        const string html = HtmlAlert(err);
        m_request->GetConnect()->Send(html);
    }

    return Page::OutBody();
}






// 定义动态库入口
DefinitinoDllEnter(Page_Attach, THIS_MODULE)

}// end of PAGE_ATTACH_SPACE
