#ifndef __LOG_H_20100423__
#define __LOG_H_20100423__
#include "Environment.h"
#include "Common.h"
#include "Lock.h"
#include <stdarg.h>     //����va_list
#include <stdio.h>      //����vsprintf()
namespace LOG_SPACE
{


#if 1

// ��־�����ࣨ����vc++�Ͽɱ���������⣩
class Log
{
public:
    typedef enum{
        DEBUG,      // ������Ϣ
        INFO,       // һ������ʾ��Ϣ
        ERROR,      // ������Ϣ
        LEVEL_MAX
    } Level;

public:
    Log(const char *file, int line);
    virtual ~Log();

    // ���ʼ������������ʱ�ĳ�ʼ����
    static int init();

    // ���û��ȡLog����
    inline static Log *instance(Log *logger=NULL)
    {
        static Log *obj = (Log *)(Environment::instance()->ClassInit("Log", logger));
        return obj;
    }

    // ������־����
    void SetLevel(Level level, bool open=true);

    // ��ȡ��־����
    bool GetLevel(Level level);

    // ȡ��־�����Ӧ�ַ���
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

    // ������Ϣ
    void DebugLog(const char * format, ...);

    // һ������ʾ��Ϣ
    void InfoLog(const char * format, ...);

    // ������Ϣ
    void ErrorLog(const char * format, ...);


protected:
    Log();

    // ���������ʼ��
    virtual int Init();

    // �����־
    virtual void Out(Level level, const Log *logger, const char *format, va_list ap);


public: // Ϊ��ʹ���ࡰ����������ʹ�ã�
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
