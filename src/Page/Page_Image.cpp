// Rocky 2010-04-22 19:43:58
#include "User.h"
#include "Page_Image.h"
namespace PAGE_IMAGE_SPACE
{






// 标明模块（注意，定义"original"在Page::New()中到，不可随意更改；）
static const string THIS_MODULE = "image";



Page_Image::Page_Image()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Page_Image::~Page_Image()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}


int Page_Image::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return Page::DoInit();
}

// 子类对象创建器
Page *Page_Image::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Page_Image;
}








/******************************** 业务代码 ********************************/


// 输出http协议头部
int Page_Image::OutHead()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    Connect * const connect = m_request->GetConnect();
    string filename = m_request->GetField("file");
    string fullpath = "";

    if("" == filename)
    {
        /*
         * 是直接请求图片，如：
         * http://192.168.1.100:17890/logo.gif
         */
        filename = m_request->GetPageName();
        fullpath = GlobalConfig::instance()->HtmlDir() + filename;
    }
    else
    {
        /*
         * 打开用户图片，如：
         *  http://192.168.1.100:17890/image?file=logo.gif
         */
        const string &username = m_request->GetCurrentUser();
        const string &key = GetCurrentKey();
        User *user = User::Get( username );
        fullpath = user->AttachDir() + key + "." + filename; // 图片存于附件目录中
    }

    LOG_DEBUG("file=[%s]", fullpath.c_str());


    if( "" == filename || !m_file.Open(fullpath) )
    {
        Page::OutHead();

        const string str = "没有图片: <font color='red'>" + filename + "</font>";
        LOG_ERROR("Can't open file: [%s]", fullpath.c_str());
        // 发送到浏览器
        connect->Send(str);
        return ERR;
    }

    const string &size = IntToString(m_file.Size());
    // 图像显示
    const string html = ""
                        "HTTP/1.1 200 OK\n"
                        "Accept-Ranges: bytes\n"
                        "Content-Length: " + size + "\n"
                        "Connection: Keep-Alive\n"
                        "Content-Type: image/png jpg jpeg gif bmp\n"
                        "\n";

    // 发送
    return connect->Send(html) == html.length() ? OK : ERR;
}

// 输出数据体
int Page_Image::OutBody()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    assert(NULL != m_request);

    Connect * const connect = m_request->GetConnect();
    int ret = SendFile(m_file, connect);

    return m_file.Size() == ret ? OK : ERR;
}














// 定义动态库入口
DefinitinoDllEnter(Page_Image, THIS_MODULE)

}// end of PAGE_DOWNLOAD_SPACE

