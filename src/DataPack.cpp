// Rocky 2010-05-18 18:09:19
#include "User.h"
#include "DataPack.h"
namespace DATAPACK_SPACE
{





// 调用外部perl代码，合并书签（使用中间文件）   [Rocky 2010-05-26 15:47:33]
static const string MergeBookmark(const string &bookmark1, const string &bookmark2)
{
    struct AutoDelete
    {
        AutoDelete(const string &file1, const string &file2, const string &file3)
            : m_file1(file1), m_file2(file2), m_file3(file3)
        {
        }
        ~AutoDelete()
        {
            Debug("Delete file: [%s] [%s] [%s]",
                    m_file1.c_str(), m_file2.c_str(), m_file3.c_str());
            DeleteFile(m_file1);
            DeleteFile(m_file2);
            DeleteFile(m_file3);
        }
    private:
        const string &m_file1;
        const string &m_file2;
        const string &m_file3;
    };
    const string tmpfile1 = StringToTempFile(bookmark1);
    const string tmpfile2 = StringToTempFile(bookmark2);
    const string tmpfile3 = StringToTempFile("");
    AutoDelete tmp(tmpfile1, tmpfile2, tmpfile3);

    Debug("[%s]", tmpfile1.c_str());
    Debug("[%s]", tmpfile2.c_str());
    Debug("[%s]", tmpfile3.c_str());

    // 调外部命令处理此文件
    const string cmd = GlobalConfig::instance()->ToolDir() + "MergeBookmark.pl "
                       "'" + tmpfile1 + "' "
                       "'" + tmpfile2 + "' "
                       "'" + tmpfile3 + "' ";
    int ret = system(cmd.c_str());
    LOG_DEBUG("ret=[%d] cmd=[%s]", ret, cmd.c_str());
    if(0 != ret)
    {
        LOG_ERROR("Command error: [%s]", cmd.c_str());
        // 出错则保留原值（bookmark2应为本地书签数据（非上传的数据））
        return bookmark2;
    }

    return FileToStr(tmpfile3);
}








const string DataPack::CURRENT_VERSION = "20100519100025";


DataPack::DataPack(const string &username)
            : m_UserData(username),
              m_username(username),
              m_opr(OPR_UNKNOWN)
{
    // 属性标志置空
    memset(m_attribs, 0, sizeof(m_attribs));
}

DataPack::~DataPack()
{
    Clear();
}

// 清理
void DataPack::Clear()
{
    LOG_DEBUG("Delete file: [%s]", m_ini.Fullname().c_str());
    DeleteFile(m_ini.Fullname());
}


// 设置打包信息（成功返回OK）
int DataPack::MakePack()
{
    // 只能设置一次操作类型
    if(OPR_UNKNOWN != m_opr)
    {
        LOG_ERROR("error: operator had beeg setted, current m_opr=[%d]", m_opr);
        return ERR;
    }

    User *user = User::Get(m_username);

    // 构造一临时文件名
    const string &file = user->TmpDir() + GetRandomString();
    m_pack.Open(file + ".pack", FileObj::W);
    m_ini.Open(file + ".data", FileObj::W);

    // 写入头部打包代码的版本信息
    Version v;
    m_pack.Write(&v, sizeof(v));

    // 标明操作类型
    m_opr = OPR_MAKEPACK;

    return OK;
}

// 设置打包属性
void DataPack::SetAttrib(Attrib type, bool valid/*=true*/)
{
    m_attribs[type] = valid;
}

// 加入需要打包的用户数据（及附件）
int DataPack::Add(const Ini &pack)
{
    // 非打包操作，则出错。
    if(OPR_MAKEPACK != m_opr)
    {
        LOG_ERROR("operator error, m_opr=[%d]", m_opr);
        return ERR;
    }

    Ini::iterator itIni( &pack );

    // 写用户时数据到临时文件，同把附件打包；
    while( itIni.next() )
    {
        /*
         * 写入数据
         */
        Conf *conf = itIni.GetValue();
        const string key = string("\n[") + itIni.GetKey() + string("]") + "\n";
        if(0 == m_ini.Write(key.c_str(), key.length()))
        {
            LOG_ERROR("Write error");
            return ERR;
        }

        Conf::iterator itConf(conf);
        while( itConf.next() )
        {
            const string pair = itConf.GetName() + string("=") + itConf.GetValue() + string("\n");
            m_ini.Write(pair.c_str(), pair.length());
        }


        // 是否允许打包附件（设置此标志说明不需打包附件）
        if( m_attribs[A_EXCLUDE_ATTACH] )
        {
            continue;
        }

        /*
         * 取key数据的附件列表，并打包；
         */
        vector<string> attachs;
        vector<string>::iterator it;

        // 取附件列表
        m_UserData.GetAttachList(itIni.GetKey(), attachs);
        LOG_DEBUG("[%d]", attachs.size());

        for(it = attachs.begin(); attachs.end() != it; it++)
        {
            const string &fullname = *it;
            LOG_DEBUG("Pack file: [%s]", fullname.c_str());
            // 打包
            int ret = FilePack(fullname, FT_ATTACH);
            if(ret < 0)
            {
                LOG_ERROR("Pack error, file: [%s]", fullname.c_str());
                return ERR;
            }
        }
    }

    /*
     * 注：对m_ini的打包由OtherPack()来做；
     */

    return OK;
}

// 打包所有用户数据
int DataPack::AddAll()
{
    // 非打包操作，则出错。
    if(OPR_MAKEPACK != m_opr)
    {
        LOG_ERROR("operator error, m_opr=[%d]", m_opr);
        return ERR;
    }

    User *user = User::Get(m_username);

    /*
     * 遍历附件目录，取出所有附件来打包；
     */
    // 是否允许打包附件（不设置此标志说明需打包附件）
    if( ! m_attribs[A_EXCLUDE_ATTACH] )
    {
        GetFileList attachs( user->AttachDir() );
        GetFileList::file_iterator itAttach( attachs );

        // 扫描每个附件名，取出前缀是key的文件
        while( itAttach.next() )
        {
            const char *filename = itAttach.name();
            int ret = FilePack(filename, FT_ATTACH);
            if(ret < 0)
            {
                LOG_ERROR("Pack error, file: [%s]", filename);
                return ERR;
            }
        }
    }

    /*
     * 再打包所有数据文件
     */
    {
        GetFileList datas( user->DataDir() );
        GetFileList::file_iterator itData( datas );

        // 扫描每个附件名，取出前缀是key的文件
        while( itData.next() )
        {
            const char *filename = itData.name();
            int ret = FilePack(filename, FT_DATA);
            if(ret < 0)
            {
                LOG_ERROR("Pack error, file: [%s]", filename);
                return ERR;
            }
        }
    }

    return OK;
}

// 打包其它项：菜单、各页面配置等；
int DataPack::OtherPack()
{
    User *user = User::Get(m_username);

    /*
     * 当是调用Add()来打包的，则m_ini不为空，若是调AddAll()，
     * 则m_ini为空；
     */
    if( m_ini.Size() > 0 )
    {
        /*
         * 打包数据文件
         */
        m_ini.Close(); // 使缓冲区数据写入，否则可能打包不完整；
        int ret = FilePack(m_ini.Fullname(), FT_DATA);
        if(ret < 0)
        {
            LOG_ERROR("Pack error, file: [%s]", m_ini.Fullname().c_str());
            return ERR;
        }
    }


    /*
     * 打包各下拉菜单数据（位于用户目录的menu子目录中的文件）
     */
    {
        GetFileList menus( user->MenuDir() );
        GetFileList::file_iterator it( menus );

        // 扫描每个附件名，取出前缀是key的文件
        while( it.next() )
        {
            const char *filename = it.name();
            int ret = FilePack(filename, FT_MENU_CFG);
            if(ret < 0)
            {
                LOG_ERROR("Pack error, file: [%s]", filename);
                return ERR;
            }
        }
    }

    /*
     * 打包各页面数据（位于用户目录出‘.txt’为后缀的文件）
     */
    {
        GetFileList cfgs( user->UserDir() );
        GetFileList::file_iterator it( cfgs );

        // 扫描每个附件名，取出前缀是key的文件
        while( it.next() )
        {
            const char *filename = it.name();
            if(string("txt") != GetFileType(filename))
            {
                continue;
            }
            int ret = FilePack(filename, FT_PAGE_CFG);
            if(ret < 0)
            {
                LOG_ERROR("Pack error, file: [%s]", filename);
                return ERR;
            }
        }
    }

    return OK;
}

// 打包一个文件（把filename文件加入包中）（成功返回OK，出错返回ERR）
int DataPack::FilePack(const string &filename, FileType type)
{
    FileObj file;
    Head head;
    char buf[512];
    int ret;
    string filename2 = filename;    // filename2指向filename，或者压缩后的文件；

    // 如是数据文件，则先压缩；
    if(FT_DATA == type)
    {
        User *user = User::Get(m_username);
        filename2 = user->TmpDir()
                    + GetBaseName(filename.c_str())
                    + "."
                    + GetRandomString()
                    + ".z";
        ret = ZipCompress(filename, filename2);
        if(ret < 0)
        {
            LOG_ERROR("ZipCompress() error, src=[%s] dest=[%s]",
                            filename.c_str(), filename2.c_str());
            return ERR;
        }
        LOG_DEBUG("ZipCompress() ok, src=[%s] dest=[%s]",
                        filename.c_str(), filename2.c_str());
    }

    if( ! file.Open(filename2) )
    {
        LOG_ERROR("Can't open file: [%s]", filename2.c_str());
        return ERR;
    }

    // 填充头部信息
    strncpy(head.basename, GetBaseName(filename2.c_str()), sizeof(head.basename) - 1);
    head.filesize = file.Size();
    head.mtime = GetFileModifyTime(filename2);
    head.type = type;

    LOG_DEBUG("head.type=[%d]", head.type);
    LOG_DEBUG("head.basename=[%s]", head.basename);
    LOG_DEBUG("head.filesize=[%u]", head.filesize);
    LOG_DEBUG("head.mtime=[%u]", head.mtime);
    LOG_DEBUG("head.magic=[0x%X]", head.magic);

    // 先写入文件头
    m_pack.Write(&head, sizeof(head));

    // 再写文件体
    while( (ret = file.Read(buf, sizeof(buf))) > 0 )
    {
        int n = m_pack.Write(buf, ret);
        if(n != ret)
        {
            LOG_ERROR("Write error, finish: [%d/%d]", n, ret);
            return ERR;
        }
    }

    /* 清除临时的压缩文件（见上面的对应的if()）
     * 当filename不等于filenam2，说明filename2是压缩的临时文件，删除之；
     */
    if( filename != filename2 )
    {
        LOG_DEBUG("Delete file: [%s]", filename2.c_str());
        DeleteFile(filename2);
    }

    return OK;
}

// 结束打包，并返回包文件名；
const string DataPack::PackFinish()
{
    // 已处理完成
    if(OPR_FINISH == m_opr)
    {
        LOG_ERROR("operator error, m_opr=[%d]", m_opr);
        return m_pack.Fullname();;
    }

    // 再执行收必需的数据打包
    if( OtherPack() < 0 )
    {
        LOG_ERROR("OtherPack() error");
        return "";
    }

    // 操作完成，设置结束标记，并关闭文件（使缓冲区写入文件中）；
    m_opr = OPR_FINISH;
    m_pack.Close();
    return m_pack.Fullname();
}


// 设置解包信息（pack: 待解的包文件名）
int DataPack::UnPack(const string &pack)
{
    // 只能设置一次操作类型
    if(OPR_UNKNOWN != m_opr)
    {
        LOG_ERROR("error: operator had beeg setted, current m_opr=[%d]", m_opr);
        return ERR;
    }

    int ret;

    if( ! m_pack.Open(pack) )
    {
        LOG_ERROR("Can't open file: [%s]", pack.c_str());
        return ERR;
    }

    // 取出版本号
    Version v;

    ret = m_pack.Read(&v, sizeof(v));
    if(sizeof(v) != ret)
    {
        LOG_ERROR("Pack file error: [%s]", pack.c_str());
        return ERR;
    }
    LOG_DEBUG("key=[%s]", v.key);

    // 测试版本（可能需要用对应的解包代码版本）
    if(! v.isValid())
    {
        LOG_ERROR("Version not match, version of pack is [%s] ", v.key);
        return ERR;
    }

    // 标明操作类型
    m_opr = OPR_MAKEPACK;

    /*
     * 开始执行解包动作。。。
     */
    while( FileUnPack() == OK )
    {
    }

    return OK;
}

// 文件解包
int DataPack::FileUnPack()
{
    int ret;
    Head head;
    struct {
        int data;   // 成功处理的数据数
        int attach; // 成功处理的附件数
    } success;

    // 先取出原数据（累加）
    success.data = atoi( m_result.Get("success.data").c_str() );
    success.attach = atoi( m_result.Get("success.attach").c_str() );

    // 读出当前处数据的头部信息
    ret = m_pack.Read(&head, sizeof(head));
    if(ret != sizeof(head))
    {
        LOG_DEBUG("Unpack end, name=[%s]", m_pack.Fullname().c_str());
        return ERR;
    }
    else if( ! head.isValid() )
    {
        LOG_ERROR("Invalid pack, name=[%s], magic=[0x%X]",
                            m_pack.Fullname().c_str(), head.magic);
        return ERR;
    }
    LOG_DEBUG("head.type=[%d]", head.type);
    LOG_DEBUG("head.basename=[%s]", head.basename);
    LOG_DEBUG("head.filesize=[%u]", head.filesize);
    LOG_DEBUG("head.mtime=[%u]", head.mtime);
    LOG_DEBUG("head.magic=[0x%X]", head.magic);

    User *user = User::Get(m_username);
    string fileFromPack = "";

    // 根据不同的文件类型，解压到不同地点
    switch(head.type)
    {
    case FT_ATTACH:     // 附件则直接解到附件目录
        fileFromPack = user->AttachDir() + head.basename;
        break;
    case FT_PAGE_CFG:   // 页面（配置）数据
    case FT_MENU_CFG:   // 菜单数据
    case FT_DATA:       // 数据文件则解到临时目录
        fileFromPack = user->TmpDir() + GetRandomString() + string(".") + head.basename;
        break;
    default:
        LOG_ERROR("Unknown type: [%d], file: [%s]", head.type, head.basename);
        return ERR;
    }

    // 若文件已存在，且最后修改时间不晚于当前包中文件时间，返回；
    int tLocalFileTime = GetFileModifyTime(fileFromPack);
    if(tLocalFileTime > 0 && tLocalFileTime >= head.mtime)
    {
        LOG_DEBUG("Local file is new than file on pack, file=[%s]", fileFromPack.c_str());
        // 跳过当前文件位置
        m_pack.SetSeek(FileObj::CUR, head.filesize);
        return OK;
    }

    /*
     * 解包：
     */
    FileObj file;
    char buf[512];
    int len = head.filesize;
    int need = min(sizeof(buf), head.filesize); // 一次调Read()需要读取的字节数

    if( ! file.Open(fileFromPack, FileObj::W) )
    {
        LOG_ERROR("Can't open file: [%s]", fileFromPack.c_str());
        return ERR;
    }

    while( len > 0 && (ret = m_pack.Read(buf, need)) > 0 )
    {
        int n = file.Write(buf, ret);
        if(n != ret)
        {
            LOG_ERROR("Write error, finish: [%d/%d]", n, ret);
            return ERR;
        }
        len -= ret;
        need = min(sizeof(buf), len);
    }

    // 关闭文件，确保缓冲数据写入；
    file.Close();


    /*
     * 若是用户数据文件，还需释放文件内的数据到相应的用户数据文件中；
     */
    switch( head.type )
    {
    case FT_ATTACH:
        // 还原文件时间
        SettFileModifyTime(fileFromPack, head.mtime);
        // 增加附件计数
        success.attach++;
        break;
    case FT_DATA:{
        // 先解压（因该类型文件是被压缩的，见打包函数中）
        const string file = fileFromPack + "." + GetRandomString();
        ret = ZipDecompress(fileFromPack, file);
        if(ret < 0)
        {
            LOG_ERROR("ZipDecompress() error, src=[%s] dest=[%s]",
                            fileFromPack.c_str(), file.c_str());
            return ERR;
        }
        // 取出数据
        Ini data(file);
        m_UserData.Set(data);   // 释放数据
        // 增加数据操作计数
        success.data += atoi( m_UserData.GetResult("success").c_str() );
        LOG_DEBUG("Delete file: [%s] [%s]", fileFromPack.c_str(), file.c_str());
        DeleteFile(fileFromPack);   // 数据已处理完毕，删除文件；
        DeleteFile(file);
        }break;
    case FT_MENU_CFG:{
        /*
         * 和下段代码重复。［XXX］
         */
        Ini merge;
        // 合并packData到oldData;
        // 先读出上传的数据
        if(merge.Read( fileFromPack ) == OK)
        {
            const string &old = user->MenuDir() + head.basename;
            // 再读出原数据
            merge.Read(old);
            // 写入
            merge.Write( old );
            LOG_DEBUG("old=[%s]", old.c_str());
        }
        LOG_DEBUG("Delete file: [%s]", fileFromPack.c_str());
        DeleteFile(fileFromPack);   // 数据已处理完毕，删除文件；
        }break;
    case FT_PAGE_CFG:{
        /*
         * 和上段代码重复。［XXX］
         */
        Ini merge;
        // 合并packData到oldData;
        // 先读出上传的数据
        if(merge.Read( fileFromPack ) == OK)
        {
            /*
             * 如是browse页，需要合并书签（[data]==>bookmark字段），此书签指在
             * browse页的左侧处的书签；
             */
            const string &bookmark1 = merge.Get("data", "bookmark");
            // 再读出原数据
            const string &old = user->UserDir() + head.basename;
            merge.Read(old);
            // 再取原书签
            const string &bookmark2 = merge.Get("data", "bookmark");
            // 合并书签（若不是browse页，或上传的包中没有bookmark信息，则不需合并）
            if("" != bookmark1)
            {
                /* 把上传的书签数据合并到本地的书签数据中（根据最新时间值）
                 * 合并出错，则保留原书签数据。
                 *   bookmark1: 上传书签
                 *   bookmark2: 原书签
                 */
                merge.Set("data", "bookmark", MergeBookmark(bookmark1, bookmark2));
            }
            // 写入
            merge.Write( old );
            LOG_DEBUG("old=[%s]", old.c_str());
        }
        LOG_DEBUG("Delete file: [%s]", fileFromPack.c_str());
        DeleteFile(fileFromPack);   // 数据已处理完毕，删除文件；
        }break;
    default:
        LOG_ERROR("Type error: [%d]", head.type);
        break;
    }

    // 做记录
    m_result.Set("success.data", IntToString(success.data));
    m_result.Set("success.attach", IntToString(success.attach));

    LOG_DEBUG("Unpack file: [%s]", fileFromPack.c_str());

    return OK;
}

// 取操作结果
const string DataPack::GetResult(const string &field) const
{
    return m_result.Get(field);
}












}// end of DATAPACK_SPACE
