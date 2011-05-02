// Rocky 2010-05-02 19:09:23
#include "UserData.h"
#include "User.h"
#include "Page.h"
#include "Tag_BrowseList.h"
namespace TAG_BROWSELIST_SPACE
{







// ����ģ��
static const string THIS_MODULE = "BrowseList";





Tag_BrowseList::Tag_BrowseList()
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_BrowseList::Tag_BrowseList(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_BrowseList::~Tag_BrowseList()
{
    FUNCTION_TRACK(); // �����켣����
}




int Tag_BrowseList::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Tag::DoInit();
}

// ������󴴽���
Tag *Tag_BrowseList::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Tag_BrowseList;
}








/******************************** ҵ����� ********************************/

// ���ش���0��ֵ���������Ҫ���˵���
int Tag_BrowseList::Filter(const string &key, const Conf *pack, Page *page, map<string, MultiStringMatch*> &matchs)
{
    const string &filter = "filter";
    PageCfg * const cfg = page->GetCurrentPageCfg();
    const string &timebeginchk  = cfg->Get(filter, "timebeginchk");
    const string &timeendchk    = cfg->Get(filter, "timeendchk");
    const string &qtbegin       = cfg->Get(filter, "qtbegin");
    const string &qtend         = cfg->Get(filter, "qtend");
    const string &query_keyword = cfg->Get(filter, "query_keyword");
    const string &query_scope   = cfg->Get(filter, "query_scope");
    const string &query_type    = cfg->Get(filter, "query_type");
    const string &result_win    = cfg->Get(filter, "result_win");

    /*
     * ����ʱ��
     */
    if("1" == timebeginchk && key < qtbegin)
    {
        LOG_DEBUG("Filter [%s] ...", key.c_str());
        return 1;
    }
    if("1" == timeendchk && key > qtend)
    {
        LOG_DEBUG("Filter [%s] ...", key.c_str());
        return 2;
    }

    /*
     * �ѱ�ɾ��������
     */
    if("delete" == pack->Get("status"))
    {
        LOG_DEBUG("Filter [%s] ...", key.c_str());
        return 11;
    }

    /*
     * �ؼ��ֹ��ˣ��ؼ��ֲ���Ϊ�գ�
     */
    // a. ѡ�е��Ǳ��⣬�����Ƿ��йؼ���
    if("title" == query_scope)
    {
        const string &title = pack->Get("title");
        // �Ҳ����ؼ���ʱ�������ݻᱻ���ˣ�
        if(title.find(query_keyword) == string::npos)
        {
            LOG_DEBUG("Filter [%s] ...", key.c_str());
            return 3;
        }
    }
    // b. ѡ�е������ݣ������Ƿ��йؼ���
    if("content" == query_scope)
    {
        // ȡ��Ӧƥ���
        MultiStringMatch *match = matchs["query_keyword"];
        const string &text = pack->Get("text");
        // �鿴text���Ƿ�����Ҫ�Ĺؼ��֣�����ķ���false����ʾҪ���˴����ݣ�
        if( NULL != match && !match->MatchOneKey( text ) )
        {
            LOG_DEBUG("Filter [%s] ...", key.c_str());
            return 4;
        }
    }

    /*
     * ������ˣ�������������ѡ����������Ĺ�ϵ��   [XXX]
     */
    if("" != query_type)
    {
        // ȡ��Ӧƥ���
        MultiStringMatch *match = matchs["query_type"];
        if(NULL != match)
        {
            const string &msg_type = pack->Get("msg_type");
            // �������йؼ���ʱ��������ʱ��MatchAllKey()����false����ʾҪ���˴����ݣ�
            if( !match->MatchAllKey( msg_type ) )
            {
                LOG_DEBUG("Filter [%s] ...", key.c_str());
                return 5;
            }
        }
    }


    // ������ˣ��򷵻�С��0ֵ��
    return -1;
}


