// Rocky 2010-02-01 14:48:17
#include "Counter.h"
namespace COUNTER_SPACE
{







Counter::Counter(const string &name/*=""*/, const string &type/*=""*/, unsigned int count/*=0*/)
            : m_count(count), m_name(name), m_type(type)
{
    Counter::instance()->Insert(this); // ��¼ÿ������������
}

// ���ʼ��
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

// ���Ӽ���
void Counter::Inc()
{
    UNIQUE_LOCK(m_lock);    // �����ж��̲߳���
    m_count++;
}

// ȡ������ֵ
unsigned int Counter::GetCount() const
{
    return m_count;
}

// ȡ����������
const string Counter::GetName() const
{
    return m_name;
}

// ȡ����
const string Counter::GetType() const
{
    return m_type;
}

// ��λ�����������㣩
void Counter::Reset()
{
    UNIQUE_LOCK(m_lock);    // ���ܺ�Inc()�г�ͻ���������
    m_count = 0;
}




















}
