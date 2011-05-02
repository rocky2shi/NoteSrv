// Rocky 2010-05-14 18:05:44
#include "Page.h"
#include "UserData.h"
#include "Menu.h"
#include "Tag_TextColorDropBoxMenu.h"
namespace TAG_TEXTCOLORDROPBOXMENU_SPACE
{








// ����ģ��
static const string THIS_MODULE = "TextColorDropBoxMenu";





Tag_TextColorDropBoxMenu::Tag_TextColorDropBoxMenu()
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_TextColorDropBoxMenu::Tag_TextColorDropBoxMenu(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_TextColorDropBoxMenu::~Tag_TextColorDropBoxMenu()
{
    FUNCTION_TRACK(); // �����켣����
}




int Tag_TextColorDropBoxMenu::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Tag::DoInit();
}

// ������󴴽���
Tag *Tag_TextColorDropBoxMenu::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Tag_TextColorDropBoxMenu;
}








/******************************** ҵ����� ********************************/





string Tag_TextColorDropBoxMenu::Get(Page *page)
{
    FUNCTION_TRACK(); // �����켣����

    const Conf *pack = page->GetCurrentPack();          // ��ǰkey��Ӧ���ݼ�
    const string &username = page->GetRequest()->GetCurrentUser();
    Menu *menu = Menu::Get(username, "TextColorDropBox.cfg");   // [XXX:56425820]
    const string &sSelectedItem = pack->Get("font_color");      // ��ǰ��ѡ�е��½Ӳ˵���

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

















// ����Ϊȫ�ֱ�ǣ�
static Tag_TextColorDropBoxMenu tmp("reading", THIS_MODULE);

}// end of TAG_TEXTCOLORDROPBOXMENU_SPACE
