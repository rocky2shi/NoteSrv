#ifndef __SUBMIT_MODIFYPASSWORD_H_20100524__
#define __SUBMIT_MODIFYPASSWORD_H_20100524__
#include "Submit.h"
namespace SUBMIT_MODIFYPASSWORD_SPACE
{







// 修改用户密码
class Submit_ModifyPassword : public Submit
{
protected:
    virtual int Deal(Page *page);


public:
    Submit_ModifyPassword();
    Submit_ModifyPassword(const string &page, const string &element);
    virtual ~Submit_ModifyPassword();

protected:
    virtual int DoInit();
    virtual Submit *DoNew();
};






}// end of SUBMIT_MODIFYPASSWORD_SPACE
//using namespace SUBMIT_MODIFYPASSWORD_SPACE;
#endif // end of __SUBMIT_MODIFYPASSWORD_H_20100524__
