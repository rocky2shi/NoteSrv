// Rocky 2010-04-23 10:49:56
#include "Page_Text.h"
namespace PAGE_TEXT_SPACE
{











// ����ģ��
static const string THIS_MODULE = "text";



Page_Text::Page_Text()
{
    FUNCTION_TRACK(); // �����켣����
}

Page_Text::~Page_Text()
{
    FUNCTION_TRACK(); // �����켣����
}


int Page_Text::DoInit()
{
    FUNCTION_TRACK(); // �����켣����
    return Page::DoInit();
}

// ������󴴽���
Page *Page_Text::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Page_Text;
}








/******************************** ҵ����� ********************************/


// ���������
int Page_Text::Deal()
{
    FUNCTION_TRACK(); // �����켣����

    assert(NULL != m_request);

    Connect * const connect = m_request->GetConnect();
    const string &filename = m_request->GetPageName();
    const string fullpath = GlobalConfig::instance()->HtmlDir() + filename;
    FileObj file;
    int ret;

    if( ! file.Open( fullpath ) )
    {
        LOG_ERROR("Can't open file: [%s]", fullpath.c_str());
        return ERR;
    }

    const string head = ""
                  "HTTP/1.1 200 OK\n"
                  "Date: " + GmgTime() + "\n"
                  "Last-Modified: " + GmgTime( GetFileModifyTime(fullpath) ) + "\n"
                  "Server: NoteSrv v0.6\n"
                  "Keep-Alive: timeout=15, max=100\n"
                  "Content-Type: text/html; charset=GB2312\n"
                  "\n";

    // ����httpͷ
    ret = connect->Send(head);

    // ����ҳ�����
    ret = SendFile(file, connect);

    return file.Size() == ret ? OK : ERR;
}













// ���嶯̬�����
DefinitinoDllEnter(Page_Text, THIS_MODULE)

}// end of PAGE_DOWNLOAD_SPACE

