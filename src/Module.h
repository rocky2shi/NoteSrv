#ifndef __MODULE_H_20100316__
#define __MODULE_H_20100316__
#include "SignalDeal.h"
#include "Common.h"
namespace MODULE_SPACE
{






// 数据表操作基类
class Module
{
public:
    // 迭代器，用于列出所有模块。注意，迭代器指向的是Module::m_Factory对象；
    class iterator
    {
    public:
        iterator(): first(true)
        {
            it = Module::m_Factory.begin();
        }

        // 指向下一个元素
        bool next()
        {
            if(first)   // 如是第一次调用next()，则不做加加；
            {
                first = false;
            }
            else
            {
                it++;
            }
            return Module::m_Factory.end() != it;
        }

        // 重载()操作符，返回模块对象地址；
        Module * operator()()
        {
            return it->second;
        }

    private:
        map<string, Module*>::iterator it;
        bool first;
    };


public:
    // 循环调用Deal()
    virtual int Run();

    // 业务处理入口，由各子类实现；
    virtual int Deal();

    // 取模块名
    const string GetName() const;


protected:
    string m_module;  // 本模块名称（id）



/*
 * 以下代码主要用于工厂处理设置
 */
public:
    virtual ~Module();

    // 初始化
    static int init();


protected:
    Module();
    Module(const string &id); // 子类调用来进行初次注册

    // 注册（把子类对象加入m_Factory）。注：接口设为static以防止Register()内部
    // 不心修改了类成员。
    static int Register(const string &id, Module *sub);
    // 关闭模块
    static int UnRegister(const string &id);

    // 各子类负责实现自已初始化操作
    virtual int DoInit() = 0 ;

private:
    typedef map<string, Module *> Factory;

    /*
     * 在进入main()函数之前，各对象已把自已插入到m_Factory，而后再
     * 在main()中调用init()时，再次调用m_Factory中子对象的初始化函
     * 数；
     */
    static Factory m_Factory;   // 初始化后填充的产生子对象工厂
};







}// end of MODULE_SPACE
using namespace MODULE_SPACE;
#endif // end of __MODULE_H_20100316__
