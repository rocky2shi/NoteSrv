// Rocky 2010-05-24 19:47:52
#include "Request.h"
#include "Menu.h"
#include "Page.h"
#include "Submit_ModifyTypeDropBoxMenu.h"
namespace SUBMIT_MODIFYTYPEDROPBOXMENU_SPACE
{



// 标明模块
static const string THIS_MODULE = "ModifyTypeDropBoxMenu";





Submit_ModifyTypeDropBoxMenu::Submit_ModifyTypeDropBoxMenu()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Submit_ModifyTypeDropBoxMenu::Submit_ModifyTypeDropBoxMenu(const string &page, const string &element)
                        : Submit(page, element)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Submit_ModifyTypeDropBoxMenu::~Submit_ModifyTypeDropBoxMenu()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Submit_ModifyTypeDropBoxMenu::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Submit::DoInit();
}

// 子类对象创建器
Submit *Submit_ModifyTypeDropBoxMenu::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Submit_ModifyTypeDropBoxMenu;
}





/******************************** 业务代码 ********************************/



// Rocky 2010-05-24 20:34:33
int Submit_ModifyTypeDropBoxMenu::Deal(Page *page)
{
    Request *request = page->GetRequest();
    const string &username = request->GetCurrentUser();
    Menu *menu = Menu::Get(username, "TypeDropBox.cfg");    // [XXX:56425820]
    string key;
    int i;

    /*
     * 取出客户传来的值
     */
    for(i=0; "" != (key = request->GetField("key_" + IntToString(i))); i++)
    {
        const string &title = request->GetField("title_" + key);
        const string &status = request->GetField("status_" + key);

        if("delete" == status)
        {
            LOG_INFO("Delete menu item: [%s]", title.c_str());
            menu->DelItem(key);
            continue;
        }

        menu->SetItem(key, "title", title);
        menu->SetItem(key, "status", status);
    }

    LOG_DEBUG("i=[%d]", i);

    // 保存
    return menu->Save();
}














static Submit_ModifyTypeDropBoxMenu tmp("TypeDropBoxMenuSetting", THIS_MODULE);

}// end of SUBMIT_MODIFYTYPEDROPBOXMENU_SPACE
