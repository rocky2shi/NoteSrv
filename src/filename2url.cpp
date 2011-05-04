// Rocky 2010-04-07 18:07:41


#include "Common.h"
#include "Connect.h"
#include "ConnectQueue.h"
#include "FileObj.h"
#include "GetFileList.h"
#include "Ini.h"
#include "UserData.h"
#include "GlobalConfig.h"
#include "ThreadPool.h"
#include "User.h"
#include "Menu.h"
#include "Encrypt.h"
#include "ChineseCoding.h"
#include "Submit.h"
#include "Compress.h"
#include "Session.h"
#include "User.h"
#include "Menu.h"
#include "Module.h"
#include "Page.h"
#include "ThreadPool.h"
#include "Counter.h"
#include "PageCfg.h"
#include "DataPack.h"
#include "MultiStringMatch.h"
#include "Timer.h"
#include "Other.h"




#undef Debug
#define Debug(format, args...)   printf("%s:%d : " format "\n", __FILE__, __LINE__, ##args)



int main(int argc, char *argv[])
{
    FUNCTION_TRACK(); // 函数轨迹跟综


    {/****************************** 初始化 开始 ******************************/

    int ret;

    // 日志显到到前台
    setenv("LOG_TERMINAL", "1", 1);
    // 关闭日志输出
    setenv("LOG_DEBUG", "0", 1);
    setenv("LOG_INFO", "0", 1);
    setenv("LOG_ERROR", "0", 1);

    // 初始化全局配置类
    ret = GlobalConfig::init(argc, argv);
    if(ret < 0)
    {
        LOG_ERROR("初始化全局配置类出错 [%d] \n", ret);
        return ret;
    }


    // 初始化日志处理类
    ret = Log::init();
    if(ret < 0)
    {
        LOG_ERROR("初始化日志处理类出错 [%d] \n", ret);
        return ret;
    }



    }/****************************** 初始化 结束 ******************************/


    {// 测试 工具：文件名转为url编码方式；[Rocky 2010-06-04 16:09:49]
    #if 1


        bool bToUrl;

        if(argc>1 && '1' == argv[1][0])
        {
            bToUrl = true;
        }
        else if(argc>1 && '2' == argv[1][0])
        {
            bToUrl = false;
        }
        else
        {
            printf("Usage: filename2url {1|2}\n"
                   " 1: filename to url\n"
                   " 2: url to filename\n"
                  );
            return 1;
        }

        GetFileList list("./");

        GetFileList::file_iterator file(list);

        if(bToUrl)
        {
            /*
             * 文件名转为url编码形式
             */

            while( file.next() )
            {
                char key[1024] = "";
                char dot = 'X';
                const string old = GetBaseName( file.name() );
                string now;
                int ret = sscanf(old.c_str(), "%[^.]%c", key, &dot);

                // 只转换符合格式的
                if(2 == ret && strlen(key) == 14 && '.' == dot)
                {
                    // 去掉前缀
                    string old2 = old.substr(15);
                    // 先尝转为gb18330编码，转换出错则保留原值；
                    ChineseCoding("utf-8", "gb18030").Converter(old2, old2);
                    now = key + string(".") + FilenameEncode(old2);
                    printf("[%s] => [%s]\n", old.c_str(), now.c_str());
                    rename(old.c_str(), now.c_str());
                }
            }
        }
        else
        {
            /*
             * 文件名由url编码形式转为可读形式
             */

            while( file.next() )
            {
                char key[1024] = "";
                char dot = 'X';
                const string old = GetBaseName( file.name() );
                int ret = sscanf(old.c_str(), "%[^.]%c", key, &dot);

                // 只转换符合格式的
                if(2 == ret && strlen(key) == 14 && '.' == dot)
                {
                    // 去掉前缀
                    const string now = key + string(".") + FilenameDecode(old.substr(15));
                    printf("[%s] => [%s]\n", old.c_str(), now.c_str());
                    rename(old.c_str(), now.c_str());
                }
            }
        }

    #endif
    }


    //sleep(2);
    return 0;
}





