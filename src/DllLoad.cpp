// Rocky 2010-04-15 19:29:32
#include "DllLoad.h"
#include "DllLoad_Linux.h"
namespace DLLLOAD_SPACE
{







DllLoad::DllLoad()
{
    FUNCTION_TRACK(); // �����켣����
}

DllLoad::~DllLoad()
{
    FUNCTION_TRACK(); // �����켣����
}


// ���ݴ����id�ӹ����в���һ���������
DllLoad *DllLoad::New(const string &id/*="linux"*/)
{
    return new DllLoad_Linux;
}



// �򿪶�̬���ļ�
int DllLoad::Open(const string &file)
{
    FUNCTION_TRACK(); // �����켣����
    LOG_ERROR("Ӧ��������ʵ��");
    return ERR;
}

// �رն�̬��
int DllLoad::Close()
{
    FUNCTION_TRACK(); // �����켣����
    LOG_ERROR("Ӧ��������ʵ��");
    return ERR;
}

// ȡִ�ж���
void *DllLoad::GetSymbol(const string &sym)
{
    FUNCTION_TRACK(); // �����켣����
    LOG_ERROR("Ӧ��������ʵ��");
    return NULL;
}










}// end of DLLLOAD_SPACE
