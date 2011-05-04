// Rocky 2010-04-30 14:11:41
#include "UserData.h"
#include "User.h"
namespace USERDATA_SPACE
{








// 读取path下所有文件内的数据
UserData::iterator::iterator(const string &path)
                        : m_FilsList(path),
                          m_itFile(m_FilsList),
                          m_first(true)
{
    m_FilsList.SortFilename(); // 排序文件名
}


// 指向下一组数据
bool UserData::iterator::next()
{
    while(1)
    {
        if( m_first )
        {
            m_first = false;
        }
        else
        {
            // 取一个记录
            if( m_itCurrent.next() )
            {
                return true; // 取到记录，返回；
            }
        }

        // 上个文件已读取完成，打开另一个文件；
        if( m_itFile.next() )
        {
            m_Curent.Clear(); // 清掉旧数据

            int ret;
            const char *filename = m_itFile.name();
            ret = m_Curent.Read( filename );
            if(ret < 0)
            {
                continue;   // 接着尝试下一文件
            }
            m_itCurrent = &m_Curent;
        }
        else
        {
            break; // 所有文件都处理完毕，退出；
        }
    }

    return false; // 无数据
}

// 取当前[key/pack]对
int UserData::iterator::Get(string &key, Conf *&pack)
{
    key = m_itCurrent.GetKey();
    pack = m_itCurrent.GetValue();
    return OK; // 取到记录，返回；
}




UserData::UserData(const string &username) : m_username(username)
{
}

UserData::~UserData()
{
}




/*
 * 读取给定具体位置数据。注意，每调用一次，都是打
 * 开一次文件；[XXX]
 */
const string UserData::Get(const string &key, const string &item)
{
    User *user = User::Get( m_username );

    if( ! user->isValid() )
    {
        LOG_ERROR("Invalid user: [%s]", m_username.c_str());
        return "";
    }

    const string &filename = KeyToFile( key ); // 取key对应的文件名
    const string &path = user->DataDir() + filename;

    Ini data( path );

    return data.Get(key, item);
}

/* （从文件中）取数据，填充pack中key对应的值，如：
 *
 *  Ini pack;
 *  pack.Set("20100515020544", "title", "");
 *  pack.Set("20100515020544", "text", "");
 *
 *  Get( pack ); // 这时将填充title、text；
 */
int UserData::Get(Ini &pack)
{
    User *user = User::Get( m_username );
    if( ! user->isValid() )
    {
        LOG_ERROR("Invalid user: [%s]", m_username.c_str());
        return ERR;
    }

    map<string, Ini*> key2ini; // 每个文件名 => ini对象
    Ini::iterator itIni( &pack );

    /*
     * 取出pack中每个数据key，然后从对应文件中读取数据，再填
     * 充到对应的value中；
     */
    while( itIni.next() )
    {
        // 取出[key/Conf]值对
        const string &key = itIni.GetKey();
        const Conf &conf = *(itIni.GetValue());

        /*
         * 1. 取key对应的文件名
         * 2. 看文件是否已被打开，还未打开则打开它，并记入key2ini中；
         * 3. 从文件中读取数据，填充到pack中；
         */
        const string &filename = KeyToFile( key );
        Ini *ini = key2ini[ filename ];
        // 对应文件未加载时，则加载(new Ini())：
        if(NULL == ini)
        {
            // 取路径
            const string &path = user->DataDir() + filename;

            ini = new Ini(path);
            if(NULL == ini)
            {
                LOG_ERROR("new Ini() error");
                return ERR;
            }
            key2ini[ filename ] = ini;
        }

        const Conf *oldConf = ini->Get(key);
        if(NULL == oldConf)
        {
            LOG_DEBUG("Can't find data, key=[%s]", key.c_str());
            continue;
        }

        /* 若外部不填充key对应的conf字段（即为空），则表示把文件中的所
         * 有值填充到pack中，非空，则只填充指定的conf字段；
         */
        if(0 == conf.Size())
        {
            /*
             * 填充key对应的所有字段
             */
            pack.Set(key, *oldConf);
        }
        else
        {
            /*
             * 只填充conf对应的指定字段
             */

            // 取出key对应的Conf值
            Conf::iterator itConf( &conf );
            while( itConf.next() )
            {
                /* 注意这里的name和value:
                 *
                 *  name -- 从外部的pack中传入
                 *  value -- 则是从文件中读取的，将填充
                 *           到pack中name对应的值中
                 */
                const string &name = itConf.GetName();
                const string &value = oldConf->Get(name);

                // 设置到pack中（原值被覆盖）
                pack.Set(key, name, value);
            }
        }
    }

    /*
     * 用完后，释放内存；
     */
    map<string, Ini*>::iterator itSave;
    for(itSave = key2ini.begin(); key2ini.end() != itSave; itSave++)
    {
        delete itSave->second;
    }

    return OK;
}


// 保存数据（单条）
int UserData::Set(const string &key, const string &item, const string &value)
{
    User *user = User::Get( m_username );

    if( ! user->isValid() )
    {
        LOG_ERROR("Invalid user: [%s]", m_username.c_str());
        return ERR;
    }

    const string &filename = KeyToFile( key ); // 取key对应的文件名
    const string &path = user->DataDir() + filename;

    // 读入原数据
    Ini data( path );

    // 写入
    data.Set(key, item, value);

    // 保存
    return data.Write();
}

/*
 * 把pack中的值保存（到文件）
 * force: 1. 为true时，强制写入所有数据；
 *        2. 为false时，只写入新的数据（modify字段决定）
 */
int UserData::Set(const Ini &pack, bool force/*=false*/)
{
    User *user = User::Get( m_username );
    if( ! user->isValid() )
    {
        LOG_ERROR("Invalid user: [%s]", m_username.c_str());
        return ERR;
    }

    map<string, Ini*> key2ini; // 每个文件名 => ini对象
    Ini::iterator itIni( &pack );
    int success = 0; // 设置数据条数成功计数器
    int ignore = 0;  // 忽略计数器

    /*
     * 把pack中的数据保存到相应的文件
     */
    while( itIni.next() )
    {
        // 取出[key/Conf]值对
        const string &key = itIni.GetKey();
        const Conf &conf = *(itIni.GetValue());

        /*
         * 1. 取key对应的文件名
         * 2. 看文件是否已被打开，还未打开则打开它，并记入key2ini中；
         * 3. 填充新数据
         */
        const string &filename = KeyToFile( key );
        Ini *ini = key2ini[ filename ];
        // 对应文件未加载时，则加载(new Ini())：
        if(NULL == ini)
        {
            // 取路径
            const string &path = user->DataDir() + filename;

            ini = new Ini(path);
            if(NULL == ini)
            {
                LOG_ERROR("new Ini() error");
                return ERR;
            }
            key2ini[ filename ] = ini;
        }

        /* 是否只写入最新数据：
         *   1. 不设置强制标志（force==false），且
         * [2010-05-22]
         */
        if( !force )
        {
            // 待写入的数据不比原有数据旧新时
            if( conf.Get("modify") <= ini->Get(key, "modify") )
            {
                ignore++;
                LOG_DEBUG("Igore old: [%s]", key.c_str());
                continue;
            }
            // 忽略已被删除的数据（是否要测试：ini->Get(key, "status") == "delete" [XXX]）
            if( conf.Get("status") == "delete" )
            {
                ignore++;
                LOG_DEBUG("Igore delete: [%s]", key.c_str());
                continue;
            }
        }

        // 填充新数据（原值被覆盖）;
        ini->Set(key, conf);
        success++;
    }

    /*
     * 保存
     */
    map<string, Ini*>::iterator itSave;
    for(itSave = key2ini.begin(); key2ini.end() != itSave; itSave++)
    {
        Ini *ini = itSave->second;
        ini->Write();   // 写入
        delete ini;     // 用完后，释放内存；
    }

    // 记录成功、忽略数
    m_result.Set("success", IntToString(success));
    m_result.Set("ignore", IntToString(ignore));

    return OK;
}

// 取key对应数据的附件名列表到attachs中     [Rocky 2010-05-18 18:08:42]
int UserData::GetAttachList(const string &key, vector<string> &attachs) const
{
    User *user = User::Get(m_username);
    // 取附件目录
    const string &path = user->AttachDir();
    GetFileList list(path);
    GetFileList::file_iterator itAttach( list );

    // 扫描每个附件名，取出前缀是key的文件
    while( itAttach.next() )
    {
        const char *filename = GetBaseName( itAttach.name() );
        if( strncmp(filename, key.c_str(), key.length()) == 0 )
        {
            attachs.push_back( itAttach.name() );
        }
    }
    return OK;
}

// 取操作结果
const string UserData::GetResult(const string &field) const
{
    return m_result.Get(field);
}













}// end of USERDATA_SPACE
