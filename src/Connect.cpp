// Rocky 2010-04-12 10:08:27
#include "Common.h"
#include "Connect.h"
#include "Connect/Connect_TCP.h"
namespace CONNECT_SPACE
{


Connect::Connect()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Connect::~Connect()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}


// 跟据传入的id从工厂中产生一个处理对象
Connect *Connect::New(const string &id/*="TCP"*/)
{
    return new Connect_TCP;
}


// 做为客户端（连接到 ip:port）
int Connect::InitAsClient(const string &ip, int port)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return OK;
}

// 做为服务端（在port上侦听）
int Connect::InitAsServer(int port)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return OK;
}


// 接受连接
Connect *Connect::Accept()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    LOG_ERROR("应在子类中实现，请查看子类中是否已实现此功能。");
    return NULL;
}

// 打开
int Connect::Open()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    LOG_ERROR("应在子类中实现，请查看子类中是否已实现此功能。");
    return ERR;
}

// 关闭
int Connect::Close()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    LOG_ERROR("应在子类中实现，请查看子类中是否已实现此功能。");
    return ERR;
}

// 绑定
int Connect::Bind()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    LOG_ERROR("应在子类中实现，请查看子类中是否已实现此功能。");
    return ERR;
}

// 侦听
int Connect::Listen()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    LOG_ERROR("应在子类中实现，请查看子类中是否已实现此功能。");
    return ERR;
}

// 连接
int Connect::ConnectTo()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    LOG_ERROR("应在子类中实现，请查看子类中是否已实现此功能。");
    return ERR;
}

// 取对端ip
const string Connect::GetPeerAddr() const
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    LOG_ERROR("应在子类中实现，请查看子类中是否已实现此功能。");
    return "";
}

// 从连接上读数据，返回实际取得的字节数；
int Connect::Recv(char *buf, unsigned int len)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    LOG_ERROR("应在子类中实现，请查看子类中是否已实现此功能。");
    return 0;
}

// 写数据入连接中，返回实际写入的字节数；
int Connect::Send(const char *buf, unsigned int len)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    LOG_ERROR("应在子类中实现，请查看子类中是否已实现此功能。");
    return 0;
}

// 取一行，返回取得的字节数；
int Connect::GetLine(char *buf, unsigned int max)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    LOG_ERROR("应在子类中实现，请查看子类中是否已实现此功能。");
    return 0;
}

// 发送字符串
int Connect::Send(const char *str)
{
    return Send(str, strlen(str));
}
int Connect::Send(const string &str)
{
    return Send(str.c_str());
}








}// end of CONNECT_SPACE
