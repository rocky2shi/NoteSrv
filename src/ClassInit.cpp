// Rocky 2010-06-03 14:14:39
#include "ClassInit.h"
#include <stdio.h>
namespace CLASSINIT_SPACE
{


list<ClassInit::ExeFunc> ClassInit::m_HighQueue;
list<ClassInit::ExeFunc> ClassInit::m_LowQueue;



ClassInit::ClassInit(ExeFunc init, InitLevel level/*=INIT_LOW*/)
{
    switch(level)
    {
    case INIT_HIGH:
            ClassInit::RegisterHL(init);
            break;
    case INIT_LOW:
            ClassInit::RegisterLL(init);
            break;
    default:
        printf("ClassInit::ClassInit() error\n");
        assert(0);
    }
}


int ClassInit::init()
{
    list<ExeFunc>::iterator it;
    int ret;

    // 先执行高优先级初始化
    for(it = m_HighQueue.begin(); m_HighQueue.end() != it; it++)
    {
        ret = (*it)();
        if(ret < 0)
        {
            printf("init error\n");
            return ERR;
        }
        // printf("Hight init [%p] OK\n", *it);
    }

    // 再执行低优先级初始化
    for(it = m_LowQueue.begin(); m_LowQueue.end() != it; it++)
    {
        ret = (*it)();
        if(ret < 0)
        {
            printf("init error\n");
            return ERR;
        }
        // printf("Low init [%p] OK\n", *it);
    }

    return OK;
}


int ClassInit::RegisterHL(const ExeFunc exector)
{
    m_HighQueue.insert(m_HighQueue.begin(), exector);
}

int ClassInit::RegisterLL(const ExeFunc exector)
{
    m_LowQueue.insert(m_LowQueue.begin(), exector);
}




}// end of CLASSINIT_SPACE
