// Rocky 2010-05-05 22:47:29
#include "User.h"
#include "Timer.h"
#include "Session.h"
namespace SESSION_SPACE
{



Session::Cache::Cache()
{
}

Session::Cache::~Cache()
{
    map<string, Session*>::iterator it;
    for(it=m_SessionList.begin(); m_SessionList.end() != it; it++)
    {
        delete it->second;
    }
}

// 取Session对应的用户信息
Session *Session::Cache::Get(const string &id)
{
    FUNCTION_TRACK(); // 函数轨迹跟踪

    // 存在多线程操作，需加锁；
    SHARE_LOCK(m_lock);

    map<string, Session*>::iterator it = m_SessionList.find( id );
    if(m_SessionList.end() != it)
    {
        // 返回存在的记录
        return it->second;
    }

    LOG_DEBUG("Can't find session cache, id=[%s]", id.c_str());

    // 无存在，返回无数据对象（以避免外部做NULL指针检测）
    static Session empty(NULL);
    return &empty;
}

// 取记录新Session信息
void Session::Cache::Set(const string &id, Request * request)
{
    FUNCTION_TRACK(); // 函数轨迹跟踪

    // 存在多线程操作，需加锁；
    UNIQUE_LOCK(m_lock);

    Session *session = new Session(request);
    if(NULL == session)
    {
        LOG_ERROR("new Session() error, id=[%s]", id.c_str());
        return;
    }
    delete m_SessionList[ id ]; // 若存在旧数据，先释放，避免内存泄漏；
    m_SessionList[ id ] = session;
    LOG_DEBUG("Set session cache, id=[%s]", id.c_str());
}


// 清理无效（过期）Session；（定时调用）
void Session::Cache::Clear()
{
    UNIQUE_LOCK(m_lock);
    map<string, Session*>::iterator it;
    for(it = m_SessionList.begin(); m_SessionList.end() != it; it++)
    {
        const string &id = it->first;
        Session *session = it->second;
        if( ! session->isValid() )
        {
            LOG_INFO("Clear invalid session: [%s]", id.c_str());
            m_SessionList.erase(it);
        }
    }
}


/******************************************************************************/




Session::Session(Request * request)
            : m_request( request ),
              m_valid( false ),
              m_LoginTime( 0 ),
              m_ActiveTime( 0 )
{
    FUNCTION_TRACK(); // 函数轨迹跟踪

    if(NULL == request)
    {
        return;
    }

    m_LoginTime = time(NULL);
    m_ActiveTime = m_LoginTime; // 开始时，登录时间为最近活跃时间；
    m_username = request->GetField("username");
    m_LoginIp = request->GetConnect()->GetPeerAddr(); // 取出客户端连接ip
    m_valid = true; // 记为有效（注意其和初始化列表中设置为false的区别(request == NULL)）
}

Session::~Session()
{
}

// 类初始化
int Session::init()
{
    int ret;
    static Cache cache;

    Cache *obj = instance( &cache );
    if(NULL == obj)
    {
        LOG_ERROR("Session::instance() error\n");
        return ERR;
    }

    // 注册定时清理过期Session任务
    Timer::Register(Clear, NULL, "Clear invalid session.");

    return OK;
}


// 由session id取session对象
Session *Session::Get(const Request *request)
{
    const string &id = request->GetCookie("id");
    return instance()->Get(id);
}
Session *Session::Get(const string &id)
{
    return instance()->Get(id);
}

// 设置session缓存
void Session::Set(const string &id, Request * request)
{
    instance()->Set(id, request);
}

// 取用户名
const string Session::GetUser() const
{
    return m_username;
}

// 取登录ip
const string Session::GetIp() const
{
    return m_LoginIp;
}

// 取最近活跃时间
long Session::GetActiveTime() const
{
    return m_ActiveTime;
}

// 取登录时间
long Session::GetLoginTime() const
{
    return m_LoginTime;
}

// 使当前会话失效（比如应用于退出登录时）
void Session::SetInvalid()
{
    m_valid = false;
}


// 检查会话有效性，有效返回true
bool Session::isValid() const
{
    // Session是否有效
    if( ! m_valid )
    {
        LOG_DEBUG("Session invalid.");
        return false;
    }

    // 是否过期
    if( time(NULL) - m_ActiveTime > KEEPALIVE_MAX )
    {
        LOG_DEBUG("Session timeout or user not login, username: [%s]", m_username.c_str());
        return false;
    }

    // 用户是否有效
    if( ! User::Get(m_username)->isValid() )
    {
        LOG_DEBUG("Session user invalid, username: [%s]", m_username.c_str());
        return false;
    }

    // 所有检查通过，返回true；
    return true;
}

// 已登录（认证）返回true
bool Session::CheckLogin(Request * request)
{
    FUNCTION_TRACK(); // 函数轨迹跟踪

    assert(NULL != request);

    // 检查会话有效性
    if( !isValid() )
    {
        LOG_INFO("Session is invalid, username=[%s]", m_username.c_str());
        return false;
    }

    // 会话有效，再检查是不是属于此ip登录的；
    const string &ip = request->GetConnect()->GetPeerAddr(); // 取出客户端连接ip
    if(ip != m_LoginIp)
    {
        LOG_INFO("This ip[%s] is not login, username: [%s]", ip.c_str(), m_username.c_str());
        return false;
    }

    // 更新活跃时间
    m_ActiveTime = time(NULL);

    LOG_DEBUG("User has logined, session is avlid.");

    // 所有都正确，返回true；
    return true;
}


// 定时清理（由定时器循环调）
void Session::Clear(void *)
{
    // 清理缓存中的会话记录
    Session::instance()->Clear();
}














}// end of SESSION_SPACE
