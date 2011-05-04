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

// 打开文件
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

// 关闭文件
int FileObj::Close()
{
    if(NULL != m_file)
    {
        fclose(m_file);
        m_file = NULL;
    }
}

// 读指定字节
int FileObj::Read(void *buf, const int len) const
{
    if( NULL == m_file )
    {
        return 0;   // -1改为0    [2010-05-11 10:48:58]
    }

    return fread(buf, 1, len, m_file);
}

// 写入指定长度字节
int FileObj::Write(const void *buf, const int len) const
{
    if( NULL == m_file )
    {
        return 0;   // -1改为0    [2010-05-11 10:48:58]
    }

    return fwrite(buf, 1, len, m_file);
}

// 文件长度
int FileObj::Size() const
{
    if(NULL == m_file)
    {
        return 0;   // -1改为0    [2010-05-11 10:48:58]
    }
    return GetSizeOfFile( m_fullname );
}

// 读取取一行（限定最长值）（返回已读取的字节数，出错返回0）
int FileObj::GetLine(char * const buf, const int max) const
{
    if(NULL == m_file)
    {
        return 0;   // -1改为0    [2010-05-11 10:48:58]
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
            break; // 是回车符，或已读取足够字节，退出；
        }
    }
    *str = '\0';
    return count;
}

// 返回当前文件全路径名
const string FileObj::Fullname() const
{
    return m_fullname;
}

// 设置文件偏移（成功返回OK，出错返回ERR）
int FileObj::SetSeek(SEEK whence/*=SET*/, int offset/*=0*/)
{
    if(NULL == m_file)
    {
        return ERR;
    }
    return fseek(m_file, offset, whence) == 0 ? OK : ERR;
}










}// end of FILEOBJ_SPACE
