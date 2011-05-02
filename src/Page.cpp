// Rocky 2010-04-15 18:51:46
#include "Page.h"
#include "UserData.h"
#include "GetFileList.h"
#include "DllLoad.h"
namespace PAGE_SPACE
{







Page::Page() : m_request(NULL),
               m_TagList(NULL),
               m_VisitCounter(NULL),
               m_bParent(false),
               m_pack(NULL),
               m_PageCfg(NULL)
{
    FUNCTION_TRACK(); // �����켣����
}

Page::~Page()
{
    FUNCTION_TRACK(); // �����켣����

    if( m_bParent )
    {
        delete m_request;
        m_request = NULL;

        if(NULL != m_TagList)
        {
            vector< Tag * >::iterator it;
            for(it = m_TagList->begin(); m_TagList->end() != it; it++)
            {
                //LOG_DEBUG("delete TAG: [%p]", *it);
                delete *it;
            }
        }
        delete m_TagList;
        m_TagList = NULL;

        delete m_VisitCounter;
        m_VisitCounter = NULL;

        delete m_pack;
        m_pack = NULL;

        delete m_PageCfg;
        m_PageCfg = NULL;
    }
}


/*
 * ���ܣ�ע��������󵽹���
 * ��д��Rocky 2009-09-07 17:09:46
 * ���أ���ȷ����>=0ֵ������<0ֵ��
 * ע�⣺�˺����ڽ���main()����֮ǰ���ɹ��캯��������
 */
int Page::Register(const string &page, Page *sub)
{
    FUNCTION_TRACK(); // �����켣����
    assert(NULL == m_Factory[ page ]);   // ��ʱ��λ��ӦΪ��
    m_Factory[ page ] = sub;  // �������
    sub->m_id = page;
    sub->m_bParent = true;    // ������ע��Ķ�����Ϊ������
    LOG_DEBUG("Register CGI deal [%s:%p] OK.", page.c_str(), sub);
    return OK;
}


/*
 * ���ܣ�ע��
 * ��д��Rocky 2009-09-07 17:09:46
 * ���أ���ȷ����>=0ֵ������<0ֵ��
 * ע�⣺�˺����ڽ���main()����֮ǰ���ɹ��캯��������
 */
int Page::UnRegister(const string &page)
{
    FUNCTION_TRACK(); // �����켣����
    m_Factory.erase(page);
    LOG_INFO("ע��ҳ��[%s]�������", page.c_str());
    return OK;
}

/*
 * ���ܣ���̬��ʼ������
 * ��д��Rocky 2009-09-23 14:14:55
 * ���أ���ȷ����>=0ֵ������<0ֵ��
 */
int Page::init()
{
    FUNCTION_TRACK(); // �����켣����


    int ret;
    static Page page;
    page.m_bParent = true;

    Page *obj = Page::instance( &page );
    if(NULL == obj)
    {
        LOG_ERROR("Page::instance() error");
        return ERR;
    }


    // ��̬���ļ�Ŀ¼
    const string &path = GlobalConfig::instance()->CgiDir();

    // ȡ��Ŀ¼�����ж�̬��
    GetFileList dll( path );

    GetFileList::file_iterator file(dll);

    // ע�ᴦ��ҳ�浽����
    while( file.next() )
    {
        DllLoad::Ptr dll;

        // �򿪶�̬��
        if( dll->Open(file.name()) < 0 )
        {
            LOG_ERROR("���ض�̬�����[%s]", file.name());
            continue;
        }

        typedef void (*FuncPtr)(void *);

        // ȡִ�ж���
        const string sym = "DllEnter";
        FuncPtr pSym = (FuncPtr)dll->GetSymbol(sym);
        if(NULL == pSym)
        {
            LOG_ERROR("ȡִ�ж������[%s]", sym.c_str());
            continue;
        }

        // ִ�У����뵱ǰ���̻���������ģ�����Ϣ��
        (*pSym)( Environment::instance() );

        LOG_INFO("Loaded: [%s]", file.name());
    }

    // ��ʼ��
    Factory::iterator it;

    for(it = Page::instance()->m_Factory.begin(); Page::instance()->m_Factory.end()!= it; it++)
    {
        const string &name = it->first;
        Page *&page = it->second;
        int ret = page->DoInit();
        if( ret < 0 )
        {
            LOG_ERROR("page->DoInit() error, name=[%s]", name.c_str());
            Page::instance()->UnRegister( name );
            continue;
        }
        LOG_DEBUG("Init page obj[%s] OK", name.c_str());
    }


    return OK;
}

/*
 * ���ܣ����ݴ����id�ӹ����в���һ��������󣨵��þ����������󴴽�������ʵ�֣�
 * ��д��Rocky 2009-09-23 14:17:54
 * ���أ���ȷ����>=0ֵ������<0ֵ��
 */
Page *Page::New(const string &page)
{
    FUNCTION_TRACK(); // �����켣����

    Page *parent = instance();

    /* ȡid��Ӧ�Ķ���Ȼ��ʹ�ô˶����DoNew()������������Ķ���
     */
    Factory::iterator it = parent->m_Factory.find( page );
    if(parent->m_Factory.end() == it)
    {
        LOG_ERROR("m_Factory[%s]=NULL", page.c_str() );
        return NULL;
    }
    Page * const obj = it->second;
    // �����������
    Page *sub = obj->DoNew();
    if(NULL == sub)
    {
        LOG_ERROR("����ҳ�洦������ڴ����");
        return NULL;
    }
    obj->m_VisitCounter->Inc();
    sub->m_TagList = obj->m_TagList;  // ָ����ͬ�ı���б����Ǹ��ƣ�
    sub->m_VisitCounter = obj->m_VisitCounter;
    sub->m_id = obj->m_id;
    return sub;
}


/*
 * ���ܣ������ฺ��ʵ�����ѳ�ʼ������������Ӧ��ʵ�ִ˽ӿڣ�
 * ��д��Rocky 2010-04-09 15:15:58
 * ���أ���ȷ����>=0ֵ������<0ֵ��
 */
int Page::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    int ret;

