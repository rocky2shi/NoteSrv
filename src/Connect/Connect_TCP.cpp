// Rocky 2010-04-12 19:13:25
#include "Connect_TCP.h"
namespace CONNECT_TCP_SPACE
{


Connect_TCP::Connect_TCP() : m_bClose(false)
{
    FUNCTION_TRACK(); // �����켣����
}

Connect_TCP::~Connect_TCP()
{
    FUNCTION_TRACK(); // �����켣����
}



// �ر�
int Connect_TCP::Close()
{
    FUNCTION_TRACK(); // �����켣����
    if( m_bClose )
    {
        LOG_ERROR("���棺�ظ�ִ�йرղ�����");
        return ERR;
    }
    delete this;
    m_bClose = true;
    return OK;
}

// ��Ϊ����ˣ���port��������
int Connect_TCP::InitAsServer(int port)
{
    FUNCTION_TRACK(); // �����켣����

    /*
     *  ִ�н���tcp����ı�׼���裺
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

// ��������
Connect *Connect_TCP::Accept()
{
    FUNCTION_TRACK(); // �����켣����

    // ������
    int n = m_tcp.Accept();
    if(n < 0)
    {
        LOG_ERROR("err: Accept()");
        return NULL;
    }

    // ���ص��¶���
    Connect_TCP *pNew = new Connect_TCP;    // ע�⣬�˲�������
    if(NULL == pNew)
    {
        yCTcp(n).Close();
        LOG_ERROR("����Connect_TCP�������");
        return NULL;
    }
    pNew->m_tcp = n;

    return pNew;
}

// д�����������У�����ʵ��д����ֽ�����
int Connect_TCP::Send(const char *buf, unsigned int len)
{
    FUNCTION_TRACK(); // �����켣����

    if( len <= 0 )
    {
        return len;
    }

    int ret = m_tcp.Send(buf, len, TCP_TIMEOUT*10);//, TCP_TIMEOUT*20); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    return ret > 0 ? ret : 0;
}

// �������϶����ݣ�����ʵ��ȡ�õ��ֽ�����
int Connect_TCP::Recv(char *buf, unsigned int len)
{
    FUNCTION_TRACK(); // �����켣����

    int ret = m_tcp.Recv(buf, len);//, TCP_TIMEOUT*20);

    return ret > 0 ? ret : 0;
}

// ȡ�Զ�ip
const string Connect_TCP::GetPeerAddr() const
{
    FUNCTION_TRACK(); // �����켣����

    char buf[20] = "";
    m_tcp.GetPeerAddr(buf);
    return buf;
}

// ȡһ�У�����ȡ�õ��ֽ�����
int Connect_TCP::GetLine(char *buf, unsigned int max)
{
    FUNCTION_TRACK(); // �����켣����

    return m_tcp.RecvLine_n(buf, max); //, TCP_TIMEOUT*20);
}




}// end of CONNECT_TCP_SPACE
