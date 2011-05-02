#ifndef __SESSION_H_20100505__
#define __SESSION_H_20100505__
#include "Request.h"
#include "Common.h"
namespace SESSION_SPACE
{





// ��֤���Ự
class Session
{
    // ����Session��Ϣ
    class Cache
    {
    public:
        Cache();
        ~Cache();

        // ȡid��Ӧ��Session��Ϣ
        Session *Get(const string &id);

        // ȡ��¼��Session��Ϣ
        void Set(const string &id, Request * request);

        // �ӻ�����ȥ��Session��Ϣ
        void Del(const string &id);

        // ������Ч�����ڣ�Session������ʱ���ã�
        void Clear();

    private:
        map<string, Session*> m_SessionList; // id => Session
        Lock m_lock;
    };


public:
    ~Session();

    // ���ʼ������������ʱ�ĳ�ʼ����
    static int init();

    // ��session idȡsession����
    static Session *Get(const Request *request);
    static Session *Get(const string &id);

    // ����session����
    static void Set(const string &id, Request * request);

    // �ѵ�¼����֤������true
    bool CheckLogin(Request * request);

    // ���Ự��Ч�ԣ���Ч����true
    bool isValid() const;

    // ȡ�û���
    const string GetUser() const;

    // ȡ��¼ip
    const string GetIp() const;

    // ȡ�����Ծʱ��
    long GetActiveTime() const;

    // ȡ��¼ʱ��
    long GetLoginTime() const;

    // ʹ��ǰ�ỰʧЧ������Ӧ�����˳���¼ʱ��
    void SetInvalid();


private:
    Session(Request * request);

    // ���浥������
    inline static Cache *instance(Cache *cache=NULL)
    {
        static Cache *obj = (Cache *)(Environment::instance()->ClassInit("Session::Cache", cache));
        return obj;
    }

    // ��ʱ����
    static void Clear(void *);


private:
    static const int KEEPALIVE_MAX = 1800; // 7200�루2Сʱ��[XXX]
    Request * m_request;    // �ͻ���������Ϣ���ⲿ�Ѿ�������
    bool m_valid;           // Session�Ƿ���Ч
    string m_username;
    string m_LoginIp;       // ��¼ʱ��ip
    long m_LoginTime;       // ��¼ʱ�䣨ʱ��㣬����ƣ�
    long m_ActiveTime;      // �����Ծʱ�䣨ʱ��㣬����ƣ�
};






}// end of SESSION_SPACE
using namespace SESSION_SPACE;
#endif // end of __SESSION_H_20100505__
