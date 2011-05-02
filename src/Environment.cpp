// Rocky 2010-04-29 10:11:47
#include "Environment.h"
#include "Log.h"
namespace ENVIRONMENT_SPACE
{





Environment::Environment()
{
    FUNCTION_TRACK(); // �����켣����
}


/*
 * ����Ҫȫ�֣�������ģ�飩������루��������¼���У�
 */
int Environment::Insert(const string &id, void *obj)
{
    // ��������£������ظ�����ͬһ����
    if( NULL != instance()->Get( id ) )
    {
        LOG_ERROR("object exist: [%s]", id.c_str());
        return ERR;
    }

    instance()->m_queue[ id ] = obj;
    return OK;
}

/*
 * �Ӽ�¼�л�ȡid��Ӧ�Ķ���
 */
void *Environment::Get(const string &id)
{
    map<string, void *>::iterator it = m_queue.find( id );
    if(m_queue.end() == it)
    {
        return NULL;
    }
    return it->second;
}

/*
 * ���ʼ��
 *  id : ��id����id�����ظ�����ʹ��������
 *  obj: Ҫ����ȫ�ֻ������ࣨ��һ����������
 */
void *Environment::ClassInit(const string &id, void *obj)
{
    if(NULL != obj)
    {
        // ���뻷����¼���Ա�������ģ���лָ�������ͬһ����
        int ret = instance()->Insert(id, obj);
        if(ret < 0)
        {
            LOG_ERROR("Environment::instance()->Insert() error");
            return NULL;
        }
        // ����
        return obj;
    }
    else
    {
        // �ӻ�����¼�����
        void *p = instance()->Get( id );
        if(NULL == p)
        {
            LOG_ERROR("no exist class info: [%s]", id.c_str());
            return NULL;
        }
        return p;
    }
}



}// end of ENVIRONMENT_SPACE
