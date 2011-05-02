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
    // �ͷ��ļ����󣨲�ɾ����Ӧ�ļ���
    map<string, FileObj*>::iterator it;
    for(it = m_FileObjAry.begin(); m_FileObjAry.end() != it; it++)
    {
        // ע�⣬m_FieldAry��Ҳ��¼�ļ�������������·�������Բ����ã�m_FieldAry[it->first]
        const string &filename = it->second->Fullname();
        if("" != filename)
        {
            LOG_DEBUG("Delete file: [%s]", filename.c_str());
            DeleteFile(filename);
        }
        delete it->second;
    }
}

// ȡ����ͷ���ֶζ�Ӧֵ�������ִ�Сд��
const string Request::GetHead(const string &field) const
{
    return GetMapValue(m_HeadFieldAry, field);
}

// һ���ֶζ�Ӧֵ�������ִ�Сд��
const string Request::GetField(const string &field) const
{
    FUNCTION_TRACK(); // �����켣����
    return GetMapValue(m_FieldAry, field);
}

// ȡ�ֶζ�Ӧ�ļ�����
FileObj *Request::GetFileObj(const string &field)
{
    static FileObj s("");   // ����NULLָ����
    map< string, FileObj* >::iterator it = m_FileObjAry.find(field);
    return m_FileObjAry.end() != it ? it->second : &s;
}

// ��������ҳ����
const string Request::GetPageName()
{
    return m_page;
}

// ȡ����
Connect *Request::GetConnect()
{
    return m_connect;
}

// ȡ��ǰ��¼�û���
const string Request::GetCurrentUser() const
{
    FUNCTION_TRACK(); // �����켣����
    return Session::Get( this )->GetUser();
}

// ȡCookie�е�ֵ
const string Request::GetCookie(const string &key) const
{
    return GetMapValue(m_Cookie, key);
}

// �Ƿ�ΪͼƬ��jpg, jpeg, png, gif, ...
bool Request::isImage()
{
    return ::isImage(m_PageType);
}

// �Ƿ�Ϊ�ı��ļ���txt, js, css, html, htm, ...
bool Request::isText()
{
    return ::isText(m_PageType);
}

// �Ƿ�Ϊcgi����ҳ��
bool Request::isCgi()
{
    // �����ı���ͼƬ�⣬����Ϊ��cgi����ҳ
    return !isText() && !isImage();
}

// �������Ƿ���ȷ����
bool Request::ok()
{
    // �޳���ʱ��m_ErrMsgӦΪ�գ�
    return "" == m_ErrMsg;
}

// ȡһ��
int Request::GetLine(char *buf, unsigned int max)
{
    //FUNCTION_TRACK(); // �����켣����
    int ret = m_connect->GetLine(buf, max - 1);
    return ret <= 0 ? 0 : ret;
}

/*
 * ���ܣ�����Cookie��
 * ��д��Rocky 2010-04-21 13:42:27
 */
void Request::ParseCookie()
{
    /*
     * Cookie:
     *
     *  ��d=xqtp5CMiojlXfhoyFH51JNaarbngn6dS; path=/;��
     */
    char buf[512];
    // Cookie�ֶ���ͷ����
    const string &cookie = GetHead("Cookie");
    // ��ΪParseStr()�ĵ�һ�������ֶα����ǿ�д���ڴ棬����copyһ�ݣ�
    strncpy(buf, cookie.c_str(), sizeof(buf) - 1);
    ParseStr(buf, m_Cookie, ';', '=');
}

/*
 * ���ܣ�����url������
 * ��д��Rocky 2010-04-21 13:42:27
 */
void Request::ParseUrlParam(char *param)
{
    FUNCTION_TRACK(); // �����켣����

    char *begin;
    char *end;
    char *name;
    char *value;

    /*
     * �����������
     *
     * 1 => [window.screen.width=1024&window.screen.height=&username=abcdefghijklmnopqrstuvwxyz&login=GET\0]
     * 2 => [user\0]
     * 3 => [user&pasd=1234\0]
     * 4 => [\0]
     */
    ParseStr(param, m_FieldAry, '&', '=');
}

