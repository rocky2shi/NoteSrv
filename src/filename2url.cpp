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
    FUNCTION_TRACK(); // �����켣����


    {/****************************** ��ʼ�� ��ʼ ******************************/

    int ret;

    // ��־�Ե���ǰ̨
    setenv("LOG_TERMINAL", "1", 1);
    // �ر���־���
    setenv("LOG_DEBUG", "0", 1);
    setenv("LOG_INFO", "0", 1);
    setenv("LOG_ERROR", "0", 1);

    // ��ʼ��ȫ��������
    ret = GlobalConfig::init(argc, argv);
    if(ret < 0)
    {
        LOG_ERROR("��ʼ��ȫ����������� [%d] \n", ret);
        return ret;
    }


    // ��ʼ����־������
    ret = Log::init();
    if(ret < 0)
    {
        LOG_ERROR("��ʼ����־��������� [%d] \n", ret);
        return ret;
    }



    }/****************************** ��ʼ�� ���� ******************************/


    {// ���� ���ߣ��ļ���תΪurl���뷽ʽ��[Rocky 2010-06-04 16:09:49]
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
             * �ļ���תΪurl������ʽ
             */

            while( file.next() )
            {
                char key[1024] = "";
                char dot = 'X';
                const string old = GetBaseName( file.name() );
                string now;
                int ret = sscanf(old.c_str(), "%[^.]%c", key, &dot);

                // ֻת�����ϸ�ʽ��
                if(2 == ret && strlen(key) == 14 && '.' == dot)
                {
                    // ȥ��ǰ׺
                    string old2 = old.substr(15);
                    // �ȳ�תΪgb18330���룬ת����������ԭֵ��
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
             * �ļ�����url������ʽתΪ�ɶ���ʽ
             */

            while( file.next() )
            {
                char key[1024] = "";
                char dot = 'X';
                const string old = GetBaseName( file.name() );
                int ret = sscanf(old.c_str(), "%[^.]%c", key, &dot);

                // ֻת�����ϸ�ʽ��
                if(2 == ret && strlen(key) == 14 && '.' == dot)
                {
                    // ȥ��ǰ׺
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





