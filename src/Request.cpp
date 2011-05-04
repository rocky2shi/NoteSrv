// Rocky 2010-04-18 16:25:35
#include "User.h"
#include "Session.h"
#include "Request.h"
namespace REQUEST_SPACE
{








Request::Request(Connect *connect) : m_connect(connect)
{
    Parse();
}

Request::~Request()
{
    // 释放文件对象（并删除对应文件）
    map<string, FileObj*>::iterator it;
    for(it = m_FileObjAry.begin(); m_FileObjAry.end() != it; it++)
    {
        // 注意，m_FieldAry里也记录文件名，但不包含路径，所以不能用：m_FieldAry[it->first]
        const string &filename = it->second->Fullname();
        if("" != filename)
        {
            LOG_DEBUG("Delete file: [%s]", filename.c_str());
            DeleteFile(filename);
        }
        delete it->second;
    }
}

// 取请求头中字段对应值（不区分大小写）
const string Request::GetHead(const string &field) const
{
    return GetMapValue(m_HeadFieldAry, field);
}

// 一般字段对应值（不区分大小写）
const string Request::GetField(const string &field) const
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
    return GetMapValue(m_FieldAry, field);
}

// 取字段对应文件对象
FileObj *Request::GetFileObj(const string &field)
{
    static FileObj s("");   // 避免NULL指针检测
    map< string, FileObj* >::iterator it = m_FileObjAry.find(field);
    return m_FileObjAry.end() != it ? it->second : &s;
}

// 返回请求页面名
const string Request::GetPageName()
{
    return m_page;
}

// 取连接
Connect *Request::GetConnect()
{
    return m_connect;
}

// 取当前登录用户名
const string Request::GetCurrentUser() const
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
    return Session::Get( this )->GetUser();
}

// 取Cookie中的值
const string Request::GetCookie(const string &key) const
{
    return GetMapValue(m_Cookie, key);
}

// 是否为图片：jpg, jpeg, png, gif, ...
bool Request::isImage()
{
    return ::isImage(m_PageType);
}

// 是否为文本文件：txt, js, css, html, htm, ...
bool Request::isText()
{
    return ::isText(m_PageType);
}

// 是否为cgi处理页面
bool Request::isCgi()
{
    // 除了文本、图片外，都认为是cgi处理页
    return !isText() && !isImage();
}

// 解析等是否正确处理
bool Request::ok()
{
    // 无出错时，m_ErrMsg应为空；
    return "" == m_ErrMsg;
}

// 取一行
int Request::GetLine(char *buf, unsigned int max)
{
    //FUNCTION_TRACK(); // 函数轨迹跟踪
    int ret = m_connect->GetLine(buf, max - 1);
    return ret <= 0 ? 0 : ret;
}

/*
 * 功能：解析Cookie串
 * 编写：Rocky 2010-04-21 13:42:27
 */
void Request::ParseCookie()
{
    /*
     * Cookie:
     *
     *  “d=xqtp5CMiojlXfhoyFH51JNaarbngn6dS; path=/;”
     */
    char buf[512];
    // Cookie字段在头部中
    const string &cookie = GetHead("Cookie");
    // 因为ParseStr()的第一个参数字段必须是可写的内存，所以copy一份；
    strncpy(buf, cookie.c_str(), sizeof(buf) - 1);
    ParseStr(buf, m_Cookie, ';', '=');
}

/*
 * 功能：解析url参数串
 * 编写：Rocky 2010-04-21 13:42:27
 */
void Request::ParseUrlParam(char *param)
{
    FUNCTION_TRACK(); // 函数轨迹跟踪

    char *begin;
    char *end;
    char *name;
    char *value;

    /*
     * 处理几种情况：
     *
     * 1 => [window.screen.width=1024&window.screen.height=&username=abcdefghijklmnopqrstuvwxyz&login=GET\0]
     * 2 => [user\0]
     * 3 => [user&pasd=1234\0]
     * 4 => [\0]
     */
    ParseStr(param, m_FieldAry, '&', '=');
}

