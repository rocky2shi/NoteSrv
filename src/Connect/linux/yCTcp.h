#ifndef __YCTCP_H__
#define __YCTCP_H__

/*
    Copyright (C) 2000 uchen (uchen@sohu.com)

    This software is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.
*/

const long TCP_TIMEOUT                      = 500000;
const int SOCK_ERROR                        = -100;
const int SOCK_TIMEOUT                      = -101;


//Tcp类
class yCTcp
{

//构造函数
public:
    yCTcp ();
    yCTcp (int nSock);
    virtual ~yCTcp ();

//重载操作符
public:
    int operator = (int);//赋值
    int operator != (int) const;//不等于操作符
    int operator == (int) const;//等于操作符

//公有成员函数
public:
    int GetHandle () const;//取出m_nSock
    int Open ();//创建socket
    int Close ();//关闭socket
    int Connect (const char *, int) const;//连接（未设置超时）
    int ConnectNoblock (const char *, int, int) const;//连接（设置超时）
    int Bind (const char *, int) const;//绑定
    int Listen (int nNum) const;//监听
    int Accept () const;//接受连接

    int Sendn (const void *, int, int = TCP_TIMEOUT) const;//发送n个字节数据
    int Send (const void *, int, int = TCP_TIMEOUT) const;//发送数据
    int Recvn (void *, int, int = TCP_TIMEOUT) const;//接收n个字节数据
    int Recv (void *, int, int = TCP_TIMEOUT) const;//接收数据
    int RecvLine_rn (void *, int, int = TCP_TIMEOUT) const;//接收一行数据(以"\r\n"结尾)
    int RecvLine_n (void *, int, int = TCP_TIMEOUT) const;//接收一行数据(以"\n"结尾)

    int GetHostName (char *) const;//得到本机机器名
    int GetPeerName (char *) const;//得到对方机器名
    char **GetHostAddr () const;//得到本机地址
    int GetPeerAddr (char *) const;//得到对方地址

    int SetNoblock ();//设置socket为非阻塞
    int SetBlock ();//设置socket为阻塞
    int SetReuseAddr ();//设置地址可重用
    int SetSendTimeout (int);//设置发送超时值
    int SetRecvTimeout (int);//设置接收超时值
    int SetSendBuf (int);//设置发送缓冲区的大小
    int SetRecvBuf (int);//设置接收缓冲区的大小
    int SetKeepalive ();//设置存活检测

//public:
    static const int SOCK_ERROR;
    static const int SOCK_TIMEOUT;
    static const long TCP_TIMEOUT;

//私有成员变量
private:
    int m_nSock;
};

#endif
