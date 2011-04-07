#ifndef __COUNTER_H_20100201__
#define __COUNTER_H_20100201__
#include "Common.h"
#include "Help.h"
#include "Debug.h"
#include "Typedef.h"
#include "Environment.h"
namespace COUNTER_SPACE
{





// 计数器类
class Counter
{
public:
    // 迭代器，用于列出所有计数器对象（值）；
    class iterator
    {
    public:
        iterator(): m_index(-1)
        {
        }

        // 指向下一个元素
        bool next()
        {
            m_index++;
            return m_index < Counter::instance()->m_list.size();
        }

        // 重载"->"操作符，返回模块对象地址；
        Counter * operator->()
        {
            return Counter::instance()->m_list[ m_index ];
        }

    private:
        int m_index;
    };

private:
    class Record
    {
    public:
        void Insert(Counter *counter)
        {
            UNIQUE_LOCK(m_lock);
            m_list.push_back(counter);
        }
        vector<Counter *> m_list;  // 计数器列表（记录所有已分配对象）
        Lock m_lock;       // 建创计数器时插入列表保护锁
    };

public:
    // count: 指定初始计数值
    Counter(const string &name="", const string &type="", unsigned int count=0);

    // 类初始化
    static int init();

    // 增加计数
    void Inc();

    // 取计数器值
    unsigned int GetCount() const ;

    // 取计数器名称
    const string GetName() const ;

    // 取类型
    const string GetType() const;

    // 复位计数器（清零）
    void Reset();


    // 单模式中用的取实例接口
    inline static Record *instance(Record *counter=NULL)
    {
        static Record *obj = (Record *)(Environment::instance()->ClassInit("Counter::Record", counter));
        return obj;
    }




private:
    unsigned int m_count;           // 计数器值
    const string m_name;            // 计数器名
    const string m_type;            // 类型（分类）
    Lock m_lock;                    // 读写锁（多读独写）
};





}
using namespace COUNTER_SPACE;
#endif // end of __COUNTER_H_20100201__
