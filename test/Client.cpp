#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include "yCTcp.h"



#define Print1  printf
#define LR      "\n"
#define ERR     "\n"

// ��ӡ������ÿn�����һ�Ρ�
#define PrintCount(n)                   \
{                                       \
    static unsigned int nCount=0;       \
    nCount++;                           \
    if( nCount%(n) == 0 )               \
    {                                   \
        printf("%d\n", nCount);         \
    }                                   \
}



// ����ָ�����Ȼ��������ݵ�socket
bool WriteToSocketN(const yCTcp &hTcp, const void *pData, int nDataLen)
{
    int ret;

    assert(NULL != pData);

    ret = hTcp.Sendn(pData, nDataLen, yCTcp::TCP_TIMEOUT*100);
    if( yCTcp::SOCK_TIMEOUT == ret )
    {
        Print1("Sendn timeout[SOCK_TIMEOUT] " ERR LR);
        return false;
    }
    else if(yCTcp::SOCK_ERROR == ret)
    {
        Print1("Sendn timeout[SOCK_ERROR] " ERR LR);
        return false;
    }
    return true;
}


int main(int argc, char *argv[])
{
    int i = 0;

    // ����socket
    yCTcp hTcp;

    if( ! hTcp.Open() )
    {
        Print1("err: ���ܴ�socket ");
        return 1;
    }

    //
    if(! hTcp.Connect("127.0.0.1", 17890))
    {
        Print1("err: ��������\n");
        return 2;
    }


    for(i=0; i<100; i++)
    {
        char buf[100] = "";
        sprintf(buf, "%s: %d", argv[1]?:"*", i);
        int ret = WriteToSocketN(hTcp, buf, sizeof(buf));
        Print1("%s, %d\n", buf, ret);
        usleep(100000);
    }


    return 0;
}
