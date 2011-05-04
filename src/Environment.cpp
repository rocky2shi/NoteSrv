// Rocky 2010-04-29 10:11:47
#include "Environment.h"
#include "Log.h"
namespace ENVIRONMENT_SPACE
{





Environment::Environment()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}


/*
 * 把需要全局（主、子模块）对象存入（环境）记录链中；
 */
int Environment::Insert(const string &id, void *obj)
{
    // 正常情况下，不会重复插入同一对象；
    if( NULL != instance()->Get( id ) )
    {
        LOG_ERROR("object exist: [%s]", id.c_str());
        return ERR;
    }

    instance()->m_queue[ id ] = obj;
    return OK;
}

/*
 * 从记录中获取id对应的对象
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
 * 类初始化
 *  id : 类id各类id不可重复，可使用类名；
 *  obj: 要记入全局环境的类（用一对象来代表）
 */
void *Environment::ClassInit(const string &id, void *obj)
{
    if(NULL != obj)
    {
        // 插入环境记录，以便在其它模块中恢复（共用同一对象）
        int ret = instance()->Insert(id, obj);
        if(ret < 0)
        {
            LOG_ERROR("Environment::instance()->Insert() error");
            return NULL;
        }
        // 返回
        return obj;
    }
    else
    {
        // 从环境记录里查找
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
