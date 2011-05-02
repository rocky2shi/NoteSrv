// Rocky 2010-05-24 19:22:07
#include "Page.h"
#include "Menu.h"
#include "Tag_TypeDropItemList.h"
namespace TAG_TYPEDROPITEMLIST_SPACE
{





// ����ģ��
static const string THIS_MODULE = "TypeDropItemList";





Tag_TypeDropItemList::Tag_TypeDropItemList()
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_TypeDropItemList::Tag_TypeDropItemList(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_TypeDropItemList::~Tag_TypeDropItemList()
{
    FUNCTION_TRACK(); // �����켣����
}




int Tag_TypeDropItemList::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Tag::DoInit();
}

// ������󴴽���
Tag *Tag_TypeDropItemList::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Tag_TypeDropItemList;
}








/******************************** ҵ����� ********************************/




// Rocky 2010-05-24 19:36:59
string Tag_TypeDropItemList::Get(Page *page)
{
    FUNCTION_TRACK(); // �����켣����

    const string &username = page->GetRequest()->GetCurrentUser();
    Menu *menu = Menu::Get(username, "TypeDropBox.cfg");    // [XXX:56425820]


    Menu::iterator it(menu);
    string html = "";
    int index = 0;
    int i;

    /* �г������½Ӳ˵���
     *��ע���г��Ĳ˵���Ŀǰ�ǰ�index�ֶ�����ģ���Menu���У�
     */
    while( it.next() )
    {
        const string &key = it->key;
        if("" == key)
        {
            continue;
        }

        const string &title = it->Get("title");
        const string &enable = it->Get("status") == "enable" ? " checked" : "";
        const string &disable = it->Get("status") == "disable" ? " checked" : "";
        const string &key_index = "key_" + IntToString(index);
        const string &title_key = "title_" + key;
        const string &status_key = "status_" + key;

        html += ""
                "  <li> \n"
                "    <input name='" + key_index + "' type='hidden' value='" + key + "'> \n"
                "    <input name='" + title_key + "' class='width NOTEMPTY' value='" + title + "'> \n"
                "    <input name='" + status_key + "' type='radio' title='����' value='enable'" + enable + ">���� \n"
                "    <input name='" + status_key + "' type='radio' title='��ͣ' value='disable'" + disable + ">��ͣ \n"
                "    <input name='" + status_key + "' type='radio' title='ɾ��' value='delete'>ɾ��&nbsp; \n"
                "  </li> \n";

        index++;
    }

    return html;
}


















static Tag_TypeDropItemList tmp("TypeDropBoxMenuSetting", THIS_MODULE);

}// end of TAG_TYPEDROPITEMLIST_SPACE
