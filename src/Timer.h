#ifndef __TIMER_H_20100525__
#define __TIMER_H_20100525__
#include "Common.h"
namespace TIMER_SPACE
{





// ��ʱ��
class Timer
{
    typedef void (*ExeFunc)(void *);
    struct Task
    {
        ExeFunc exector;    // ����ִ����
        void *param;        // ִ������ʱ�����
        string remark;      // ��ע
        bool invalid;       // �Ƿ���Ч����Ч�Ľ��������

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
        // ��exector����
        bool operator==(const Task& rhs) const
        {
            return (exector == rhs.exector);
        }
    };


public:
    ~Timer();

    // ���ʼ������������ʱִ��һ�Σ�
    static int init();

    /*
     * ע����Ҫ��ʱִ�е�����
     *  exector : ������ӿ�
     *  param   : ��ʱ������exectorʱ�������Ĳ���
     *  remark  : ����˵��
     */
    static int Register(const ExeFunc exector, void *param, const string &remark);

    // ע����ʱ����
    static int UnRegister(const ExeFunc exector);

private:
    Timer();

    // ��ģʽ���õ�ȡʵ���ӿ�
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
    list<Task> m_queue;     // ��ʱ�������
    bool m_exit;            // ���ƶ�ʱ�߳��˳�
    Lock m_lock;
};






}// end of TIMER_SPACE
using namespace TIMER_SPACE;
#endif // end of __TIMER_H_20100525__
