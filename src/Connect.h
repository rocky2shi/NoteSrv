#ifndef __CONNECT_H_20100412__
#define __CONNECT_H_20100412__
#include "Common.h"
namespace CONNECT_SPACE
{




// �������ӻ���
class Connect
{
public:
    // ��������ָ������
    typedef AutoPtr<Connect> auto_ptr;

public:
    virtual ~Connect();




    // ��Ϊ�ͻ��ˣ����ӵ� ip:port��
    virtual int InitAsClient(const string &ip, int port);

    // ��Ϊ����ˣ���port��������
    virtual int InitAsServer(int port);

    // �������϶����ݣ�����ʵ��ȡ�õ��ֽ�����
    virtual int Recv(char *buf, unsigned int len);

    // д�����������У�����ʵ��д����ֽ�����
    virtual int Send(const char *buf, unsigned int len);
    // �����ַ���
    int Send(const char *str);
    int Send(const string &str);

    // ȡһ�У�����ʵ��ȡ�õ��ֽ�����
    virtual int GetLine(char *buf, unsigned int max);



    virtual int Open();
    virtual int Close();
    virtual int Bind();
    virtual int Listen();
    // virtual int Accept(Connect &connect);
    virtual Connect *Accept();
    virtual int ConnectTo();
    virtual const string GetPeerAddr() const; // ȡ�Զ�ip




    // ���ݴ����id�ӹ����в���һ���������
    static Connect *New(const string &id="TCP");

protected:
    Connect();
};







}// end of CONNECT_SPACE
using namespace CONNECT_SPACE;
#endif // end of __CONNECT_H_20100412__
