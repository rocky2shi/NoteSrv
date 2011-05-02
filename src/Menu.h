#ifndef __MENU_H_20100506__
#define __MENU_H_20100506__
#include "Common.h"
#include "Ini.h"
namespace MENU_SPACE
{






// �˵���ز���
class Menu
{
    // ÿ���˵�������
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

        // ָ��������Ϊ��index���ֶΣ��Ӵ�С��
        bool operator<(Node& rhs) const
        {
            int a = atol( GetMapValue(item, "index").c_str() );
            int b = atol( GetMapValue(rhs.item, "index").c_str() );
            return a > b;
        }
        // ��key����
        bool operator==(const Node& rhs) const
        {
            return (key == rhs.key);
        }
    };

    // ��������˵�
    class Cache
    {
    public:
        Cache();
        ~Cache();

        // ȡ�˵���Ϣ
        Menu *Get(const string &username, const string &menuname);

        // �ӻ�����ȥ���˵���Ϣ
        void Del(const string &username, const string &menuname);

    private:
        // ���ز˵���Ϣ�����ļ���
        Menu *Load(const string &username, const string &menuname);

    private:
        // [�û�, �˵���] => �˵�
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

        // ����
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

    // ���ʼ������������ʱ�ĳ�ʼ����
    static int init();

    // ȡ�ض��˵���username--�û�����menuname--�˵�����
    static Menu *Get(const string &username, const string &menuname);

    // ȡ�˵���
    const string GetItem(const string &key, const string &field);

    // ���ò˵���
    int SetItem(const string &key, const string &field, const string &value);

    // ɾ���˵���
    int DelItem(const string &key);

    // ����˵�����ļ���
    int Save();

    // �˵���Ч���ǿյȣ�����true��
    bool isValid();

    // ��Ҫͬ������
    void Syn();

    // ����ָ�����ֶΣ�Ĭ��Ϊtitle�����ҵ��򷵻�keyֵ�����򷵻ؿմ���
    const string Find(const string &value, const string &field="title") const;


private:
    Menu(const string &username, const string &menuname);

    // ���浥������
    inline static Cache *instance(Cache *cache=NULL)
    {
        static Cache *obj = (Cache *)(Environment::instance()->ClassInit("Menu::Cache", cache));
        return obj;
    }



private:
    list< Node > m_items;   // �˵����Ҫ������ʾ����Ҫ����ȣ�
    Ini m_menu;             // �˵����m_items������ͬ������Ҫ���ڲ��������ҵȣ�
    string m_cfg;           // ��ǰ�˵���Ӧ�������ļ�
    bool m_valid;           // �˵��Ƿ���Ч
};











}// end of MENU_SPACE
using namespace MENU_SPACE;
#endif // end of __MENU_H_20100506__
