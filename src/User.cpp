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

// ȡ�û�����Ӧ���û���Ϣ
User *User::Cache::Get(const string &username)
{
    FUNCTION_TRACK(); // �����켣����


    // ���ڶ��̲߳��������������ע�⴦��������Χ�������ͻ����
    {
        SHARE_LOCK(m_lock);

        map<string, User*>::iterator it = m_UserList.find( username );
        // �û��Ƿ�����ڻ����У�����Ч��δ��ʱ��
        if(m_UserList.end() != it && it->second->m_valid)
        {
            return it->second;
        }
    }


    {
        // ���ڶ��̲߳������������
        UNIQUE_LOCK(m_lock);

        // ���治���ڸ��û���Ϣ�����ؼ��أ�
        User *info = Load(username);
        if(NULL == info)
        {
            // ���������ݶ����Ա����ⲿ��NULLָʲ��⣩
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

// �ӻ�����ȥ���û���Ϣ
void User::Cache::Del(const string &username)
{
    FUNCTION_TRACK(); // �����켣����

    // ���ڶ��̲߳������������
    UNIQUE_LOCK(m_lock);

    delete m_UserList[username];
    m_UserList.erase(username);
}

// �����û���Ϣ�����ļ���
User *User::Cache::Load(const string &username)
{
    FUNCTION_TRACK(); // �����켣����

    User *info = new User(username);
    if(NULL == info)
    {
        LOG_ERROR("New User error: [%s]", username.c_str());
        return NULL;
    }

    // ע����ʹ�û���Ч���������������Ϣ��
    //if( ! info->isValid() )
    //{
    //    LOG_DEBUG("Invalid user: [%s]", username.c_str());
    //    delete info;
    //    return NULL;
    //}

    // ��ȥ�������ݣ�ע����ʹm_UserList[ username ]ΪNULL��delete NULLҲ���������
    delete m_UserList[ username ];
    m_UserList[ username ] = info;
    LOG_DEBUG("Loading user: [%s]... OK", username.c_str());
    return info;
}





/******************************************************************************/



// һ����ʹ�õĹ��캯��
User::User(const string &username) : m_username( username ), m_valid( true )
{
    // �û��������ļ�
    const string &cfg = GetCfgFile();

    // ����
    if( "" != username && m_mycfg.Read(cfg) < 0 )
    {
        m_valid = false;
        LOG_ERROR("Loading file error: [%s]", cfg.c_str());
    }
}

// ֻ��ע�����û�ʱ�õĹ��캯����int����ֻ������ã�
User::User(const string &username, int) : m_username( username ), m_valid( false )
{
}

User::~User()
{
}

// ���ʼ��
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

// ȡ�û�����Ӧ���û�����
User *User::Get(const string &username)
{
    FUNCTION_TRACK(); // �����켣����

    return instance()->Get(username);
}

// ��ʼ���û��ռ䣨ע��ʱ�ã�
int User::Init()
{
    int ret;
    int i;
    // �û�����Ŀ¼�б�
    const string dirs[] = {
        UserDir(),
        DataDir(),
        AttachDir(),
        CacheDir(),
        TmpDir(),
        MenuDir(),
        ""
    };

    // ������Ŀ¼
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

/* �����û����ɹ�������ָ���û�����ָ�룬ʧ�ܷ���NULL��
 * ��ע�⣺�ڶ���߲���ʱ���ⲿ����Ӧ��������
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

    // д�����Ϣ
    user.SetInfo("status", "enable");
    ret = user.Save();
    if(ret < 0)
    {
        LOG_ERROR("Create user cfg error: [%s]", username.c_str());
        return NULL;
    }

    // ����
    User *p = User::Get(username);
    if( ! p->isValid() )
    {
        return NULL;
    }
    return p;
}

// �û��������ļ����ദʹ�ã����԰��⵽һ�����У�
const string User::GetCfgFile() const
{
    return UserDir() + "my.cfg";   // [XXX]
}

// ��ǰ�û�Ŀ¼
const string User::UserDir() const
{
    return GlobalConfig::instance()->UserRootDir() + m_username + "/";
}

// ȡ�û�����·��
const string User::DataDir() const
{
    return UserDir() + "data/";
}

// ȡ��������·��
const string User::AttachDir() const
{
    return UserDir() + "attach/";
}

// ȡ����Ŀ¼
const string User::CacheDir() const
{
    return UserDir() + "cache/";
}

// ȡ�û���ʱĿ¼
const string User::TmpDir() const
{
    return UserDir() + "tmp/";
}

// ȡ��Ų˵������Ϣ��Ŀ¼
const string User::MenuDir() const
{
    return UserDir() + "menu/";
}

// ����Ч���û�����true;
bool User::isValid() const
{
    // ��������regtime�ֶΣ���˵��m_mycfg�����ݣ����û������ڣ���Ч����
    return (m_valid && "enable" == m_mycfg.Get("userinfo", "status"))
           ? true : false;
}

// ȡ�û���Ϣ�����ֶ�ָ����
const string User::GetInfo(const string &field) const
{
    return m_mycfg.Get("userinfo", field);
}
const string User::GetInfo(const string &section, const string &field) const
{
    return m_mycfg.Get(section, field);
}

// �����û���Ϣ�����ֶ�ָ����
int User::SetInfo(const string &field, const string &value)
{
    return m_mycfg.Set("userinfo", field, value);
}
int User::SetInfo(const string &section, const string &field, const string &value)
{
    return m_mycfg.Set(section, field, value);
}

// ����
int User::Save()
{
    // �û��������ļ�
    const string &cfg = GetCfgFile();
    if(m_mycfg.Write( cfg ) < 0)
    {
        LOG_ERROR("Save user cfg error: [%s]", m_username.c_str());
        return ERR;
    }
    // ���»���
    Syn();
    return OK;
}

// ��Ҫͬ�����棨ʹ������Ч���Ա��´ζ�ȡʱ���¼����û����ݣ�
void User::Syn()
{
    m_valid = false;
}

// ��������ע�⣬�����ã���Ӧ���UNIQUE_LOCK�����ʹ�ã�
Lock &User::GetLock()
{
    return m_lock;
}




}// end of USER_SPACE
