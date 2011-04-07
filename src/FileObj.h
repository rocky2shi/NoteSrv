#ifndef __FILEOBJ_H_20100418__
#define __FILEOBJ_H_20100418__
#include "Common.h"
namespace FILEOBJ_SPACE
{


using namespace std;


// 简单的文件读写
class FileObj
{
public:
    // 文件操作模式
    static const string R;
    static const string W;
    static const string RW;

    // 文件偏移模式（参看fseek()函数）
    typedef enum{
        SET = SEEK_SET,     // 从文件头偏移
        CUR = SEEK_CUR,     // 从当前位置偏移
        END = SEEK_END,     // 从文件尾偏移（偏移量可为负值）
    } SEEK;

public:
    FileObj();
    FileObj(const string &file);
    ~FileObj();

    // 打开文件（默认为只读形式）
    bool Open(const string &file, const string &mode=R);

    // 关闭文件
    int Close();

    // 读指定字节
    int Read(void *buf, const int len) const;

    // 写入指定长度字节
    int Write(const void *buf, const int len) const;

    // 读取取一行（限定最长值，即返回值：0 <= ret <= max）（返回已读取的字节数，出错返回0）
    int GetLine(char * const buf, const int max) const;

    // 文件长度（字节数）
    int Size() const;

    // 返回当前文件全路径名
    const string Fullname() const;

    // 设置文件偏移（成功返回OK，出错返回ERR）
    int SetSeek(SEEK whence=SET, int offset=0);


private:
    FILE *m_file;
    string m_fullname;  // 文件全路径名
};






}// end of FILEOBJ_SPACE
using namespace FILEOBJ_SPACE;
#endif // end of __FILEOBJ_H_20100418__
