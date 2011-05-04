#ifndef __SUBMIT_DELETEUSER_H_20100530__
#define __SUBMIT_DELETEUSER_H_20100530__
#include "Submit.h"
namespace SUBMIT_DELETEUSER_SPACE
{





// 删除用户
class Submit_DeleteUser : public Submit
{
protected:
    virtual int Deal(Page *page);


public:
    Submit_DeleteUser();
    Submit_DeleteUser(const string &page, const string &element);
    virtual ~Submit_DeleteUser();

protected:
    virtual int DoInit();
    virtual Submit *DoNew();
};







}// end of SUBMIT_DELETEUSER_SPACE
//using namespace SUBMIT_DELETEUSER_SPACE;
#endif // end of __SUBMIT_DELETEUSER_H_20100530__
