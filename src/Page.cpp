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
    FUNCTION_TRACK(); // 函数轨迹跟踪
}

Page::~Page()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪

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
 * 功能：注册子类对象到工厂
 * 编写：Rocky 2009-09-07 17:09:46
 * 返回：正确返回>=0值，出错返<0值；
 * 注意：此函数在进入main()函数之前就由构造函数调用了
 */
int Page::Register(const string &page, Page *sub)
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
    assert(NULL == m_Factory[ page ]);   // 这时的位置应为空
    m_Factory[ page ] = sub;  // 加入队列
    sub->m_id = page;
    sub->m_bParent = true;    // 所有新注册的对象都做为父对象
    LOG_DEBUG("Register CGI deal [%s:%p] OK.", page.c_str(), sub);
    return OK;
}


/*
 * 功能：注销
 * 编写：Rocky 2009-09-07 17:09:46
 * 返回：正确返回>=0值，出错返<0值；
 * 注意：此函数在进入main()函数之前就由构造函数调用了
 */
int Page::UnRegister(const string &page)
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
    m_Factory.erase(page);
    LOG_INFO("注销页面[%s]处理对象", page.c_str());
    return OK;
}

/*
 * 功能：静态初始化设置
 * 编写：Rocky 2009-09-23 14:14:55
 * 返回：正确返回>=0值，出错返<0值；
 */
int Page::init()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪


    int ret;
    static Page page;
    page.m_bParent = true;

    Page *obj = Page::instance( &page );
    if(NULL == obj)
    {
        LOG_ERROR("Page::instance() error");
        return ERR;
    }


    // 动态库文件目录
    const string &path = GlobalConfig::instance()->CgiDir();

    // 取出目录下所有动态库
    GetFileList dll( path );

    GetFileList::file_iterator file(dll);

    // 注册处理页面到队列
    while( file.next() )
    {
        DllLoad::Ptr dll;

        // 打开动态库
        if( dll->Open(file.name()) < 0 )
        {
            LOG_ERROR("加载动态库出错：[%s]", file.name());
            continue;
        }

        typedef void (*FuncPtr)(void *);

        // 取执行对象
        const string sym = "DllEnter";
        FuncPtr pSym = (FuncPtr)dll->GetSymbol(sym);
        if(NULL == pSym)
        {
            LOG_ERROR("取执行对象出错：[%s]", sym.c_str());
            continue;
        }

        // 执行（传入当前进程环境，即主模块的信息）
        (*pSym)( Environment::instance() );

        LOG_INFO("Loaded: [%s]", file.name());
    }

    // 初始化
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
 * 功能：跟据传入的id从工厂中产生一个处理对象（调用具体的子类对象创建函数来实现）
 * 编写：Rocky 2009-09-23 14:17:54
 * 返回：正确返回>=0值，出错返<0值；
 */
Page *Page::New(const string &page)
{
    FUNCTION_TRACK(); // 函数轨迹跟踪

    Page *parent = instance();

    /* 取id对应的对象，然再使用此对象的DoNew()来创建这种类的对象
     */
    Factory::iterator it = parent->m_Factory.find( page );
    if(parent->m_Factory.end() == it)
    {
        LOG_ERROR("m_Factory[%s]=NULL", page.c_str() );
        return NULL;
    }
    Page * const obj = it->second;
    // 创建子类对象
    Page *sub = obj->DoNew();
    if(NULL == sub)
    {
        LOG_ERROR("分配页面处理对象内存出错");
        return NULL;
    }
    obj->m_VisitCounter->Inc();
    sub->m_TagList = obj->m_TagList;  // 指向相同的标记列表（不是复制）
    sub->m_VisitCounter = obj->m_VisitCounter;
    sub->m_id = obj->m_id;
    return sub;
}


/*
 * 功能：各子类负责实现自已初始化操作（子类应重实现此接口）
 * 编写：Rocky 2010-04-09 15:15:58
 * 返回：正确返回>=0值，出错返<0值；
 */
int Page::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪

    int ret;

    // 解析请求
    ret = Parse();
    if(ret < 0)
    {
        LOG_ERROR("Parse error");
        return ERR;
    }

    // 取页面计数器存放文件
    const string &cfg = GlobalConfig::instance()->CounterDir() + "page.txt";

    // 先读出原数据
    Ini counter( cfg );
    unsigned int count = atol( counter.Get("page", m_id).c_str() );

    // 设置页面计数器
    m_VisitCounter = new Counter(m_id, "page", count);

    if(NULL == m_VisitCounter)
    {
        LOG_ERROR("Create page counter error [%s]", m_id.c_str());
        return ERR;
    }

    return OK;
}

