// Rocky 2010-05-27 16:51:56
#include "Timer.h"
#include "GetFileList.h"
#include "User.h"
#include "Counter.h"
#include "Other.h"
namespace OTHER_SPACE
{




/*
 * 清理临时目录（由定时器循环调用）
 */
static void ClearTmpDir(const string &path)
{
    // 遍历目录
    GetFileList dir( path );
    GetFileList::file_iterator it(dir);
    const int LONG_TIME = 259200; // = 3 * 24 * 3600秒（三天前） [XXX]

    while( it.next() )
    {
        // 删除比较早的文件
        if(time(NULL) - GetFileModifyTime(it.name()) > LONG_TIME)
        {
            LOG_DEBUG("Delete file: [%s]", it.name());
            DeleteFile(it.name());
        }
    }
}

/*
 * 清理全局临时目录
 */
static void ClearGlobalTmpDir(void *)
{
    const string &global = GlobalConfig::instance()->SystemTmpDir();
    ClearTmpDir( global );
    LOG_DEBUG("Global temp dir: [%s]", global.c_str());
}


// 检查是否为需要清理（删除的用户），是则删除用户目录（及所有数据）
static int DealDeleteUser(User *user)
{
    /*
     * 检测用户状态
     */

    if("delete" != user->GetInfo("status"))
    {
        LOG_DEBUG("Ignore...");
        // 不是被删除的用户，返回ERR（则接着执行下步操作）
        return ERR;
    }

    const int MAX_DEADLINE = 1800; // 半小时
    long modify = KeyToSecond( user->GetInfo("modify") ); // 修改（删除）时间
    if(time(NULL) - modify < MAX_DEADLINE)
    {
        LOG_DEBUG("Ignore...");
        // 不到达最后期限，保留；（返回OK则表示不需要再执行下步操作）
        return OK;
    }

    /*
     * 执行清理被删除用户（及数据）
     */

    // 锁定
    UNIQUE_LOCK( user->GetLock() );

    // 取用户目录
    const string dir = user->UserDir();
    int ret;

    ret = DeleteDir(dir.c_str());
    if(ret < 0)
    {
        LOG_ERROR("Delete directory error: [%s]", dir.c_str());
        return ERR;
    }
    LOG_INFO("Delete OK, user: [%s]", dir.c_str());

    // 更新缓存
    user->Syn();

    return OK;
}

/*
 * 遍历每个用户，执行清理：对用户临时目录调用清理函数，或删除用户；
 */
static void ClearUser(void *)
{
    // 取用户存放目录
    const string &users = GlobalConfig::instance()->UserRootDir();
    // 遍历
    GetFileList dir( users );
    GetFileList::dir_iterator it(dir);

    while( it.next() )
    {
        const string &username = GetBaseName(it.name());

        /*
         * 清理当前目录
         *
         *      1. 用户相关数据的操作一律通过User类操作，不应使用硬编码；
         *      2. 目录名即为用户名
         */
        User *user = User::Get( username );

        // 查看当前用户状态，若为删除状，则清理之；
        if( DealDeleteUser(user) == OK )
        {
            LOG_DEBUG("Ignore... [%s]", username.c_str());
            continue;
        }

        // 跳过无效用户
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

// 检查系统目录，不存在则报警，并创建；
static int CheckSystemDir(const string &dir)
{
    // 是否为目录
    if( ! isDir(dir) )
    {
        printf("Directory [%s] lost.\n", dir.c_str());
        return ERR;
    }
    return OK;
}

// 保存页面计数器
static void SavePageCounter(void *)
{
    // 计数器列表
    Counter::iterator it;
    // 取页面计数器存放文件
    const string &cfg = GlobalConfig::instance()->CounterDir() + "page.txt";

    // 先读出原数据
    Ini data( cfg );

    // 检查所有计数器
    while( it.next() )
    {
        // 跳过不属于页面的计数器
        if("page" != it->GetType())
        {
            continue;
        }

        // 设置
        data.Set("page", it->GetName(), IntToString(it->GetCount()));
    }

    // 保存
    data.Write();
}


// main()中调用的初始化
int OtherInit()
{
    int ret;

    // 注册[清理全局临时目录功能]到定时器
    ret = Timer::Register(ClearGlobalTmpDir, NULL, "Clear global temp dir.");
    if(ret < 0)
    {
        LOG_ERROR("Timer::Register(global) error.");
        return ERR;
    }

    // 注册[清理用户临时目录功能]到定时器
    ret = Timer::Register(ClearUser, NULL, "Clear users temp dir.");
    if(ret < 0)
    {
        LOG_ERROR("Timer::Register(users) error.");
        return ERR;
    }

    // 注册[保存页面计数器]到定时器
    ret = Timer::Register(SavePageCounter, NULL, "Save page counter.");
    if(ret < 0)
    {
        LOG_ERROR("Timer::Register error: Save page counter.");
        return ERR;
    }

    return OK;
}


// 系统初始化（检查、创建系统目录等）
int SystemInit()
{
    // 设置工作目录
    if( CheckSystemDir( GlobalConfig::instance()->Root() ) < 0
        // || ChangeDir( GlobalConfig::instance()->Root() ) < 0
      )
    {
        return ERR;
    }

    // 注意这里的执行顺序（如：先有SystemDataDir()，才能有UserRootDir()）
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
