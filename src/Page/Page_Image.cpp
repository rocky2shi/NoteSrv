// Rocky 2010-04-22 19:43:58
#include "User.h"
#include "Page_Image.h"
namespace PAGE_IMAGE_SPACE
{






// ����ģ�飨ע�⣬����"original"��Page::New()�е�������������ģ���
static const string THIS_MODULE = "image";



Page_Image::Page_Image()
{
    FUNCTION_TRACK(); // �����켣����
}

Page_Image::~Page_Image()
{
    FUNCTION_TRACK(); // �����켣����
}


int Page_Image::DoInit()
{
    FUNCTION_TRACK(); // �����켣����
    return Page::DoInit();
}

// ������󴴽���
Page *Page_Image::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Page_Image;
}








/******************************** ҵ����� ********************************/


// ���httpЭ��ͷ��
int Page_Image::OutHead()
{
    FUNCTION_TRACK(); // �����켣����

    Connect * const connect = m_request->GetConnect();
    string filename = m_request->GetField("file");
    string fullpath = "";

    if("" == filename)
    {
        /*
         * ��ֱ������ͼƬ���磺
         * http://192.168.1.100:17890/logo.gif
         */
        filename = m_request->GetPageName();
        fullpath = GlobalConfig::instance()->HtmlDir() + filename;
    }
    else
    {
        /*
         * ���û�ͼƬ���磺
         *  http://192.168.1.100:17890/image?file=logo.gif
         */
        const string &username = m_request->GetCurrentUser();
        const string &key = GetCurrentKey();
        User *user = User::Get( username );
        fullpath = user->AttachDir() + key + "." + filename; // ͼƬ���ڸ���Ŀ¼��
    }

    LOG_DEBUG("file=[%s]", fullpath.c_str());


    if( "" == filename || !m_file.Open(fullpath) )
    {
        Page::OutHead();

        const string str = "û��ͼƬ: <font color='red'>" + filename + "</font>";
        LOG_ERROR("Can't open file: [%s]", fullpath.c_str());
        // ���͵������
        connect->Send(str);
        return ERR;
    }

    const string &size = IntToString(m_file.Size());
    // ͼ����ʾ
    const string html = ""
                        "HTTP/1.1 200 OK\n"
                        "Accept-Ranges: bytes\n"
                        "Content-Length: " + size + "\n"
                        "Connection: Keep-Alive\n"
                        "Content-Type: image/png jpg jpeg gif bmp\n"
                        "\n";

    // ����
    return connect->Send(html) == html.length() ? OK : ERR;
}

// ���������
int Page_Image::OutBody()
{
    FUNCTION_TRACK(); // �����켣����

    assert(NULL != m_request);

    Connect * const connect = m_request->GetConnect();
    int ret = SendFile(m_file, connect);

    return m_file.Size() == ret ? OK : ERR;
}














// ���嶯̬�����
DefinitinoDllEnter(Page_Image, THIS_MODULE)

}// end of PAGE_DOWNLOAD_SPACE

