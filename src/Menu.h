#ifndef __MENU_H_20100506__
#define __MENU_H_20100506__
#include "Common.h"
#include "Ini.h"
namespace MENU_SPACE
{






// 菜单相关操作
class Menu
{
    // 每个菜单项数据
    struct Node
    {
        string key;
        map<string, string> item;

        Node(const string &k=""): key(k)
        {
        }

        const string Get(const string &name) const
        {
            return GetMapValue(item, name);
        }

        // 指定排序项为“index”字段（从大到小）
        bool operator<(Node& rhs) const
        {
            int a = atol( GetMapValue(item, "index").c_str() );
            int b = atol( GetMapValue(rhs.item, "index").c_str() );
            return a > b;
        }
        // 按key查找
        bool operator==(const Node& rhs) const
        {
            return (key == rhs.key);
        }
    };

    // 缓存各个菜单
    class Cache
    {
    public:
        Cache();
        ~Cache();

        // 取菜单信息
        Menu *Get(const string &username, const string &menuname);

        // 从缓存中去除菜单信息
        void Del(const string &username, const string &menuname);

    private:
        // 加载菜单信息（从文件）
        Menu *Load(const string &username, const string &menuname);

    private:
        // [用户, 菜单名] => 菜单
        map< string, map<string, Menu*> > m_MenuList;
        Lock m_lock;
    };

public:
    class iterator
    {
    public:
        iterator(Menu *menu): m_menu(menu)
        {
            m_itItem = m_menu->m_items.begin();
            m_itItem--;
        }

        bool next()
        {
            m_itItem++;
            return m_menu->m_items.end() != m_itItem;
        }

        // 重载
        Node * operator->()
        {
            return &(*m_itItem);
        }

    private:
        list< Node >::iterator m_itItem;
        Menu *const m_menu;
    };

public:
    ~Menu();

    // 类初始化（程序启动时的初始化）
    static int init();

    // 取特定菜单（username--用户名，menuname--菜单名）
    static Menu *Get(const string &username, const string &menuname);

    // 取菜单项
    const string GetItem(const string &key, const string &field);

    // 设置菜单项
    int SetItem(const string &key, const string &field, const string &value);

    // 删除菜单项
    int DelItem(const string &key);

    // 保存菜单项（到文件）
    int Save();

    // 菜单有效（非空等）返回true；
    bool isValid();

    // 需要同步缓存
    void Syn();

    // 查找指定的字段（默认为title），找到则返回key值，否则返回空串；
    const string Find(const string &value, const string &field="title") const;


private:
    Menu(const string &username, const string &menuname);

    // 缓存单例设置
    inline static Cache *instance(Cache *cache=NULL)
    {
        static Cache *obj = (Cache *)(Environment::instance()->ClassInit("Menu::Cache", cache));
        return obj;
    }



private:
    list< Node > m_items;   // 菜单项（主要用于显示，需要排序等）
    Ini m_menu;             // 菜单项（和m_items内容相同，但主要用于操作，查找等）
    string m_cfg;           // 当前菜单对应的数据文件
    bool m_valid;           // 菜单是否还有效
};











}// end of MENU_SPACE
using namespace MENU_SPACE;
#endif // end of __MENU_H_20100506__
