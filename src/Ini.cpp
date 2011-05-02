// Rocky 2010-04-29 18:24:51
#include "Ini.h"
namespace INI_SPACE
{







Ini::Ini(const string &filename/*=""*/) : m_filename(filename)
{
    if("" != filename)
    {
        Read(filename);
    }
}
Ini::~Ini()
{
    Clear();
}


// �Ƿ�Ϊ��[...]�������򷵻��������ڵ����ݣ����򷵻�NULL
char *Ini::Section(char *buf, int len)
{
    char *pBuf = buf;
    char *begin;
    char *end;
    char *section = NULL;

    // ����ע�͡�����
    if( '#' == *pBuf || IS_CR( *pBuf ) )
    {
        return NULL;
    }

    end = pBuf;
    JUMP_TO_LINE_END(end);  // ������β

    // ����ʽ
    if('[' != *pBuf || ']' != *(end - 1))
    {
        return NULL;
    }

    *(end - 1) = '\0'; // �����]��
    section = pBuf + 1; // ������[��

    return section;
}

// ��ȡ��ʽ�ļ�
int Ini::Read(const string &filename)
{
    FileObj ini;

    if( !ini.Open( filename ) )
    {
        LOG_INFO("Can't open file: [%s]", filename.c_str());
        return ERR;
    }


    int ret;
    char buf[256];
    char *pBuf;
    char *begin;
    char *end;
    string section;
    Conf *conf = NULL;



    /*
     * ��ȡsection�����¶�Ӧ������
     */
    while( (ret = ini.GetLine(buf, sizeof(buf) - 1)) > 0 )
    {
        pBuf = buf;

        // ɾ���س����з�
        CUT_CR(pBuf, ret);

        // ����ע�͡�����
        if( '#' == *pBuf || IS_CR( *pBuf ) )
        {
            continue;
        }


        /*
         * ��λ����һ����[...]����
         */
        begin = Section(buf, ret);
        if(NULL != begin)
        {
            // �����µ�Section��
            section = begin;
            // �Ȳ鿴����ԭ����
            conf = m_ConfList[ section ];
            if(NULL == conf)
            {
                // �����½��
                conf = new Conf;
                if(NULL == conf)
                {
                    LOG_ERROR("new Conf error");
                    return ERR;
                }
                m_ConfList[ section ] = conf;
            }
            continue;
        }

        if("" == section || NULL == conf)
        {
            continue;
        }


        /*
         * ȡ������
         */
        end = strchr(pBuf, '=');
        if(NULL == end)
        {
            //LOG_DEBUG("ini format error: [%s]", buf);
            continue;
        }
        *end = '\0'; // ��ȥ��=��
        string attribute = pBuf;

        /*
         * ȡ����ֵ
         */
        string value = end + 1;
        //LOG_DEBUG("%s=%s", attribute, value);

        // ��ȡһ�л�����ȫȡ�����ݣ�������ȡ��
        while( (sizeof(buf) - 1) == ret
               && ( (ret = ini.GetLine(buf, sizeof(buf) - 1)) > 0 ) )
        {
            // ɾ���س����з������鿴�Ƿ�ȡ����һ������
            CUT_CR(buf, ret);
            value += buf;
        }

        // ����
        conf->Set(attribute, value);
    }

    //LOG_DEBUG("m_ConfList.size=[%d]", m_ConfList.size());

    return OK;
}

// ����д���ļ�
int Ini::Write(string filename/*=""*/)
{
    if("" == filename)
    {
        filename = m_filename;
    }

    FileObj ini;

    if( !ini.Open(filename, FileObj::W) )
    {
        LOG_ERROR("Can't open file: [%s]", filename.c_str());
        return ERR;
    }


    Ini::iterator itIni(this);

    while( itIni.next() )
    {
        Conf *conf = itIni.GetValue();
        const string key = string("\n[") + itIni.GetKey() + string("]") + "\n";
        ini.Write(key.c_str(), key.length());

        Conf::iterator itConf(conf);
        while( itConf.next() )
        {
            const string pair = itConf.GetName() + string("=") + itConf.GetValue() + string("\n");
            ini.Write(pair.c_str(), pair.length());
        }
    }
    return OK;
}

// �������
void Ini::Clear()
{
    Ini::iterator it(this);

    while( it.next() )
    {
        //LOG_DEBUG("delete: [%s]", it.GetKey());
        delete it.GetValue();
    }

    m_ConfList.clear();
}



// ��ʾ��������
void Ini::Dump() const
{
    Ini::iterator itIni(this);

    while( itIni.next() )
    {
        Conf *conf = itIni.GetValue();
        const char *key = itIni.GetKey();
        LOG_DEBUG("[%s]", key);

        Conf::iterator itConf(conf);
        while( itConf.next() )
        {
            LOG_DEBUG("%s=%s", itConf.GetName(), itConf.GetValue());
        }
    }
}

const Conf *Ini::Get(const string &key) const
{
    map<string, Conf *>::const_iterator it = m_ConfList.find( key );
    if(m_ConfList.end() != it)
    {
        return it->second;
    }
    return NULL;
}

const string Ini::Get(const string &section, const string &attribute) const
{
    const Conf *conf = Get(section);
    if(NULL == conf)
    {
        return "";
    }
    return conf->Get(attribute);
}

int Ini::Set(const string &section, const string &attribute, const string &value)
{
    Conf *conf = m_ConfList[ section ];
    if( NULL == conf )
    {
        // �½�һ���
        conf = new Conf;
        if(NULL == conf)
        {
            LOG_ERROR("new Conf error");
            return ERR;
        }
        // �����б�
        m_ConfList[ section ] = conf;
    }
    // ����ֵ
    conf->Set(attribute, value);
    return OK;
}

// pack���ܱ�copyһ�ݺ�ŵ�Set�У�������ָ�룻
int Ini::Set(const string &key, const Conf &pack)
{
    // �Ȳ鿴��ǰ�б����Ƿ��Ѵ���key��Ӧ��ֵ���޴���������½�㣻
    Conf *conf = m_ConfList[ key ];
    if( NULL == conf )
    {
        // �����б�
        conf = new Conf(pack);
        if(NULL == conf)
        {
            return ERR;
        }
        m_ConfList[ key ] = conf;
    }

    // �Ѵ��ڣ��������ֵ����ֵ�������ǣ�
    conf->Set(pack);
    return OK;
}


void Ini::Del(const string &key)
{
    delete Get(key);
    m_ConfList.erase( key );
}

void Ini::Del(const string &section, const string &attribute)
{
    /* �Ȳ鿴�Ƿ���section��Ӧ�����ݣ������ٵ���Del()��ִ�У�
     *   ע�⣬����ֱ����m_ConfList[ section ]->Del(...)����Ϊ
     *   ���䲻����ʱ���ۣݲ���������Ԫ�أ�
     */
    const Conf *conf = Get(section);
    if( NULL != conf )
    {
        m_ConfList[ section ]->Del( attribute );
    }
}









}// end of INI_SPACE
