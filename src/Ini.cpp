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


// 是否为‘[...]’，是则返回中括号内的内容，否则返回NULL
char *Ini::Section(char *buf, int len)
{
    char *pBuf = buf;
    char *begin;
    char *end;
    char *section = NULL;

    // 跳过注释、空行
    if( '#' == *pBuf || IS_CR( *pBuf ) )
    {
        return NULL;
    }

    end = pBuf;
    JUMP_TO_LINE_END(end);  // 跳到行尾

    // 检查格式
    if('[' != *pBuf || ']' != *(end - 1))
    {
        return NULL;
    }

    *(end - 1) = '\0'; // 清掉‘]’
    section = pBuf + 1; // 跳过‘[’

    return section;
}

// 读取格式文件
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
     * 读取section及其下对应的内容
     */
    while( (ret = ini.GetLine(buf, sizeof(buf) - 1)) > 0 )
    {
        pBuf = buf;

        // 删除回车换行符
        CUT_CR(pBuf, ret);

        // 跳过注释、空行
        if( '#' == *pBuf || IS_CR( *pBuf ) )
        {
            continue;
        }


        /*
         * 定位到第一个‘[...]’处
         */
        begin = Section(buf, ret);
        if(NULL != begin)
        {
            // 遇到新的Section了
            section = begin;
            // 先查看集合原数据
            conf = m_ConfList[ section ];
            if(NULL == conf)
            {
                // 插入新结点
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
         * 取属性名
         */
        end = strchr(pBuf, '=');
        if(NULL == end)
        {
            //LOG_DEBUG("ini format error: [%s]", buf);
            continue;
        }
        *end = '\0'; // 削去‘=’
        string attribute = pBuf;

        /*
         * 取属性值
         */
        string value = end + 1;
        //LOG_DEBUG("%s=%s", attribute, value);

        // 如取一行还不完全取完数据，接着再取；
        while( (sizeof(buf) - 1) == ret
               && ( (ret = ini.GetLine(buf, sizeof(buf) - 1)) > 0 ) )
        {
            // 删除回车换行符，并查看是否取到了一完整行
            CUT_CR(buf, ret);
            value += buf;
        }

        // 插入
        conf->Set(attribute, value);
    }

    //LOG_DEBUG("m_ConfList.size=[%d]", m_ConfList.size());

    return OK;
}

// 数据写入文件
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

// 清除数据
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



// 显示所有数据
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
        // 新建一结点
        conf = new Conf;
        if(NULL == conf)
        {
            LOG_ERROR("new Conf error");
            return ERR;
        }
        // 插入列表
        m_ConfList[ section ] = conf;
    }
    // 设置值
    conf->Set(attribute, value);
    return OK;
}

// pack可能被copy一份后放到Set中，它不是指针；
int Ini::Set(const string &key, const Conf &pack)
{
    // 先查看当前列表中是否已存在key对应的值，无存在则插入新结点；
    Conf *conf = m_ConfList[ key ];
    if( NULL == conf )
    {
        // 插入列表
        conf = new Conf(pack);
        if(NULL == conf)
        {
            return ERR;
        }
        m_ConfList[ key ] = conf;
    }

    // 已存在，则放入新值（旧值将被覆盖）
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
    /* 先查看是否存的section对应的数据，有则再调用Del()来执行；
     *   注意，不能直接用m_ConfList[ section ]->Del(...)，因为
     *   当其不存在时，［］操作会新增元素；
     */
    const Conf *conf = Get(section);
    if( NULL != conf )
    {
        m_ConfList[ section ]->Del( attribute );
    }
}









}// end of INI_SPACE
