// Rocky 2010-05-14 18:29:16
#include "Page.h"
#include "UserData.h"
#include "Menu.h"
#include "Tag_TextBgColorDropBoxMenu.h"
namespace TAG_TEXTBGCOLORDROPBOXMENU_SPACE
{







// ����ģ��
static const string THIS_MODULE = "TextBgColorDropBoxMenu";





Tag_TextBgColorDropBoxMenu::Tag_TextBgColorDropBoxMenu()
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_TextBgColorDropBoxMenu::Tag_TextBgColorDropBoxMenu(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_TextBgColorDropBoxMenu::~Tag_TextBgColorDropBoxMenu()
{
    FUNCTION_TRACK(); // �����켣����
}




int Tag_TextBgColorDropBoxMenu::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Tag::DoInit();
}

// ������󴴽���
Tag *Tag_TextBgColorDropBoxMenu::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Tag_TextBgColorDropBoxMenu;
}








/******************************** ҵ����� ********************************/





// ȡ���� [2010-05]
string Tag_TextBgColorDropBoxMenu::Get(Page *page)
{
    FUNCTION_TRACK(); // �����켣����

    const string &key = page->GetCurrentKey();
    const Conf *pack = page->GetCurrentPack();          // ��ǰkey��Ӧ���ݼ�
    const string &username = page->GetRequest()->GetCurrentUser();
    Menu *menu = Menu::Get(username, "TextBgColorDropBox.cfg");   // [XXX:56425820]
    const string &sSelectedItem = pack->Get("font_bgcolor");  // ��ǰ��ѡ�е��½Ӳ˵���

    Menu::iterator it(menu);
    string html = "";
    int i;

    for(i=1; it.next(); i++)
    {
        const string &title = it->Get("title");
        const string &value = it->Get("value");
        const string &index = IntToString(i);
        const string &checked = (sSelectedItem == title) ? " checked" : "";

        html += "  <label for='chk_bgcolor_family_" + index + "' title='" + value + "'>\n"
                "    " + index + ".<input type='radio' id='chk_bgcolor_family_" + index + "' name='chk_bgcolor_family' value='" + title + "'" + checked + "/>\n"
                "    <font color='" + title + "'>" + title + "</font>\n"
                "  </label>";
    }

    return html;
}

















// ����Ϊȫ�ֱ�ǣ�
static Tag_TextBgColorDropBoxMenu tmp("reading", THIS_MODULE);

}// end of TAG_TEXTBGCOLORDROPBOXMENU_SPACE
