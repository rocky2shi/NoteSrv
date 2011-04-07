#ifndef __PAGE_LOGIN_H_20100417__
#define __PAGE_LOGIN_H_20100417__
#include "Page.h"
namespace PAGE_LOGIN_SPACE
{






// µ«¬º¥¶¿Ì¿‡
class Page_Login : public Page
{
protected:
    virtual int Deal();



public:
    Page_Login();
    virtual ~Page_Login();

protected:
    virtual int DoInit();
    virtual Page *DoNew();
};










}// end of PAGE_LOGIN_SPACE
//using namespace PAGE_LOGIN_SPACE;
#endif // end of __PAGE_LOGIN_H_20100417__
