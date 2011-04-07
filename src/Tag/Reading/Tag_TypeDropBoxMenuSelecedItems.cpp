// Rocky 2010-05-13 18:19:40
#include "UserData.h"
#include "Page.h"
#include "Menu.h"
#include "Tag_TypeDropBoxMenuSelecedItems.h"
namespace TAG_TYPEDROPBOXMENUSELECEDITEMS_SPACE
{



// 标明模块
static const string THIS_MODULE = "TypeDropBoxMenuSelecedItems";





Tag_TypeDropBoxMenuSelecedItems::Tag_TypeDropBoxMenuSelecedItems()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_TypeDropBoxMenuSelecedItems::Tag_TypeDropBoxMenuSelecedItems(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_TypeDropBoxMenuSelecedItems::~Tag_TypeDropBoxMenuSelecedItems()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Tag_TypeDropBoxMenuSelecedItems::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Tag::DoInit();
}

// 子类对象创建器
Tag *Tag_TypeDropBoxMenuSelecedItems::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Tag_TypeDropBoxMenuSelecedItems;
}








/******************************** 业务代码 ********************************/





// 取标题 [2010-05]
string Tag_TypeDropBoxMenuSelecedItems::Get(Page *page)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    const Conf *pack = page->GetCurrentPack();          // 当前key对应数据集
    const string &username = page->GetRequest()->GetCurrentUser();
    Menu *menu = Menu::Get(username, "TypeDropBox.cfg");    // [XXX:56425820]
    vector<string> menulist;

    if( !menu->isValid() )
    {
        LOG_ERROR("Invalid menu, username=[%s], menu=[%s]",
                                username.c_str(), "TypeDropBox.cfg"); // [XXX:56425820]
        return "";
    }

    // 把菜单串分割为单独的: 20081013103040,20081008220609,20081008220619
    Split(pack->Get("msg_type"), ",", menulist);

    // 每个key换成菜单项名，再连接起来；
    vector<string>::iterator it;
    for(it = menulist.begin(); menulist.end() != it; it++)
    {
        (*it) = menu->GetItem(*it, "title");
    }

    // 再合并各项为串后返回
    return Join(menulist);
}

















// 设置为全局标记；
static Tag_TypeDropBoxMenuSelecedItems tmp("reading", THIS_MODULE);

}// end of TAG_TYPEDROPBOXMENUSELECEDITEMS_SPACE
