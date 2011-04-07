// Rocky 2010-04-12 19:13:25
#include "Connect_TCP.h"
namespace CONNECT_TCP_SPACE
{


Connect_TCP::Connect_TCP() : m_bClose(false)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Connect_TCP::~Connect_TCP()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}



// 关闭
int Connect_TCP::Close()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    if( m_bClose )
    {
        LOG_ERROR("警告：重复执行关闭操作！");
        return ERR;
    }
    delete this;
    m_bClose = true;
    return OK;
}

// 做为服务端（在port上侦听）
int Connect_TCP::InitAsServer(int port)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    /*
     *  执行建立tcp服务的标准步骤：
     */

    if(!m_tcp.Open())
    {
        LOG_ERROR("err: Open()");
        return ERR;
    }

    if(!m_tcp.Bind(0, port))
    {
        LOG_ERROR("err: Bind()");
        return ERR;
    }

    if(!m_tcp.Listen(1000))
    {
        LOG_ERROR("err: Listen(1000)");
        return ERR;
    }

    return OK;
}

// 接受连接
Connect *Connect_TCP::Accept()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    // 新连接
    int n = m_tcp.Accept();
    if(n < 0)
    {
        LOG_ERROR("err: Accept()");
        return NULL;
    }

    // 返回的新对象
    Connect_TCP *pNew = new Connect_TCP;    // 注意，此操作将在
    if(NULL == pNew)
    {
        yCTcp(n).Close();
        LOG_ERROR("创建Connect_TCP对象出错");
        return NULL;
    }
    pNew->m_tcp = n;

    return pNew;
}

// 写数据入连接中，返回实际写入的字节数；
int Connect_TCP::Send(const char *buf, unsigned int len)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    if( len <= 0 )
    {
        return len;
    }

    int ret = m_tcp.Send(buf, len, TCP_TIMEOUT*10);//, TCP_TIMEOUT*20); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    return ret > 0 ? ret : 0;
}

// 从连接上读数据，返回实际取得的字节数；
int Connect_TCP::Recv(char *buf, unsigned int len)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    int ret = m_tcp.Recv(buf, len);//, TCP_TIMEOUT*20);

    return ret > 0 ? ret : 0;
}

// 取对端ip
const string Connect_TCP::GetPeerAddr() const
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    char buf[20] = "";
    m_tcp.GetPeerAddr(buf);
    return buf;
}

// 取一行，返回取得的字节数；
int Connect_TCP::GetLine(char *buf, unsigned int max)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return m_tcp.RecvLine_n(buf, max); //, TCP_TIMEOUT*20);
}




}// end of CONNECT_TCP_SPACE
