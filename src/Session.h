#ifndef __SESSION_H_20100505__
#define __SESSION_H_20100505__
#include "Request.h"
#include "Common.h"
namespace SESSION_SPACE
{





// 认证、会话
class Session
{
    // 缓存Session信息
    class Cache
    {
    public:
        Cache();
        ~Cache();

        // 取id对应的Session信息
        Session *Get(const string &id);

        // 取记录新Session信息
        void Set(const string &id, Request * request);

        // 从缓存中去除Session信息
        void Del(const string &id);

        // 清理无效（过期）Session；（定时调用）
        void Clear();

    private:
        map<string, Session*> m_SessionList; // id => Session
        Lock m_lock;
    };


public:
    ~Session();

    // 类初始化（程序启动时的初始化）
    static int init();

    // 由session id取session对象
    static Session *Get(const Request *request);
    static Session *Get(const string &id);

    // 设置session缓存
    static void Set(const string &id, Request * request);

    // 已登录（认证）返回true
    bool CheckLogin(Request * request);

    // 检查会话有效性，有效返回true
    bool isValid() const;

    // 取用户名
    const string GetUser() const;

    // 取登录ip
    const string GetIp() const;

    // 取最近活跃时间
    long GetActiveTime() const;

    // 取登录时间
    long GetLoginTime() const;

    // 使当前会话失效（比如应用于退出登录时）
    void SetInvalid();


private:
    Session(Request * request);

    // 缓存单例设置
    inline static Cache *instance(Cache *cache=NULL)
    {
        static Cache *obj = (Cache *)(Environment::instance()->ClassInit("Session::Cache", cache));
        return obj;
    }

    // 定时清理
    static void Clear(void *);


private:
    static const int KEEPALIVE_MAX = 1800; // 7200秒（2小时）[XXX]
    Request * m_request;    // 客户端请求信息（外部已经解析）
    bool m_valid;           // Session是否有效
    string m_username;
    string m_LoginIp;       // 登录时的ip
    long m_LoginTime;       // 登录时间（时间点，以秒计）
    long m_ActiveTime;      // 最近活跃时间（时间点，以秒计）
};






}// end of SESSION_SPACE
using namespace SESSION_SPACE;
#endif // end of __SESSION_H_20100505__
