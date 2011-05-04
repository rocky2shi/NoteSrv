#ifndef __TIMER_H_20100525__
#define __TIMER_H_20100525__
#include "Common.h"
namespace TIMER_SPACE
{





// 定时器
class Timer
{
    typedef void (*ExeFunc)(void *);
    struct Task
    {
        ExeFunc exector;    // 任务执行器
        void *param;        // 执行器调时的入参
        string remark;      // 备注
        bool invalid;       // 是否有效（无效的将被清除）

        Task(): exector(NULL),
                param(NULL),
                invalid(false)
        {
        }
        Task(const Task &task): exector(task.exector),
                                param(task.param),
                                remark(task.remark),
                                invalid(task.invalid)
        {
        }
        // 按exector查找
        bool operator==(const Task& rhs) const
        {
            return (exector == rhs.exector);
        }
    };


public:
    ~Timer();

    // 类初始化（程序启动时执行一次）
    static int init();

    /*
     * 注册需要定时执行的任务
     *  exector : 任务处理接口
     *  param   : 定时器调用exector时传入它的参数
     *  remark  : 任务说明
     */
    static int Register(const ExeFunc exector, void *param, const string &remark);

    // 注销定时任务
    static int UnRegister(const ExeFunc exector);

private:
    Timer();

    // 单模式中用的取实例接口
    inline static Timer *instance(Timer *exector=NULL)
    {
        static Timer *obj = (Timer *)(Environment::instance()->ClassInit("::Timer", exector));
        return obj;
    }

    int Insert(const Task &task);
    int Delete(const ExeFunc exector);
    int ClearInvalid();
    static void Run(Timer *timer);

private:
    list<Task> m_queue;     // 定时任务队列
    bool m_exit;            // 控制定时线程退出
    Lock m_lock;
};






}// end of TIMER_SPACE
using namespace TIMER_SPACE;
#endif // end of __TIMER_H_20100525__
