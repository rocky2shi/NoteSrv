// Rocky 2010-05-10 19:31:08
#include "Page.h"
#include "UserData.h"
#include "Tag_AttachList.h"
namespace TAG_ATTACHLIST_SPACE
{






// 标明模块
static const string THIS_MODULE = "AttachList";





Tag_AttachList::Tag_AttachList()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_AttachList::Tag_AttachList(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_AttachList::~Tag_AttachList()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Tag_AttachList::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Tag::DoInit();
}

// 子类对象创建器
Tag *Tag_AttachList::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Tag_AttachList;
}








/******************************** 业务代码 ********************************/





// 取列表 [Rocky 2010-05-20]
string Tag_AttachList::Get(Page *page)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    const Conf *pack = page->GetCurrentPack();  // 当前key对应数据集
    const string &password = pack->Get("password");
    const string &decryption = page->GetResult("decryption");

    // 若当前用户加密了数据，则不显示附件列表；
    if( "" != password && "YES" != decryption )
    {
        LOG_DEBUG("User data have been encrypted.");
        return "";
    }

    // 取当前用户
    const string &username = page->GetRequest()->GetCurrentUser();
    // 当前数据条目
    const string &key = page->GetCurrentKey();
    const string &pagename = page->GetRequest()->GetPageName();
    vector<string> attachs;
    vector<string>::iterator it;
    UserData(username).GetAttachList(key, attachs);   // 取附件列表
    string html = "";

    for(it = attachs.begin(); attachs.end() != it; it++)
    {
        const char *basename = GetBaseName( it->c_str() );

        // 跳过不属于本条目的文件
        if(strncmp(key.c_str(), basename, key.length()) != 0)
        {
            //LOG_DEBUG("Ignore: key=[%s], basename=[%s]", key.c_str(), basename);
            continue;
        }

        // 取文件大小
        FileObj file( *it );
        const string &sFileSize = ByteSizeFormat( file.Size() );

        // 跳过前缀(key + '.')
        basename += key.length() + 1;

        // 文件名转为url编码
        const string sFileOrg = FilenameDecode(basename);  // 文件名，解码为可显示形式；
        const string sFileUrl = UrlCode(basename);         // 文件名，url格式；

        /*
         * 不同的页面输出不同的处理形式
         */
        if("reading" == pagename)
        {
            // reading页面（顶上的附件列表）
            // <a href='download?key=20100521163715&file=logo%2Egif%2Epng' title='2.29KB' target='tmp_iframe'>logo.gif.png </a>
            html += " <a href='download?key=" + key + "&file=" + sFileUrl + "' title='" + sFileSize + "' target='tmp_iframe'>" + sFileOrg + " </a>\n";
        }
        else
        {
            // 其它（附件）页面的输出
            html += "  <li>\n"
                    "    <a href='download?key=" + key + "&file=" + sFileUrl + "' title='" + sFileSize + "' target='tmp_iframe'>" + sFileOrg + " </a>\n"
                    "    <a href='#' title='删除文件' OnClick=\"Delete(this, '" + key + "', '" + sFileUrl + "', '" + sFileOrg + "')\">×</a>\n"
                    "  </li>\n"
                    "\n";
        }
    }// end of for(it = attachs.begin()...

    return html;
}













// 设置为全局标记；
static Tag_AttachList tmp(TAG_GLOBAL, THIS_MODULE);

}// end of TAG_ATTACHLIST_SPACE
