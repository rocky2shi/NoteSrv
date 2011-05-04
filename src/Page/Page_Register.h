#ifndef __PAGE_REGISTER_H_20100523__
#define __PAGE_REGISTER_H_20100523__
#include "Page.h"
namespace PAGE_REGISTER_SPACE
{





// 注册页
class Page_Register : public Page
{
protected:
    virtual int OutBody();


public:
    Page_Register();
    virtual ~Page_Register();

protected:
    virtual int DoInit();
    virtual Page *DoNew();
};










}// end of PAGE_REGISTER_SPACE
//using namespace PAGE_REGISTER_SPACE;
#endif // end of __PAGE_REGISTER_H_20100523__
