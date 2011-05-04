// Rocky 2010-02-01 14:48:17
#include "Counter.h"
namespace COUNTER_SPACE
{







Counter::Counter(const string &name/*=""*/, const string &type/*=""*/, unsigned int count/*=0*/)
            : m_count(count), m_name(name), m_type(type)
{
    Counter::instance()->Insert(this); // 记录每个计数器对象
}

// 类初始化
int Counter::init()
{
    int ret;
    static Record counter;

    Record *obj = Counter::instance( &counter );
    if(NULL == obj)
    {
        LOG_ERROR("Counter::instance() error");
        return ERR;
    }

    return OK;
}

// 增加计数
void Counter::Inc()
{
    UNIQUE_LOCK(m_lock);    // 可能有多线程操作
    m_count++;
}

// 取计数器值
unsigned int Counter::GetCount() const
{
    return m_count;
}

// 取计数器名称
const string Counter::GetName() const
{
    return m_name;
}

// 取类型
const string Counter::GetType() const
{
    return m_type;
}

// 复位计数器（清零）
void Counter::Reset()
{
    UNIQUE_LOCK(m_lock);    // 可能和Inc()有冲突，需加锁；
    m_count = 0;
}




















}
