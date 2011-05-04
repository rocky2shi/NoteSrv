#ifndef __PAGE_ERROR_H_20100507__
#define __PAGE_ERROR_H_20100507__
#include "Page.h"
namespace PAGE_ERROR_SPACE
{






// 出错提示页面
class Page_Error : public Page
{
protected:
    virtual int Deal();



public:
    Page_Error();
    virtual ~Page_Error();

protected:
    virtual int DoInit();
    virtual Page *DoNew();
};





}// end of PAGE_ERROR_SPACE
//using namespace PAGE_ERROR_SPACE;
#endif // end of __PAGE_ERROR_H_20100507__
