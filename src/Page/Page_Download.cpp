// Rocky 2010-04-23 09:49:54
#include "User.h"
#include "Page_Download.h"
namespace PAGE_DOWNLOAD_SPACE
{









// 标明模块
static const string THIS_MODULE = "download";



Page_Download::Page_Download()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Page_Download::~Page_Download()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}


int Page_Download::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return Page::DoInit();
}

// 子类对象创建器
Page *Page_Download::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Page_Download;
}








/******************************** 业务代码 ********************************/


// 输出http协议头部
int Page_Download::OutHead()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    Connect * const connect = m_request->GetConnect();
    string filename = m_request->GetField("file");
    string fullpath = "";


    /*
     * 打开用户文件，如：
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

        const string str = HtmlAlert("没有文件: " + filename + "，可能文件已被删除。");
        LOG_ERROR("Can't open file: [%s]", fullpath.c_str());
        // 发送到浏览器
        connect->Send(str);
        return ERR;
    }

    const string &size = IntToString(m_file.Size());

    // 文件下载头部格式
    const string html = ""
                        "HTTP/1.1 200 OK\n"
                        "Accept-Ranges: bytes\n"
                        "Content-Disposition: attachment; filename=\"" + FilenameDecode(filename) + "\"\n"
                        "Content-length: " + size + "\n"
                        "Connection: Keep-Alive\n"
                        "Content-Type: application/ms-excel\n"
                        "\n";

    // 发送
    return connect->Send(html) == html.length() ? OK : ERR;
}

// 输出数据体
int Page_Download::OutBody()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    assert(NULL != m_request);

    Connect * const connect = m_request->GetConnect();
    int ret = SendFile(m_file, connect);

    return m_file.Size() == ret ? OK : ERR;
}







// 定义动态库入口
DefinitinoDllEnter(Page_Download, THIS_MODULE)

}// end of PAGE_DOWNLOAD_SPACE
