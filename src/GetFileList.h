#ifndef __GETFILELIST_H_20100423__
#define __GETFILELIST_H_20100423__
#include "Common.h"
namespace GETFILELIST_SPACE
{






// 取指定目录下的所有文件及递归子目录
class GetFileList
{
public:
    // 文件迭代器，列出所有当前目录下的文件；
    class file_iterator
    {
    public:
        file_iterator(GetFileList &obj): m_obj(obj), first(true)
        {
            it = m_obj.m_FileList.begin();
        }

        // 指向下一个元素
        bool next()
        {
            if(first)   // 如是第一次调用next()，则不做加加；
            {
                first = false;
            }
            else
            {
                it++;
            }
            return m_obj.m_FileList.end() != it;
        }

        // 返回文件名
        const char *name() const
        {
            return (*it).c_str();
        }

    private:
        GetFileList &m_obj;
        vector<string>::iterator it;
        bool first;
    };

    // 目录迭代器，列出所有当前目录下的子目录（对象）；
    class dir_iterator
    {
    public:
        dir_iterator(GetFileList &obj): m_obj(obj), first(true)
        {
            it = m_obj.m_SubDir.begin();
        }

        // 指向下一个元素
        bool next()
        {
            if(first)   // 如是第一次调用next()，则不做加加；
            {
                first = false;
            }
            else
            {
                it++;
            }
            return m_obj.m_SubDir.end() != it;
        }

        // 取目录名
        const char *name() const
        {
            return (it->first).c_str();
        }

        // 取子目录对象
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
    // deep=0表示只取一层目录，即当前目录；
    GetFileList(const string &dir, int deep=0);
    virtual ~GetFileList();

    // 按ascii排序文件名
    void SortFilename();

protected:
    // 扫描目录
    virtual void ScanDir(const string &dir, int deep);


protected:
    vector<string> m_FileList;          // 当前目录下文件列表
    map<string, GetFileList*> m_SubDir; //  所有子目录对象
};










}// end of GETFILELIST_SPACE
using namespace GETFILELIST_SPACE;
#endif // end of __GETFILELIST_H_20100423__
