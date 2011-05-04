#ifndef __PAGE_EDIT_H_20100504__
#define __PAGE_EDIT_H_20100504__
#include "Page.h"
namespace PAGE_EDIT_SPACE
{







// 编辑页面
class Page_Edit : public Page
{
protected:
    virtual int OutBody();


public:
    Page_Edit();
    virtual ~Page_Edit();

protected:
    virtual int DoInit();
    virtual Page *DoNew();
};




}// end of PAGE_EDIT_SPACE
//using namespace PAGE_EDIT_SPACE;
#endif // end of __PAGE_EDIT_H_20100504__
