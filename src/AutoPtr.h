#ifndef __AUTOPTR_H_20100413__
#define __AUTOPTR_H_20100413__
#include <string>
namespace AUTOPTR_SPACE
{

using namespace std;

/*
 * 智能指针类，主要用于配置工厂处理中使用，如：
 *
 *      class Foo
 *      {
 *      public:
 *          ...
 *          ...
 *          // 定义智能指针类型
 *          typedef AutoPtr< Foo > auto_ptr;
 *
 *          // 接口
 *          void Get();
 *      };
 *
 *      Foo::auto_ptr foo;
 *      foo->Get(); // 无需测试foo是否为NULL;
 */
template<typename Type>
class AutoPtr
{
public:
    AutoPtr() : obj( Type::New() )
    {
    }
    AutoPtr(const string &id) : obj( Type::New(id) )
    {
    }
    AutoPtr(Type *e) : obj( e )
    {
    }
    ~AutoPtr()
    {
        delete obj;
    }
    // 重载()操作符，返回模块对象地址；
    Type * operator->()
    {
        if(NULL == obj)
        {
            // 使用该类，以避免每次调用时做指针测试；
            class Empty : public Type
            {
            public:
                Empty(){};
                Empty(const string &id){};
                ~Empty(){};
            private:
                int DoInit()
                {
                    return ERR;
                }
                Type *DoNew()
                {
                    return NULL;
                }
            };
            static Empty empty;
            return &empty;
        }
        return obj;
    }
private:
    Type *obj;
};


















}// end of AUTOPTR_SPACE
using namespace AUTOPTR_SPACE;
#endif // end of __AUTOPTR_H_20100413__

