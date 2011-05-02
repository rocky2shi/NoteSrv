#ifndef __TEMPLATE_H_20100430__
#define __TEMPLATE_H_20100430__
#include "Environment.h"
#include "Typedef.h"
#include <iostream>
namespace TEMPLATE_SPACE
{

using namespace std;

/*
 *
 *      ͨ�ã����ã�ģ�嶨��
 *
 */


// ������������ģ���д��ݻ����õĶ���
template<class Type, ENV_OBJ_ID id>
class EnvInit
{
public:
    static Type *doit(bool bNew)
    {
        if( bNew )
        {
            // ʹ���µĶ���ʵ��
            static Type obj;
            return &obj;
        }
        else
        {
            // �ָ�Ϊ����ʹ�õĶ���
            return (Type *)( Environment::instance()->Get( id ) );
        }
    }
};







}// end of TEMPLATE_SPACE
using namespace TEMPLATE_SPACE;
#endif // end of __TEMPLATE_H_20100430__
