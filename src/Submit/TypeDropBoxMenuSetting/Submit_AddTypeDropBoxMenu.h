#ifndef __SUBMIT_ADDTYPEDROPBOXMENU_H_20100524__
#define __SUBMIT_ADDTYPEDROPBOXMENU_H_20100524__
#include "Submit.h"
namespace SUBMIT_ADDTYPEDROPBOXMENU_SPACE
{





// 新增类型下拉菜单项
class Submit_AddTypeDropBoxMenu : public Submit
{
protected:
    virtual int Deal(Page *page);


public:
    Submit_AddTypeDropBoxMenu();
    Submit_AddTypeDropBoxMenu(const string &page, const string &element);
    virtual ~Submit_AddTypeDropBoxMenu();

protected:
    virtual int DoInit();
    virtual Submit *DoNew();
};






}// end of SUBMIT_ADDTYPEDROPBOXMENU_SPACE
using namespace SUBMIT_ADDTYPEDROPBOXMENU_SPACE;
#endif // end of __SUBMIT_ADDTYPEDROPBOXMENU_H_20100524__