    // ��������
    ret = Parse();
    if(ret < 0)
    {
        LOG_ERROR("Parse error");
        return ERR;
    }

    // ȡҳ�����������ļ�
    const string &cfg = GlobalConfig::instance()->CounterDir() + "page.txt";

    // �ȶ���ԭ����
    Ini counter( cfg );
    unsigned int count = atol( counter.Get("page", m_id).c_str() );

    // ����ҳ�������
    m_VisitCounter = new Counter(m_id, "page", count);

    if(NULL == m_VisitCounter)
    {
        LOG_ERROR("Create page counter error [%s]", m_id.c_str());
        return ERR;
    }

    return OK;
}

/*
 * ���ܣ������ฺ��ʵ�����ѵĶ��󴴽���������Ӧ��ʵ�ִ˽ӿڣ�
 * ��д��Rocky 2010-04-09 15:16:12
 * ���أ���ȷ����>=0ֵ������<0ֵ��
 */
Page *Page::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    LOG_ERROR("����δʵ��");
    return NULL;
}






/*
 * ========================== ����Ϊҵ������� ==========================
 */



// ��������
void Page::SetRequest(Request *request)
{
    FUNCTION_TRACK(); // �����켣����
    m_request = request;
}

// ȡ��ҳ�浱ǰ���õ��������ӣ�
Request *Page::GetRequest()
{
    return m_request;
}

