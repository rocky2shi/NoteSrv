// Rocky 2010-05-14 17:28:43
#include "Page.h"
#include "UserData.h"
#include "Menu.h"
#include "Tag_FontDropBoxMenu.h"
namespace TAG_FONTDROPBOXMENU_SPACE
{





// 标明模块
static const string THIS_MODULE = "FontDropBoxMenu";





Tag_FontDropBoxMenu::Tag_FontDropBoxMenu()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_FontDropBoxMenu::Tag_FontDropBoxMenu(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_FontDropBoxMenu::~Tag_FontDropBoxMenu()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Tag_FontDropBoxMenu::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Tag::DoInit();
}

// 子类对象创建器
Tag *Tag_FontDropBoxMenu::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Tag_FontDropBoxMenu;
}








/******************************** 业务代码 ********************************/





string Tag_FontDropBoxMenu::Get(Page *page)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    const Conf *pack = page->GetCurrentPack();          // 当前key对应数据集
    const string &username = page->GetRequest()->GetCurrentUser();
    Menu *menu = Menu::Get(username, "FontDropBox.cfg");  // [XXX:56425820]
    const string &sSelectedItem = pack->Get("font_family"); // 当前被选中的下接菜单项

    Menu::iterator it(menu);
    string html = "";
    int i;

    for(i=1; it.next(); i++)
    {
        const string &title = it->Get("title");
        const string &index = IntToString(i);
        const string &checked = (sSelectedItem == title) ? " checked" : "";

        html += "\t<label for='chk_font_family_" + index + "'>\n";
        html += "\t\t" + index + ".<input type='radio' id='chk_font_family_" + index + "' name='chk_font_family' value='" + title + "'" + checked + "/>" + title + "</label>\n";
    }

    return html;
}

















// 设置为全局标记；
static Tag_FontDropBoxMenu tmp("reading", THIS_MODULE);

}// end of TAG_FONTDROPBOXMENU_SPACE
