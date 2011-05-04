// Rocky 2010-03-16 09:57:12
#include "Module.h"
namespace MODULE_SPACE
{








Module::Factory Module::m_Factory;


Module::Module()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Module::Module(const string &id): m_module(id)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    // 在未进入主函数前已调用，这时的m_TmpFactory[ index ]应为NULL
    assert(NULL == m_Factory[ id ]);

    Register(id, this);
}

Module::~Module()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}


/*
 * 功能：注册子类对象到工厂
 * 编写：Rocky 2009-09-07 17:09:46
 * 返回：正确返回>=0值，出错返<0值；
 * 注意：此函数在进入main()函数之前就由构造函数调用了（因些不能使用MB_LOGGER_xxx()）
 */
int Module::Register(const string &id, Module *sub)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    assert("" != id && NULL == m_Factory[ id ]);
    m_Factory[ id ] = sub;  // 加入队列
    return OK;
}

/*
 * 功能：关闭模块
 * 编写：Rocky 2010-03-16 14:52:25
 * 返回：正确返回>=0值，出错返<0值；
 * 注意：此函数在进入main()函数之前就由构造函数调用了
 */
int Module::UnRegister(const string &id)
{
    if(NULL == m_Factory[ id ])
    {
        return ERR;
    }
    m_Factory.erase(id); // 删除
    return OK;
}


/*
 * 功能：静态初始化设置
 * 编写：Rocky 2009-09-23 14:14:55
 * 返回：正确返回>=0值，出错返<0值；
 */
int Module::init()
{
    FUNCTION_TRACK(); // 函数轨迹跟综


    Factory::iterator it;

    for (it = m_Factory.begin(); it != m_Factory.end(); ++it)
    {
        const string &id = it->first;
        Module * const &db = it->second;

        // 调用子类初始化函数
        if( db->DoInit() < 0 )
        {
            LOG_ERROR("初始化Module的子类[%s]出错", id.c_str() );
            return ERR;
        }
        LOG_DEBUG("初始化Module的子类[%s]结束", id.c_str());
    }

    return OK;
}

/*
 * 功能：各彩种的初始化操作都类似，所以抽到基类（但必须在子类中显式调用）
 * 编写：Rocky 2009-09-15 18:30:21
 * 返回：正确返回>=0值，出错返<0值；
 */
int Module::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

//    // 各模块统一使用的代码
//    const string close = GetCfgValue(m_module, "close", "");
//    if("1" == close)
//    {
//        LOG_ERROR("模块[%s]配置为关闭", m_module.c_str() );
//        UnRegister(m_module);
//        return OK;
//    }

    return OK;
}





















/******************** 子类重载以下操作来执行具体业务 ********************/
/*
 *  Run(): 主循环，调用Deal()
 */

// 取模块名
const string Module::GetName() const
{
    return m_module;
}


// 循环调用处理函数
int Module::Run()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    // 先查看当前功能设置，如不设，再查看共用的设置，再不，则使用默认值（10秒）
    const int second = 3; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<MyBox::S2I( GetCfgValue("Other", "main_loop_interval", "10") );

    LOG_INFO("main_loop_interval=[%d]", second);

    // 处理每个彩种
    while( !SignalDeal::exit() )
    {
        // 调用处理函数
        Deal();

        // 处理完成，暂停second秒。
        sleep(second);
    }// end of while( ...

    return OK;
}

// 处理函数
int Module::Deal()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    LOG_ERROR("暂不支持此模块[%s]", m_module.c_str() );
    return ERR;
}




}// end of MODULE_SPACE