// ����ҳ��ģ��
int Page::Parse()
{
    FUNCTION_TRACK(); // �����켣����

    FileObj file;

    // �ļ����գ��򲻽�����
    if("" == m_html)
    {
        LOG_DEBUG("no html file. [%s]", m_id.c_str());
        return OK;
    }

    if( !file.Open(m_html) )
    {
        LOG_ERROR("Can't open html template file: [%s]", m_html.c_str());
        return ERR;
    }

    int ret;
    const int nFileSize = file.Size();
    std::auto_ptr<char> tmp(new char [nFileSize + 1]);
    char * const buf = tmp.get();

    if(NULL == buf)
    {
        LOG_ERROR("�����ڴ����");
        return ERR;
    }

    ret = file.Read(buf, nFileSize);
    if(nFileSize != ret)
    {
        LOG_ERROR("��ȡ�ļ���������[%d/%d]", ret, nFileSize);
        return ERR;
    }
    buf[ret] = '\0';
    //LOG_DEBUG("nFileSize=[%d], file=[%s]", nFileSize, buf);

    m_TagList = new vector< Tag * >;
    if(NULL == m_TagList)
    {
        LOG_ERROR("�����������ڴ����");
        return ERR;
    }


    vector< Tag * > &list = (*m_TagList);


    char *pBuf = buf;

    while(NULL != pBuf)
    {
        char *left;
        char *right;
        char *txt = pBuf;   // �����ı�����
        char *tag = NULL;     // ��ǲ���

        // ���ұ���磺"<!--$.......$-->"
        if( (left = strstr(pBuf, TAG_LEFT)) != NULL
            && (right = strstr(left, TAG_RIGHT)) != NULL
          )
        {
            *left = '\0';   // ȥ���������
            *right = '\0';  // ȥ���ұ�����
            tag = left + (sizeof(TAG_LEFT) - 1);    // ����������ţ�ָ���ǲ��֣�
            pBuf = right + (sizeof(TAG_RIGHT) - 1);    // �����ұ����ź�ָ���¶��ı���
        }
        else
        {
            pBuf = NULL;    // ׼����������
        }

        /*
         * ÿ�����ļ�������ǣ�����Ӧһ���������
         */

        // �����ı�
        {
            Tag *objTxt;
            if( '\0' != *txt
                && (objTxt = Tag::New("", "TXT")) != NULL // "TXT"��Ӧ�ļ��������
              )
            {
                // ���ñ�ǣ�������Ϊ�ı���
                objTxt->Set(txt, Tag::TXT);
                // �����ж�
                list.push_back(objTxt);
                //LOG_DEBUG("�����Ƕ���[%d][%s]", Tag::TXT, txt);
            }
            else
            {
                LOG_ERROR("Can't find object of tag: [%s] [%s]", m_html.c_str(), txt);
            }
        }

        // ������
        if(NULL != tag && '\0' != tag)
        {
            /*
             * ��Ǹ�ʽ�磺
             *      <!--$��ǼǺ�$����$-->'
             *
             *      �����Ϳ�ʱ����ʾ�ñ��ֻ�����ڵ�ǰҳ�棬�������
             *      ��ȫ����Ч�ı�ǣ�[2010-05-01]
             */

            // �ָ������������ֶ�
            char *tmp;
            const char *cmd = tag;
            const char *type = NULL;

            tmp = strchr(tag, '$');
            if(NULL != tmp)
            {
                *tmp = '\0';        // �����$��
                type = tmp + 1;     // ָ������
            }

            if(NULL == type || '\0' == *type)
            {
                // û�������ֶ���ָ��ǰҳ��id
                type = m_id.c_str();
            }

            Tag *objTag = Tag::New(type, cmd);
            if( objTag != NULL )
            {
                // ���ñ�ǣ�������Ϊ��ǣ�
                objTag->Set(cmd, Tag::TAG);
                // �����ж�
                list.push_back(objTag);
                //LOG_DEBUG("�����Ƕ���[%d][%s]", Tag::TAG, cmd);
            }
            else
            {
                LOG_ERROR("Can't find object of tag: [%s] [%s] [%s]", m_html.c_str(), type, cmd);
            }
        }
    }// end of while(NULL != pBuf)...



    return OK;
}// end of int Page::Parse()...

// ����ҳ������
int Page::Deal()
{
    FUNCTION_TRACK(); // �����켣����

    Connect * const connect = m_request->GetConnect();

    // ���httpЭ��ͷ��
    if( OutHead() < 0 )
    {
        LOG_ERROR("Send http head error");
        return ERR;
    }

    // ���������
    return OutBody();
}

// ���httpЭ��ͷ��
int Page::OutHead()
{
    FUNCTION_TRACK(); // �����켣����
    const string head = ""
                  "HTTP/1.1 200 OK\n"
                  "Date: " + GmgTime() + "\n"
                  "Server: NoteSrv v0.6\n"
                  "Keep-Alive: timeout=15, max=100\n"
                  "Content-Type: text/html; charset=GB2312\n"
                  "\n";
    // ����
    m_request->GetConnect()->Send(head);
    return OK;
}

