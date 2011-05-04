#ifndef __SUBMIT_DELETEATTACH_H_20100523__
#define __SUBMIT_DELETEATTACH_H_20100523__
#include "Submit.h"
namespace SUBMIT_DELETEATTACH_SPACE
{



// 删除附件
class Submit_DeleteAttach : public Submit
{
protected:
    virtual int Deal(Page *page);


public:
    Submit_DeleteAttach();
    Submit_DeleteAttach(const string &page, const string &element);
    virtual ~Submit_DeleteAttach();

protected:
    virtual int DoInit();
    virtual Submit *DoNew();
};











}// end of SUBMIT_DELETEATTACH_SPACE
using namespace SUBMIT_DELETEATTACH_SPACE;
#endif // end of __SUBMIT_DELETEATTACH_H_20100523__
