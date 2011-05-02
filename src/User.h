#ifndef __USER_H_20100504__
#define __USER_H_20100504__
#include "Common.h"
#include "Ini.h"
namespace USER_SPACE
{






// ��Ե����û�
class User
{
    // �����û���Ϣ
    class Cache
    {
    public:
        Cache();
        ~Cache();

        // ȡ�û�����Ӧ���û���Ϣ
        User *Get(const string &username);

        // �ӻ�����ȥ���û���Ϣ
        void Del(const string &username);

    private:
        // �����û���Ϣ�����ļ���
        User *Load(const string &username);

    private:
        map<string, User*> m_UserList;  // username => User
        Lock m_lock;
    };


public:
    ~User();

    // ���ʼ������������ʱ�ĳ�ʼ����
    static int init();

    // ȡ�û�����Ӧ���û�����
    static User *Get(const string &username);

    // �����û����ɹ�������ָ���û�����ָ�룬ʧ�ܷ���NULL��
    static User *Create(const string &username);

    // ��ǰ�û�Ŀ¼
    const string UserDir() const;

    // ȡ�û�����·��
    const string DataDir() const;

    // ȡ��������·��
    const string AttachDir() const;

    // ȡ����Ŀ¼
    const string CacheDir() const;

    // ȡ�û���ʱĿ¼
    const string TmpDir() const;

    // ȡ��Ų˵������Ϣ��Ŀ¼
    const string MenuDir() const;

    // ����Ч���û�����true;
    bool isValid() const;

    // ȡ�û���Ϣ�����ֶ�ָ����
    const string GetInfo(const string &section, const string &field) const;
    // ȡ�û���Ϣ��sectionĬ��Ϊ"userinfo"��
    const string GetInfo(const string &field) const;


    // �����û���Ϣ�����ֶ�ָ����
    int SetInfo(const string &field, const string &value);
    int SetInfo(const string &section, const string &field, const string &value);

    // ����
    int Save();

    // ��Ҫͬ������
    void Syn();

    // ��������ע�⣬�����ã���Ӧ���UNIQUE_LOCK�����ʹ�ã�
    Lock &GetLock();

private:
    // һ����ʹ�õĹ��캯��
    User(const string &username);
    // ֻ��ע�����û�ʱ�õĹ��캯��
    User(const string &username, int);

    // ���浥������
    inline static Cache *instance(Cache *cache=NULL)
    {
        static Cache *obj = (Cache *)(Environment::instance()->ClassInit("User::Cache", cache));
        return obj;
    }

    // ��ʼ���û��ռ䣨ע��ʱ�ã�
    int Init();

    // �û��������ļ����ദʹ�ã����԰��⵽һ�����У�
    const string GetCfgFile() const;



private:
    const string m_username;    // �û���
    Ini m_mycfg;     // �û��������ļ�
    bool m_valid;    // �û���Ч������δ��ʱ�ȣ�
    Lock m_lock;     // �����޸��û���Ϣ�����¼ʱ�������
};





}// end of USER_SPACE
using namespace USER_SPACE;
#endif // end of __USER_H_20100504__
