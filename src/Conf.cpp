// Rocky 2010-04-29 18:25:27
#include "Conf.h"
namespace CONF_SPACE
{







Conf::Conf()
{
}

// 拷贝构造函数     [2010-05-10 14:31:51］
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
 * 功能：放入值（原key对应值若相同，将被覆盖），如：
 * 编写：Rocky 2010-05-10 16:33:35
 * 说明：
 *          ------------1). 原值------------------
 *          a1=1111
 *          a2=2222222
 *          a3=333
 *          a4=44444444444444
 *
 *          ------------2). pack中的值------------------
 *          a2=88
 *          b1=777
 *          b3=9999999
 *
 *          ------------3). 合并后的新值------------------
 *          a1=1111
 *          a2=88
 *          a3=333
 *          a4=44444444444444
 *          b1=777
 *          b3=9999999
 *
 *  即1)中的a3被2)中的a3覆盖了
 */
void Conf::Set(const Conf &pack)
{
    iterator it( &pack );

    while( it.next() )
    {
        // 设置值，旧将被覆盖；
        m_list[ it.GetName() ] = it.GetValue();
    }
}










}// end of CONF_SPACE
