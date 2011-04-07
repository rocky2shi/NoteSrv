// Rocky 2010-04-14 19:18:50
#include "GlobalConfig.h"
namespace GLOBALCONFIG_SPACE
{






GlobalConfig::GlobalConfig(int argc, char *argv[])
{
    /*
     * 查看环境配置
     */
    m_option["::RootDir"] = getenv("RootDir") ? : ".";
    m_option["::ServicePort"] = getenv("ServicePort") ? : "80";

    /*
     * 查看命令行（比环境变量优先）
     */
    // 第一个参数为服务端口
    if(argc > 1)
    {
        m_option["::ServicePort"] = argv[1];
    }

    // 第二个参数为程序根目录
    if(argc > 2)
    {
        m_option["::RootDir"] = argv[2];
    }
}

// 类初始化
int GlobalConfig::init(int argc, char *argv[])
{
    int ret;
    static GlobalConfig config(argc, argv);

    if(NULL == GlobalConfig::instance( &config ))
    {
        printf("GlobalConfig::instance() error");   // [XXX]
        return ERR;
    }

    return OK;
}

// 系统根目录
const string GlobalConfig::Root() const
{
    // 若用户未配置，则默认为当前目录；
    return GetOption("::RootDir", ".") + "/";
}

// 用户目录存放地
const string GlobalConfig::UserRootDir() const
{
    return Root() + "data/user/";
}

// 页面处理动态库（模块）目录
const string GlobalConfig::CgiDir() const
{
    return Root() + "cgi/";
}

// 文本页面目录（存放html、js、txt、css等能直接在页面上显示的文件）
const string GlobalConfig::HtmlDir() const
{
    return Root() + "html/";
}

// 模板文件目录
const string GlobalConfig::TemplateDir() const
{
    return Root() + "template/";
}


// 系统日志目录
const string GlobalConfig::SystemLogDir() const
{
    return Root() + "log/";
}

// 系统临时目录
const string GlobalConfig::SystemTmpDir() const
{
    return Root() + "tmp/";
}

// 全局数据目录
const string GlobalConfig::SystemDataDir() const
{
    return Root() + "data/";
}

// 存放工具、外部处理程序等的目录
const string GlobalConfig::ToolDir() const
{
    return Root() + "tool/";
}

// 全局计数器目录
const string GlobalConfig::CounterDir() const
{
    return Root() + "data/counter/";
}

// 获取命令行参数等配置选项（获取field字段对应的参数，不存在则返回默认的def值）
const string GlobalConfig::GetOption(const string &field, const string def/*=""*/) const
{
    return GetMapValue(m_option, field, def);
}



















}// end of GLOBALCONFIG_SPACE
