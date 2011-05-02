// Rocky 2010-04-23 09:49:54
#include "User.h"
#include "Page_Download.h"
namespace PAGE_DOWNLOAD_SPACE
{









// ����ģ��
static const string THIS_MODULE = "download";



Page_Download::Page_Download()
{
    FUNCTION_TRACK(); // �����켣����
}

Page_Download::~Page_Download()
{
    FUNCTION_TRACK(); // �����켣����
}


int Page_Download::DoInit()
{
    FUNCTION_TRACK(); // �����켣����
    return Page::DoInit();
}

// ������󴴽���
Page *Page_Download::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Page_Download;
}








/******************************** ҵ����� ********************************/


// ���httpЭ��ͷ��
int Page_Download::OutHead()
{
    FUNCTION_TRACK(); // �����켣����

    Connect * const connect = m_request->GetConnect();
    string filename = m_request->GetField("file");
    string fullpath = "";


    /*
     * ���û��ļ����磺
     *  http://192.168.1.100:17890/download?file=logo.gif
     */
    const string &username = m_request->GetCurrentUser();
    const string &key = GetCurrentKey();
    User *user = User::Get( username );
    fullpath = user->AttachDir() + key + "." + filename;

    LOG_DEBUG("file=[%s]", fullpath.c_str());

    if( "" == filename || !m_file.Open(fullpath) )
    {
        Page::OutHead();

        const string str = HtmlAlert("û���ļ�: " + filename + "�������ļ��ѱ�ɾ����");
        LOG_ERROR("Can't open file: [%s]", fullpath.c_str());
        // ���͵������
        connect->Send(str);
        return ERR;
    }

    const string &size = IntToString(m_file.Size());

    // �ļ�����ͷ����ʽ
    const string html = ""
                        "HTTP/1.1 200 OK\n"
                        "Accept-Ranges: bytes\n"
                        "Content-Disposition: attachment; filename=\"" + FilenameDecode(filename) + "\"\n"
                        "Content-length: " + size + "\n"
                        "Connection: Keep-Alive\n"
                        "Content-Type: application/ms-excel\n"
                        "\n";

    // ����
    return connect->Send(html) == html.length() ? OK : ERR;
}

// ���������
int Page_Download::OutBody()
{
    FUNCTION_TRACK(); // �����켣����

    assert(NULL != m_request);

    Connect * const connect = m_request->GetConnect();
    int ret = SendFile(m_file, connect);

    return m_file.Size() == ret ? OK : ERR;
}







// ���嶯̬�����
DefinitinoDllEnter(Page_Download, THIS_MODULE)

}// end of PAGE_DOWNLOAD_SPACE