// 处理请求头部
void Request::ParseHead()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪

    static char EMPYT[2]; // 防止出错用
    char buf[BUF_MAX_LEN + 1] = "";
    char *begin;
    char *end;
    char *tmp;
    int ret;


    /* 循环处理http头部各字段，如
     *      Host: 192.168.0.230:17890\n\
     *      Accept: application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png;q=0.5\n\
     *      Accept-Encoding: gzip,deflate,sdch\n\
     */
    while(1)
    {
        char *name = EMPYT;
        char *value = EMPYT;

        // 指定空串，以防写了不可写的位置；
        begin = end = tmp = EMPYT;

        // 取一行，再分析这行；
        ret = GetLine(buf, BUF_MAX_LEN);
        if( ret <= 0 || IS_CR(*buf))
        {
            LOG_DEBUG("Http head analyze finish.");
            break;
        }

        begin = buf;

        // 取键名
        name = begin;
        end = strchr(begin, ':');
        if(NULL == end)
        {
            LOG_ERROR("Field error, not find separating character ':'");
            LOG_ERROR("[%s]", begin);
            return;
        }
        *end = '\0';
        name = begin;
        //LOG_DEBUG("[%s]", name);

        // 取键值
        begin = end + 1;
        IGNORE_BLANK(begin);
        value = begin;
        end = value;
        JUMP_TO_LINE_END(end);
        *end = '\0';
        //LOG_DEBUG("[%s]", value);

        if( strcasecmp("Content-Type", name) == 0 )
        {
            // 是Content-Type，再取出boundary；
            char BOUNDARY_KEY[] = "boundary=";
            char *pBoundary = strstr(value, BOUNDARY_KEY);
            if(NULL != pBoundary)
            {
                pBoundary += (sizeof(BOUNDARY_KEY) - 1);
                m_boundary = pBoundary;
                m_boundary = "--" + m_boundary; // http协议内，boundary字段前加‘--’
            }
            LOG_DEBUG("boundary=[%s]", m_boundary.c_str());
        }
        //toLower(name); // 是否需转小写？[XXX]
        m_HeadFieldAry[name] = value;
        //LOG_DEBUG("[%s]=[%s]", name, value);
    }// end of while(1)...
}// end of void Request::ParseHead()...

// 处理GET的情况
void Request::ParseGet(char *str)
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
    ParseUrlParam(str);
}

// 处理POST的情况
void Request::ParsePost()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪

    const unsigned long len = atol( m_HeadFieldAry["Content-Length"].c_str() );
    LOG_DEBUG("len=[%ld]", len);
    if(len <= 0)
    {
        LOG_INFO("no POST data");
        return;
    }

    char *buf = new char[len + 1];
    if(NULL == buf)
    {
        LOG_ERROR("取存放post数据内存出错，len=[%ld]", len);
        return;
    }
    const int ret = m_connect->GetLine(buf, len);
    buf[ret] = '\0';
    if(ret == len) // 取的数据完整时才做解析
    {
        ParseUrlParam(buf);
    }
    delete [] buf;
}

