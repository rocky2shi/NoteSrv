// Rocky 2009-08-11 10:48:08
#include "Changes.h"
#include "Version.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>


void Help()
{
    printf("\
*********************************************************************        \n\
          rocky2shi@126.com  , "__DATE__" "__TIME__" , "VV"                  \n\
*********************************************************************        \n\
                                                                             \n\
 ����: ���ּ�¼���µ���Ϣ                                                    \n\
                                                                             \n\
 Usage: NoteSrv [port [root_dir]]                                            \n\
    --changes   : �г��޸ļ�¼                                               \n\
    --help      : ��ʾ�˰���                                                 \n\
    -h          : ��ʾ�˰���                                                 \n\
                                                                             \n"
        );
}

// �г��޸ļ�¼
void Changes()
{
    printf("*********************** �޸ļ�¼ ***********************\n");
    printf("%s\n", CHANGES);
}
