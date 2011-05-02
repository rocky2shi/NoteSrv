// Rocky 2010-04-23 16:30:08
#include "GetFileList.h"
#include <dirent.h>
namespace GETFILELIST_SPACE
{







GetFileList::GetFileList(const string &dir, int deep/*=0*/)
{
    FUNCTION_TRACK(); // �����켣����

    ScanDir(dir, deep);
}

GetFileList::~GetFileList()
{
    FUNCTION_TRACK(); // �����켣����
}

// ɨ��Ŀ¼
void GetFileList::ScanDir(const string &dir, int deep)
{
    FUNCTION_TRACK(); // �����켣����

    DIR *pDir;
    struct dirent *pDirent;

    pDir = opendir(dir.c_str());
    if( NULL == pDir )
    {
        LOG_ERROR("���ܴ�Ŀ¼[%s] ", dir.c_str());
        return;
    }

    while( (pDirent = readdir(pDir)) != NULL )
    {
        if( strcmp(".", pDirent->d_name) == 0 ||
            strcmp("..", pDirent->d_name) == 0 )
        {
            continue;
        }

        // ȡĿ¼��Ϣ
        struct stat stDir;
        const string file = dir + pDirent->d_name;
        if( stat(file.c_str(), &stDir) == -1)
        {
            LOG_DEBUG("ȡ�ļ���Ŀ¼����Ϣ����[%s]", pDirent->d_name);
            continue;
        }

        if( S_ISDIR(stDir.st_mode) )
        {
            // Ŀ¼
            m_SubDir[ file ] = NULL;
        }
        else
        {
            // �ļ�
            m_FileList.push_back(file);
        }
    }// end of while( (pDirent ...
    closedir(pDir);

    LOG_DEBUG("[%d] [%d] [%s]", m_SubDir.size(), m_FileList.size(), dir.c_str());
}


// ��ascii�����ļ���
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

    //sort(m_FileList.begin(), m_FileList.end()); // ��������
    sort(m_FileList.begin(), m_FileList.end(), Comparer::doit); // ��������
}








}// end of GETFILELIST_SPACE
