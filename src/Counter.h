#ifndef __COUNTER_H_20100201__
#define __COUNTER_H_20100201__
#include "Common.h"
#include "Help.h"
#include "Debug.h"
#include "Typedef.h"
#include "Environment.h"
namespace COUNTER_SPACE
{





// ��������
class Counter
{
public:
    // �������������г����м���������ֵ����
    class iterator
    {
    public:
        iterator(): m_index(-1)
        {
        }

        // ָ����һ��Ԫ��
        bool next()
        {
            m_index++;
            return m_index < Counter::instance()->m_list.size();
        }

        // ����"->"������������ģ������ַ��
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
        vector<Counter *> m_list;  // �������б���¼�����ѷ������
        Lock m_lock;       // ����������ʱ�����б�����
    };

public:
    // count: ָ����ʼ����ֵ
    Counter(const string &name="", const string &type="", unsigned int count=0);

    // ���ʼ��
    static int init();

    // ���Ӽ���
    void Inc();

    // ȡ������ֵ
    unsigned int GetCount() const ;

    // ȡ����������
    const string GetName() const ;

    // ȡ����
    const string GetType() const;

    // ��λ�����������㣩
    void Reset();


    // ��ģʽ���õ�ȡʵ���ӿ�
    inline static Record *instance(Record *counter=NULL)
    {
        static Record *obj = (Record *)(Environment::instance()->ClassInit("Counter::Record", counter));
        return obj;
    }




private:
    unsigned int m_count;           // ������ֵ
    const string m_name;            // ��������
    const string m_type;            // ���ͣ����ࣩ
    Lock m_lock;                    // ��д���������д��
};





}
using namespace COUNTER_SPACE;
#endif // end of __COUNTER_H_20100201__
