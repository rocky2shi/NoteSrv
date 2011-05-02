#ifndef __FILEOBJ_H_20100418__
#define __FILEOBJ_H_20100418__
#include "Common.h"
namespace FILEOBJ_SPACE
{


using namespace std;


// �򵥵��ļ���д
class FileObj
{
public:
    // �ļ�����ģʽ
    static const string R;
    static const string W;
    static const string RW;

    // �ļ�ƫ��ģʽ���ο�fseek()������
    typedef enum{
        SET = SEEK_SET,     // ���ļ�ͷƫ��
        CUR = SEEK_CUR,     // �ӵ�ǰλ��ƫ��
        END = SEEK_END,     // ���ļ�βƫ�ƣ�ƫ������Ϊ��ֵ��
    } SEEK;

public:
    FileObj();
    FileObj(const string &file);
    ~FileObj();

    // ���ļ���Ĭ��Ϊֻ����ʽ��
    bool Open(const string &file, const string &mode=R);

    // �ر��ļ�
    int Close();

    // ��ָ���ֽ�
    int Read(void *buf, const int len) const;

    // д��ָ�������ֽ�
    int Write(const void *buf, const int len) const;

    // ��ȡȡһ�У��޶��ֵ��������ֵ��0 <= ret <= max���������Ѷ�ȡ���ֽ�����������0��
    int GetLine(char * const buf, const int max) const;

    // �ļ����ȣ��ֽ�����
    int Size() const;

    // ���ص�ǰ�ļ�ȫ·����
    const string Fullname() const;

    // �����ļ�ƫ�ƣ��ɹ�����OK��������ERR��
    int SetSeek(SEEK whence=SET, int offset=0);


private:
    FILE *m_file;
    string m_fullname;  // �ļ�ȫ·����
};






}// end of FILEOBJ_SPACE
using namespace FILEOBJ_SPACE;
#endif // end of __FILEOBJ_H_20100418__
