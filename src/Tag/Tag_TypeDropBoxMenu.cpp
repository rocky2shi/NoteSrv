// Rocky 2010-05-06 19:23:20
#include "Page.h"
#include "Menu.h"
#include "Tag_TypeDropBoxMenu.h"
namespace TAG_TYPEDROPBOXMENU_SPACE
{






// ����ģ��
static const string THIS_MODULE = "TypeDropBoxMenu";





Tag_TypeDropBoxMenu::Tag_TypeDropBoxMenu()
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_TypeDropBoxMenu::Tag_TypeDropBoxMenu(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_TypeDropBoxMenu::~Tag_TypeDropBoxMenu()
{
    FUNCTION_TRACK(); // �����켣����
}




int Tag_TypeDropBoxMenu::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Tag::DoInit();
}

// ������󴴽���
Tag *Tag_TypeDropBoxMenu::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Tag_TypeDropBoxMenu;
}








/******************************** ҵ����� ********************************/





string Tag_TypeDropBoxMenu::Get(Page *page)
{
    FUNCTION_TRACK(); // �����켣����

    const string &username = page->GetRequest()->GetCurrentUser();
    Menu *menu = Menu::Get(username, "TypeDropBox.cfg");    // [XXX:56425820]
    const string &sSelectType = GetSelectedItems(page);     // ��ǰ��ѡ�е��½Ӳ˵�

    LOG_DEBUG("sSelectType=[%s]", sSelectType.c_str());

    Menu::iterator it(menu);
    string html = "";
    int i;

    /* �г������½Ӳ˵���
     *��ע���г��Ĳ˵���Ŀǰ�ǰ�index�ֶ�����ģ���Menu���У�
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
                "    <label for='chk_" + key + "' title='�ۼ�ѡ������" + hotspot + "'>\n"
                "        " + index + "<input type=checkbox id='chk_" + key + "' value='" + key + "' title='" + title + "' " + checked + ">" + title + "</label>\n";
    }

    if("" == html)
    {
        html = "<label OnClick='OpenPage(\"set_type_select.cgi?type=message\")'>\n"
               "  <input class='hidden'><span class='hand'>����δ���ã�ת����������</span></label>\n"
               "";
    }

    return html;
}

/* ȡ��ǰ�˵���ѡ�е���
 *   ���ݲ�ͬҳ�棬ȡ��ֵͬ��
 */
const string Tag_TypeDropBoxMenu::GetSelectedItems(Page *page) const
{
    const string &name = page->GetRequest()->GetPageName(); // ��ǰҳ����

    // ��browseҳ�棬ȡ���ǵ�ǰҳ������
    if("browse" == name)
    {
        return page->GetCurrentPageCfg()->Get("filter", "query_type");
    }

    // ��reading��editҳ�棬ȡ���ǵ�ǰKEY��Ӧ����������
    if( "reading" == name
        || "edit" == name
      )
    {
        return page->GetCurrentPack()->Get("msg_type");
    }

    return "";
}

















// ����Ϊȫ�ֱ�ǣ�
static Tag_TypeDropBoxMenu tmp(TAG_GLOBAL, THIS_MODULE);

}// end of TAG_TYPEDROPBOXMENU_SPACE
