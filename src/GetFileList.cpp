// Rocky 2010-04-23 16:30:08
#include "GetFileList.h"
#include <dirent.h>
namespace GETFILELIST_SPACE
{







GetFileList::GetFileList(const string &dir, int deep/*=0*/)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    ScanDir(dir, deep);
}

GetFileList::~GetFileList()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

// 扫描目录
void GetFileList::ScanDir(const string &dir, int deep)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    DIR *pDir;
    struct dirent *pDirent;

    pDir = opendir(dir.c_str());
    if( NULL == pDir )
    {
        LOG_ERROR("不能打开目录[%s] ", dir.c_str());
        return;
    }

    while( (pDirent = readdir(pDir)) != NULL )
    {
        if( strcmp(".", pDirent->d_name) == 0 ||
            strcmp("..", pDirent->d_name) == 0 )
        {
            continue;
        }

        // 取目录信息
        struct stat stDir;
        const string file = dir + pDirent->d_name;
        if( stat(file.c_str(), &stDir) == -1)
        {
            LOG_DEBUG("取文件（目录）信息出错：[%s]", pDirent->d_name);
            continue;
        }

        if( S_ISDIR(stDir.st_mode) )
        {
            // 目录
            m_SubDir[ file ] = NULL;
        }
        else
        {
            // 文件
            m_FileList.push_back(file);
        }
    }// end of while( (pDirent ...
    closedir(pDir);

    LOG_DEBUG("[%d] [%d] [%s]", m_SubDir.size(), m_FileList.size(), dir.c_str());
}


// 按ascii排序文件名
void GetFileList::SortFilename()
{
    class Comparer
    {
    public:
        static bool doit(const string &a, const string &b)
        {
            return a > b;
        }
    };

    //sort(m_FileList.begin(), m_FileList.end()); // 升序排列
    sort(m_FileList.begin(), m_FileList.end(), Comparer::doit); // 降序排列
}








}// end of GETFILELIST_SPACE
