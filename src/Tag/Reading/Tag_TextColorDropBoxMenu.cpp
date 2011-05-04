// Rocky 2010-05-14 18:05:44
#include "Page.h"
#include "UserData.h"
#include "Menu.h"
#include "Tag_TextColorDropBoxMenu.h"
namespace TAG_TEXTCOLORDROPBOXMENU_SPACE
{








// 标明模块
static const string THIS_MODULE = "TextColorDropBoxMenu";





Tag_TextColorDropBoxMenu::Tag_TextColorDropBoxMenu()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_TextColorDropBoxMenu::Tag_TextColorDropBoxMenu(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_TextColorDropBoxMenu::~Tag_TextColorDropBoxMenu()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Tag_TextColorDropBoxMenu::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Tag::DoInit();
}

// 子类对象创建器
Tag *Tag_TextColorDropBoxMenu::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Tag_TextColorDropBoxMenu;
}








/******************************** 业务代码 ********************************/





string Tag_TextColorDropBoxMenu::Get(Page *page)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    const Conf *pack = page->GetCurrentPack();          // 当前key对应数据集
    const string &username = page->GetRequest()->GetCurrentUser();
    Menu *menu = Menu::Get(username, "TextColorDropBox.cfg");   // [XXX:56425820]
    const string &sSelectedItem = pack->Get("font_color");      // 当前被选中的下接菜单项

    Menu::iterator it(menu);
    string html = "";
    int i;

    for(i=1; it.next(); i++)
    {
        const string &title = it->Get("title");
        const string &value = it->Get("value");
        const string &index = IntToString(i);
        const string &checked = (sSelectedItem == title) ? " checked" : "";

        html += "  <label for='chk_color_family_" + index + "' title='" + value + "'>\n"
                "    " + index + ".<input type='radio' id='chk_color_family_" + index + "' name='chk_color_family' value='" + title + "'" + checked + "/>\n"
                "    <font color='" + title + "'>" + title + "</font>\n"
                "  </label>";
    }

    return html;
}

















// 设置为全局标记；
static Tag_TextColorDropBoxMenu tmp("reading", THIS_MODULE);

}// end of TAG_TEXTCOLORDROPBOXMENU_SPACE
