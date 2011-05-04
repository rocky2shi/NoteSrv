// Rocky 2010-04-07 18:07:41
#include "Help.h"
#include "Other.h"
#include "Timer.h"
#include "Submit.h"
#include "Compress.h"
#include "Session.h"
#include "User.h"
#include "Menu.h"
#include "Module.h"
#include "Page.h"
#include "ThreadPool.h"
#include "Counter.h"
#include "Common.h"









static void ModuleThread(Module *module)
{
    assert(NULL != module);

    LOG_INFO("Start service: [%s]", module->GetName().c_str());

    // 执行业务循环
    module->Run();

    LOG_INFO("Service quit: [%s]", module->GetName().c_str());
}



int main(int argc, char *argv[])
{
    FUNCTION_TRACK(); // 函数轨迹跟综


    if( (2 == argc) && ((strncmp(argv[1], "-h", 2) == 0) || (strncmp(argv[1], "--help", 6) == 0)) )
    {
        Help();
        return 0;
    }
    if( (2 == argc) && (strncmp(argv[1], "--changes", 2) == 0) )
    {
        Changes();
        return 0;
    }

    {/****************************** 初始化 开始 ******************************/

    int ret;



    // 初始化全局配置类
    ret = GlobalConfig::init(argc, argv);
    if(ret < 0)
    {
        printf("初始化全局配置类出错 [%d] \n", ret);
        return ret;
    }

    // 初始化系统设置
    ret = SystemInit();
    if(ret < 0)
    {
        printf("初始化系统设置出错 [%d] \n", ret);
        return ret;
    }

    // 初始化日志处理类
    ret = Log::init();
    if(ret < 0)
    {
        printf("初始化日志处理类出错 [%d] \n", ret);
        return ret;
    }

    // 初始化全局类设置
    ret = ClassInit::init();
    if(ret < 0)
    {
        LOG_ERROR("初始化全局类设置出错 [%d] \n", ret);
        return ret;
    }

    // 初始化计数器类
    ret = Counter::init();
    if(ret < 0)
    {
        LOG_ERROR("初始化初始化计数器类出错 [%d]", ret);
        return ret;
    }

    // 初始化定时器处理类
    ret = Timer::init();
    if(ret < 0)
    {
        LOG_ERROR("初始化定时器处理类出错 [%d] ", ret);
        return ret;
    }

    // 初始化线程池类
    ret = ThreadPool::init();
    if(ret < 0)
    {
        LOG_ERROR("初始化线程池类出错 [%d] ", ret);
        return ret;
    }

    // // 初始化系统信号类
    // ret = SignalDeal::init();
    // if(ret < 0)
    // {
    //     LOG_ERROR("初始化系统信号类出错 [%d] ", ret);
    //     return ret;
    // }

    // 初始化模块调用类
    ret = Module::init();
    if(ret < 0)
    {
        LOG_ERROR("初始化模块调用类出错 [%d] ", ret);
        return ret;
    }

    // 初始化页面处理类
    ret = Page::init();
    if(ret < 0)
    {
        LOG_ERROR("初始化页面处理类出错 [%d] ", ret);
        return ret;
    }

    // 初始化会话类
    ret = Session::init();
    if(ret < 0)
    {
        LOG_ERROR("初始化会话类出错 [%d] ", ret);
        return ret;
    }

    // 初始化用户信息类
    ret = User::init();
    if(ret < 0)
    {
        LOG_ERROR("初始化用户信息类出错 [%d] ", ret);
        return ret;
    }

    // 初始化菜单处理类
    ret = Menu::init();
    if(ret < 0)
    {
        LOG_ERROR("初始化菜单处理类出错 [%d] ", ret);
        return ret;
    }

    // 初始化压缩处理类
    ret = Compress::init();
    if(ret < 0)
    {
        LOG_ERROR("初始化压缩处理类出错 [%d] ", ret);
        return ret;
    }

    // 初始化提交事件处理类
    ret = Submit::init();
    if(ret < 0)
    {
        LOG_ERROR("初始化提交事件处理类出错 [%d] ", ret);
        return ret;
    }

    // 初始化其它处理操作
    ret = OtherInit();
    if(ret < 0)
    {
        LOG_ERROR("初始化其它处理操作出错 [%d] ", ret);
        return ret;
    }






    }/****************************** 初始化 结束 ******************************/




    Module::iterator module;    // 跌代器module内部指向了Module里的成员。
    boost::thread_group grp;

    // 读取所有打开的模块，每个模块对应一个处理线程；
    while( module.next() )
    {
        /*
         * module()是调用重载了的()运算符，取得模块指针，作为参数传给线程函
         * 数ModuleThread()，在线程函数里再执行处理循环；
         */
        grp.create_thread( boost::bind( &ModuleThread, module() ) );
    }

    grp.join_all();


    LOG_INFO("主线程退出。。。");
    pthread_exit(NULL);
    // return 0;
}





