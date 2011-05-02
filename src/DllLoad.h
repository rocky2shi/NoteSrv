#ifndef __DLLLOAD_H_20100415__
#define __DLLLOAD_H_20100415__
#include "Common.h"
namespace DLLLOAD_SPACE
{



// ƽ̨�޹ض�̬�������
class DllLoad
{
public:
    // ��������ָ������
    typedef AutoPtr<DllLoad> Ptr;

public:
    virtual ~DllLoad();

    // �򿪶�̬���ļ�
    virtual int Open(const string &file);

    // �رն�̬��
    virtual int Close();

    // ȡִ�ж���
    virtual void *GetSymbol(const string &sym);


    // ���ݴ����id�ӹ����в���һ���������
    static DllLoad *New(const string &id="linux");

protected:
    DllLoad();
};







}// end of DLLLOAD_SPACE
using namespace DLLLOAD_SPACE;
#endif // end of __DLLLOAD_H_20100415__
