#ifndef __CONNECT_H_20100412__
#define __CONNECT_H_20100412__
#include "Common.h"
namespace CONNECT_SPACE
{




// 网络连接基类
class Connect
{
public:
    // 定义智能指针类型
    typedef AutoPtr<Connect> auto_ptr;

public:
    virtual ~Connect();




    // 做为客户端（连接到 ip:port）
    virtual int InitAsClient(const string &ip, int port);

    // 做为服务端（在port上侦听）
    virtual int InitAsServer(int port);

    // 从连接上读数据，返回实际取得的字节数；
    virtual int Recv(char *buf, unsigned int len);

    // 写数据入连接中，返回实际写入的字节数；
    virtual int Send(const char *buf, unsigned int len);
    // 发送字符串
    int Send(const char *str);
    int Send(const string &str);

    // 取一行，返回实际取得的字节数；
    virtual int GetLine(char *buf, unsigned int max);



    virtual int Open();
    virtual int Close();
    virtual int Bind();
    virtual int Listen();
    // virtual int Accept(Connect &connect);
    virtual Connect *Accept();
    virtual int ConnectTo();
    virtual const string GetPeerAddr() const; // 取对端ip




    // 跟据传入的id从工厂中产生一个处理对象
    static Connect *New(const string &id="TCP");

protected:
    Connect();
};







}// end of CONNECT_SPACE
using namespace CONNECT_SPACE;
#endif // end of __CONNECT_H_20100412__
