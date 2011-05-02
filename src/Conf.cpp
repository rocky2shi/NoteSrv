// Rocky 2010-04-29 18:25:27
#include "Conf.h"
namespace CONF_SPACE
{







Conf::Conf()
{
}

// �������캯��     [2010-05-10 14:31:51��
Conf::Conf(const Conf &pack)
{
    if(this == &pack)
    {
        return;
    }
    m_list = pack.m_list;
}

Conf::~Conf()
{
}

/*
 * ���ܣ�����ֵ��ԭkey��Ӧֵ����ͬ���������ǣ����磺
 * ��д��Rocky 2010-05-10 16:33:35
 * ˵����
 *          ------------1). ԭֵ------------------
 *          a1=1111
 *          a2=2222222
 *          a3=333
 *          a4=44444444444444
 *
 *          ------------2). pack�е�ֵ------------------
 *          a2=88
 *          b1=777
 *          b3=9999999
 *
 *          ------------3). �ϲ������ֵ------------------
 *          a1=1111
 *          a2=88
 *          a3=333
 *          a4=44444444444444
 *          b1=777
 *          b3=9999999
 *
 *  ��1)�е�a3��2)�е�a3������
 */
void Conf::Set(const Conf &pack)
{
    iterator it( &pack );

    while( it.next() )
    {
        // ����ֵ���ɽ������ǣ�
        m_list[ it.GetName() ] = it.GetValue();
    }
}










}// end of CONF_SPACE