// ���������
int Page::OutBody()
{
    FUNCTION_TRACK(); // �����켣����

    if(NULL == m_TagList)
    {
        LOG_DEBUG("��Ǵ��������п�");
        return OK;
    }

    Connect * const connect = m_request->GetConnect();

    int ret;
    vector< Tag * > &list = (*m_TagList);
    vector< Tag * >::iterator it;

    LOG_DEBUG("list.size=[%d]", list.size());

    // ��˳�����ҳ��ģ���и����
    for(it = list.begin(); list.end() != it; it++)
    {
        Tag * const tag = (*it);
        // ȡ��Ƕ��������ݣ�ÿ����Ƕ�������ҳ�棬�������봦�����Ĵ����ҳ��ָ�룻��
        const string &txt = tag->Get( this );
        if("" != txt)
        {
            // ���͵������
            ret = connect->Send(txt.c_str(), txt.length());
            if(ret < txt.length())
            {
                LOG_ERROR("Send() not complete: [%d/%d]", ret, txt.length());
                return ERR;
            }
        }
    }

    return OK;
}

/*
 * ���ܣ�ҳ����ת
 * ��д��Rocky 2010-05-08
 */
void Page::ChangeTo(const string &url)
{
    const string html = "HTTP/1.0 302 Moved Temporarily\n"
                        "Server: MoteSrv v0.5\n"
                        "Connection: close\n"
                        "Status: 302 Found\n"
                        "Location: " + url + "\n"
                        "\n";

    Connect * const connect = m_request->GetConnect();
    connect->Send(html);
}


// ���á���ȡtag�䴦��������
const string Page::GetResult(const string &key)
{
    return GetMapValue(m_result, key);
}

void Page::SetResult(const string &key, const string &value)
{
    m_result[ key ] = value;
}

// ȡ��ǰ�ͻ��˴�����KEYֵ
const string Page::GetCurrentKey()
{
    if("" == m_key)
    {
        // ���ͻ����д�key�ֶΣ�ʹ�ø�ֵ��
        m_key = m_request->GetField("key");
        if("" == m_key)
        {
            // ���򴴽�һ�µ�KEY
            m_key = NowTime("%Y%m%d%H%M%S");
        }
    }
    return m_key;
}

/* ȡ��ǰKEY��Ӧ�����ݣ�����
 *   ע�⣬���������޸�ʱ���������Ǹ��°������ݣ�����
 *   ������const���ͣ�
 */
Conf *Page::GetCurrentPack()
{
    // �գ����ȼ�����ֵ��
    if(NULL == m_pack)
    {
        const string &key = GetCurrentKey();
        const string &username = m_request->GetCurrentUser();
        UserData data( username );
        Conf *pack = new Conf;
        if(NULL != pack)
        {
            Ini ini;
            // ���һ����Conf���Ա����key��Ӧ������ֵ��
            ini.Set(key, *pack);
            data.Get( ini );
            pack->Set( *(ini.Get(key)) );
            m_pack = pack;
            LOG_DEBUG("Pack[key:%s] loading... ok", key.c_str());
        }
    }
    if(NULL == m_pack)
    {
        // ����һ��ֵ�������ⲿ�������NULL��⣻
        static Conf pack;
        return &pack;
    }
    return m_pack;
}

// ͳһ�����棨������ͬ����������£�
int Page::Save(const Ini &pack)
{
    const string &username = m_request->GetCurrentUser();
    UserData save( username );

    // ����
    if(save.Set( pack ) < 0)
    {
        return ERR;
    }
    // ���»���
    const string &key = GetCurrentKey();
    Conf *syn = GetCurrentPack();
    syn->Set( *(pack.Get(key)) );

    return OK;
}

// ȡ��ǰҳ������
PageCfg *Page::GetCurrentPageCfg()
{
    // �գ����ȼ�����ֵ��
    if(NULL == m_PageCfg)
    {
        const string &username = m_request->GetCurrentUser();
        const string &cfg = m_request->GetPageName() + ".txt"; // �����ļ�������ҳ����������.txt��
        m_PageCfg = new PageCfg(username, cfg);
    }
    if(NULL == m_PageCfg)
    {
        // ����һ��ֵ�������ⲿ�������NULL��⣻
        static PageCfg cfg("", "");
        return &cfg;
    }

    return m_PageCfg;
}












static ClassInit tmp(Page::init, ClassInit::INIT_HIGH);



}// end of PAGE_SPACE

