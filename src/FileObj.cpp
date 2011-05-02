// Rocky 2010-04-18 14:18:13
#include "Common.h"
#include "FileObj.h"
namespace FILEOBJ_SPACE
{


const string FileObj::R = "rb";
const string FileObj::W = "wb";
const string FileObj::RW = "a+b";

FileObj::FileObj() : m_file(NULL)
{
}

FileObj::FileObj(const string &file)
{
    Open(file, RW);
}

FileObj::~FileObj()
{
    Close();
}

// ���ļ�
bool FileObj::Open(const string &file, const string &mode/*=R*/)
{
    if("" == file)
    {
        return false;
    }
    m_file = fopen(file.c_str(), mode.c_str());
    if(NULL == m_file)
    {
        return false;
    }
    m_fullname = file;
    return true;
}

// �ر��ļ�
int FileObj::Close()
{
    if(NULL != m_file)
    {
        fclose(m_file);
        m_file = NULL;
    }
}

// ��ָ���ֽ�
int FileObj::Read(void *buf, const int len) const
{
    if( NULL == m_file )
    {
        return 0;   // -1��Ϊ0    [2010-05-11 10:48:58]
    }

    return fread(buf, 1, len, m_file);
}

// д��ָ�������ֽ�
int FileObj::Write(const void *buf, const int len) const
{
    if( NULL == m_file )
    {
        return 0;   // -1��Ϊ0    [2010-05-11 10:48:58]
    }

    return fwrite(buf, 1, len, m_file);
}

// �ļ�����
int FileObj::Size() const
{
    if(NULL == m_file)
    {
        return 0;   // -1��Ϊ0    [2010-05-11 10:48:58]
    }
    return GetSizeOfFile( m_fullname );
}

// ��ȡȡһ�У��޶��ֵ���������Ѷ�ȡ���ֽ�����������0��
int FileObj::GetLine(char * const buf, const int max) const
{
    if(NULL == m_file)
    {
        return 0;   // -1��Ϊ0    [2010-05-11 10:48:58]
    }

    char *str = buf;
    int c;
    int count = 0;
    while( ((c = fgetc(m_file)) != EOF))
    {
        *str = c;
        count++;
        str++;
        if( '\n' == c || max == count )
        {
            break; // �ǻس��������Ѷ�ȡ�㹻�ֽڣ��˳���
        }
    }
    *str = '\0';
    return count;
}

// ���ص�ǰ�ļ�ȫ·����
const string FileObj::Fullname() const
{
    return m_fullname;
}

// �����ļ�ƫ�ƣ��ɹ�����OK��������ERR��
int FileObj::SetSeek(SEEK whence/*=SET*/, int offset/*=0*/)
{
    if(NULL == m_file)
    {
        return ERR;
    }
    return fseek(m_file, offset, whence) == 0 ? OK : ERR;
}










}// end of FILEOBJ_SPACE
