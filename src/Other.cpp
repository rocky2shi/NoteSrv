// Rocky 2010-05-27 16:51:56
#include "Timer.h"
#include "GetFileList.h"
#include "User.h"
#include "Counter.h"
#include "Other.h"
namespace OTHER_SPACE
{




/*
 * ������ʱĿ¼���ɶ�ʱ��ѭ�����ã�
 */
static void ClearTmpDir(const string &path)
{
    // ����Ŀ¼
    GetFileList dir( path );
    GetFileList::file_iterator it(dir);
    const int LONG_TIME = 259200; // = 3 * 24 * 3600�루����ǰ�� [XXX]

    while( it.next() )
    {
        // ɾ���Ƚ�����ļ�
        if(time(NULL) - GetFileModifyTime(it.name()) > LONG_TIME)
        {
            LOG_DEBUG("Delete file: [%s]", it.name());
            DeleteFile(it.name());
        }
    }
}

/*
 * ����ȫ����ʱĿ¼
 */
static void ClearGlobalTmpDir(void *)
{
    const string &global = GlobalConfig::instance()->SystemTmpDir();
    ClearTmpDir( global );
    LOG_DEBUG("Global temp dir: [%s]", global.c_str());
}


// ����Ƿ�Ϊ��Ҫ����ɾ�����û���������ɾ���û�Ŀ¼�����������ݣ�
static int DealDeleteUser(User *user)
{
    /*
     * ����û�״̬
     */

    if("delete" != user->GetInfo("status"))
    {
        LOG_DEBUG("Ignore...");
        // ���Ǳ�ɾ�����û�������ERR�������ִ���²�������
        return ERR;
    }

    const int MAX_DEADLINE = 1800; // ��Сʱ
    long modify = KeyToSecond( user->GetInfo("modify") ); // �޸ģ�ɾ����ʱ��
    if(time(NULL) - modify < MAX_DEADLINE)
    {
        LOG_DEBUG("Ignore...");
        // ������������ޣ�������������OK���ʾ����Ҫ��ִ���²�������
        return OK;
    }

    /*
     * ִ������ɾ���û��������ݣ�
     */

    // ����
    UNIQUE_LOCK( user->GetLock() );

    // ȡ�û�Ŀ¼
    const string dir = user->UserDir();
    int ret;

    ret = DeleteDir(dir.c_str());
    if(ret < 0)
    {
        LOG_ERROR("Delete directory error: [%s]", dir.c_str());
        return ERR;
    }
    LOG_INFO("Delete OK, user: [%s]", dir.c_str());

    // ���»���
    user->Syn();

    return OK;
}

/*
 * ����ÿ���û���ִ���������û���ʱĿ¼��������������ɾ���û���
 */
static void ClearUser(void *)
{
    // ȡ�û����Ŀ¼
    const string &users = GlobalConfig::instance()->UserRootDir();
    // ����
    GetFileList dir( users );
    GetFileList::dir_iterator it(dir);

    while( it.next() )
    {
        const string &username = GetBaseName(it.name());

        /*
         * ����ǰĿ¼
         *
         *      1. �û�������ݵĲ���һ��ͨ��User���������Ӧʹ��Ӳ���룻
         *      2. Ŀ¼����Ϊ�û���
         */
        User *user = User::Get( username );

        // �鿴��ǰ�û�״̬����Ϊɾ��״��������֮��
        if( DealDeleteUser(user) == OK )
        {
            LOG_DEBUG("Ignore... [%s]", username.c_str());
            continue;
        }

        // ������Ч�û�
        if( ! user->isValid() )
        {
            LOG_DEBUG("Ignore... [%s]", username.c_str());
            continue;
        }

        const string &tmp = user->TmpDir();
        ClearTmpDir( tmp );
        LOG_DEBUG("Users temp dir: [%s]", tmp.c_str());
    }
}

// ���ϵͳĿ¼���������򱨾�����������
static int CheckSystemDir(const string &dir)
{
    // �Ƿ�ΪĿ¼
    if( ! isDir(dir) )
    {
        printf("Directory [%s] lost.\n", dir.c_str());
        return ERR;
    }
    return OK;
}

// ����ҳ�������
static void SavePageCounter(void *)
{
    // �������б�
    Counter::iterator it;
    // ȡҳ�����������ļ�
    const string &cfg = GlobalConfig::instance()->CounterDir() + "page.txt";

    // �ȶ���ԭ����
    Ini data( cfg );

    // ������м�����
    while( it.next() )
    {
        // ����������ҳ��ļ�����
        if("page" != it->GetType())
        {
            continue;
        }

        // ����
        data.Set("page", it->GetName(), IntToString(it->GetCount()));
    }

    // ����
    data.Write();
}


// main()�е��õĳ�ʼ��
int OtherInit()
{
    int ret;

    // ע��[����ȫ����ʱĿ¼����]����ʱ��
    ret = Timer::Register(ClearGlobalTmpDir, NULL, "Clear global temp dir.");
    if(ret < 0)
    {
        LOG_ERROR("Timer::Register(global) error.");
        return ERR;
    }

    // ע��[�����û���ʱĿ¼����]����ʱ��
    ret = Timer::Register(ClearUser, NULL, "Clear users temp dir.");
    if(ret < 0)
    {
        LOG_ERROR("Timer::Register(users) error.");
        return ERR;
    }

    // ע��[����ҳ�������]����ʱ��
    ret = Timer::Register(SavePageCounter, NULL, "Save page counter.");
    if(ret < 0)
    {
        LOG_ERROR("Timer::Register error: Save page counter.");
        return ERR;
    }

    return OK;
}


// ϵͳ��ʼ������顢����ϵͳĿ¼�ȣ�
int SystemInit()
{
    // ���ù���Ŀ¼
    if( CheckSystemDir( GlobalConfig::instance()->Root() ) < 0
        // || ChangeDir( GlobalConfig::instance()->Root() ) < 0
      )
    {
        return ERR;
    }

    // ע�������ִ��˳���磺����SystemDataDir()��������UserRootDir()��
    if( CheckSystemDir( GlobalConfig::instance()->CgiDir() ) < 0
        || CheckSystemDir( GlobalConfig::instance()->HtmlDir() ) < 0
        || CheckSystemDir( GlobalConfig::instance()->TemplateDir() ) < 0
        || CheckSystemDir( GlobalConfig::instance()->SystemLogDir() ) < 0
        || CheckSystemDir( GlobalConfig::instance()->SystemTmpDir() ) < 0
        || CheckSystemDir( GlobalConfig::instance()->SystemDataDir() ) < 0
        || CheckSystemDir( GlobalConfig::instance()->ToolDir() ) < 0
        || CheckSystemDir( GlobalConfig::instance()->UserRootDir() ) < 0
        || CheckSystemDir( GlobalConfig::instance()->CounterDir() ) < 0
      )
    {
        printf("Directory not integrality.\n");
        return ERR;
    }


    return OK;
}

}// end of OTHER_SPACE
