#ifndef __CLASSINIT_H_20100603__
#define __CLASSINIT_H_20100603__
#include "Typedef.h"
#include <list>
#include <assert.h>
namespace CLASSINIT_SPACE
{

using namespace std;



// ִ����ĳ�ʼ��
class ClassInit
{
public:
    typedef int (*ExeFunc)();
    typedef enum {
        INIT_HIGH,  // �����ȼ���־
        INIT_LOW,   // �����ȼ���־
    }InitLevel;

public:
    ClassInit(ExeFunc init, InitLevel level=INIT_LOW);

    // ��������ע��ĳ�ʼ��
    static int init();

    //
    static int RegisterHL(const ExeFunc exector);   // �����ȼ�
    static int RegisterLL(const ExeFunc exector);   // �����ȼ�

private:
    static list<ExeFunc> m_HighQueue;
    static list<ExeFunc> m_LowQueue;
};






}// end of CLASSINIT_SPACE
using namespace CLASSINIT_SPACE;
#endif // end of __CLASSINIT_H_20100603__
