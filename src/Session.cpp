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

// ȡSession��Ӧ���û���Ϣ
Session *Session::Cache::Get(const string &id)
{
    FUNCTION_TRACK(); // �����켣����

    // ���ڶ��̲߳������������
    SHARE_LOCK(m_lock);

    map<string, Session*>::iterator it = m_SessionList.find( id );
    if(m_SessionList.end() != it)
    {
        // ���ش��ڵļ�¼
        return it->second;
    }

    LOG_DEBUG("Can't find session cache, id=[%s]", id.c_str());

    // �޴��ڣ����������ݶ����Ա����ⲿ��NULLָʲ��⣩
    static Session empty(NULL);
    return &empty;
}

// ȡ��¼��Session��Ϣ
void Session::Cache::Set(const string &id, Request * request)
{
    FUNCTION_TRACK(); // �����켣����

    // ���ڶ��̲߳������������
    UNIQUE_LOCK(m_lock);

    Session *session = new Session(request);
    if(NULL == session)
    {
        LOG_ERROR("new Session() error, id=[%s]", id.c_str());
        return;
    }
    delete m_SessionList[ id ]; // �����ھ����ݣ����ͷţ������ڴ�й©��
    m_SessionList[ id ] = session;
    LOG_DEBUG("Set session cache, id=[%s]", id.c_str());
}


// ������Ч�����ڣ�Session������ʱ���ã�
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
    FUNCTION_TRACK(); // �����켣����

    if(NULL == request)
    {
        return;
    }

    m_LoginTime = time(NULL);
    m_ActiveTime = m_LoginTime; // ��ʼʱ����¼ʱ��Ϊ�����Ծʱ�䣻
    m_username = request->GetField("username");
    m_LoginIp = request->GetConnect()->GetPeerAddr(); // ȡ���ͻ�������ip
    m_valid = true; // ��Ϊ��Ч��ע����ͳ�ʼ���б�������Ϊfalse������(request == NULL)��
}

Session::~Session()
{
}

// ���ʼ��
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

    // ע�ᶨʱ�������Session����
    Timer::Register(Clear, NULL, "Clear invalid session.");

    return OK;
}


// ��session idȡsession����
Session *Session::Get(const Request *request)
{
    const string &id = request->GetCookie("id");
    return instance()->Get(id);
}
Session *Session::Get(const string &id)
{
    return instance()->Get(id);
}

// ����session����
void Session::Set(const string &id, Request * request)
{
    instance()->Set(id, request);
}

// ȡ�û���
const string Session::GetUser() const
{
    return m_username;
}

// ȡ��¼ip
const string Session::GetIp() const
{
    return m_LoginIp;
}

// ȡ�����Ծʱ��
long Session::GetActiveTime() const
{
    return m_ActiveTime;
}

// ȡ��¼ʱ��
long Session::GetLoginTime() const
{
    return m_LoginTime;
}

// ʹ��ǰ�ỰʧЧ������Ӧ�����˳���¼ʱ��
void Session::SetInvalid()
{
    m_valid = false;
}


// ���Ự��Ч�ԣ���Ч����true
bool Session::isValid() const
{
    // Session�Ƿ���Ч
    if( ! m_valid )
    {
        LOG_DEBUG("Session invalid.");
        return false;
    }

    // �Ƿ����
    if( time(NULL) - m_ActiveTime > KEEPALIVE_MAX )
    {
        LOG_DEBUG("Session timeout or user not login, username: [%s]", m_username.c_str());
        return false;
    }

    // �û��Ƿ���Ч
    if( ! User::Get(m_username)->isValid() )
    {
        LOG_DEBUG("Session user invalid, username: [%s]", m_username.c_str());
        return false;
    }

    // ���м��ͨ��������true��
    return true;
}

// �ѵ�¼����֤������true
bool Session::CheckLogin(Request * request)
{
    FUNCTION_TRACK(); // �����켣����

    assert(NULL != request);

    // ���Ự��Ч��
    if( !isValid() )
    {
        LOG_INFO("Session is invalid, username=[%s]", m_username.c_str());
        return false;
    }

    // �Ự��Ч���ټ���ǲ������ڴ�ip��¼�ģ�
    const string &ip = request->GetConnect()->GetPeerAddr(); // ȡ���ͻ�������ip
    if(ip != m_LoginIp)
    {
        LOG_INFO("This ip[%s] is not login, username: [%s]", ip.c_str(), m_username.c_str());
        return false;
    }

    // ���»�Ծʱ��
    m_ActiveTime = time(NULL);

    LOG_DEBUG("User has logined, session is avlid.");

    // ���ж���ȷ������true��
    return true;
}


// ��ʱ�����ɶ�ʱ��ѭ������
void Session::Clear(void *)
{
    // �������еĻỰ��¼
    Session::instance()->Clear();
}














}// end of SESSION_SPACE
