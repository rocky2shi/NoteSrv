#ifndef __USER_H_20100504__
#define __USER_H_20100504__
#include "Common.h"
#include "Ini.h"
namespace USER_SPACE
{






// 针对单个用户
class User
{
    // 缓存用户信息
    class Cache
    {
    public:
        Cache();
        ~Cache();

        // 取用户名对应的用户信息
        User *Get(const string &username);

        // 从缓存中去除用户信息
        void Del(const string &username);

    private:
        // 加载用户信息（从文件）
        User *Load(const string &username);

    private:
        map<string, User*> m_UserList;  // username => User
        Lock m_lock;
    };


public:
    ~User();

    // 类初始化（程序启动时的初始化）
    static int init();

    // 取用户名对应的用户对象
    static User *Get(const string &username);

    // 创建用户（成功：返回指向用户对象指针，失败返回NULL）
    static User *Create(const string &username);

    // 当前用户目录
    const string UserDir() const;

    // 取用户数据路径
    const string DataDir() const;

    // 取附件数据路径
    const string AttachDir() const;

    // 取缓存目录
    const string CacheDir() const;

    // 取用户临时目录
    const string TmpDir() const;

    // 取存放菜单相关信息的目录
    const string MenuDir() const;

    // 是有效的用户返回true;
    bool isValid() const;

    // 取用户信息（由字段指定）
    const string GetInfo(const string &section, const string &field) const;
    // 取用户信息（section默认为"userinfo"）
    const string GetInfo(const string &field) const;


    // 设置用户信息（由字段指定）
    int SetInfo(const string &field, const string &value);
    int SetInfo(const string &section, const string &field, const string &value);

    // 保存
    int Save();

    // 需要同步缓存
    void Syn();

    // 返回锁（注意，是引用）（应配合UNIQUE_LOCK宏等来使用）
    Lock &GetLock();

private:
    // 一般性使用的构造函数
    User(const string &username);
    // 只在注册新用户时用的构造函数
    User(const string &username, int);

    // 缓存单例设置
    inline static Cache *instance(Cache *cache=NULL)
    {
        static Cache *obj = (Cache *)(Environment::instance()->ClassInit("User::Cache", cache));
        return obj;
    }

    // 初始化用户空间（注册时用）
    int Init();

    // 用户主配置文件（多处使用，所以包封到一函数中）
    const string GetCfgFile() const;



private:
    const string m_username;    // 用户名
    Ini m_mycfg;     // 用户主配置文件
    bool m_valid;    // 用户有效（比如未超时等）
    Lock m_lock;     // 当有修改用户信息，如登录时等需加锁
};





}// end of USER_SPACE
using namespace USER_SPACE;
#endif // end of __USER_H_20100504__
