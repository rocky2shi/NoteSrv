// Rocky 2010-04-23 15:21:35
#include "Log.h"
#include "Environment.h"
namespace LOG_SPACE
{

#if 1


// -----------------------------------------------------
// 日志存在本地文件
class Log_Local : public Log
{
public:
    Log_Local() : m_log(NULL)
    {
    }

    ~Log_Local()
    {
        if(NULL != m_log)
        {
            fclose(m_log);
        }
    }

    virtual int Init()
    {
        const string file = GlobalConfig::instance()->SystemLogDir() + "NoteSrv.log";
        m_log = fopen(file.c_str(), "a+b");
        if(NULL == m_log)
        {
            printf("Can't open file: [%s]\n", file.c_str());
            return ERR;
        }
        return Log::Init();
    }

    virtual void Out(Level level, const Log *logger, const char *format, va_list ap)
    {
        FUNCTION_TRACK(); // 函数轨迹跟综

        fprintf(m_log, "[%s] [%s] [%s:%d] [%lu]>>> ",
                            logger->m_time,
                            LevelToStr(level),
                            logger->m_file,
                            logger->m_line,
                            pthread_self());
        vfprintf(m_log, format, ap);
        fprintf(m_log, "\n");
        fflush(m_log);
    }

private:
    FILE *m_log;
};

// -----------------------------------------------------


#endif









Log::Log() : m_file(NULL), m_line(-1), m_level(NULL)
{
}

Log::Log(const char *file, int line) : m_file(file), m_line(line)
{
    time_t t;
    t = time(NULL);
    strftime(m_time, sizeof(m_time), "%Y-%m-%d %H:%M:%S", localtime(&t) ); // "%b %d %T"
}

Log::~Log()
{
}

// 类初始化
int Log::init()
{
    int ret;
    Log *logger = NULL;
    char *which = getenv("LOG_TERMINAL");

    // 由环境变量决定日志输出到哪   [Rocky 2010-06-04 16:05:30]
    if(NULL != which && '1' == *which)
    {
        static Log log;         // 输出到终端
        logger = &log;
    }
    else
    {
        static Log_Local log;    // 输出到本地文件
        logger = &log;
    }

    Log *obj = Log::instance( logger );
    if(NULL == obj)
    {
        printf("Log::instance() error\n");
        return ERR;
    }

    return Log::instance()->Init();
}

// 执行对象初始化，设置运行级别等数据结构；
int Log::Init()
{
    m_level = new char[LEVEL_MAX + 1];
    if(NULL == m_level)
    {
        printf("new char[] error\n");
        assert(0);  // 这是系统必须完成的初始化
        return ERR;
    }
    m_lock = new Lock;
    if(NULL == m_lock)
    {
        printf("new Lock() error\n");
        assert(0);  // 这是系统必须完成的初始化
        return ERR;
    }

    memset(m_level, 0, LEVEL_MAX);
    m_level[ DEBUG ] = (getenv("LOG_DEBUG") && getenv("LOG_DEBUG")[0]) ? 0 : 1;
    m_level[ INFO ] = (getenv("LOG_INFO") && getenv("LOG_INFO")[0]) ? 0 : 1;
    m_level[ ERROR ] = (getenv("LOG_ERROR") && getenv("LOG_ERROR")[0]) ? 0 : 1;
    return OK;
}

// 设置日志级别
void Log::SetLevel(Level level, bool open/*=true*/)
{
    m_level[ level ] = (open ? 1 : 0);
}

// 获取日志级别：true为开，false为关
bool Log::GetLevel(Level level)
{
    return m_level[ level ] == 1 ? true : false;
}

void Log::Out(Level level, const Log *logger, const char *format, va_list ap)
{
    // printf("[%s] [%s:%d] [%s]>>> ", m_time, m_file, m_line, level);
    printf("[%s] [%s] [%s:%d] [%lu]>>> ",
                logger->m_time,
                LevelToStr(level),
                logger->m_file,
                logger->m_line,
                pthread_self());
    vprintf(format, ap);
    printf("\n");
}


// 调试信息
void Log::DebugLog(const char * format, ...)
{
    if( 0 == Log::instance()->m_level[ DEBUG ] )
    {
        return;
    }

    // 注意，有多线程写日志，须加锁；
    UNIQUE_LOCK( *(Log::instance()->m_lock) );

    va_list ap;
    va_start(ap, format);
    Log::instance()->Out(DEBUG, this, format, ap);
    va_end(ap);
}

// 一般性提示信息
void Log::InfoLog(const char * format, ...)
{
    if( 0 == Log::instance()->m_level[ INFO ] )
    {
        return;
    }

    // 注意，有多线程写日志，须加锁；
    UNIQUE_LOCK( *(Log::instance()->m_lock) );

    va_list ap;
    va_start(ap, format);
    Log::instance()->Out(INFO, this, format, ap);
    va_end(ap);
}

// 出错信息
void Log::ErrorLog(const char * format, ...)
{
    if( 0 == Log::instance()->m_level[ ERROR ] )
    {
        return;
    }

    // 注意，有多线程写日志，须加锁；
    UNIQUE_LOCK( *(Log::instance()->m_lock) );

    va_list ap;
    va_start(ap, format);
    Log::instance()->Out(ERROR, this, format, ap);
    va_end(ap);
}



















}// end of LOG_SPACE
