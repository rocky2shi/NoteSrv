// Rocky 2010-04-12 10:08:27
#include "Common.h"
#include "Connect.h"
#include "Connect/Connect_TCP.h"
namespace CONNECT_SPACE
{


Connect::Connect()
{
    FUNCTION_TRACK(); // �����켣����
}

Connect::~Connect()
{
    FUNCTION_TRACK(); // �����켣����
}


// ���ݴ����id�ӹ����в���һ���������
Connect *Connect::New(const string &id/*="TCP"*/)
{
    return new Connect_TCP;
}


// ��Ϊ�ͻ��ˣ����ӵ� ip:port��
int Connect::InitAsClient(const string &ip, int port)
{
    FUNCTION_TRACK(); // �����켣����
    return OK;
}

// ��Ϊ����ˣ���port��������
int Connect::InitAsServer(int port)
{
    FUNCTION_TRACK(); // �����켣����
    return OK;
}


// ��������
Connect *Connect::Accept()
{
    FUNCTION_TRACK(); // �����켣����
    LOG_ERROR("Ӧ��������ʵ�֣���鿴�������Ƿ���ʵ�ִ˹��ܡ�");
    return NULL;
}

// ��
int Connect::Open()
{
    FUNCTION_TRACK(); // �����켣����
    LOG_ERROR("Ӧ��������ʵ�֣���鿴�������Ƿ���ʵ�ִ˹��ܡ�");
    return ERR;
}

// �ر�
int Connect::Close()
{
    FUNCTION_TRACK(); // �����켣����
    LOG_ERROR("Ӧ��������ʵ�֣���鿴�������Ƿ���ʵ�ִ˹��ܡ�");
    return ERR;
}

// ��
int Connect::Bind()
{
    FUNCTION_TRACK(); // �����켣����
    LOG_ERROR("Ӧ��������ʵ�֣���鿴�������Ƿ���ʵ�ִ˹��ܡ�");
    return ERR;
}

// ����
int Connect::Listen()
{
    FUNCTION_TRACK(); // �����켣����
    LOG_ERROR("Ӧ��������ʵ�֣���鿴�������Ƿ���ʵ�ִ˹��ܡ�");
    return ERR;
}

// ����
int Connect::ConnectTo()
{
    FUNCTION_TRACK(); // �����켣����
    LOG_ERROR("Ӧ��������ʵ�֣���鿴�������Ƿ���ʵ�ִ˹��ܡ�");
    return ERR;
}

// ȡ�Զ�ip
const string Connect::GetPeerAddr() const
{
    FUNCTION_TRACK(); // �����켣����
    LOG_ERROR("Ӧ��������ʵ�֣���鿴�������Ƿ���ʵ�ִ˹��ܡ�");
    return "";
}

// �������϶����ݣ�����ʵ��ȡ�õ��ֽ�����
int Connect::Recv(char *buf, unsigned int len)
{
    FUNCTION_TRACK(); // �����켣����
    LOG_ERROR("Ӧ��������ʵ�֣���鿴�������Ƿ���ʵ�ִ˹��ܡ�");
    return 0;
}

// д�����������У�����ʵ��д����ֽ�����
int Connect::Send(const char *buf, unsigned int len)
{
    FUNCTION_TRACK(); // �����켣����
    LOG_ERROR("Ӧ��������ʵ�֣���鿴�������Ƿ���ʵ�ִ˹��ܡ�");
    return 0;
}

// ȡһ�У�����ȡ�õ��ֽ�����
int Connect::GetLine(char *buf, unsigned int max)
{
    FUNCTION_TRACK(); // �����켣����
    LOG_ERROR("Ӧ��������ʵ�֣���鿴�������Ƿ���ʵ�ִ˹��ܡ�");
    return 0;
}

// �����ַ���
int Connect::Send(const char *str)
{
    return Send(str, strlen(str));
}
int Connect::Send(const string &str)
{
    return Send(str.c_str());
}








}// end of CONNECT_SPACE
