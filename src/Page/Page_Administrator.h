#ifndef __PAGE_ADMINISTRATOR_H_20100528__
#define __PAGE_ADMINISTRATOR_H_20100528__
#include "Page.h"
namespace PAGE_ADMINISTRATOR_SPACE
{






// 管理页面
class Page_Administrator : public Page
{
protected:
    virtual int OutBody();



public:
    Page_Administrator();
    virtual ~Page_Administrator();

protected:
    virtual int DoInit();
    virtual Page *DoNew();
};





}// end of PAGE_ADMINISTRATOR_SPACE
//using namespace PAGE_ADMINISTRATOR_SPACE;
#endif // end of __PAGE_ADMINISTRATOR_H_20100528__
