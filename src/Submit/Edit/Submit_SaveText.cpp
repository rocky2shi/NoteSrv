// Rocky 2010-05-15 16:34:16
#include "Page.h"
#include "UserData.h"
#include "Encrypt.h"
#include "Menu.h"
#include "MultiStringMatch.h"
#include "Submit_SaveText.h"
namespace SUBMIT_SAVETEXT_SPACE
{




// 标明模块
static const string THIS_MODULE = "SaveText";





Submit_SaveText::Submit_SaveText()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Submit_SaveText::Submit_SaveText(const string &page, const string &element)
                        : Submit(page, element)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Submit_SaveText::~Submit_SaveText()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Submit_SaveText::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Submit::DoInit();
}

// 子类对象创建器
Submit *Submit_SaveText::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Submit_SaveText;
}





/******************************** 业务代码 ********************************/


// 保存正文
int Submit_SaveText::Save(Page *page)
{
    /*
     * 取出数据
     */
    Request * const &request = page->GetRequest();
    const string &key = page->GetCurrentKey();
    const string &title = WebEditToText( request->GetField("title") );

    if("" == title)
    {
        LOG_ERROR("title can't empty, key=[%s]", key.c_str());
        return ERR;
    }

    const Conf *pack = page->GetCurrentPack();      // 当前key对应数据集
    const string &text = WebEditToText( request->GetField("text") );  // 注意，只处理回车换行符；
    const string &modify = NowTime("%Y%m%d%H%M%S"); // 修改时间为当前时间
    const string &msg_type = request->GetField("msg_type");
    const string &password = request->GetField("password");

    /*
     * 数据打包
     */
    Ini data;
    // 加密部分
    if("" != password)
    {
        data.Set(key, "text", Encrypt(password).encrypt(text));
    }
    else
    {
        data.Set(key, "text", text);
    }
    data.Set(key, "title", title);
    data.Set(key, "modify", modify);
    data.Set(key, "msg_type", msg_type);

    // 写入
    page->Save( data );

    // 调整类别显示顺序
    AdjustType(page, msg_type);

    return OK;
}

// 调整类别，使常用的靠前；types:当前选中的类别
int Submit_SaveText::AdjustType(Page *page, const string &types)
{
    const string &username = page->GetRequest()->GetCurrentUser();
    Menu *menu = Menu::Get(username, "TypeDropBox.cfg");    // [XXX:56425820]
    vector<string> items;
    vector<string>::iterator it;
    Split(types, ",", items); // 分割：20081004213824,20081004213826,20081004213827,20081004214214
    for(it = items.begin(); items.end() != it; it++)
    {
        const string &key = *it;

        // 更新排序所用的index字段为当前的最大值（秒数是一顺序值），使之靠前；
        menu->SetItem(key, "index", NowTime(""));

        // 增加引用计数
        const string &hotspot = menu->GetItem(key, "hotspot");
        menu->SetItem(key, "hotspot", Inc(hotspot));
    }

    // 保存
    return menu->Save();
}

// 认证通过，返回的["decryption"]为空串，不通过则返回出错信息；
int Submit_SaveText::Deal(Page *page)
{
    int ret = Save(page);
    const string &key = page->GetCurrentKey();
    const string &operate = page->GetRequest()->GetField("operate");
    string err;
    string html;

    err = (OK == ret) ? "提交成功！[" + key + "]"
                      : "保存失败！可能您需要重新登录。";

    // 保存成功，且是新建时，打开新页面；
    if(OK == ret && "new" == operate)
    {
        // 保存，并刷新页面（新建）
        html = "<script>\n"
               "if(parent)\n"
               "{\n"
               "    parent.document.location.href = 'edit';\n"
               "}\n"
               "</script>\n"
               "\n";
    }
    else
    // 显示当前信息
    {
        // 保存，留在当前页面（不刷新）
        html = "<script>\n"
               "var tmp = parent.document.getElementById('error');\n"
               "if(null != tmp)\n"
               "{\n"
               "    tmp.innerHTML = '" + err + "';\n"
               "    parent.ClearStatus();\n"
               "}\n"
               "</script>\n"
               "\n";
    }

    LOG_INFO("%s", err.c_str());

    // 发送页面代码
    page->GetRequest()->GetConnect()->Send(html);
    return OK;
}













// 设置为全局标记；
static Submit_SaveText tmp("edit", THIS_MODULE);

}// end of SUBMIT_SAVETEXT_SPACE

