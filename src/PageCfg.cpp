// Rocky 2010-05-18 10:41:54
#include "User.h"
#include "PageCfg.h"
namespace PAGECFG_SPACE
{




// 指定用户名、配置文件
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

    m_fullname = user->UserDir() + cfg; // 当前页面的配制、数据文件

    // 取出数据
    m_data.Read(m_fullname);
}

// 取某项配制值
const string PageCfg::Get(const string &key, const string &item) const
{
    return m_data.Get(key, item); // 这里可用缓存 （及Menu.cpp类）[XXX]
}

// 设置某项配制值
const int PageCfg::Set(const string &key, const string &item, const string &value)
{
    return m_data.Set(key, item, value);
}

// 保存（写入文件）
int PageCfg::Save()
{
    return m_data.Write(m_fullname);
}














}// end of PAGECFG_SPACE
