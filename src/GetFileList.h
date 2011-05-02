#ifndef __GETFILELIST_H_20100423__
#define __GETFILELIST_H_20100423__
#include "Common.h"
namespace GETFILELIST_SPACE
{






// ȡָ��Ŀ¼�µ������ļ����ݹ���Ŀ¼
class GetFileList
{
public:
    // �ļ����������г����е�ǰĿ¼�µ��ļ���
    class file_iterator
    {
    public:
        file_iterator(GetFileList &obj): m_obj(obj), first(true)
        {
            it = m_obj.m_FileList.begin();
        }

        // ָ����һ��Ԫ��
        bool next()
        {
            if(first)   // ���ǵ�һ�ε���next()�������Ӽӣ�
            {
                first = false;
            }
            else
            {
                it++;
            }
            return m_obj.m_FileList.end() != it;
        }

        // �����ļ���
        const char *name() const
        {
            return (*it).c_str();
        }

    private:
        GetFileList &m_obj;
        vector<string>::iterator it;
        bool first;
    };

    // Ŀ¼���������г����е�ǰĿ¼�µ���Ŀ¼�����󣩣�
    class dir_iterator
    {
    public:
        dir_iterator(GetFileList &obj): m_obj(obj), first(true)
        {
            it = m_obj.m_SubDir.begin();
        }

        // ָ����һ��Ԫ��
        bool next()
        {
            if(first)   // ���ǵ�һ�ε���next()�������Ӽӣ�
            {
                first = false;
            }
            else
            {
                it++;
            }
            return m_obj.m_SubDir.end() != it;
        }

        // ȡĿ¼��
        const char *name() const
        {
            return (it->first).c_str();
        }

        // ȡ��Ŀ¼����
        const GetFileList *sub() const
        {
            return it->second;
        }

    private:
        GetFileList &m_obj;
        map<string, GetFileList*>::iterator it;
        bool first;
    };


public:
    // deep=0��ʾֻȡһ��Ŀ¼������ǰĿ¼��
    GetFileList(const string &dir, int deep=0);
    virtual ~GetFileList();

    // ��ascii�����ļ���
    void SortFilename();

protected:
    // ɨ��Ŀ¼
    virtual void ScanDir(const string &dir, int deep);


protected:
    vector<string> m_FileList;          // ��ǰĿ¼���ļ��б�
    map<string, GetFileList*> m_SubDir; //  ������Ŀ¼����
};










}// end of GETFILELIST_SPACE
using namespace GETFILELIST_SPACE;
#endif // end of __GETFILELIST_H_20100423__
