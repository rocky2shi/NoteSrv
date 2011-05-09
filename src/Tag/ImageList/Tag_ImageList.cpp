// Rocky 2010-05-09 23:37:20
#include "Page.h"
#include "User.h"
#include "GetFileList.h"
#include "Tag_ImageList.h"
#include "ChineseCoding.h"
namespace TAG_IMAGELIST_SPACE
{








// 标明模块
static const string THIS_MODULE = "ImageList";





Tag_ImageList::Tag_ImageList()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_ImageList::Tag_ImageList(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_ImageList::~Tag_ImageList()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Tag_ImageList::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Tag::DoInit();
}

// 子类对象创建器
Tag *Tag_ImageList::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Tag_ImageList;
}








/******************************** 业务代码 ********************************/





// 取列表 [2010-05]
string Tag_ImageList::Get(Page *page)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    // 取当前用户
    const string &username = page->GetRequest()->GetCurrentUser();
    User *user = User::Get(username);
    // 当前数据条目
    const string &key = page->GetCurrentKey();
    // 图片存于附件目录
    const string &path = user->AttachDir();
    // 打开目录，取文件列表；
    GetFileList image( path );
    GetFileList::file_iterator it( image );
    string html = "";

    int nImageCount = 0;
    while( it.next() )
    {
        const char *basename = GetBaseName( it.name() );

        // 跳过不属于本条目的文件
        if(strncmp(key.c_str(), basename, key.length()) != 0)
        {
            //LOG_DEBUG("Ignore: key=[%s], basename=[%s]", key.c_str(), basename);
            continue;
        }

        // 跳过前缀(key + '.')
        basename += key.length() + 1;

        // 文件名转为url编码（类似代码还位于Tag_AttachList.cpp中 [XXX]）
        string sFileOrg = FilenameDecode(basename); // 文件名，解码为可显示形式；
        const string sFileUrl = UrlCode(basename);  // 文件名，url格式；
        ChineseCoding::GB18030ToUTF8(sFileOrg);      /* 早期代码中，存放的文件名
                                                     * 是gb2312格式，需转换；
                                                     */

        const char *type = GetFileType( sFileOrg.c_str() );
        if( ! isImage(type) )
        {
            //LOG_DEBUG("Ignore: basename=[%s] type=[%s]", basename, type);
            continue;
        }

        html += ""
                "<li>"
                "  <input name='image' type='radio' id='image_" + IntToString(nImageCount) + "' "
                "     OnClick=\"PreviewImg('" + sFileUrl + "', '" + sFileOrg + "');\">"
                "  <label for='image_" + IntToString(nImageCount) + "'>" + sFileOrg + "</label>"
                "</li>"
                "\n";
        nImageCount++;
    }

    if(0 == nImageCount)
    {
        html =  ""
                "<h1>无备选图片</h1><br>"
                "（可先在编辑页面里上传图片后，再刷新本页面；）"
                "\n";
    }

    LOG_DEBUG("nImageCount=[%d]", nImageCount);

    // 设置是否显标志
    page->SetResult("ImageListCount", IntToString(nImageCount));

    return html;
}

















// 设置为全局标记；
static Tag_ImageList tmp("ImageList", THIS_MODULE);

}// end of TAG_IMAGELIST_SPACE

