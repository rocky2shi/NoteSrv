#ifndef __INI_H_20100429__
#define __INI_H_20100429__
#include "Conf.h"
namespace INI_SPACE
{



/*
 * [Rocky 2010-04-29 18:33:48]
 * �����ʽ�ļ����磺
 *
 *   [worker]
 *   user=rocky
 *   id=8816
 *   ...
 */
class Ini
{
public:
    // ���������г����е�Conf����       // �ظ����� [XXX:26741890]
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

        // ָ����һ��Ԫ��
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

        // ȡkey
        const char *GetKey()
        {
            return it->first.c_str();
        }

        // ����()������������ģ������ַ��
        Conf *GetValue()
        {
            return it->second;
        }

        // ���ظ�ֵ�� =
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

    // ������������г����е�Conf����   // �ظ����� [XXX:26741890]
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

        // ָ����һ��Ԫ��
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

        // ȡkey
        const char *GetKey()
        {
            return it->first.c_str();
        }

        // ����()������������ģ������ַ��
        Conf *GetValue()
        {
            return it->second;
        }

        // ���ظ�ֵ�� =
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

    // ��ȡ��ʽ�ļ�
    int Read(const string &filename);

    // ����д���ļ�
    int Write(string filename="");

    const Conf *Get(const string &key) const;
    const string Get(const string &section, const string &attribute) const;

    int Set(const string &section, const string &attribute, const string &value);
    int Set(const string &key, const Conf &pack);

    void Del(const string &key);

    void Del(const string &section, const string &attribute);

    // �������
    void Clear();

    // ��ʾ��������
    void Dump() const;


private:
    // �Ƿ�Ϊ��[...]�������򷵻��������ڵ����ݣ����򷵻�NULL
    char *Section(char *buf, int len);




private:
    string m_filename;
    map<string, Conf *> m_ConfList;
};







}// end of INI_SPACE
using namespace INI_SPACE;
#endif // end of __INI_H_20100429__
