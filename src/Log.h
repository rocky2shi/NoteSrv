#ifndef __LOG_H_20100423__
#define __LOG_H_20100423__
#include "Environment.h"
#include "Common.h"
#include "Lock.h"
#include <stdarg.h>     //包含va_list
#include <stdio.h>      //包含vsprintf()
namespace LOG_SPACE
{


#if 1

// 日志处理类（因处理vc++上可变参数宏问题）
class Log
{
public:
    typedef enum{
        DEBUG,      // 调试信息
        INFO,       // 一般性提示信息
        ERROR,      // 出错信息
        LEVEL_MAX
    } Level;

public:
    Log(const char *file, int line);
    virtual ~Log();

    // 类初始化（程序启动时的初始化）
    static int init();

    // 设置或获取Log单例
    inline static Log *instance(Log *logger=NULL)
    {
        static Log *obj = (Log *)(Environment::instance()->ClassInit("Log", logger));
        return obj;
    }

    // 设置日志级别
    void SetLevel(Level level, bool open=true);

    // 获取日志级别
    bool GetLevel(Level level);

    // 取日志级别对应字符串
    const char *LevelToStr(Level level)
    {
        static const char *str[] = {
            "DEBUG",
            "INFO",
            COLOR_RED "ERROR" COLOR_NONE,
            ""
        };
        return str[level];
    }

    // 调试信息
    void DebugLog(const char * format, ...);

    // 一般性提示信息
    void InfoLog(const char * format, ...);

    // 出错信息
    void ErrorLog(const char * format, ...);


protected:
    Log();

    // 单个对象初始化
    virtual int Init();

    // 输出日志
    virtual void Out(Level level, const Log *logger, const char *format, va_list ap);


public: // 为了使子类“函数”中能使用，
    const char *m_file;
    const int m_line;
    char *m_level;
    char m_time[32];

private:
    Lock *m_lock;
};



#define LOG_DEBUG   Log(__FILE__, __LINE__).DebugLog
#define LOG_INFO    Log(__FILE__, __LINE__).InfoLog
#define LOG_ERROR   Log(__FILE__, __LINE__).ErrorLog



#else

#define LOG_DEBUG(format, args...)                                             \
{                                                                              \
    printf("[%s:%d] [DEBUG]>>> " format "\n", __FILE__, __LINE__, ##args);     \
}

#define LOG_INFO(format, args...)                                              \
{                                                                              \
    printf("[%s:%d] [INFO]>>> " format "\n", __FILE__, __LINE__, ##args);      \
}

#define LOG_ERROR(format, args...)                                             \
{                                                                              \
    printf("[%s:%d] [ERROR]>>> " format "\n", __FILE__, __LINE__, ##args);     \
}

#endif


}// end of LOG_SPACE
using namespace LOG_SPACE;
#endif // end of __LOG_H_20100423__
