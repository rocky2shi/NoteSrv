#ifndef __SUBMIT_GETPASSWORD_H_20100524__
#define __SUBMIT_GETPASSWORD_H_20100524__
#include "Submit.h"
namespace SUBMIT_GETPASSWORD_SPACE
{







// ’“ªÿ”√ªß√‹¬Î
class Submit_GetPassword : public Submit
{
protected:
    virtual int Deal(Page *page);


public:
    Submit_GetPassword();
    Submit_GetPassword(const string &page, const string &element);
    virtual ~Submit_GetPassword();

protected:
    virtual int DoInit();
    virtual Submit *DoNew();
};







}// end of SUBMIT_GETPASSWORD_SPACE
//using namespace SUBMIT_GETPASSWORD_SPACE;
#endif // end of __SUBMIT_GETPASSWORD_H_20100524__
