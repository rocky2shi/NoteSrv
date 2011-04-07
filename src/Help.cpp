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
 简述: 随手记录文章等信息                                                    \n\
                                                                             \n\
 Usage: NoteSrv [port [root_dir]]                                            \n\
    --changes   : 列出修改记录                                               \n\
    --help      : 显示此帮助                                                 \n\
    -h          : 显示此帮助                                                 \n\
                                                                             \n"
        );
}

// 列出修改记录
void Changes()
{
    printf("*********************** 修改记录 ***********************\n");
    printf("%s\n", CHANGES);
}