// ��������ͷ��
void Request::ParseHead()
{
    FUNCTION_TRACK(); // �����켣����

    static char EMPYT[2]; // ��ֹ������
    char buf[BUF_MAX_LEN + 1] = "";
    char *begin;
    char *end;
    char *tmp;
    int ret;


    /* ѭ������httpͷ�����ֶΣ���
     *      Host: 192.168.0.230:17890\n\
     *      Accept: application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png;q=0.5\n\
     *      Accept-Encoding: gzip,deflate,sdch\n\
     */
    while(1)
    {
        char *name = EMPYT;
        char *value = EMPYT;

        // ָ���մ����Է�д�˲���д��λ�ã�
        begin = end = tmp = EMPYT;

        // ȡһ�У��ٷ������У�
        ret = GetLine(buf, BUF_MAX_LEN);
        if( ret <= 0 || IS_CR(*buf))
        {
            LOG_DEBUG("Http head analyze finish.");
            break;
        }

        begin = buf;

        // ȡ����
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

        // ȡ��ֵ
        begin = end + 1;
        IGNORE_BLANK(begin);
        value = begin;
        end = value;
        JUMP_TO_LINE_END(end);
        *end = '\0';
        //LOG_DEBUG("[%s]", value);

        if( strcasecmp("Content-Type", name) == 0 )
        {
            // ��Content-Type����ȡ��boundary��
            char BOUNDARY_KEY[] = "boundary=";
            char *pBoundary = strstr(value, BOUNDARY_KEY);
            if(NULL != pBoundary)
            {
                pBoundary += (sizeof(BOUNDARY_KEY) - 1);
                m_boundary = pBoundary;
                m_boundary = "--" + m_boundary; // httpЭ���ڣ�boundary�ֶ�ǰ�ӡ�--��
            }
            LOG_DEBUG("boundary=[%s]", m_boundary.c_str());
        }
        //toLower(name); // �Ƿ���תСд��[XXX]
        m_HeadFieldAry[name] = value;
        //LOG_DEBUG("[%s]=[%s]", name, value);
    }// end of while(1)...
}// end of void Request::ParseHead()...

// ����GET�����
void Request::ParseGet(char *str)
{
    FUNCTION_TRACK(); // �����켣����
    ParseUrlParam(str);
}

// ����POST�����
void Request::ParsePost()
{
    FUNCTION_TRACK(); // �����켣����

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
        LOG_ERROR("ȡ���post�����ڴ����len=[%ld]", len);
        return;
    }
    const int ret = m_connect->GetLine(buf, len);
    buf[ret] = '\0';
    if(ret == len) // ȡ����������ʱ��������
    {
        ParseUrlParam(buf);
    }
    delete [] buf;
}

