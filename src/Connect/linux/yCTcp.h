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


//Tcp��
class yCTcp
{

//���캯��
public:
    yCTcp ();
    yCTcp (int nSock);
    virtual ~yCTcp ();

//���ز�����
public:
    int operator = (int);//��ֵ
    int operator != (int) const;//�����ڲ�����
    int operator == (int) const;//���ڲ�����

//���г�Ա����
public:
    int GetHandle () const;//ȡ��m_nSock
    int Open ();//����socket
    int Close ();//�ر�socket
    int Connect (const char *, int) const;//���ӣ�δ���ó�ʱ��
    int ConnectNoblock (const char *, int, int) const;//���ӣ����ó�ʱ��
    int Bind (const char *, int) const;//��
    int Listen (int nNum) const;//����
    int Accept () const;//��������

    int Sendn (const void *, int, int = TCP_TIMEOUT) const;//����n���ֽ�����
    int Send (const void *, int, int = TCP_TIMEOUT) const;//��������
    int Recvn (void *, int, int = TCP_TIMEOUT) const;//����n���ֽ�����
    int Recv (void *, int, int = TCP_TIMEOUT) const;//��������
    int RecvLine_rn (void *, int, int = TCP_TIMEOUT) const;//����һ������(��"\r\n"��β)
    int RecvLine_n (void *, int, int = TCP_TIMEOUT) const;//����һ������(��"\n"��β)

    int GetHostName (char *) const;//�õ�����������
    int GetPeerName (char *) const;//�õ��Է�������
    char **GetHostAddr () const;//�õ�������ַ
    int GetPeerAddr (char *) const;//�õ��Է���ַ

    int SetNoblock ();//����socketΪ������
    int SetBlock ();//����socketΪ����
    int SetReuseAddr ();//���õ�ַ������
    int SetSendTimeout (int);//���÷��ͳ�ʱֵ
    int SetRecvTimeout (int);//���ý��ճ�ʱֵ
    int SetSendBuf (int);//���÷��ͻ������Ĵ�С
    int SetRecvBuf (int);//���ý��ջ������Ĵ�С
    int SetKeepalive ();//���ô����

//public:
    static const int SOCK_ERROR;
    static const int SOCK_TIMEOUT;
    static const long TCP_TIMEOUT;

//˽�г�Ա����
private:
    int m_nSock;
};

#endif