// ������ݣ����У�
string Tag_BrowseList::Get(Page *page)
{
    FUNCTION_TRACK(); // �����켣����


    string html;
    int i;
    const string username = page->GetRequest()->GetCurrentUser();
    PageCfg * const cfg = page->GetCurrentPageCfg();
    User *user = User::Get( username );

    if( ! user->isValid() )
    {
        LOG_ERROR("invalid user: [%s]", username.c_str());
        return "";
    }

    /*
     * ��������ƥ���
     */
    map<string, MultiStringMatch *> matchs;
    // ���ͣ����ࣩ���˻�
    const string &query_type = cfg->Get("filter", "query_type");
    if("" != query_type)
    {
        vector<string> query_type_set;
        Split(query_type, ",", query_type_set);
        LOG_DEBUG("query_type_set.size=[%d]", query_type_set.size());
        matchs["query_type"] = new MultiStringMatch( query_type_set );
    }
    LOG_DEBUG("query_type=[%s]", query_type.c_str());
    // �������ݹ��˻�
    const string &query_keyword = cfg->Get("filter", "query_keyword");
    if("" != query_keyword)
    {
        vector<string> query_keyword_set;
        query_keyword_set.push_back(query_keyword);
        matchs["query_keyword"] = new MultiStringMatch( query_keyword_set );
    }
    LOG_DEBUG("query_keyword=[%s]", query_keyword.c_str());

    // �Զ��ͷŹ���ƥ����ڴ�
    struct AutoMatchsFree
    {
        AutoMatchsFree(map<string, MultiStringMatch *> &matchs): m_matchs(matchs)
        {
        }
        ~AutoMatchsFree()
        {
            map<string, MultiStringMatch *>::iterator it;
            for(it = m_matchs.begin(); m_matchs.end() != it; it++)
            {
                LOG_DEBUG("delete matchs: [%s]", it->first.c_str());
                delete it->second;
            }
        }
    private:
        map<string, MultiStringMatch *> &m_matchs;
    } AutoMatchsFree( matchs ); // �Զ��ͷ�ִ�ж���


    // ȡ��ǰ�ͻ����û����õ���Ļ��ȣ�ת��Ϊÿ��Ӧ��ʾ���ֽ�����
    const string &width = page->GetRequest()->GetCookie("window.screen.width");
    const string &byte = EmptyStringToDefault(cfg->Get("screen.width.to.byte", width), "90");
    const int LINE_LENl = atoi(byte.c_str());

    LOG_DEBUG("width=[%s] LINE_LENl=[%d]", width.c_str(), LINE_LENl);

    const string &datapath = user->DataDir();
    Connect * const connect = page->GetRequest()->GetConnect();

    /*
     * �滻html�����ַ���ע�⣬����ļ��ɣ���Ϊ���������
     * ���滻ֻ�����ݱ�����أ����������վ�̬�ģ�������
     * �ظ��ĳ�ʼ����
     */
    struct InitDelHtmlMatch
    {
        static MultiStringMatch & doit()
        {
            vector<string> from;
            vector<string> to;
            from.push_back("<");
            from.push_back(">");
            from.push_back("\n");
            to.push_back("[");
            to.push_back("]");
            to.push_back(" ");

            static MultiStringMatch DelHtml(from ,to);
            return DelHtml;
        }
    };
    static MultiStringMatch &DelHtml = InitDelHtmlMatch::doit();

    UserData::iterator it( datapath );

    // �г���������
    for(i=0; it.next(); i++ )
    {
        int ret;
        string key;
        Conf *pack = NULL;

        // ȡ����ǰ��ָ��key��Pack
        ret = it.Get(key, pack);
        if(ret < 0)
        {
            continue;
        }

        // ���˴���������ǰ�����Ƿ���Ҫ��ʾ���ͻ��ˣ�
        ret = Filter(key, pack, page, matchs);
        if(ret > 0)
        {
            /*
             * ע�⣬���ﷵ��ֵΪ1ʱ����ʾ������������ʱ��
             * �����趨���磬ֱ���˳��������Ƕ�ȡ�������
             * ������Ϊȡ�����ļ��ǽ�����˳������ġ�
             */
            if(1 == ret)
            {
                break;
            }
            continue;
        }

        char buf[1024] = "";
        string text;
        const string &txt = pack->Get("text");    // ���Ż�Ϊȡ�޶����ȴ� [XXX]
        const string &title = TextToWeb( pack->Get("title") );
        string modify = pack->Get("modify");
        string sTextLen;


        // ���ⲻ��Ϊ��
        if("" == title)
        {
            continue;
        }

        if( pack->Get("password") != "" )
        {
            text = "<i>[�����Ѽ���]</i>";
        }
        else if("" == txt)
        {
            // ������
            if("�ռ�" == title)
            {
                continue;
            }
            text = "<i>[��]</i>";
        }
        else
        {
            sTextLen = ByteSizeFormat( txt.length() );
            int len = txt.length() + title.length();
            if(len > LINE_LENl)
            {
                // �ض̵�ָ������
                len = LINE_LENl - title.length();
                text = txt.substr(0, len) + "...";
            }
            else
            {
                text = txt;
            }
            // תΪ��ҳ����ʾ��ʽ
            text = TextToWeb(text);
            // ����text�еģ�'<' '>' '\n'
            text = DelHtml.Replace(text);
        }

        // ת��Ϊ�׶���ʽ
        modify = TimeTo( KeyToSecond(modify) );

        snprintf(buf, sizeof(buf)-1,
                "    <li id='%s'>\n"
                "    <a name='%s' />\n"
                "    <a href='reading?key=%s&index=%d' class=time title='����Ķ�[%s] ����޸�[%s]'>%s</a>\n"
                "    <span class=title>[<a href='javascript:EditMsg(%d);' title='�޸�'>%s</a>]</span>\n"
                "    <span class=text>%s</span></li> \n"
                "\n",
                        key.c_str(),
                        key.c_str(),
                        key.c_str(), i, sTextLen.c_str(), modify.c_str(), key.c_str(),
                        i, title.c_str(),
                        text.c_str()
        );

        ret = connect->Send(buf);
        if(ret <= 0)
        {
            LOG_ERROR("send error, not complete: [%d/%d]", ret, strlen(buf));
            break;
        }
    }

    return "";
}
















static Tag_BrowseList tmp("browse", THIS_MODULE);

}// end of TAG_BROWSELIST_SPACE

