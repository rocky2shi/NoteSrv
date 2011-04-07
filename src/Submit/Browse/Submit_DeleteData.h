#ifndef __SUBMIT_DELETEDATA_H_20100522__
#define __SUBMIT_DELETEDATA_H_20100522__
#include "Submit.h"
namespace SUBMIT_DELETEDATA_SPACE
{




// 删除浏览页页上选定的数据
class Submit_DeleteData : public Submit
{
protected:
    virtual int Deal(Page *page);


public:
    Submit_DeleteData();
    Submit_DeleteData(const string &page, const string &element);
    virtual ~Submit_DeleteData();

protected:
    virtual int DoInit();
    virtual Submit *DoNew();
};










}// end of SUBMIT_DELETEDATA_SPACE
//using namespace SUBMIT_DELETEDATA_SPACE;
#endif // end of __SUBMIT_DELETEDATA_H_20100522__
