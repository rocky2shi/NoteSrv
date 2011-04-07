#ifndef __SUBMIT_LOGCONFIGITEMS_H_20100528__
#define __SUBMIT_LOGCONFIGITEMS_H_20100528__
#include "Submit.h"
namespace SUBMIT_LOGCONFIGITEMS_SPACE
{







// 设置日志配制
class Submit_LogConfigItems : public Submit
{
protected:
    virtual int Deal(Page *page);


public:
    Submit_LogConfigItems();
    Submit_LogConfigItems(const string &page, const string &element);
    virtual ~Submit_LogConfigItems();

protected:
    virtual int DoInit();
    virtual Submit *DoNew();
};




}// end of SUBMIT_LOGCONFIGITEMS_SPACE
//using namespace SUBMIT_LOGCONFIGITEMS_SPACE;
#endif // end of __SUBMIT_LOGCONFIGITEMS_H_20100528__
