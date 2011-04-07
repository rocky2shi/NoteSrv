// Rocky 2010-05-06 16:00:44
#include "Menu.h"
#include "User.h"
namespace MENU_SPACE
{












Menu::Cache::Cache()
{
}

Menu::Cache::~Cache()
{
    map< string, map<string, Menu*> >::iterator itUser;

    // 遍历每个用户
    for(itUser=m_MenuList.begin(); m_MenuList.end() != itUser; itUser++)
    {
        // 当前用户下的菜单
        map<string, Menu*> &menu = itUser->second;
        map<string, Menu*>::iterator itMenu;

        // 遍历每个菜单
        for(itMenu=menu.begin(); menu.end() != itMenu; itMenu++)
        {
            delete itMenu->second;
        }
    }
}


// 取菜单对应的信息
Menu *Menu::Cache::Get(const string &username, const string &menuname)
{
    FUNCTION_TRACK(); // 函数轨迹跟踪

    // 存在多线程操作，需加锁；（注意处理锁定范围，避免冲突；）
    {
        SHARE_LOCK(m_lock);

        // 菜单是否存在于缓存中，且有效
        map< string, map<string, Menu*> >::iterator itUser = m_MenuList.find( username );
        if( m_MenuList.end() != itUser )
        {
            map<string, Menu*> &menu = itUser->second;
            map<string, Menu*>::iterator itMenu = menu.find( menuname );
            if(menu.end() != itMenu)
            {
                return itMenu->second;
            }
        }
    }

    {
        // 存在多线程操作，需加锁；
        UNIQUE_LOCK(m_lock);

        // 缓存不存在该菜单信息，则重加载；
        Menu *menu = Load(username, menuname);
        if(NULL == menu)
        {
            // 返回无数据对象（以避免外部做NULL指什检测）
            struct Empty : public Menu
            {
                Empty(): Menu("", "")
                {
                }
            };
            static Empty menu;
            return &menu;
        }
        return menu;
    }
}

// 从缓存中去除菜单信息
void Menu::Cache::Del(const string &username, const string &menuname)
{
    FUNCTION_TRACK(); // 函数轨迹跟踪

    // 存在多线程操作，需加锁；
    UNIQUE_LOCK(m_lock);

    map< string, map<string, Menu*> >::iterator itUser = m_MenuList.find( username );
    if( m_MenuList.end() != itUser )
    {
        map<string, Menu*> &menu = itUser->second;
        map<string, Menu*>::iterator itMenu = menu.find( menuname );
        if(menu.end() != itMenu)
        {
            delete itMenu->second;
            menu.erase( itMenu );
        }

        if(menu.size() == 0)
        {
            m_MenuList.erase( itUser );
        }
    }
}

// 加载菜单信息（从文件）
Menu *Menu::Cache::Load(const string &username, const string &menuname)
{
    FUNCTION_TRACK(); // 函数轨迹跟踪

    Menu *menu = new Menu(username, menuname);
    if(NULL == menu)
    {
        return NULL;
    }
    // 无效（或空菜单）
    if( ! menu->isValid() )
    {
        delete menu;
        return NULL;
    }
    // 先去掉旧数据（注：即使m_MenuList[ username ] [ menuname ]为NULL，delete NULL也不会出错；）
    delete m_MenuList[ username ] [ menuname ];
    m_MenuList[ username ] [ menuname ] = menu;
    LOG_DEBUG("[%s:%s] Loading... ok", username.c_str(), menuname.c_str());
    return menu;
}





/******************************************************************************/



// name: 菜单配置文件名，如TypeDropBox.cfg；
Menu::Menu(const string &username, const string &menuname) : m_valid( true )
{
    FUNCTION_TRACK(); // 函数轨迹跟踪

    if("" == username || "" == menuname)
    {
        LOG_DEBUG("username or menuname empty");
        return;
    }

    User *user = User::Get(username);
    if( !user->isValid() )
    {
        LOG_ERROR("Invalue user: [%s]", username.c_str());
        return;
    }

    // 菜单配置文件
    m_cfg = user->MenuDir() + menuname;
    // 加载
    if( m_menu.Read(m_cfg) < 0 )
    {
        LOG_ERROR("Loading file error: [%s]", m_cfg.c_str());
        return;
    }

    // 读出每一项，存到m_items中
    Ini::iterator itIni( &m_menu );
    list< Node >::iterator itMenu;

    while( itIni.next() )
    {
        Conf *conf = itIni.GetValue();
        const char *key = itIni.GetKey();
        // LOG_DEBUG("[%s]", key);

        // 先插入一空项
        itMenu = m_items.insert(m_items.begin(), Node());

        Conf::iterator itConf(conf);
        while( itConf.next() )
        {
            //LOG_DEBUG("%s=%s", itConf.GetName(), itConf.GetValue());

            // 再设置值
            itMenu->key = key;
            itMenu->item[ itConf.GetName() ] = itConf.GetValue();
        }
    }// end of while( itIni.next()...

    // 排序
    m_items.sort();

}// end of Menu::Menu(const string...

Menu::~Menu()
{
}

// 类初始化
int Menu::init()
{
    int ret;
    static Cache cache;

    Cache *obj = instance( &cache );
    if(NULL == obj)
    {
        LOG_ERROR("Menu::instance() error\n");
        return ERR;
    }

    return OK;
}

Menu *Menu::Get(const string &username, const string &menuname)
{
    FUNCTION_TRACK(); // 函数轨迹跟踪

    return instance()->Get(username, menuname);
}

// 需要同步缓存（使缓存无效，以像下次读取是重新加载菜单数据）
void Menu::Syn()
{
    m_valid = false;
}

//
bool Menu::isValid()
{
    return m_items.size() != 0 && m_valid;
}

// 取菜单项
const string Menu::GetItem(const string &key, const string &field)
{
    return m_menu.Get(key, field);
}

// 设置菜单项
int Menu::SetItem(const string &key, const string &field, const string &value)
{
    /*
     * 注意，要同时对更新m_items和m_menu；
     */
    list< Node >::iterator it;
    it = std::find(m_items.begin(), m_items.end(), Node(key));
    if(m_items.end() != it)
    {
        it->item[field] = value;
    }
    else
    {
        // 插入新项（先插入一空项）
        it = m_items.insert(m_items.begin(), Node());
        // 再设置值
        it->key = key;
        it->item[ field ] = value;
    }
    return m_menu.Set(key, field, value);
}

// 删除菜单项
int Menu::DelItem(const string &key)
{
    /*
     * 注意，要同时对m_items和m_menu操作；
     */
    list< Node >::iterator it;
    it = std::find(m_items.begin(), m_items.end(), Node(key));
    if(m_items.end() != it)
    {
        m_items.erase(it);
    }
    m_menu.Del(key);
    return OK;
}

// 保存菜单项（到文件）
int Menu::Save()
{
    // 执行设置完后，执行排序；
    m_items.sort();
    return m_menu.Write(m_cfg);
}

// 查找指定的字段（默认为title），找到则返回key值，否则返回空串；
const string Menu::Find(const string &value, const string &field/*="title"*/) const
{
    list< Node >::const_iterator it;
    for(it = m_items.begin(); m_items.end() != it; it++)
    {
        if(value == it->Get(field))
        {
            return it->key;
        }
    }
    return "";
}




















}// end of MENU_SPACE