// �����������
void Request::ParseAttach()
{
    FUNCTION_TRACK(); // �����켣����

    static char EMPYT[2]; // ��ֹ������
    char buf[BUF_MAX_LEN + 1] = "";
    char *begin;
    char *end;
    char *tmp;
    int ret;


    /* ����ҳ�����ݣ������磺
     *
     *      ------WebKitFormBoundaryiGleMlTLa04QAWIX
     *      Content-Disposition: form-data; name="key"
     *
     *      20100419194646
     *      ------WebKitFormBoundaryiGleMlTLa04QAWIX
     *      Content-Disposition: form-data; name="attach"; filename="12347890[�й���].txt"
     *      Content-Type: text/plain
     *
     *      abcdefg
     *      abcdefg
     *      abcdefg
     *      ------WebKitFormBoundaryiGleMlTLa04QAWIX
     *      Content-Disposition: form-data; name="upfile"
     *
     *      �ϴ�
     *      ------WebKitFormBoundaryiGleMlTLa04QAWIX--
     *
     */

    // ����boundary
    ret = GetLine(buf, BUF_MAX_LEN);
    CUT_CR(buf, ret);

    // �ڿյĵ�һ��Ӧ��boundary���������
    if(m_boundary != buf)
    {
        LOG_ERROR("���ݸ�ʽ����[%s]", buf);
        return;
    }

    const string BOUNDARY_END = m_boundary + "--"; // ������־Ϊ��boundary + "--"��

    while(1)
    {
        // ָ���մ����Է�д�˲���д��λ�ã�
        begin = end = tmp = EMPYT;

        // ȡһ�У��ٷ������У�
        ret = GetLine(buf, BUF_MAX_LEN);
        if( (ret <= 0) || (strncmp(buf, BOUNDARY_END.c_str(), BOUNDARY_END.length()) == 0) )
        {
            LOG_DEBUG("�������������");
            break;
        }


        /* ȡ�ؼ�����
         *  Content-Disposition: form-data; name="attach"; filename="12347890[�й���].txt"
         *  Content-Disposition: form-data; name="username"
         */

        CUT_CR(buf, ret);


        // ȡ�ļ����ֶ�filename���������ļ��ϴ�ʱ��û��filename�ֶΣ�����ѡ�
        const char NAMEFILE_KEY[] = "filename=";
        string filename; // �����ļ���
        begin = strstr(buf, NAMEFILE_KEY);
        if(NULL != begin)
        {
            begin = begin + (sizeof(NAMEFILE_KEY) - 1); // ����NAMEFILE_KEY����ֶ�
            end = begin;
            JUMP_TO_LINE_END(end);
            *end = '\0';
            /* ��������
             * ע�⣬��ie�У��ϴ������ļ��ֶ�filename="xxx"�е�xxx�����˿ͻ���
             * �ļ�·����Ҫȥ����ֻ���ļ�����
             */
            filename = GetBaseName( UrlDecode( CUT_QUOTE(begin) ) );
            LOG_DEBUG("filename=[%s]", filename.c_str());
        }

        // ȡ�ؼ�����
        const char NAME_KEY[] = "name=";
        string element;
        begin = strstr(buf, NAME_KEY);
        if(NULL == begin)
        {
            // name�Ǳ�ѡ�ֶΣ�û�������
            LOG_ERROR("���ݸ�ʽ����[%s]", buf);
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


        // ��λ�����У�Э�����Կ��зָ�����ͷ�������壩
        while( (GetLine(buf, BUF_MAX_LEN) > 0) && !IS_CR(*buf) );

        /* ȡ�ؼ���Ӧ��ֵ�����ļ����ݣ�����filename����ʱΪ�ļ���
         */
        char buf2[BUF_MAX_LEN + 1] = "";
        struct {
            char *buf;
            int len;
        } prev, next, tmp;   // prev��next����ָ��buf��buf2���Դ��������Ƿ�Ϊboundary�Ĳ���

        prev.buf = buf;
        prev.len = 0;
        next.buf = buf2;
        next.len = 0;

        // ȡ��һ��
        prev.len = GetLine(prev.buf, BUF_MAX_LEN);
        if( prev.len <= 0 )
        {
            LOG_ERROR("���ݸ�ʽ����[%s]", prev.buf);
            break;
        }



        /* �������������������ȡ�Ĵ�����ͬ����д��Ĵ��벻ͬ�����ļ��ϴ�
         * �ģ���д���ļ���������һ���ύ���ݣ���ֻ�浽�������ڴ棩�У�
         */
        if("" != filename)
        {
            /* ���ļ������ݴ浽�û���ʱĿ¼
             */
            const string &username = Session::Get(this)->GetUser();
            User *user = User::Get(username);
            const string &fullpath = user->TmpDir() + "." + GetRandomString(); /* ������ʱ���в�����ԭ
                                                                                * �ļ������Է���ͬϵͳ
                                                                                * �к������롣[XXX]
                                                                                */
            FileObj file;

            // �ļ��ֶ� <==> ����
            m_FieldAry[element] = filename;

            LOG_DEBUG("fullpath=[%s]", fullpath.c_str());
            if( ! file.Open(fullpath, FileObj::W) )
            {
                LOG_ERROR("���ܴ��ļ���[%s]", fullpath.c_str());
                return; // ֱ�ӷ���
            }

            while( ((next.len=GetLine(next.buf, BUF_MAX_LEN)) > 0)
                   && (strncmp(next.buf, m_boundary.c_str(), m_boundary.length()) != 0)
                 )
            {
                // LOG_DEBUG("[%d]", prev.len);
                file.Write(prev.buf, prev.len);
                prev.buf[0] = '\0';

                // ǰ��ָ�򽻻�
                tmp = prev;
                prev = next;
                next = tmp;
            }
            int n = CUT_CR(prev.buf, prev.len); // ע�⣬��ͬ��ϵͳ������׷�ӡ�\r����\r\n���ȣ�
            // LOG_DEBUG("[%d]", prev.len - n);
            file.Write(prev.buf, prev.len - n);
            file.Close();

            // ����m_FileObjAry
            FileObj *obj = new FileObj(fullpath);
            if(NULL == obj)
            {
                LOG_ERROR("�����ļ������ڴ����: [%s]", fullpath.c_str());
                return;
            }
            m_FileObjAry[element] = obj;
            LOG_DEBUG("���룺 m_FileObjAry[\"%s\"]=[%p]", element.c_str(), obj);
        }
        else
        {
            /* ��һ����ύ����
             */
            string &field = m_FieldAry[element]; //
            while( ((next.len=GetLine(next.buf, BUF_MAX_LEN)) > 0) && (strncmp(next.buf, m_boundary.c_str(), m_boundary.length()) != 0) )
            {
                // LOG_DEBUG("[%s]", prev.buf);
                field =+ prev.buf;
                prev.buf[0] = '\0';

                // ǰ��ָ�򽻻�
                tmp = prev;
                prev = next;
                next = tmp;
            }
            CUT_CR(prev.buf, prev.len);
            field =+ prev.buf;
            // LOG_DEBUG("[%s]", prev.buf);
        }


        // �Ƿ񵽽������
        if( ('\0' == next.buf[0]) || (strncmp(next.buf, m_boundary.c_str(), m_boundary.length()) != 0) )
        {
            break;
        }

    }// end of while(1)...
}// end of void Request::ParseAttach()...

// ��������
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
    FUNCTION_TRACK(); // �����켣����

    static char EMPYT[2] = ""; // ��ֹ������
    char buf[BUF_MAX_LEN + 1] = "";
    char *begin;
    char *end;
    char *tmp;
    char *param = NULL; // url�����еĲ���
    int ret;


    // ȡ��һ��
    ret = GetLine(buf, BUF_MAX_LEN);
    CUT_CR(buf, ret);
    LOG_DEBUG("buf=[%s]", buf);

    /*
     * �Ӻ�������ȡurl�����еĲ�������?���󲿷־��ǲ�����
     *      GET /login.cgi?window.screen.width=&window.screen.height=&username=abcdef&login=GET HTTP/1.1
     */
    begin = strchr(buf, '?');
    if(NULL != begin)
    {
        end = strchr(begin, ' '); // �����ո�Ϊ����
        if(NULL != end)
        {
            *end = '\0';
            *begin = '\0'; // �����?�������У�
            param = begin + 1;
        }
    }
    //LOG_DEBUG("param=[%s]", param);

    /*
     * ȡ�������ͣ�
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
    toUpper(begin); // תΪ��д
    m_type = begin;
    LOG_DEBUG("type=[%s]", m_type.c_str());

    /*
     * ȡҳ����
     *      GET /cgi-bin/login.cgi?user=rocky HTTP/1.1\n\
     *      GET /cgi-bin/login.cgi HTTP/1.1\n\
     */
    begin = end + 1;    // �������ͺ����ҳ����
    IGNORE_BLANK(begin);
    tmp = begin;
    /* �������һ����/��
     *   ���ǣ�1. �ո�
     *         2. �س����з���\r��\n��
     *         3. ���Ǵ�β
     */
    while(' ' != *tmp && !IS_CR(*tmp) && '\0' != *tmp)
    {
        if('/' == *tmp)
        {
            begin = tmp + 1; // ��¼���һ����/��
        }
        tmp++;
    }
    // �ҽ�βλ��
    if( (end = strchr(begin, '?')) != NULL
        || (end = strchr(begin, ' ')) != NULL
      )
    {
        *end = '\0';
    }
    // toLower(begin); // תΪСд
    m_page =  begin;
    LOG_DEBUG("page=[%s]", m_page.c_str());

    // ȡ��׺
    m_PageType = GetFileType(m_page.c_str());
    LOG_DEBUG("m_PageType=[%s]", m_PageType.c_str());

    /*
     * ��������ͷ����Ϣ
     */
    ParseHead();

    /*
     * ����Cookie�ֶ�
     */
    ParseCookie();


    /*
     * ������ִ����֤���Ա��������÷�����
     * ����¼ҳ��������֤��
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
     * �����ͷ��������
     */

    // ����GET����ע�⣬��POST��Ҳ���ܰ���GET�Ĵ���
    if(NULL != param)
    {
        ParseGet(param);
    }

    if("" != m_boundary)
    {
        // �������ύ
        ParseAttach();
    }
    else if("POST" == m_type)
    {
        // ����һ���POST����
        ParsePost();
    }

    //LOG_DEBUG("m_HeadFieldAry.size=[%d]", m_HeadFieldAry.size());
    //LOG_DEBUG("m_FieldAry.size=[%d]", m_FieldAry.size());
    //LOG_DEBUG("----------------------------");
    //LOG_DEBUG("m_HeadFieldAry=[%s]", map2string(m_HeadFieldAry).c_str());
    //LOG_DEBUG("m_FieldAry=[%s]", map2string(m_FieldAry).c_str());

}// end of void Request::Parse()...









}// end of REQUEST_SPACE






