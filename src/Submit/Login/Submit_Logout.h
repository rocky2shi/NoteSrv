#ifndef __SUBMIT_LOGOUT_H_20100523__
#define __SUBMIT_LOGOUT_H_20100523__
#include "Submit.h"
namespace SUBMIT_LOGOUT_SPACE
{





// 处理注销事件
class Submit_Logout : public Submit
{
protected:
    virtual int Deal(Page *page);


public:
    Submit_Logout();
    Submit_Logout(const string &page, const string &element);
    virtual ~Submit_Logout();

protected:
    virtual int DoInit();
    virtual Submit *DoNew();
};










}// end of SUBMIT_LOGOUT_SPACE
//using namespace SUBMIT_LOGOUT_SPACE;
#endif // end of __SUBMIT_LOGOUT_H_20100523__
