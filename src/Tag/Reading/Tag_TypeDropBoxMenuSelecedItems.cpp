// Rocky 2010-05-13 18:19:40
#include "UserData.h"
#include "Page.h"
#include "Menu.h"
#include "Tag_TypeDropBoxMenuSelecedItems.h"
namespace TAG_TYPEDROPBOXMENUSELECEDITEMS_SPACE
{



// ����ģ��
static const string THIS_MODULE = "TypeDropBoxMenuSelecedItems";





Tag_TypeDropBoxMenuSelecedItems::Tag_TypeDropBoxMenuSelecedItems()
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_TypeDropBoxMenuSelecedItems::Tag_TypeDropBoxMenuSelecedItems(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_TypeDropBoxMenuSelecedItems::~Tag_TypeDropBoxMenuSelecedItems()
{
    FUNCTION_TRACK(); // �����켣����
}




int Tag_TypeDropBoxMenuSelecedItems::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Tag::DoInit();
}

// ������󴴽���
Tag *Tag_TypeDropBoxMenuSelecedItems::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Tag_TypeDropBoxMenuSelecedItems;
}








/******************************** ҵ����� ********************************/





// ȡ���� [2010-05]
string Tag_TypeDropBoxMenuSelecedItems::Get(Page *page)
{
    FUNCTION_TRACK(); // �����켣����

    const Conf *pack = page->GetCurrentPack();          // ��ǰkey��Ӧ���ݼ�
    const string &username = page->GetRequest()->GetCurrentUser();
    Menu *menu = Menu::Get(username, "TypeDropBox.cfg");    // [XXX:56425820]
    vector<string> menulist;

    if( !menu->isValid() )
    {
        LOG_ERROR("Invalid menu, username=[%s], menu=[%s]",
                                username.c_str(), "TypeDropBox.cfg"); // [XXX:56425820]
        return "";
    }

    // �Ѳ˵����ָ�Ϊ������: 20081013103040,20081008220609,20081008220619
    Split(pack->Get("msg_type"), ",", menulist);

    // ÿ��key���ɲ˵�������������������
    vector<string>::iterator it;
    for(it = menulist.begin(); menulist.end() != it; it++)
    {
        (*it) = menu->GetItem(*it, "title");
    }

    // �ٺϲ�����Ϊ���󷵻�
    return Join(menulist);
}

















// ����Ϊȫ�ֱ�ǣ�
static Tag_TypeDropBoxMenuSelecedItems tmp("reading", THIS_MODULE);

}// end of TAG_TYPEDROPBOXMENUSELECEDITEMS_SPACE
