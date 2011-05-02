// Rocky 2010-04-30 14:11:41
#include "UserData.h"
#include "User.h"
namespace USERDATA_SPACE
{








// ��ȡpath�������ļ��ڵ�����
UserData::iterator::iterator(const string &path)
                        : m_FilsList(path),
                          m_itFile(m_FilsList),
                          m_first(true)
{
    m_FilsList.SortFilename(); // �����ļ���
}


// ָ����һ������
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
            // ȡһ����¼
            if( m_itCurrent.next() )
            {
                return true; // ȡ����¼�����أ�
            }
        }

        // �ϸ��ļ��Ѷ�ȡ��ɣ�����һ���ļ���
        if( m_itFile.next() )
        {
            m_Curent.Clear(); // ���������

            int ret;
            const char *filename = m_itFile.name();
            ret = m_Curent.Read( filename );
            if(ret < 0)
            {
                continue;   // ���ų�����һ�ļ�
            }
            m_itCurrent = &m_Curent;
        }
        else
        {
            break; // �����ļ���������ϣ��˳���
        }
    }

    return false; // ������
}

// ȡ��ǰ[key/pack]��
int UserData::iterator::Get(string &key, Conf *&pack)
{
    key = m_itCurrent.GetKey();
    pack = m_itCurrent.GetValue();
    return OK; // ȡ����¼�����أ�
}




UserData::UserData(const string &username) : m_username(username)
{
}

UserData::~UserData()
{
}




/*
 * ��ȡ��������λ�����ݡ�ע�⣬ÿ����һ�Σ����Ǵ�
 * ��һ���ļ���[XXX]
 */
const string UserData::Get(const string &key, const string &item)
{
    User *user = User::Get( m_username );

    if( ! user->isValid() )
    {
        LOG_ERROR("Invalid user: [%s]", m_username.c_str());
        return "";
    }

    const string &filename = KeyToFile( key ); // ȡkey��Ӧ���ļ���
    const string &path = user->DataDir() + filename;

    Ini data( path );

    return data.Get(key, item);
}

/* �����ļ��У�ȡ���ݣ����pack��key��Ӧ��ֵ���磺
 *
 *  Ini pack;
 *  pack.Set("20100515020544", "title", "");
 *  pack.Set("20100515020544", "text", "");
 *
 *  Get( pack ); // ��ʱ�����title��text��
 */
