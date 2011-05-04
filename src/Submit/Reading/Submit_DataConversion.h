#ifndef __SUBMIT_DATACONVERSION_H_20100525__
#define __SUBMIT_DATACONVERSION_H_20100525__
#include "Submit.h"
namespace SUBMIT_DATACONVERSION_SPACE
{





// 用于新老格式间的数据转换（调用正则式代码）
class Submit_DataConversion : public Submit
{
protected:
    virtual int Deal(Page *page);


public:
    Submit_DataConversion();
    Submit_DataConversion(const string &page, const string &element);
    virtual ~Submit_DataConversion();

protected:
    virtual int DoInit();
    virtual Submit *DoNew();
};






}// end of SUBMIT_DATACONVERSION_SPACE
//using namespace SUBMIT_DATACONVERSION_SPACE;
#endif // end of __SUBMIT_DATACONVERSION_H_20100525__
