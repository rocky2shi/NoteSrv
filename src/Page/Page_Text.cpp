// Rocky 2010-04-23 10:49:56
#include "Page_Text.h"
namespace PAGE_TEXT_SPACE
{











// 标明模块
static const string THIS_MODULE = "text";



Page_Text::Page_Text()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Page_Text::~Page_Text()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}


int Page_Text::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return Page::DoInit();
}

// 子类对象创建器
Page *Page_Text::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Page_Text;
}








/******************************** 业务代码 ********************************/


// 输出数据体
int Page_Text::Deal()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

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

    // 发送http头
    ret = connect->Send(head);

    // 发送页面代码
    ret = SendFile(file, connect);

    return file.Size() == ret ? OK : ERR;
}













// 定义动态库入口
DefinitinoDllEnter(Page_Text, THIS_MODULE)

}// end of PAGE_DOWNLOAD_SPACE

