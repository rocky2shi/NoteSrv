// Rocky 2010-05-04 15:45:09
#include "User.h"
namespace USER_SPACE
{









User::Cache::Cache()
{
}

User::Cache::~Cache()
{
    map<string, User*>::iterator it;
    for(it=m_UserList.begin(); m_UserList.end() != it; it++)
    {
        delete it->second;
    }
}

// 取用户名对应的用户信息
User *User::Cache::Get(const string &username)
{
    FUNCTION_TRACK(); // 函数轨迹跟踪


    // 存在多线程操作，需加锁；（注意处理锁定范围，避免冲突；）
    {
        SHARE_LOCK(m_lock);

        map<string, User*>::iterator it = m_UserList.find( username );
        // 用户是否存在于缓存中，且有效（未超时）
        if(m_UserList.end() != it && it->second->m_valid)
        {
            return it->second;
        }
    }


    {
        // 存在多线程操作，需加锁；
        UNIQUE_LOCK(m_lock);

        // 缓存不存在该用户信息，则重加载；
        User *info = Load(username);
        if(NULL == info)
        {
            // 返回无数据对象（以避免外部做NULL指什检测）
            struct Empty : public User
            {
                Empty(): User("")
                {
                }
            };
            static Empty user;
            return &user;
        }
        return info;
    }
}

// 从缓存中去除用户信息
void User::Cache::Del(const string &username)
{
    FUNCTION_TRACK(); // 函数轨迹跟踪

    // 存在多线程操作，需加锁；
    UNIQUE_LOCK(m_lock);

    delete m_UserList[username];
    m_UserList.erase(username);
}

// 加载用户信息（从文件）
User *User::Cache::Load(const string &username)
{
    FUNCTION_TRACK(); // 函数轨迹跟踪

    User *info = new User(username);
    if(NULL == info)
    {
        LOG_ERROR("New User error: [%s]", username.c_str());
        return NULL;
    }

    // 注：即使用户无效，但乃需加载其信息。
    //if( ! info->isValid() )
    //{
    //    LOG_DEBUG("Invalid user: [%s]", username.c_str());
    //    delete info;
    //    return NULL;
    //}

    // 先去掉旧数据（注：即使m_UserList[ username ]为NULL，delete NULL也不会出错；）
    delete m_UserList[ username ];
    m_UserList[ username ] = info;
    LOG_DEBUG("Loading user: [%s]... OK", username.c_str());
    return info;
}





/******************************************************************************/



// 一般性使用的构造函数
User::User(const string &username) : m_username( username ), m_valid( true )
{
    // 用户主配置文件
    const string &cfg = GetCfgFile();

    // 加载
    if( "" != username && m_mycfg.Read(cfg) < 0 )
    {
        m_valid = false;
        LOG_ERROR("Loading file error: [%s]", cfg.c_str());
    }
}

// 只在注册新用户时用的构造函数（int参数只做标记用）
User::User(const string &username, int) : m_username( username ), m_valid( false )
{
}

User::~User()
{
}

// 类初始化
int User::init()
{
    int ret;
    static Cache cache;

    Cache *obj = instance( &cache );
    if(NULL == obj)
    {
        LOG_ERROR("User::instance() error\n");
        return ERR;
    }

    return OK;
}

// 取用户名对应的用户对象
User *User::Get(const string &username)
{
    FUNCTION_TRACK(); // 函数轨迹跟踪

    return instance()->Get(username);
}

// 初始化用户空间（注册时用）
int User::Init()
{
    int ret;
    int i;
    // 用户基本目录列表
    const string dirs[] = {
        UserDir(),
        DataDir(),
        AttachDir(),
        CacheDir(),
        TmpDir(),
        MenuDir(),
        ""
    };

    // 创建各目录
    for(i = 0; "" != dirs[i]; i++)
    {
        const string &dir = dirs[i];
        LOG_DEBUG("dir=[%s]", dir.c_str());
        ret = CreateDir( dir );
        if(ret < 0)
        {
            LOG_ERROR("Create dir error: [%s]", dir.c_str());
            return ERR;
        }
    }

    return OK;
}

/* 创建用户（成功：返回指向用户对象指针，失败返回NULL）
 * （注意：在多程线操作时，外部调用应加锁；）
 */
User *User::Create(const string &username)
{
    User user(username, 0);
    int ret = user.Init();
    if(ret < 0)
    {
        LOG_ERROR("Init user error: [%s]", username.c_str());
        return NULL;
    }

    // 写标记信息
    user.SetInfo("status", "enable");
    ret = user.Save();
    if(ret < 0)
    {
        LOG_ERROR("Create user cfg error: [%s]", username.c_str());
        return NULL;
    }

    // 加载
    User *p = User::Get(username);
    if( ! p->isValid() )
    {
        return NULL;
    }
    return p;
}

// 用户主配置文件（多处使用，所以包封到一函数中）
const string User::GetCfgFile() const
{
    return UserDir() + "my.cfg";   // [XXX]
}

// 当前用户目录
const string User::UserDir() const
{
    return GlobalConfig::instance()->UserRootDir() + m_username + "/";
}

// 取用户数据路径
const string User::DataDir() const
{
    return UserDir() + "data/";
}

// 取附件数据路径
const string User::AttachDir() const
{
    return UserDir() + "attach/";
}

// 取缓存目录
const string User::CacheDir() const
{
    return UserDir() + "cache/";
}

// 取用户临时目录
const string User::TmpDir() const
{
    return UserDir() + "tmp/";
}

// 取存放菜单相关信息的目录
const string User::MenuDir() const
{
    return UserDir() + "menu/";
}

// 是有效的用户返回true;
bool User::isValid() const
{
    // 若不存在regtime字段，则说明m_mycfg无数据，即用户不存在（无效）；
    return (m_valid && "enable" == m_mycfg.Get("userinfo", "status"))
           ? true : false;
}

// 取用户信息（由字段指定）
const string User::GetInfo(const string &field) const
{
    return m_mycfg.Get("userinfo", field);
}
const string User::GetInfo(const string &section, const string &field) const
{
    return m_mycfg.Get(section, field);
}

// 设置用户信息（由字段指定）
int User::SetInfo(const string &field, const string &value)
{
    return m_mycfg.Set("userinfo", field, value);
}
int User::SetInfo(const string &section, const string &field, const string &value)
{
    return m_mycfg.Set(section, field, value);
}

// 保存
int User::Save()
{
    // 用户主配置文件
    const string &cfg = GetCfgFile();
    if(m_mycfg.Write( cfg ) < 0)
    {
        LOG_ERROR("Save user cfg error: [%s]", m_username.c_str());
        return ERR;
    }
    // 更新缓存
    Syn();
    return OK;
}

// 需要同步缓存（使缓存无效，以便下次读取时重新加载用户数据）
void User::Syn()
{
    m_valid = false;
}

// 返回锁（注意，是引用）（应配合UNIQUE_LOCK宏等来使用）
Lock &User::GetLock()
{
    return m_lock;
}




}// end of USER_SPACE
