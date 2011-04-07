// Rocky 2010-05-24 20:35:23
#include "Request.h"
#include "Menu.h"
#include "Page.h"
#include "Submit_AddTypeDropBoxMenu.h"
namespace SUBMIT_ADDTYPEDROPBOXMENU_SPACE
{


// 标明模块
static const string THIS_MODULE = "AddTypeDropBoxMenu";





Submit_AddTypeDropBoxMenu::Submit_AddTypeDropBoxMenu()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Submit_AddTypeDropBoxMenu::Submit_AddTypeDropBoxMenu(const string &page, const string &element)
                        : Submit(page, element)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Submit_AddTypeDropBoxMenu::~Submit_AddTypeDropBoxMenu()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Submit_AddTypeDropBoxMenu::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Submit::DoInit();
}

// 子类对象创建器
Submit *Submit_AddTypeDropBoxMenu::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Submit_AddTypeDropBoxMenu;
}





/******************************** 业务代码 ********************************/



// Rocky 2010-05-24 20:34:33
int Submit_AddTypeDropBoxMenu::Deal(Page *page)
{
    Request *request = page->GetRequest();
    const string &username = request->GetCurrentUser();
    Menu *menu = Menu::Get(username, "TypeDropBox.cfg");    // [XXX:56425820]
    const string &key = page->GetCurrentKey();
    const string &type_title = request->GetField("type_title");

    // 标题字段不能空，也不能重复；
    if("" == type_title || "" != menu->Find(type_title, "title"))
    {
        LOG_ERROR("Title error, repeat or empty: [%s]", type_title.c_str());
        return ERR;
    }

    menu->SetItem(key, "title", type_title);
    menu->SetItem(key, "status", "enable");
    menu->SetItem(key, "hotspot", "0");

    // 保存
    return menu->Save();
}














static Submit_AddTypeDropBoxMenu tmp("TypeDropBoxMenuSetting", THIS_MODULE);

}// end of SUBMIT_ADDTYPEDROPBOXMENU_SPACE
