#ifndef __SUBMIT_SETPASSWORD_H_20100513__
#define __SUBMIT_SETPASSWORD_H_20100513__
#include "Submit.h"
namespace SUBMIT_SETPASSWORD_SPACE
{







// ���õ�ǰ���ݼ�������
class Submit_SetPassword : public Submit
{
protected:
    virtual int Deal(Page *page);


public:
    Submit_SetPassword();
    Submit_SetPassword(const string &page, const string &element);
    virtual ~Submit_SetPassword();

protected:
    virtual int DoInit();
    virtual Submit *DoNew();
};







}// end of SUBMIT_SETPASSWORD_SPACE
//using namespace SUBMIT_SETPASSWORD_SPACE;
#endif // end of __SUBMIT_SETPASSWORD_H_20100513__
