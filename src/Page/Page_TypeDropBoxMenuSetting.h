#ifndef __PAGE_TYPEDROPBOXMENUSETTING_H_20100524__
#define __PAGE_TYPEDROPBOXMENUSETTING_H_20100524__
#include "Page.h"
namespace PAGE_TYPEDROPBOXMENUSETTING_SPACE
{







// 类型选择下拉菜单设置页
class Page_TypeDropBoxMenuSetting : public Page
{
protected:
    virtual int OutBody();


public:
    Page_TypeDropBoxMenuSetting();
    virtual ~Page_TypeDropBoxMenuSetting();

protected:
    virtual int DoInit();
    virtual Page *DoNew();
};





}// end of PAGE_TYPEDROPBOXMENUSETTING_SPACE
//using namespace PAGE_TYPEDROPBOXMENUSETTING_SPACE;
#endif // end of __PAGE_TYPEDROPBOXMENUSETTING_H_20100524__
