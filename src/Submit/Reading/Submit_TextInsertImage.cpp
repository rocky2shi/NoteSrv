// Rocky 2010-05-14 18:59:27
#include "UserData.h"
#include "Page.h"
#include "Encrypt.h"
#include "ChineseCoding.h"
#include "Submit_TextInsertImage.h"
namespace SUBMIT_TEXTINSERTIMAGE_SPACE
{




// 标明模块（对应于页面中的提交字段）
static const string THIS_MODULE = "TextInsertImage";





Submit_TextInsertImage::Submit_TextInsertImage()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Submit_TextInsertImage::Submit_TextInsertImage(const string &page, const string &element)
                        : Submit(page, element)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Submit_TextInsertImage::~Submit_TextInsertImage()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Submit_TextInsertImage::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Submit::DoInit();
}

// 子类对象创建器
Submit *Submit_TextInsertImage::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Submit_TextInsertImage;
}





/******************************** 业务代码 ********************************/




/*
 * 处理在reading页面提交的事件，目前处理：
 *   1. 插入图片（operate=InsertObj，type=""）（注：只修改正文字段）
 *   2. 插入书签（operate=InsertObj，type=bookmark）（注：修改正文、及书签字段）
 */
int Submit_TextInsertImage::Deal(Page *page)
{
    const string &key = page->GetCurrentKey();
    const string &password = page->GetRequest()->GetField("password");
    const Conf *pack = page->GetCurrentPack();          // 当前key对应数据集

    /*
     * 先核对密码
     */
    if( "" != pack->Get("password")
        && Crypt(password, CRYPT_VERSION) != pack->Get("password")
      )
    {
        LOG_DEBUG("password=[%s]", password.c_str());
        LOG_ERROR("Password error, text is't saved.");
        return ERR;
    }

    /*
     * 在正文中插入图片操作，为提高性能，先尝试在插入点处修改，为防止
     * 重复，所以需要多次（扩大匹配串）查找，再存在重复时，则执行全文
     * 保存，因此和客户端之间会有多次交互；
     */

    // 取出数据
    string text = page->GetRequest()->GetField("text");
    string find = page->GetRequest()->GetField("find");        // 替换前的串
    string replace = page->GetRequest()->GetField("replace");  // 替换后的串

    // 编码转换：utf8 => gbk
    // ChineseCoding code("utf-8", "gb18030"); delete -- Rocky 2011-05-06 17:06:52 <<<<<<<<<<<<<<<<
    int ret;

    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    // if( code.Converter(text, text) < 0
    //     || code.Converter(find, find) < 0
    //     || code.Converter(replace, replace) < 0
    //   )
    // {
    //     LOG_ERROR("Converter error");
    //     return ERR;
    // }

    // text空，说明客户端发来的是部分串，即等修改部分，则执行增量保存；
    if("" == text)
    {
        // 读取正文数据（读的是后台数据），加密的则先解密；
        text = pack->Get("text");
        if("" != password)
        {
            LOG_DEBUG("Decrypt...");
            text = Encrypt(password).decrypt(text);
        }

        // 转换成web显示格式，以便匹配；
        text = TextToWeb( text );

        /*
         * 在正文中查找，如两次都找到，说明有重复的"$find"，则出错；
         */
        const unsigned int pos = text.find(find);
        if(pos == text.npos)
        {
            LOG_DEBUG("Can't find: [%s]", find.c_str());
            return ERR;
        }
        // 再次查找
        const unsigned int pos2 = text.find(find, pos + find.length());
        if(pos2 != text.npos)
        {
            LOG_DEBUG("String repeat: find=[%s], pos=[%u], pos2=[%u]",
                            find.c_str(), pos, pos2);
            return ERR;
        }

        /*
         * 数据正常，用replace替换find；
         *  （重新组装三段）
         */
        const string &before = text.substr(0, pos);             // 原文中，替换串之前部分
        const string &after = text.substr(pos + find.length()); // 原文中，替换串之后部分

        text = before + replace + after;
    }
    else
    {
        LOG_DEBUG("whole text save...");
    }

    text = WebHtmlToText( text );   // [XXX]

    // 原是加密数据，则执行加密；
    if("" != password)
    {
        text = Encrypt(password).encrypt( text );
    }

    /*
     * 数据打包
     */
    Ini data;
    data.Set(key, "text", text);                      // 正文
    data.Set(key, "modify", NowTime("%Y%m%d%H%M%S")); // 最后修改时间

    /*
     * 可处理的操作
     */
    const string &type = page->GetRequest()->GetField("type");
    // 插入书签
    if("bookmark" == type)
    {
        string bookmark = page->GetRequest()->GetField("bookmark_data");
        // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        // if(code.Converter(bookmark, bookmark) < 0)
        // {
        //     LOG_ERROR("Converter error");
        //     return ERR;
        // }
        data.Set(key, "bookmark", bookmark);    // 书签数据
    }

    // 保存
    return page->Save(data);
}














static Submit_TextInsertImage tmp("reading", THIS_MODULE);

}// end of SUBMIT_TEXTINSERTIMAGE_SPACE

