#ifndef __GLOBALCONFIG_H_20100414__
#define __GLOBALCONFIG_H_20100414__
#include "Common.h"
#include "Environment.h"
namespace GLOBALCONFIG_SPACE
{





// 全局配置管理
class GlobalConfig
{
public:
    // 类初始化
    static int init(int argc, char *argv[]);

    // 设置或获取单例
    inline static GlobalConfig *instance(GlobalConfig *config=NULL)
    {
        static GlobalConfig *obj = (GlobalConfig *)(Environment::instance()->ClassInit("GlobalConfig", config));
        return obj;
    }

    // 系统根目录
    const string Root() const;

    // 全局数据目录
    const string SystemDataDir() const;

    // 用户目录存放地
    const string UserRootDir() const;

    // 页面处理动态库（模块）目录
    const string CgiDir() const;

    // 文本页面目录（存放html、js、txt、css等能直接在页面上显示的文件）
    const string HtmlDir() const;

    // 模板文件目录
    const string TemplateDir() const;

    // 系统日志目录
    const string SystemLogDir() const;

    // 系统临时目录
    const string SystemTmpDir() const;

    // 存放工具、外部处理程序等的目录
    const string ToolDir() const;

    // 全局计数器目录
    const string CounterDir() const;

    // 获取命令行参数等配置选项（获取field字段对应的参数，不存在则返回默认的def值）
    const string GetOption(const string &field, const string def="") const;

private:
    GlobalConfig(int argc, char *argv[]);


private:
    map<string, string> m_option;   // 配置选项
};









}// end of GLOBALCONFIG_SPACE
using namespace GLOBALCONFIG_SPACE;
#endif // end of __GLOBALCONFIG_H_20100414__
