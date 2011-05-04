#ifndef __MALLOC_H_20100517__
#define __MALLOC_H_20100517__
#include "Common.h"
#include <iconv.h>
namespace MALLOC_SPACE
{




// 内存按需分配 [2010-05-17]
class Malloc
{
public:
    Malloc(char *pFixBuf, const int nFixLen, const int nNeedLen)
    {
        if(nNeedLen > nFixLen)
        {
            m_buf = new char[ nNeedLen ];
            m_len = nNeedLen;
            m_dynamic = true;
        }
        else
        {
            m_buf = pFixBuf;
            m_len = nFixLen;
            m_dynamic = false;
        }
    }
    ~Malloc()
    {
        // 若是动态分配的，需释放；
        if( m_dynamic )
        {
            Debug("delete...");
            delete [] m_buf;
        }
    }

    // 取内存区
    char *Get() const
    {
        return m_buf;
    }

    // 返回内存长度
    unsigned int Size() const
    {
        return m_len;
    }


private:
    char *m_buf;
    int m_len;
    bool m_dynamic; // m_buf指向是动态分配的则为true;
};










}// end of MALLOC_SPACE
using namespace MALLOC_SPACE;
#endif // end of __MALLOC_H_20100517__

