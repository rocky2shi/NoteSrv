// Rocky 2010-05-10 20:35:23
#include "User.h"
#include "UserData.h"
#include "Page_Delete.h"
namespace PAGE_DELETE_SPACE
{





// ����ģ��
static const string THIS_MODULE = "delete";



Page_Delete::Page_Delete()
{
    FUNCTION_TRACK(); // �����켣����
}

Page_Delete::~Page_Delete()
{
    FUNCTION_TRACK(); // �����켣����
}


int Page_Delete::DoInit()
{
    FUNCTION_TRACK(); // �����켣����
    m_html = "";
    return Page::DoInit();
}

// ������󴴽���
Page *Page_Delete::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Page_Delete;
}








/******************************** ҵ����� ********************************/


// �������
int Page_Delete::Deal()
{
    FUNCTION_TRACK(); // �����켣����

    assert(NULL != m_request);

    int ret;
    Connect * const connect = m_request->GetConnect();
    string filename = m_request->GetField("file");
    string fullpath = "";


    /*
     * ɾ���û��ļ����磺
     *  http://192.168.1.100:17890/delete?file=logo.gif
     */
    const string &username = m_request->GetCurrentUser();
    const string &key = GetCurrentKey();
    User *user = User::Get( username );
    fullpath = user->AttachDir() + key + "." + filename;

    LOG_DEBUG("file=[%s]", fullpath.c_str());

    ret = DeleteFile( fullpath.c_str() );
    if( ret < 0 )
    {
        Page::OutHead();

        const string str = "ɾ���ļ������ļ���: <font color='red'>" + filename + "</font>";
        LOG_ERROR("Can't delete file: [%s]", fullpath.c_str());
        // ���͵������
        connect->Send(str);
        return ERR;
    }

    /*
     * ���¸������ֶ�
     */
    Ini data;
    vector<string> attachs;
    UserData(username).GetAttachList(key, attachs);     // �����б�

    data.Set(key, "modify", NowTime("%Y%m%d%H%M%S"));
    data.Set(key, "attach_num", IntToString(attachs.size()));

    ret = Save(data);
    if(ret < 0)
    {
        LOG_ERROR("Update attach counte error, key=[%s]", key.c_str());
        return ERR;
    }


    LOG_INFO("File had been deleted: [%s]", fullpath.c_str());

    return Page::Deal();
}


// ���������
int Page_Delete::OutBody()
{
    return m_request->GetConnect()->Send("OK");
}











// ���嶯̬�����
DefinitinoDllEnter(Page_Delete, THIS_MODULE)

}// end of PAGE_DELETE_SPACE
