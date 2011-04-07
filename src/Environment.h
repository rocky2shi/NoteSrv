#ifndef __ENVIRONMENT_H_20100429__
#define __ENVIRONMENT_H_20100429__
#include <string>
#include <map>
namespace ENVIRONMENT_SPACE
{


using namespace std;



// 环境传递（在模块、动态库之间）
class Environment
{
public:
    inline static Environment *instance(Environment *exterior=NULL)
    {
        /*
         * ptr指向初情况而定：
         *
         *   (1). 在主模块（main()）中用时，第一次调instance()，无传参数，即env使
         *        用默认的NULL值，这时instance()调用将返回内部的静态定义对象引用；
         *
         *   (2). 当是在子模块中使用时，第一次调instance()，需要传入参数（正常情
         *        下应传入(1)中返回的参数，以便在整个程序中，主、子模块指向相同的
         *        执行代码（及数据）；
         */
        static Environment interior;
        static Environment *ptr = exterior ? : &interior;
        return ptr;
    }

    int Insert(const string &id, void *obj);
    void *Get(const string &id);

    // 初始化全局类
    void *ClassInit(const string &id, void *obj);

private:
    Environment();

private:
    map<string, void *> m_queue;
};




/*
用法示例说明：

----------------------------------- 主程序中 -----------------------------------

--------------Step 1: 定义获取（设置）类全局实例
// 设置或获取单例
inline static GlobalConfig *GlobalConfig::instance(GlobalConfig *config=NULL)
{
    static GlobalConfig *obj = (GlobalConfig *)(Environment::instance()->ClassInit("GlobalConfig", config));
    return obj;
}


--------------Step 2: 在程序初始化时设置类全局实例到记录中，以便在子模块（动态库）中取出来使用；
// 类初始化
int GlobalConfig::init()
{
    int ret;
    static GlobalConfig config;

    if(NULL == GlobalConfig::instance( &config ))
    {
        LOG_ERROR("GlobalConfig::instance() error");
        return ERR;
    }

    return OK;
}


调用如：
    GlobalConfig::instance()->Get(...);

    即instance()中的入参为空；




----------------------------------- 子模块（动态库）中 -------------------------
无需初始化，直接调用，如：

    GlobalConfig::instance()->Get(...);

    即instance()中的入参为空；

这时，如是第一次调用，它内部会调用：
    Environment::instance()->ClassInit()
来初始化它内部的静态变量，即使用主模块中传入的对象实例为执行操作，以达到主、子模
块统一的目的；






*/













}// end of ENVIRONMENT_SPACE
using namespace ENVIRONMENT_SPACE;
#endif // end of __ENVIRONMENT_H_20100429__
