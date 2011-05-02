// Rocky 2010-05-18 10:41:54
#include "User.h"
#include "PageCfg.h"
namespace PAGECFG_SPACE
{




// ָ���û����������ļ�
PageCfg::PageCfg(const string &username, const string &cfg)
{
    if("" == username || "" == cfg)
    {
        LOG_DEBUG("PageCfg empty");
        return;
    }

    User *user = User::Get(username);
    if( ! user->isValid() )
    {
        LOG_ERROR("Invalid user: [%s]", username.c_str());
        return;
    }

    m_fullname = user->UserDir() + cfg; // ��ǰҳ������ơ������ļ�

    // ȡ������
    m_data.Read(m_fullname);
}

// ȡĳ������ֵ
const string PageCfg::Get(const string &key, const string &item) const
{
    return m_data.Get(key, item); // ������û��� ����Menu.cpp�ࣩ[XXX]
}

// ����ĳ������ֵ
const int PageCfg::Set(const string &key, const string &item, const string &value)
{
    return m_data.Set(key, item, value);
}

// ���棨д���ļ���
int PageCfg::Save()
{
    return m_data.Write(m_fullname);
}














}// end of PAGECFG_SPACE
