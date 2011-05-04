// Rocky 2010-05-02 19:09:23
#include "UserData.h"
#include "User.h"
#include "Page.h"
#include "Tag_BrowseList.h"
namespace TAG_BROWSELIST_SPACE
{







// 标明模块
static const string THIS_MODULE = "BrowseList";





Tag_BrowseList::Tag_BrowseList()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_BrowseList::Tag_BrowseList(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_BrowseList::~Tag_BrowseList()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Tag_BrowseList::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Tag::DoInit();
}

// 子类对象创建器
Tag *Tag_BrowseList::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Tag_BrowseList;
}








/******************************** 业务代码 ********************************/

// 返回大于0的值，表该数据要过滤掉；
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
     * 过滤时间
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
     * 已被删除的数据
     */
    if("delete" == pack->Get("status"))
    {
        LOG_DEBUG("Filter [%s] ...", key.c_str());
        return 11;
    }

    /*
     * 关键字过滤（关键字不能为空）
     */
    // a. 选中的是标题，测试是否含有关键字
    if("title" == query_scope)
    {
        const string &title = pack->Get("title");
        // 找不到关键字时，该数据会被过滤；
        if(title.find(query_keyword) == string::npos)
        {
            LOG_DEBUG("Filter [%s] ...", key.c_str());
            return 3;
        }
    }
    // b. 选中的是内容，测试是否含有关键字
    if("content" == query_scope)
    {
        // 取对应匹配机
        MultiStringMatch *match = matchs["query_keyword"];
        const string &text = pack->Get("text");
        // 查看text中是否存的所要的关键字，不存的返回false，表示要过滤此数据；
        if( NULL != match && !match->MatchOneKey( text ) )
        {
            LOG_DEBUG("Filter [%s] ...", key.c_str());
            return 4;
        }
    }

    /*
     * 归类过滤（须满足所有已选条件，即与的关系）   [XXX]
     */
    if("" != query_type)
    {
        // 取对应匹配机
        MultiStringMatch *match = matchs["query_type"];
        if(NULL != match)
        {
            const string &msg_type = pack->Get("msg_type");
            // 不是所有关键字时都被命中时，MatchAllKey()返回false，表示要过滤此数据；
            if( !match->MatchAllKey( msg_type ) )
            {
                LOG_DEBUG("Filter [%s] ...", key.c_str());
                return 5;
            }
        }
    }


    // 无需过滤，则返回小于0值；
    return -1;
}


// 浏览数据（表列）
string Tag_BrowseList::Get(Page *page)
{
    FUNCTION_TRACK(); // 函数轨迹跟综


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
     * 创建过滤匹配机
     */
    map<string, MultiStringMatch *> matchs;
    // 类型（归类）过滤机
    const string &query_type = cfg->Get("filter", "query_type");
    if("" != query_type)
    {
        vector<string> query_type_set;
        Split(query_type, ",", query_type_set);
        LOG_DEBUG("query_type_set.size=[%d]", query_type_set.size());
        matchs["query_type"] = new MultiStringMatch( query_type_set );
    }
    LOG_DEBUG("query_type=[%s]", query_type.c_str());
    // 正文内容过滤机
    const string &query_keyword = cfg->Get("filter", "query_keyword");
    if("" != query_keyword)
    {
        vector<string> query_keyword_set;
        query_keyword_set.push_back(query_keyword);
        matchs["query_keyword"] = new MultiStringMatch( query_keyword_set );
    }
    LOG_DEBUG("query_keyword=[%s]", query_keyword.c_str());

    // 自动释放过滤匹配机内存
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
    } AutoMatchsFree( matchs ); // 自动释放执行对象


    // 取当前客户端用户所用的屏幕宽度，转换为每行应显示的字节数；
    const string &width = page->GetRequest()->GetCookie("window.screen.width");
    const string &byte = EmptyStringToDefault(cfg->Get("screen.width.to.byte", width), "90");
    const int LINE_LENl = atoi(byte.c_str());

    LOG_DEBUG("width=[%s] LINE_LENl=[%d]", width.c_str(), LINE_LENl);

    const string &datapath = user->DataDir();
    Connect * const connect = page->GetRequest()->GetConnect();

    /*
     * 替换html特殊字符。注意，这里的技巧，因为对这里的字
     * 符替换只和内容本身相关，所以设置为静态的，避免多次
     * 重复的初始化；
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

    // 列出所有数据
    for(i=0; it.next(); i++ )
    {
        int ret;
        string key;
        Conf *pack = NULL;

        // 取出当前所指的key和Pack
        ret = it.Get(key, pack);
        if(ret < 0)
        {
            continue;
        }

        // 过滤处理（决定当前数据是否需要显示到客户端）
        ret = Filter(key, pack, page, matchs);
        if(ret > 0)
        {
            /*
             * 注意，这里返回值为1时，表示当前这条数据以后的数
             * 据时间都比设定的早，直接退出，不需是读取往后的文
             * 件。因为取出的文件是接日期顺序排序的。
             */
            if(1 == ret)
            {
                break;
            }
            continue;
        }

        char buf[1024] = "";
        string text;
        const string &txt = pack->Get("text");    // 可优化为取限定长度串 [XXX]
        const string &title = TextToWeb( pack->Get("title") );
        string modify = pack->Get("modify");
        string sTextLen;


        // 标题不能为空
        if("" == title)
        {
            continue;
        }

        if( pack->Get("password") != "" )
        {
            text = "<i>[内容已加密]</i>";
        }
        else if("" == txt)
        {
            // 旧数据
            if("日记" == title)
            {
                continue;
            }
            text = "<i>[空]</i>";
        }
        else
        {
            sTextLen = ByteSizeFormat( txt.length() );
            int len = txt.length() + title.length();
            if(len > LINE_LENl)
            {
                // 截短到指定长度
                len = LINE_LENl - title.length();
                text = txt.substr(0, len) + "...";
            }
            else
            {
                text = txt;
            }
            // 转为网页可显示格式
            text = TextToWeb(text);
            // 处理text中的：'<' '>' '\n'
            text = DelHtml.Replace(text);
        }

        // 转换为易读格式
        modify = TimeTo( KeyToSecond(modify) );

        snprintf(buf, sizeof(buf)-1,
                "    <li id='%s'>\n"
                "    <a name='%s' />\n"
                "    <a href='reading?key=%s&index=%d' class=time title='点击阅读[%s] 最后修改[%s]'>%s</a>\n"
                "    <span class=title>[<a href='javascript:EditMsg(%d);' title='修改'>%s</a>]</span>\n"
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

