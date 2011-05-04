#ifndef __SUBMIT_MODIFYTYPEDROPBOXMENU_H_20100524__
#define __SUBMIT_MODIFYTYPEDROPBOXMENU_H_20100524__
#include "Submit.h"
namespace SUBMIT_MODIFYTYPEDROPBOXMENU_SPACE
{





// 修改类型下拉菜单项
class Submit_ModifyTypeDropBoxMenu : public Submit
{
protected:
    virtual int Deal(Page *page);


public:
    Submit_ModifyTypeDropBoxMenu();
    Submit_ModifyTypeDropBoxMenu(const string &page, const string &element);
    virtual ~Submit_ModifyTypeDropBoxMenu();

protected:
    virtual int DoInit();
    virtual Submit *DoNew();
};






}// end of SUBMIT_MODIFYTYPEDROPBOXMENU_SPACE
//using namespace SUBMIT_MODIFYTYPEDROPBOXMENU_SPACE;
#endif // end of __SUBMIT_MODIFYTYPEDROPBOXMENU_H_20100524__