// 处理附件的情况
void Request::ParseAttach()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪

    static char EMPYT[2]; // 防止出错用
    char buf[BUF_MAX_LEN + 1] = "";
    char *begin;
    char *end;
    char *tmp;
    int ret;


    /* 分析页面内容，数据如：
     *
     *      ------WebKitFormBoundaryiGleMlTLa04QAWIX
     *      Content-Disposition: form-data; name="key"
     *
     *      20100419194646
     *      ------WebKitFormBoundaryiGleMlTLa04QAWIX
     *      Content-Disposition: form-data; name="attach"; filename="12347890[中国人].txt"
     *      Content-Type: text/plain
     *
     *      abcdefg
     *      abcdefg
     *      abcdefg
     *      ------WebKitFormBoundaryiGleMlTLa04QAWIX
     *      Content-Disposition: form-data; name="upfile"
     *
     *      上传
     *      ------WebKitFormBoundaryiGleMlTLa04QAWIX--
     *
     */

    // 跳过boundary
    ret = GetLine(buf, BUF_MAX_LEN);
    CUT_CR(buf, ret);

    // 内空的第一行应是boundary，否则出错；
    if(m_boundary != buf)
    {
        LOG_ERROR("数据格式出错，[%s]", buf);
        return;
    }

    const string BOUNDARY_END = m_boundary + "--"; // 结束标志为‘boundary + "--"’

    while(1)
    {
        // 指定空串，以防写了不可写的位置；
        begin = end = tmp = EMPYT;

        // 取一行，再分析这行；
        ret = GetLine(buf, BUF_MAX_LEN);
        if( (ret <= 0) || (strncmp(buf, BOUNDARY_END.c_str(), BOUNDARY_END.length()) == 0) )
        {
            LOG_DEBUG("数据体分析结束");
            break;
        }


        /* 取控件名称
         *  Content-Disposition: form-data; name="attach"; filename="12347890[中国人].txt"
         *  Content-Disposition: form-data; name="username"
         */

        CUT_CR(buf, ret);


        // 取文件名字段filename（当不是文件上传时，没有filename字段）（可选项）
        const char NAMEFILE_KEY[] = "filename=";
        string filename; // 附件文件名
        begin = strstr(buf, NAMEFILE_KEY);
        if(NULL != begin)
        {
            begin = begin + (sizeof(NAMEFILE_KEY) - 1); // 跳过NAMEFILE_KEY标记字段
            end = begin;
            JUMP_TO_LINE_END(end);
            *end = '\0';
            /* 解码后存入
             * 注意，在ie中，上传来的文件字段filename="xxx"中的xxx包含了客户端
             * 文件路径，要去掉，只留文件名；
             */
            filename = GetBaseName( UrlDecode( CUT_QUOTE(begin) ) );
            LOG_DEBUG("filename=[%s]", filename.c_str());
        }

        // 取控件名称
        const char NAME_KEY[] = "name=";
        string element;
        begin = strstr(buf, NAME_KEY);
        if(NULL == begin)
        {
            // name是必选字段，没有则出错；
            LOG_ERROR("数据格式出错，[%s]", buf);
            return;
        }
        begin = begin + (sizeof(NAME_KEY) - 1);
        end = strchr(begin, ';');
        if(NULL != end)
        {
            *end = '\0';
        }
        begin = CUT_QUOTE(begin);
        toLower(begin);
        element = begin;
        LOG_DEBUG("element=[%s]", element.c_str());


        // 定位到空行（协议中以空行分隔数据头、数据体）
        while( (GetLine(buf, BUF_MAX_LEN) > 0) && !IS_CR(*buf) );

        /* 取控件对应的值（或文件内容），当filename不空时为文件；
         */
        char buf2[BUF_MAX_LEN + 1] = "";
        struct {
            char *buf;
            int len;
        } prev, next, tmp;   // prev、next轮流指向buf、buf2，以处理下行是否为boundary的测试

        prev.buf = buf;
        prev.len = 0;
        next.buf = buf2;
        next.len = 0;

        // 取第一行
        prev.len = GetLine(prev.buf, BUF_MAX_LEN);
        if( prev.len <= 0 )
        {
            LOG_ERROR("数据格式出错，[%s]", prev.buf);
            break;
        }



        /* 下面区分两种情况，读取的代码相同，但写入的代码不同，是文件上传
         * 的，则写入文件。否则是一般提交内容，则只存到变量（内存）中；
         */
        if("" != filename)
        {
            /* 是文件，先暂存到用户临时目录
             */
            const string &username = Session::Get(this)->GetUser();
            User *user = User::Get(username);
            const string &fullpath = user->TmpDir() + "." + GetRandomString(); /* 这里临时名中不包含原
                                                                                * 文件名，以防不同系统
                                                                                * 中汉字乱码。[XXX]
                                                                                */
            FileObj file;

            // 文件字段 <==> 文名
            m_FieldAry[element] = filename;

            LOG_DEBUG("fullpath=[%s]", fullpath.c_str());
            if( ! file.Open(fullpath, FileObj::W) )
            {
                LOG_ERROR("不能打开文件：[%s]", fullpath.c_str());
                return; // 直接返回
            }

            while( ((next.len=GetLine(next.buf, BUF_MAX_LEN)) > 0)
                   && (strncmp(next.buf, m_boundary.c_str(), m_boundary.length()) != 0)
                 )
            {
                // LOG_DEBUG("[%d]", prev.len);
                file.Write(prev.buf, prev.len);
                prev.buf[0] = '\0';

                // 前后指向交换
                tmp = prev;
                prev = next;
                next = tmp;
            }
            int n = CUT_CR(prev.buf, prev.len); // 注意，不同的系统，可能追加‘\r’‘\r\n’等；
            // LOG_DEBUG("[%d]", prev.len - n);
            file.Write(prev.buf, prev.len - n);
            file.Close();

            // 记入m_FileObjAry
            FileObj *obj = new FileObj(fullpath);
            if(NULL == obj)
            {
                LOG_ERROR("分配文件对象内存出错: [%s]", fullpath.c_str());
                return;
            }
            m_FileObjAry[element] = obj;
            LOG_DEBUG("加入： m_FileObjAry[\"%s\"]=[%p]", element.c_str(), obj);
        }
        else
        {
            /* 是一般的提交内容
             */
            string &field = m_FieldAry[element]; //
            while( ((next.len=GetLine(next.buf, BUF_MAX_LEN)) > 0) && (strncmp(next.buf, m_boundary.c_str(), m_boundary.length()) != 0) )
            {
                // LOG_DEBUG("[%s]", prev.buf);
                field =+ prev.buf;
                prev.buf[0] = '\0';

                // 前后指向交换
                tmp = prev;
                prev = next;
                next = tmp;
            }
            CUT_CR(prev.buf, prev.len);
            field =+ prev.buf;
            // LOG_DEBUG("[%s]", prev.buf);
        }


        // 是否到结束标记
        if( ('\0' == next.buf[0]) || (strncmp(next.buf, m_boundary.c_str(), m_boundary.length()) != 0) )
        {
            break;
        }

    }// end of while(1)...
}// end of void Request::ParseAttach()...

