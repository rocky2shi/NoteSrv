#ifndef __REQUEST_H_20100418__
#define __REQUEST_H_20100418__
#include "Common.h"
#include "Connect.h"
#include "FileObj.h"
namespace REQUEST_SPACE
{




// 客户端发来的请求数据类
class Request
{
public:
    Request(Connect *connect);
    ~Request();

    // 取请求头中字段对应值（不区分大小写）
    const string GetHead(const string &field) const;

    // 一般字段对应值（不区分大小写）
    const string GetField(const string &field) const;

    // 取字段对应文件对象
    FileObj *GetFileObj(const string &field);

    // 返回请求页面名
    const string GetPageName();

    // 取连接
    Connect *GetConnect();

    // 是否为图片：jpg, jpeg, png, gif, ...
    bool isImage();

    // 是否为文本文件：txt, js, css, html, htm, ...
    bool isText();

    // 是否为cgi处理页面
    bool isCgi();

    // 解析等是否正确处理
    bool ok();

    // 取当前登录用户名
    const string GetCurrentUser() const;

    // 取Cookie中的值
    const string GetCookie(const string &key) const;


private:
    void Parse();       // 解析请求串
    void ParseHead();   // 处理请求头部
    void ParseAttach(); // 处理附件的情况
    void ParsePost();   // 处理POST的情况
    void ParseGet(char *str);        // 处理GET的情况
    void ParseUrlParam(char *param); // 解析url参数串
    void ParseCookie();              // 解析Cookie串

    int GetLine(char *buf, unsigned int max);   // 取一行

private:
    string m_page;      // 页名名
    string m_boundary;  // post附件时所用的分隔串
    string m_type;      // 请求的类型（目前处理三种情况）
    Connect * const m_connect;          // 客户端连接
    string m_PageType;                  // 页面名后缀
    map<string, string> m_HeadFieldAry; // 请求头字段列表
    map<string, string> m_FieldAry;     // 字段列表
    map<string, FileObj*> m_FileObjAry; // 附件等文件列表
    map<string, string> m_Cookie;       // Cookie中的[键/值]对
    string m_ErrMsg;                    // 存放错误信息，处理全正确时，该值应为空；
};







}// end of REQUEST_SPACE
using namespace REQUEST_SPACE;
#endif // end of __REQUEST_H_20100418__
