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
 *      通用（共用）模板定义
 *
 */


// 用于在主、子模块中传递环境用的定义
template<class Type, ENV_OBJ_ID id>
class EnvInit
{
public:
    static Type *doit(bool bNew)
    {
        if( bNew )
        {
            // 使用新的对象实例
            static Type obj;
            return &obj;
        }
        else
        {
            // 恢复为正在使用的对象
            return (Type *)( Environment::instance()->Get( id ) );
        }
    }
};







}// end of TEMPLATE_SPACE
using namespace TEMPLATE_SPACE;
#endif // end of __TEMPLATE_H_20100430__