int UserData::Get(Ini &pack)
{
    User *user = User::Get( m_username );
    if( ! user->isValid() )
    {
        LOG_ERROR("Invalid user: [%s]", m_username.c_str());
        return ERR;
    }

    map<string, Ini*> key2ini; // ÿ���ļ��� => ini����
    Ini::iterator itIni( &pack );

    /*
     * ȡ��pack��ÿ������key��Ȼ��Ӷ�Ӧ�ļ��ж�ȡ���ݣ�����
     * �䵽��Ӧ��value�У�
     */
    while( itIni.next() )
    {
        // ȡ��[key/Conf]ֵ��
        const string &key = itIni.GetKey();
        const Conf &conf = *(itIni.GetValue());

        /*
         * 1. ȡkey��Ӧ���ļ���
         * 2. ���ļ��Ƿ��ѱ��򿪣���δ���������������key2ini�У�
         * 3. ���ļ��ж�ȡ���ݣ���䵽pack�У�
         */
        const string &filename = KeyToFile( key );
        Ini *ini = key2ini[ filename ];
        // ��Ӧ�ļ�δ����ʱ�������(new Ini())��
        if(NULL == ini)
        {
            // ȡ·��
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

        /* ���ⲿ�����key��Ӧ��conf�ֶΣ���Ϊ�գ������ʾ���ļ��е���
         * ��ֵ��䵽pack�У��ǿգ���ֻ���ָ����conf�ֶΣ�
         */
        if(0 == conf.Size())
        {
            /*
             * ���key��Ӧ�������ֶ�
             */
            pack.Set(key, *oldConf);
        }
        else
        {
            /*
             * ֻ���conf��Ӧ��ָ���ֶ�
             */

            // ȡ��key��Ӧ��Confֵ
            Conf::iterator itConf( &conf );
            while( itConf.next() )
            {
                /* ע�������name��value:
                 *
                 *  name -- ���ⲿ��pack�д���
                 *  value -- ���Ǵ��ļ��ж�ȡ�ģ������
                 *           ��pack��name��Ӧ��ֵ��
                 */
                const string &name = itConf.GetName();
                const string &value = oldConf->Get(name);

                // ���õ�pack�У�ԭֵ�����ǣ�
                pack.Set(key, name, value);
            }
        }
    }

    /*
     * ������ͷ��ڴ棻
     */
    map<string, Ini*>::iterator itSave;
    for(itSave = key2ini.begin(); key2ini.end() != itSave; itSave++)
    {
        delete itSave->second;
    }

    return OK;
}


// �������ݣ�������
int UserData::Set(const string &key, const string &item, const string &value)
{
    User *user = User::Get( m_username );

    if( ! user->isValid() )
    {
        LOG_ERROR("Invalid user: [%s]", m_username.c_str());
        return ERR;
    }

    const string &filename = KeyToFile( key ); // ȡkey��Ӧ���ļ���
    const string &path = user->DataDir() + filename;

    // ����ԭ����
    Ini data( path );

    // д��
    data.Set(key, item, value);

    // ����
    return data.Write();
}

/*
 * ��pack�е�ֵ���棨���ļ���
 * force: 1. Ϊtrueʱ��ǿ��д���������ݣ�
 *        2. Ϊfalseʱ��ֻд���µ����ݣ�modify�ֶξ�����
 */
int UserData::Set(const Ini &pack, bool force/*=false*/)
{
    User *user = User::Get( m_username );
    if( ! user->isValid() )
    {
        LOG_ERROR("Invalid user: [%s]", m_username.c_str());
        return ERR;
    }

    map<string, Ini*> key2ini; // ÿ���ļ��� => ini����
    Ini::iterator itIni( &pack );
    int success = 0; // �������������ɹ�������
    int ignore = 0;  // ���Լ�����

    /*
     * ��pack�е����ݱ��浽��Ӧ���ļ�
     */
    while( itIni.next() )
    {
        // ȡ��[key/Conf]ֵ��
        const string &key = itIni.GetKey();
        const Conf &conf = *(itIni.GetValue());

        /*
         * 1. ȡkey��Ӧ���ļ���
         * 2. ���ļ��Ƿ��ѱ��򿪣���δ���������������key2ini�У�
         * 3. ���������
         */
        const string &filename = KeyToFile( key );
        Ini *ini = key2ini[ filename ];
        // ��Ӧ�ļ�δ����ʱ�������(new Ini())��
        if(NULL == ini)
        {
            // ȡ·��
            const string &path = user->DataDir() + filename;

            ini = new Ini(path);
            if(NULL == ini)
            {
                LOG_ERROR("new Ini() error");
                return ERR;
            }
            key2ini[ filename ] = ini;
        }

        /* �Ƿ�ֻд���������ݣ�
         *   1. ������ǿ�Ʊ�־��force==false������
         * [2010-05-22]
         */
        if( !force )
        {
            // ��д������ݲ���ԭ�����ݾ���ʱ
            if( conf.Get("modify") <= ini->Get(key, "modify") )
            {
                ignore++;
                LOG_DEBUG("Igore old: [%s]", key.c_str());
                continue;
            }
            // �����ѱ�ɾ�������ݣ��Ƿ�Ҫ���ԣ�ini->Get(key, "status") == "delete" [XXX]��
            if( conf.Get("status") == "delete" )
            {
                ignore++;
                LOG_DEBUG("Igore delete: [%s]", key.c_str());
                continue;
            }
        }

        // ��������ݣ�ԭֵ�����ǣ�;
        ini->Set(key, conf);
        success++;
    }

    /*
     * ����
     */
    map<string, Ini*>::iterator itSave;
    for(itSave = key2ini.begin(); key2ini.end() != itSave; itSave++)
    {
        Ini *ini = itSave->second;
        ini->Write();   // д��
        delete ini;     // ������ͷ��ڴ棻
    }

    // ��¼�ɹ���������
    m_result.Set("success", IntToString(success));
    m_result.Set("ignore", IntToString(ignore));

    return OK;
}

// ȡkey��Ӧ���ݵĸ������б�attachs��     [Rocky 2010-05-18 18:08:42]
int UserData::GetAttachList(const string &key, vector<string> &attachs) const
{
    User *user = User::Get(m_username);
    // ȡ����Ŀ¼
    const string &path = user->AttachDir();
    GetFileList list(path);
    GetFileList::file_iterator itAttach( list );

    // ɨ��ÿ����������ȡ��ǰ׺��key���ļ�
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

// ȡ�������
const string UserData::GetResult(const string &field) const
{
    return m_result.Get(field);
}













}// end of USERDATA_SPACE
