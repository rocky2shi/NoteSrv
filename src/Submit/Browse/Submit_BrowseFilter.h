#ifndef __SUBMIT_BROWSEFILTER_H_20100517__
#define __SUBMIT_BROWSEFILTER_H_20100517__
#include "Submit.h"
namespace SUBMIT_BROWSEFILTER_SPACE
{






// 正文风格设置
class Submit_BrowseFilter : public Submit
{
protected:
    virtual int Deal(Page *page);


public:
    Submit_BrowseFilter();
    Submit_BrowseFilter(const string &page, const string &element);
    virtual ~Submit_BrowseFilter();

protected:
    virtual int DoInit();
    virtual Submit *DoNew();
};







}// end of SUBMIT_BROWSEFILTER_SPACE
//using namespace SUBMIT_BROWSEFILTER_SPACE;
#endif // end of __SUBMIT_BROWSEFILTER_H_20100517__

