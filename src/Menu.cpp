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

    // ����ÿ���û�
    for(itUser=m_MenuList.begin(); m_MenuList.end() != itUser; itUser++)
    {
        // ��ǰ�û��µĲ˵�
        map<string, Menu*> &menu = itUser->second;
        map<string, Menu*>::iterator itMenu;

        // ����ÿ���˵�
        for(itMenu=menu.begin(); menu.end() != itMenu; itMenu++)
        {
            delete itMenu->second;
        }
    }
}


// ȡ�˵���Ӧ����Ϣ
Menu *Menu::Cache::Get(const string &username, const string &menuname)
{
    FUNCTION_TRACK(); // �����켣����

    // ���ڶ��̲߳��������������ע�⴦��������Χ�������ͻ����
    {
        SHARE_LOCK(m_lock);

        // �˵��Ƿ�����ڻ����У�����Ч
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
        // ���ڶ��̲߳������������
        UNIQUE_LOCK(m_lock);

        // ���治���ڸò˵���Ϣ�����ؼ��أ�
        Menu *menu = Load(username, menuname);
        if(NULL == menu)
        {
            // ���������ݶ����Ա����ⲿ��NULLָʲ��⣩
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

// �ӻ�����ȥ���˵���Ϣ
void Menu::Cache::Del(const string &username, const string &menuname)
{
    FUNCTION_TRACK(); // �����켣����

    // ���ڶ��̲߳������������
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

// ���ز˵���Ϣ�����ļ���
Menu *Menu::Cache::Load(const string &username, const string &menuname)
{
    FUNCTION_TRACK(); // �����켣����

    Menu *menu = new Menu(username, menuname);
    if(NULL == menu)
    {
        return NULL;
    }
    // ��Ч����ղ˵���
    if( ! menu->isValid() )
    {
        delete menu;
        return NULL;
    }
    // ��ȥ�������ݣ�ע����ʹm_MenuList[ username ] [ menuname ]ΪNULL��delete NULLҲ���������
    delete m_MenuList[ username ] [ menuname ];
    m_MenuList[ username ] [ menuname ] = menu;
    LOG_DEBUG("[%s:%s] Loading... ok", username.c_str(), menuname.c_str());
    return menu;
}





/******************************************************************************/



// name: �˵������ļ�������TypeDropBox.cfg��
Menu::Menu(const string &username, const string &menuname) : m_valid( true )
{
    FUNCTION_TRACK(); // �����켣����

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

    // �˵������ļ�
    m_cfg = user->MenuDir() + menuname;
    // ����
    if( m_menu.Read(m_cfg) < 0 )
    {
        LOG_ERROR("Loading file error: [%s]", m_cfg.c_str());
        return;
    }

    // ����ÿһ��浽m_items��
    Ini::iterator itIni( &m_menu );
    list< Node >::iterator itMenu;

    while( itIni.next() )
    {
        Conf *conf = itIni.GetValue();
        const char *key = itIni.GetKey();
        // LOG_DEBUG("[%s]", key);

        // �Ȳ���һ����
        itMenu = m_items.insert(m_items.begin(), Node());

        Conf::iterator itConf(conf);
        while( itConf.next() )
        {
            //LOG_DEBUG("%s=%s", itConf.GetName(), itConf.GetValue());

            // ������ֵ
            itMenu->key = key;
            itMenu->item[ itConf.GetName() ] = itConf.GetValue();
        }
    }// end of while( itIni.next()...

    // ����
    m_items.sort();

}// end of Menu::Menu(const string...

Menu::~Menu()
{
}

// ���ʼ��
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
    FUNCTION_TRACK(); // �����켣����

    return instance()->Get(username, menuname);
}

// ��Ҫͬ�����棨ʹ������Ч�������´ζ�ȡ�����¼��ز˵����ݣ�
void Menu::Syn()
{
    m_valid = false;
}

//
bool Menu::isValid()
{
    return m_items.size() != 0 && m_valid;
}

// ȡ�˵���
const string Menu::GetItem(const string &key, const string &field)
{
    return m_menu.Get(key, field);
}

// ���ò˵���
int Menu::SetItem(const string &key, const string &field, const string &value)
{
    /*
     * ע�⣬Ҫͬʱ�Ը���m_items��m_menu��
     */
    list< Node >::iterator it;
    it = std::find(m_items.begin(), m_items.end(), Node(key));
    if(m_items.end() != it)
    {
        it->item[field] = value;
    }
    else
    {
        // ��������Ȳ���һ���
        it = m_items.insert(m_items.begin(), Node());
        // ������ֵ
        it->key = key;
        it->item[ field ] = value;
    }
    return m_menu.Set(key, field, value);
}

// ɾ���˵���
int Menu::DelItem(const string &key)
{
    /*
     * ע�⣬Ҫͬʱ��m_items��m_menu������
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

// ����˵�����ļ���
int Menu::Save()
{
    // ִ���������ִ������
    m_items.sort();
    return m_menu.Write(m_cfg);
}

// ����ָ�����ֶΣ�Ĭ��Ϊtitle�����ҵ��򷵻�keyֵ�����򷵻ؿմ���
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
