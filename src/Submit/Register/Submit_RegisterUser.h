#ifndef __SUBMIT_REGISTERUSER_H_20100523__
#define __SUBMIT_REGISTERUSER_H_20100523__
#include "Submit.h"
namespace SUBMIT_REGISTERUSER_SPACE
{







// ÓÃ»§×¢²á
class Submit_RegisterUser : public Submit
{
protected:
    virtual int Deal(Page *page);


public:
    Submit_RegisterUser();
    Submit_RegisterUser(const string &page, const string &element);
    virtual ~Submit_RegisterUser();

protected:
    virtual int DoInit();
    virtual Submit *DoNew();
};









}// end of SUBMIT_REGISTERUSER_SPACE
//using namespace SUBMIT_REGISTERUSER_SPACE;
#endif // end of __SUBMIT_REGISTERUSER_H_20100523__
