#ifndef __CONF_H_20100429__
#define __CONF_H_20100429__
#include "Common.h"
namespace CONF_SPACE
{





/*
 * [Rocky 2010-04-29 18:30:39]
 * 处理格式文件，如：
 *
 *   user=rocky
 *   id=8816
 *   ...
 */
class Conf
{
public:
    // 迭代器，列出所有的数据对
    class iterator
    {
    public:
        iterator(const Conf *conf): m_conf(conf), first(true)
        {
            it = m_conf->m_list.begin();
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
            return m_conf->m_list.end() != it;
        }

        // 取name
        const char *GetName()
        {
            return it->first.c_str();
        }

        // 取value
        const char *GetValue()
        {
            return it->second.c_str();
        }


    private:
        const Conf *m_conf;
        map<string, string>::const_iterator it;
        bool first;
    };

public:
    Conf();
    Conf(const Conf &pack);
    ~Conf();

    inline int Get(const string &key, string &value) const
    {
        map<string, string>::const_iterator it = m_list.find( key );
        if(m_list.end() != it)
        {
            value = it->second;
            return OK;
        }
        return ERR;
    }
    inline string Get(const string &key) const
    {
        string value;
        Get(key, value);
        return value;
    }

    inline void Set(const string &key, const string &value)
    {
        m_list[ key ] = value;
    }

    // 放入值（旧值将被覆盖）
    void Set(const Conf &pack);

    inline void Del(const string &key)
    {
        m_list.erase( key );
    }

    inline unsigned int Size() const
    {
        return m_list.size();
    }


private:
    map<string, string> m_list; // 存放[key/value]对
};









}// end of CONF_SPACE
using namespace CONF_SPACE;
#endif // end of __CONF_H_20100429__
