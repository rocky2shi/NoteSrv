// Rocky 2010-05-18 18:09:19
#include "User.h"
#include "DataPack.h"
namespace DATAPACK_SPACE
{





// �����ⲿperl���룬�ϲ���ǩ��ʹ���м��ļ���   [Rocky 2010-05-26 15:47:33]
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

    // ���ⲿ�������ļ�
    const string cmd = GlobalConfig::instance()->ToolDir() + "MergeBookmark.pl "
                       "'" + tmpfile1 + "' "
                       "'" + tmpfile2 + "' "
                       "'" + tmpfile3 + "' ";
    int ret = system(cmd.c_str());
    LOG_DEBUG("ret=[%d] cmd=[%s]", ret, cmd.c_str());
    if(0 != ret)
    {
        LOG_ERROR("Command error: [%s]", cmd.c_str());
        // ��������ԭֵ��bookmark2ӦΪ������ǩ���ݣ����ϴ������ݣ���
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
    // ���Ա�־�ÿ�
    memset(m_attribs, 0, sizeof(m_attribs));
}

DataPack::~DataPack()
{
    Clear();
}

// ����
void DataPack::Clear()
{
    LOG_DEBUG("Delete file: [%s]", m_ini.Fullname().c_str());
    DeleteFile(m_ini.Fullname());
}


// ���ô����Ϣ���ɹ�����OK��
int DataPack::MakePack()
{
    // ֻ������һ�β�������
    if(OPR_UNKNOWN != m_opr)
    {
        LOG_ERROR("error: operator had beeg setted, current m_opr=[%d]", m_opr);
        return ERR;
    }

    User *user = User::Get(m_username);

    // ����һ��ʱ�ļ���
    const string &file = user->TmpDir() + GetRandomString();
    m_pack.Open(file + ".pack", FileObj::W);
    m_ini.Open(file + ".data", FileObj::W);

    // д��ͷ���������İ汾��Ϣ
    Version v;
    m_pack.Write(&v, sizeof(v));

    // ������������
    m_opr = OPR_MAKEPACK;

    return OK;
}

// ���ô������
void DataPack::SetAttrib(Attrib type, bool valid/*=true*/)
{
    m_attribs[type] = valid;
}

// ������Ҫ������û����ݣ���������
int DataPack::Add(const Ini &pack)
{
    // �Ǵ�������������
    if(OPR_MAKEPACK != m_opr)
    {
        LOG_ERROR("operator error, m_opr=[%d]", m_opr);
        return ERR;
    }

    Ini::iterator itIni( &pack );

    // д�û�ʱ���ݵ���ʱ�ļ���ͬ�Ѹ��������
    while( itIni.next() )
    {
        /*
         * д������
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


        // �Ƿ����������������ô˱�־˵��������������
        if( m_attribs[A_EXCLUDE_ATTACH] )
        {
            continue;
        }

        /*
         * ȡkey���ݵĸ����б��������
         */
        vector<string> attachs;
        vector<string>::iterator it;

        // ȡ�����б�
        m_UserData.GetAttachList(itIni.GetKey(), attachs);
        LOG_DEBUG("[%d]", attachs.size());

        for(it = attachs.begin(); attachs.end() != it; it++)
        {
            const string &fullname = *it;
            LOG_DEBUG("Pack file: [%s]", fullname.c_str());
            // ���
            int ret = FilePack(fullname, FT_ATTACH);
            if(ret < 0)
            {
                LOG_ERROR("Pack error, file: [%s]", fullname.c_str());
                return ERR;
            }
        }
    }

    /*
     * ע����m_ini�Ĵ����OtherPack()������
     */

    return OK;
}

// ��������û�����
int DataPack::AddAll()
{
    // �Ǵ�������������
    if(OPR_MAKEPACK != m_opr)
    {
        LOG_ERROR("operator error, m_opr=[%d]", m_opr);
        return ERR;
    }

    User *user = User::Get(m_username);

    /*
     * ��������Ŀ¼��ȡ�����и����������
     */
    // �Ƿ������������������ô˱�־˵������������
    if( ! m_attribs[A_EXCLUDE_ATTACH] )
    {
        GetFileList attachs( user->AttachDir() );
        GetFileList::file_iterator itAttach( attachs );

        // ɨ��ÿ����������ȡ��ǰ׺��key���ļ�
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
     * �ٴ�����������ļ�
     */
    {
        GetFileList datas( user->DataDir() );
        GetFileList::file_iterator itData( datas );

        // ɨ��ÿ����������ȡ��ǰ׺��key���ļ�
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

// ���������˵�����ҳ�����õȣ�
int DataPack::OtherPack()
{
    User *user = User::Get(m_username);

    /*
     * ���ǵ���Add()������ģ���m_ini��Ϊ�գ����ǵ�AddAll()��
     * ��m_iniΪ�գ�
     */
    if( m_ini.Size() > 0 )
    {
        /*
         * ��������ļ�
         */
        m_ini.Close(); // ʹ����������д�룬������ܴ����������
        int ret = FilePack(m_ini.Fullname(), FT_DATA);
        if(ret < 0)
        {
            LOG_ERROR("Pack error, file: [%s]", m_ini.Fullname().c_str());
            return ERR;
        }
    }


    /*
     * ����������˵����ݣ�λ���û�Ŀ¼��menu��Ŀ¼�е��ļ���
     */
    {
        GetFileList menus( user->MenuDir() );
        GetFileList::file_iterator it( menus );

        // ɨ��ÿ����������ȡ��ǰ׺��key���ļ�
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
     * �����ҳ�����ݣ�λ���û�Ŀ¼����.txt��Ϊ��׺���ļ���
     */
    {
        GetFileList cfgs( user->UserDir() );
        GetFileList::file_iterator it( cfgs );

        // ɨ��ÿ����������ȡ��ǰ׺��key���ļ�
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

// ���һ���ļ�����filename�ļ�������У����ɹ�����OK��������ERR��
int DataPack::FilePack(const string &filename, FileType type)
{
    FileObj file;
    Head head;
    char buf[512];
    int ret;
    string filename2 = filename;    // filename2ָ��filename������ѹ������ļ���

    // ���������ļ�������ѹ����
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

    // ���ͷ����Ϣ
    strncpy(head.basename, GetBaseName(filename2.c_str()), sizeof(head.basename) - 1);
    head.filesize = file.Size();
    head.mtime = GetFileModifyTime(filename2);
    head.type = type;

    LOG_DEBUG("head.type=[%d]", head.type);
    LOG_DEBUG("head.basename=[%s]", head.basename);
    LOG_DEBUG("head.filesize=[%u]", head.filesize);
    LOG_DEBUG("head.mtime=[%u]", head.mtime);
    LOG_DEBUG("head.magic=[0x%X]", head.magic);

    // ��д���ļ�ͷ
    m_pack.Write(&head, sizeof(head));

    // ��д�ļ���
    while( (ret = file.Read(buf, sizeof(buf))) > 0 )
    {
        int n = m_pack.Write(buf, ret);
        if(n != ret)
        {
            LOG_ERROR("Write error, finish: [%d/%d]", n, ret);
            return ERR;
        }
    }

    /* �����ʱ��ѹ���ļ���������Ķ�Ӧ��if()��
     * ��filename������filenam2��˵��filename2��ѹ������ʱ�ļ���ɾ��֮��
     */
    if( filename != filename2 )
    {
        LOG_DEBUG("Delete file: [%s]", filename2.c_str());
        DeleteFile(filename2);
    }

    return OK;
}

// ��������������ذ��ļ�����
const string DataPack::PackFinish()
{
    // �Ѵ������
    if(OPR_FINISH == m_opr)
    {
        LOG_ERROR("operator error, m_opr=[%d]", m_opr);
        return m_pack.Fullname();;
    }

    // ��ִ���ձ�������ݴ��
    if( OtherPack() < 0 )
    {
        LOG_ERROR("OtherPack() error");
        return "";
    }

    // ������ɣ����ý�����ǣ����ر��ļ���ʹ������д���ļ��У���
    m_opr = OPR_FINISH;
    m_pack.Close();
    return m_pack.Fullname();
}


// ���ý����Ϣ��pack: ����İ��ļ�����
int DataPack::UnPack(const string &pack)
{
    // ֻ������һ�β�������
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

    // ȡ���汾��
    Version v;

    ret = m_pack.Read(&v, sizeof(v));
    if(sizeof(v) != ret)
    {
        LOG_ERROR("Pack file error: [%s]", pack.c_str());
        return ERR;
    }
    LOG_DEBUG("key=[%s]", v.key);

    // ���԰汾����ͬ�ĸ�������Ҫ�ö�Ӧ�Ľ������汾��
    if(! v.isValid())
    {
        LOG_ERROR("Version not matching, version on pack is [%s] ", v.key);
        return ERR;
    }

    // ������������
    m_opr = OPR_MAKEPACK;

    /*
     * ��ʼִ�н������������
     */
    while( FileUnPack() == OK )
    {
    }

    return OK;
}

// �ļ����
int DataPack::FileUnPack()
{
    int ret;
    Head head;
    struct {
        int data;   // �ɹ������������
        int attach; // �ɹ�����ĸ�����
    } success;

    // ��ȡ��ԭ���ݣ��ۼӣ�
    success.data = atoi( m_result.Get("success.data").c_str() );
    success.attach = atoi( m_result.Get("success.attach").c_str() );

    // ������ǰ�����ݵ�ͷ����Ϣ
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

    // ���ݲ�ͬ���ļ����ͣ���ѹ����ͬ�ص�
    switch(head.type)
    {
    case FT_ATTACH:     // ������ֱ�ӽ⵽����Ŀ¼
        fileFromPack = user->AttachDir() + head.basename;
        break;
    case FT_PAGE_CFG:   // ҳ�棨���ã�����
    case FT_MENU_CFG:   // �˵�����
    case FT_DATA:       // �����ļ���⵽��ʱĿ¼
        fileFromPack = user->TmpDir() + GetRandomString() + string(".") + head.basename;
        break;
    default:
        LOG_ERROR("Unknown type: [%d], file: [%s]", head.type, head.basename);
        return ERR;
    }

    // ���ļ��Ѵ��ڣ�������޸�ʱ�䲻���ڵ�ǰ�����ļ�ʱ�䣬���أ�
    int tLocalFileTime = GetFileModifyTime(fileFromPack);
    if(tLocalFileTime > 0 && tLocalFileTime >= head.mtime)
    {
        LOG_DEBUG("Local file is new than file on pack, file=[%s]", fileFromPack.c_str());
        // ������ǰ�ļ�λ��
        m_pack.SetSeek(FileObj::CUR, head.filesize);
        return OK;
    }

    /*
     * �����
     */
    FileObj file;
    char buf[512];
    int len = head.filesize;
    int need = min(sizeof(buf), head.filesize); // һ�ε�Read()��Ҫ��ȡ���ֽ���

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

    // �ر��ļ���ȷ����������д�룻
    file.Close();


    /*
     * �����û������ļ��������ͷ��ļ��ڵ����ݵ���Ӧ���û������ļ��У�
     */
    switch( head.type )
    {
    case FT_ATTACH:
        // ��ԭ�ļ�ʱ��
        SettFileModifyTime(fileFromPack, head.mtime);
        // ���Ӹ�������
        success.attach++;
        break;
    case FT_DATA:{
        // �Ƚ�ѹ����������ļ��Ǳ�ѹ���ģ�����������У�
        const string file = fileFromPack + "." + GetRandomString();
        ret = ZipDecompress(fileFromPack, file);
        if(ret < 0)
        {
            LOG_ERROR("ZipDecompress() error, src=[%s] dest=[%s]",
                            fileFromPack.c_str(), file.c_str());
            return ERR;
        }
        // ȡ������
        Ini data(file);
        m_UserData.Set(data);   // �ͷ�����
        // �������ݲ�������
        success.data += atoi( m_UserData.GetResult("success").c_str() );
        LOG_DEBUG("Delete file: [%s] [%s]", fileFromPack.c_str(), file.c_str());
        DeleteFile(fileFromPack);   // �����Ѵ�����ϣ�ɾ���ļ���
        DeleteFile(file);
        }break;
    case FT_MENU_CFG:{
        /*
         * ���¶δ����ظ�����XXX��
         */
        Ini merge;
        // �ϲ�packData��oldData;
        // �ȶ����ϴ�������
        if(merge.Read( fileFromPack ) == OK)
        {
            const string &old = user->MenuDir() + head.basename;
            // �ٶ���ԭ����
            merge.Read(old);
            // д��
            merge.Write( old );
            LOG_DEBUG("old=[%s]", old.c_str());
        }
        LOG_DEBUG("Delete file: [%s]", fileFromPack.c_str());
        DeleteFile(fileFromPack);   // �����Ѵ�����ϣ�ɾ���ļ���
        }break;
    case FT_PAGE_CFG:{
        /*
         * ���϶δ����ظ�����XXX��
         */
        Ini merge;
        // �ϲ�packData��oldData;
        // �ȶ����ϴ�������
        if(merge.Read( fileFromPack ) == OK)
        {
            /*
             * ����browseҳ����Ҫ�ϲ���ǩ��[data]==>bookmark�ֶΣ�������ǩָ��
             * browseҳ����ദ����ǩ��
             */
            const string &bookmark1 = merge.Get("data", "bookmark");
            // �ٶ���ԭ����
            const string &old = user->UserDir() + head.basename;
            merge.Read(old);
            // ��ȡԭ��ǩ
            const string &bookmark2 = merge.Get("data", "bookmark");
            // �ϲ���ǩ��������browseҳ�����ϴ��İ���û��bookmark��Ϣ������ϲ���
            if("" != bookmark1)
            {
                /* ���ϴ�����ǩ���ݺϲ������ص���ǩ�����У���������ʱ��ֵ��
                 * �ϲ���������ԭ��ǩ���ݡ�
                 *   bookmark1: �ϴ���ǩ
                 *   bookmark2: ԭ��ǩ
                 */
                merge.Set("data", "bookmark", MergeBookmark(bookmark1, bookmark2));
            }
            // д��
            merge.Write( old );
            LOG_DEBUG("old=[%s]", old.c_str());
        }
        LOG_DEBUG("Delete file: [%s]", fileFromPack.c_str());
        DeleteFile(fileFromPack);   // �����Ѵ�����ϣ�ɾ���ļ���
        }break;
    default:
        LOG_ERROR("Type error: [%d]", head.type);
        break;
    }

    // ����¼
    m_result.Set("success.data", IntToString(success.data));
    m_result.Set("success.attach", IntToString(success.attach));

    LOG_DEBUG("Unpack file: [%s]", fileFromPack.c_str());

    return OK;
}

// ȡ�������
const string DataPack::GetResult(const string &field) const
{
    return m_result.Get(field);
}












}// end of DATAPACK_SPACE
