// Rocky 2010-05-10 19:24:01
#include "User.h"
#include "UserData.h"
#include "Page_Attach.h"
namespace PAGE_ATTACH_SPACE
{






// ����ģ��
static const string THIS_MODULE = "attach";



Page_Attach::Page_Attach()
{
    FUNCTION_TRACK(); // �����켣����
}

Page_Attach::~Page_Attach()
{
    FUNCTION_TRACK(); // �����켣����
}


int Page_Attach::DoInit()
{
    FUNCTION_TRACK(); // �����켣����
    m_html = GlobalConfig::instance()->TemplateDir() + "Attach.html";
    return Page::DoInit();
}

// ������󴴽���
Page *Page_Attach::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Page_Attach;
}








/******************************** ҵ����� ********************************/


// �ϴ�����
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
                       + FilenameEncode(basename); /* ���ļ������루�������⣩
                                                    * [Rocky 2010-06-06 00:54:01]
                                                    */

    LOG_DEBUG("src=[%s], dest=[%s]", src.c_str(), dest.c_str());

    if( isExistFile(dest) )
    {
        LOG_ERROR("File exist: [%s]", basename.c_str());
        const string err = "�ļ��Ѵ��ڣ�[" + basename + "]";
        SetResult("UploadError", err);
        return ERR;
    }

    if( ! MoveFile(src, dest) )
    {
        LOG_ERROR("Move file error: src=[%s], dest=[%s]", src.c_str(), dest.c_str());
        const string err = "�ļ��ϴ�����[" + basename + "]";
        SetResult("UploadError", err);
        return ERR;
    }

    // ���¼�¼��Ϣ
    const string &modify = NowTime("%Y%m%d%H%M%S");     // �޸�ʱ��Ϊ��ǰʱ��
    vector<string> attachs;
    Ini data;

    UserData(username).GetAttachList(key, attachs);     // �����б�����ȡ������

    data.Set(key, "modify", modify);
    data.Set(key, "attach_num", IntToString(attachs.size()));

    LOG_INFO("Upload file ok: [%s]", basename.c_str());

    return Save( data );
}

// �������
int Page_Attach::Deal()
{
    FUNCTION_TRACK(); // �����켣����

    assert(NULL != m_request);

    int ret;
    const string &upfile = m_request->GetField("upfile");
    if("" != upfile)
    {
        /*
         * �����ϴ�
         *  ע�⣬���½���Ŀ����δ��д���ݣ�δ���棩����ʱ���ϴ��ļ�Ҳ�ǿ��Եģ�
         *  ���ڱ�������ǰˢ��ҳ�棬���ϴ����ļ�����ʧ����Ϊ��������ϵͳӦ��ʱ
         *  �����ⲿ���ļ�����д��  [Rocky 2010-05-11 16:56:39] [XXX]
         */
        Upload();
    }

    /*
     * ����ɾ�������ύ�¼�
     */
    const string &delete_attach = m_request->GetField("delete_attach");
    if("" != delete_attach)
    {
        Submit::Ptr submit( Submit::New("attach", "DeleteAttach") );
        if(submit->Deal(this) < 0)
        {
            m_request->GetConnect()->Send("ɾ����������");
        }
        else
        {
            m_request->GetConnect()->Send("OK");
        }
        return OK;
    }

    return Page::Deal();
}

// ���������
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






// ���嶯̬�����
DefinitinoDllEnter(Page_Attach, THIS_MODULE)

}// end of PAGE_ATTACH_SPACE