/*
 * 功能：各子类负责实现自已的对象创建器（子类应重实现此接口）
 * 编写：Rocky 2010-04-09 15:16:12
 * 返回：正确返回>=0值，出错返<0值；
 */
Page *Page::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
    LOG_ERROR("功能未实现");
    return NULL;
}






/*
 * ========================== 以下为业务处理代码 ==========================
 */



// 设置连接
void Page::SetRequest(Request *request)
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
    m_request = request;
}

// 取出页面当前所用的请求（连接）
Request *Page::GetRequest()
{
    return m_request;
}

// 解析页面模板
int Page::Parse()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪

    FileObj file;

    // 文件名空，则不解析；
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
        LOG_ERROR("分配内存出错");
        return ERR;
    }

    ret = file.Read(buf, nFileSize);
    if(nFileSize != ret)
    {
        LOG_ERROR("读取文件不完整，[%d/%d]", ret, nFileSize);
        return ERR;
    }
    buf[ret] = '\0';
    //LOG_DEBUG("nFileSize=[%d], file=[%s]", nFileSize, buf);

    m_TagList = new vector< Tag * >;
    if(NULL == m_TagList)
    {
        LOG_ERROR("分配标记容器内存出错");
        return ERR;
    }


    vector< Tag * > &list = (*m_TagList);


    char *pBuf = buf;

    while(NULL != pBuf)
    {
        char *left;
        char *right;
        char *txt = pBuf;   // 正常文本部分
        char *tag = NULL;     // 标记部分

        // 查找标记如："<!--$.......$-->"
        if( (left = strstr(pBuf, TAG_LEFT)) != NULL
            && (right = strstr(left, TAG_RIGHT)) != NULL
          )
        {
            *left = '\0';   // 去掉左边括号
            *right = '\0';  // 去掉右边括号
            tag = left + (sizeof(TAG_LEFT) - 1);    // 跳过左边括号（指向标记部分）
            pBuf = right + (sizeof(TAG_RIGHT) - 1);    // 跳过右边括号后，指向下段文本；
        }
        else
        {
            pBuf = NULL;    // 准备结束操作
        }

        /*
         * 每个段文件（及标记），对应一个处理对象；
         */

        // 正常文本
        {
            Tag *objTxt;
            if( '\0' != *txt
                && (objTxt = Tag::New("", "TXT")) != NULL // "TXT"对应文件处理对象
              )
            {
                // 设置标记，及类型为文本；
                objTxt->Set(txt, Tag::TXT);
                // 插入列队
                list.push_back(objTxt);
                //LOG_DEBUG("插入标记对象：[%d][%s]", Tag::TXT, txt);
            }
            else
            {
                LOG_ERROR("Can't find object of tag: [%s] [%s]", m_html.c_str(), txt);
            }
        }

        // 处理标记
        if(NULL != tag && '\0' != tag)
        {
            /*
             * 标记格式如：
             *      <!--$标记记号$类型$-->'
             *
             *      当类型空时，表示该标记只从属于当前页面，否则可能
             *      是全局有效的标记；[2010-05-01]
             */

            // 分割出命令和类型字段
            char *tmp;
            const char *cmd = tag;
            const char *type = NULL;

            tmp = strchr(tag, '$');
            if(NULL != tmp)
            {
                *tmp = '\0';        // 清掉‘$’
                type = tmp + 1;     // 指向类型
            }

            if(NULL == type || '\0' == *type)
            {
                // 没有类型字段则指向当前页面id
                type = m_id.c_str();
            }

            Tag *objTag = Tag::New(type, cmd);
            if( objTag != NULL )
            {
                // 设置标记，及类型为标记；
                objTag->Set(cmd, Tag::TAG);
                // 插入列队
                list.push_back(objTag);
                //LOG_DEBUG("插入标记对象：[%d][%s]", Tag::TAG, cmd);
            }
            else
            {
                LOG_ERROR("Can't find object of tag: [%s] [%s] [%s]", m_html.c_str(), type, cmd);
            }
        }
    }// end of while(NULL != pBuf)...



    return OK;
}// end of int Page::Parse()...

// 处理页求请求
int Page::Deal()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪

    Connect * const connect = m_request->GetConnect();

    // 输出http协议头部
    if( OutHead() < 0 )
    {
        LOG_ERROR("Send http head error");
        return ERR;
    }

    // 输出数据体
    return OutBody();
}

