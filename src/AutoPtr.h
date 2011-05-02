#ifndef __AUTOPTR_H_20100413__
#define __AUTOPTR_H_20100413__
#include <string>
namespace AUTOPTR_SPACE
{

using namespace std;

/*
 * ����ָ���࣬��Ҫ�������ù���������ʹ�ã��磺
 *
 *      class Foo
 *      {
 *      public:
 *          ...
 *          ...
 *          // ��������ָ������
 *          typedef AutoPtr< Foo > auto_ptr;
 *
 *          // �ӿ�
 *          void Get();
 *      };
 *
 *      Foo::auto_ptr foo;
 *      foo->Get(); // �������foo�Ƿ�ΪNULL;
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
    // ����()������������ģ������ַ��
    Type * operator->()
    {
        if(NULL == obj)
        {
            // ʹ�ø��࣬�Ա���ÿ�ε���ʱ��ָ����ԣ�
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