// 解析请求串
//
// GET / HTTP/1.1
// Host: 192.168.1.100:17890
// Connection: keep-alive
// User-Agent: Mozilla/5.0 (X11; U; Linux i686; en-US) AppleWebKit/533.4 (KHTML, like Gecko) Chrome/5.0.375.3 Safari/533.4
// Cache-Control: max-age=0
// Accept: application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5
// Accept-Encoding: gzip,deflate,sdch
// Accept-Language: zh-CN,zh;q=0.8
// Accept-Charset: gb18030,utf-8;q=0.7,*;q=0.3
void Request::Parse()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪

    static char EMPYT[2] = ""; // 防止出错用
    char buf[BUF_MAX_LEN + 1] = "";
    char *begin;
    char *end;
    char *tmp;
    char *param = NULL; // url请求中的参数
    int ret;


    // 取第一行
    ret = GetLine(buf, BUF_MAX_LEN);
    CUT_CR(buf, ret);
    LOG_DEBUG("buf=[%s]", buf);

    /*
     * 从后往，先取url请求中的参数；‘?’后部分就是参数：
     *      GET /login.cgi?window.screen.width=&window.screen.height=&username=abcdef&login=GET HTTP/1.1
     */
    begin = strchr(buf, '?');
    if(NULL != begin)
    {
        end = strchr(begin, ' '); // 遇到空格为结束
        if(NULL != end)
        {
            *end = '\0';
            *begin = '\0'; // 清掉‘?’，断行；
            param = begin + 1;
        }
    }
    //LOG_DEBUG("param=[%s]", param);

    /*
     * 取请求类型：
     *      GET /cgi-bin/login.cgi?user=rocky HTTP/1.1\n\
     */
    begin = buf;
    end = strchr(buf, ' ');
    if(NULL == end)
    {
        LOG_ERROR("data format error: [%s]", buf);
        return;
    }
    *end = '\0';
    toUpper(begin); // 转为大写
    m_type = begin;
    LOG_DEBUG("type=[%s]", m_type.c_str());

    /*
     * 取页面名
     *      GET /cgi-bin/login.cgi?user=rocky HTTP/1.1\n\
     *      GET /cgi-bin/login.cgi HTTP/1.1\n\
     */
    begin = end + 1;    // 请求类型后就是页面名
    IGNORE_BLANK(begin);
    tmp = begin;
    /* 跳到最后一个‘/’
     *   不是：1. 空格
     *         2. 回车换行符（\r、\n）
     *         3. 不是串尾
     */
    while(' ' != *tmp && !IS_CR(*tmp) && '\0' != *tmp)
    {
        if('/' == *tmp)
        {
            begin = tmp + 1; // 记录最后一个‘/’
        }
        tmp++;
    }
    // 找结尾位置
    if( (end = strchr(begin, '?')) != NULL
        || (end = strchr(begin, ' ')) != NULL
      )
    {
        *end = '\0';
    }
    // toLower(begin); // 转为小写
    m_page =  begin;
    LOG_DEBUG("page=[%s]", m_page.c_str());

    // 取后缀
    m_PageType = GetFileType(m_page.c_str());
    LOG_DEBUG("m_PageType=[%s]", m_PageType.c_str());

    /*
     * 分析请求头部信息
     */
    ParseHead();

    /*
     * 处理Cookie字段
     */
    ParseCookie();


    /*
     * 在这里执行认证，以避免做无用分析；
     * （登录页面无作认证）
     */
    if( "login" != m_page
        && "register" != m_page
        && "catchphrase" != m_page
        && "ping" != m_page
        && isCgi()
        && ! Session::Get(this)->CheckLogin(this)
      )
    {
        m_ErrMsg = "User not login.";
        LOG_DEBUG("%s", m_ErrMsg.c_str());
        return;
    }



    /*
     * 按类型分情况处理
     */

    // 处理GET请求（注意，在POST中也可能包含GET的处理；
    if(NULL != param)
    {
        ParseGet(param);
    }

    if("" != m_boundary)
    {
        // 处理附件提交
        ParseAttach();
    }
    else if("POST" == m_type)
    {
        // 处理一般的POST请求
        ParsePost();
    }

    //LOG_DEBUG("m_HeadFieldAry.size=[%d]", m_HeadFieldAry.size());
    //LOG_DEBUG("m_FieldAry.size=[%d]", m_FieldAry.size());
    //LOG_DEBUG("----------------------------");
    //LOG_DEBUG("m_HeadFieldAry=[%s]", map2string(m_HeadFieldAry).c_str());
    //LOG_DEBUG("m_FieldAry=[%s]", map2string(m_FieldAry).c_str());

}// end of void Request::Parse()...









}// end of REQUEST_SPACE