// 输出http协议头部
int Page::OutHead()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
    const string head = ""
                  "HTTP/1.1 200 OK\r\n"
                  "Date: " + GmgTime() + "\r\n"
                  "Server: NoteSrv v0.7\r\n"
                  "Keep-Alive: timeout=15, max=100\r\n"
                  "Content-Type: text/html; charset=utf-8\r\n"
                  "\r\n";
    // 发送
    m_request->GetConnect()->Send(head);
    return OK;
}

// 输出数据体
int Page::OutBody()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪

    if(NULL == m_TagList)
    {
        LOG_DEBUG("标记处理对象队列空");
        return OK;
    }

    Connect * const connect = m_request->GetConnect();

    int ret;
    vector< Tag * > &list = (*m_TagList);
    vector< Tag * >::iterator it;

    LOG_DEBUG("list.size=[%d]", list.size());

    // 按顺序输出页面模块中各标记
    for(it = list.begin(); list.end() != it; it++)
    {
        Tag * const tag = (*it);
        // 取标记对象处理内容（每个标记都从属于页面，所出传入处理正的处理的页面指针；）
        const string &txt = tag->Get( this );
        if("" != txt)
        {
            // 发送到浏览器
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
 * 功能：页面跳转
 * 编写：Rocky 2010-05-08
 */
void Page::ChangeTo(const string &url)
{
    const string html = "HTTP/1.0 302 Moved Temporarily\r\n"
                        "Server: MoteSrv v0.5\r\n"
                        "Connection: close\r\n"
                        "Status: 302 Found\r\n"
                        "Location: " + url + "\r\n"
                        "\r\n";

    Connect * const connect = m_request->GetConnect();
    connect->Send(html);
}


// 设置、获取tag间处理结果传递
const string Page::GetResult(const string &key)
{
    return GetMapValue(m_result, key);
}

void Page::SetResult(const string &key, const string &value)
{
    m_result[ key ] = value;
}

// 取当前客户端传来的KEY值
const string Page::GetCurrentKey()
{
    if("" == m_key)
    {
        // 若客户端有传key字段，使用该值：
        m_key = m_request->GetField("key");
        if("" == m_key)
        {
            // 否则创建一新的KEY
            m_key = NowTime("%Y%m%d%H%M%S");
        }
    }
    return m_key;
}

/* 取当前KEY对应的数据（包）
 *   注意，当数据有修改时，可能需是更新包的内容，所以
 *   不返回const类型；
 */
Conf *Page::GetCurrentPack()
{
    // 空，则先加载其值；
    if(NULL == m_pack)
    {
        const string &key = GetCurrentKey();
        const string &username = m_request->GetCurrentUser();
        UserData data( username );
        Conf *pack = new Conf;
        if(NULL != pack)
        {
            Ini ini;
            // 填充一个空Conf，以便插入key对应的所有值；
            ini.Set(key, *pack);
            data.Get( ini );
            pack->Set( *(ini.Get(key)) );
            m_pack = pack;
            LOG_DEBUG("Pack[key:%s] loading... ok", key.c_str());
        }
    }
    if(NULL == m_pack)
    {
        // 返回一空值，避免外部做过多的NULL检测；
        static Conf pack;
        return &pack;
    }
    return m_pack;
}

// 统一处理保存（出便于同步处理缓存更新）
int Page::Save(const Ini &pack)
{
    const string &username = m_request->GetCurrentUser();
    UserData save( username );

    // 保存
    if(save.Set( pack ) < 0)
    {
        return ERR;
    }
    // 更新缓存
    const string &key = GetCurrentKey();
    Conf *syn = GetCurrentPack();
    syn->Set( *(pack.Get(key)) );

    return OK;
}

// 取当前页面设置
PageCfg *Page::GetCurrentPageCfg()
{
    // 空，则先加载其值；
    if(NULL == m_PageCfg)
    {
        const string &username = m_request->GetCurrentUser();
        const string &cfg = m_request->GetPageName() + ".txt"; // 配置文件名：“页面名”＋“.txt”
        m_PageCfg = new PageCfg(username, cfg);
    }
    if(NULL == m_PageCfg)
    {
        // 返回一空值，避免外部做过多的NULL检测；
        static PageCfg cfg("", "");
        return &cfg;
    }

    return m_PageCfg;
}












static ClassInit tmp(Page::init, ClassInit::INIT_HIGH);



}// end of PAGE_SPACE

