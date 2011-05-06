// Rocky 2010-05-06 19:23:20
#include "Page.h"
#include "Menu.h"
#include "Tag_TypeDropBoxMenu.h"
namespace TAG_TYPEDROPBOXMENU_SPACE
{






// 标明模块
static const string THIS_MODULE = "TypeDropBoxMenu";





Tag_TypeDropBoxMenu::Tag_TypeDropBoxMenu()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_TypeDropBoxMenu::Tag_TypeDropBoxMenu(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_TypeDropBoxMenu::~Tag_TypeDropBoxMenu()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Tag_TypeDropBoxMenu::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Tag::DoInit();
}

// 子类对象创建器
Tag *Tag_TypeDropBoxMenu::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Tag_TypeDropBoxMenu;
}








/******************************** 业务代码 ********************************/





string Tag_TypeDropBoxMenu::Get(Page *page)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    const string &username = page->GetRequest()->GetCurrentUser();
    Menu *menu = Menu::Get(username, "TypeDropBox.cfg");    // [XXX:56425820]
    const string &sSelectType = GetSelectedItems(page);     // 当前被选中的下接菜单

    LOG_DEBUG("sSelectType=[%s]", sSelectType.c_str());

    Menu::iterator it(menu);
    string html = "";
    int i;

    /* 列出类型下接菜单项
     *（注：列出的菜单项目前是按index字段排序的，见Menu类中）
     */
    for(i=1; it.next(); i++)
    {
        const string &key = it->key;
        if( key == "" || it->Get("status") != "enable" )
        {
            continue;
        }
        const string &title = it->Get("title");
        const string &hotspot = it->Get("hotspot");
        const string &index = IntToString(i);
        const string &checked = (sSelectType.find( key ) != string::npos)
                               ? "checked"
                               : "";

        html += ""
                "    <label for='chk_" + key + "' title='累计选用数：" + hotspot + "'>\n"
                "        " + index + "<input type=checkbox id='chk_" + key + "' value='" + key + "' title='" + title + "' " + checked + ">" + title + "</label>\n";
    }

    if("" == html)
    {
        // 注意，下面的<input class='hidden'>用于焦点设置
        html = "<label OnClick='OpenPage(\"/TypeDropBoxMenuSetting\")'>\n"
               "  <input class='hidden'><span class='hand'>类型未设置，转到归类设置</span></label>\n"
               "";
    }

    return html;
}

/* 取当前菜单中选中的项
 *   根据不同页面，取不同值；
 */
const string Tag_TypeDropBoxMenu::GetSelectedItems(Page *page) const
{
    const string &name = page->GetRequest()->GetPageName(); // 当前页面名

    // 是browse页面，取的是当前页面配置
    if("browse" == name)
    {
        return page->GetCurrentPageCfg()->Get("filter", "query_type");
    }

    // 是reading、edit页面，取的是当前KEY对应的数据配置
    if( "reading" == name
        || "edit" == name
      )
    {
        return page->GetCurrentPack()->Get("msg_type");
    }

    return "";
}

















// 设置为全局标记；
static Tag_TypeDropBoxMenu tmp(TAG_GLOBAL, THIS_MODULE);

}// end of TAG_TYPEDROPBOXMENU_SPACE
