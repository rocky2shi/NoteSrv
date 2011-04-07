#ifndef __INI_H_20100429__
#define __INI_H_20100429__
#include "Conf.h"
namespace INI_SPACE
{



/*
 * [Rocky 2010-04-29 18:33:48]
 * 处理格式文件，如：
 *
 *   [worker]
 *   user=rocky
 *   id=8816
 *   ...
 */
class Ini
{
public:
    // 迭代器，列出所有的Conf对象       // 重复代码 [XXX:26741890]
    class iterator
    {
    public:
        iterator(): m_ini(NULL)
        {
        }
        iterator(const Ini *ini): m_ini(ini), first(true)
        {
            it = m_ini->m_ConfList.begin();
        }

        // 指向下一个元素
        bool next()
        {
            if( first )
            {
                first = false;
            }
            else
            {
                it++;
            }
            return m_ini->m_ConfList.end() != it;
        }

        // 取key
        const char *GetKey()
        {
            return it->first.c_str();
        }

        // 重载()操作符，返回模块对象地址；
        Conf *GetValue()
        {
            return it->second;
        }

        // 重载赋值号 =
        iterator & operator=(Ini *ini)
        {
            m_ini = ini;
            first = true;
            it = m_ini->m_ConfList.begin();
            return *this;
        }

    private:
        const Ini *m_ini;
        map<string, Conf *>::const_iterator it;
        bool first;
    };

    // 反向迭代器，列出所有的Conf对象   // 重复代码 [XXX:26741890]
    class reverse_iterator
    {
    public:
        reverse_iterator(): m_ini(NULL)
        {
        }
        reverse_iterator(Ini *ini): m_ini(ini), first(true)
        {
            it = m_ini->m_ConfList.rbegin();
        }

        // 指向下一个元素
        bool next()
        {
            if( first )
            {
                first = false;
            }
            else
            {
                it++;
            }
            return m_ini->m_ConfList.rend() != it;
        }

        // 取key
        const char *GetKey()
        {
            return it->first.c_str();
        }

        // 重载()操作符，返回模块对象地址；
        Conf *GetValue()
        {
            return it->second;
        }

        // 重载赋值号 =
        reverse_iterator & operator=(Ini *ini)
        {
            m_ini = ini;
            first = true;
            it = m_ini->m_ConfList.rbegin();
            return *this;
        }

    private:
        Ini *m_ini;
        map<string, Conf *>::reverse_iterator it;
        bool first;
    };







public:
    Ini(const string &filename="");
    ~Ini();

    // 读取格式文件
    int Read(const string &filename);

    // 数据写入文件
    int Write(string filename="");

    const Conf *Get(const string &key) const;
    const string Get(const string &section, const string &attribute) const;

    int Set(const string &section, const string &attribute, const string &value);
    int Set(const string &key, const Conf &pack);

    void Del(const string &key);

    void Del(const string &section, const string &attribute);

    // 清除数据
    void Clear();

    // 显示所有数据
    void Dump() const;


private:
    // 是否为‘[...]’，是则返回中括号内的内容，否则返回NULL
    char *Section(char *buf, int len);




private:
    string m_filename;
    map<string, Conf *> m_ConfList;
};







}// end of INI_SPACE
using namespace INI_SPACE;
#endif // end of __INI_H_20100429__
