#ifndef __SUBMIT_TEXTSTYLESETTING_H_20100514__
#define __SUBMIT_TEXTSTYLESETTING_H_20100514__
#include "Submit.h"
namespace SUBMIT_TEXTSTYLESETTING_SPACE
{





// 正文风格设置
class Submit_TextStyleSetting : public Submit
{
protected:
    virtual int Deal(Page *page);


public:
    Submit_TextStyleSetting();
    Submit_TextStyleSetting(const string &page, const string &element);
    virtual ~Submit_TextStyleSetting();

protected:
    virtual int DoInit();
    virtual Submit *DoNew();
};






}// end of SUBMIT_TEXTSTYLESETTING_SPACE
//using namespace SUBMIT_TEXTSTYLESETTING_SPACE;
#endif // end of __SUBMIT_TEXTSTYLESETTING_H_20100514__
