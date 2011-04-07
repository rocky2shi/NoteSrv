#ifndef __PAGE_BROWSE_H_20100502__
#define __PAGE_BROWSE_H_20100502__
#include "Page.h"
namespace PAGE_BROWSE_SPACE
{




// Êý¾Ý¼ÇÂ¼Ô¤ÀÀ
class Page_Browse : public Page
{
protected:
    virtual int OutBody();



public:
    Page_Browse();
    virtual ~Page_Browse();

protected:
    virtual int DoInit();
    virtual Page *DoNew();
};






}// end of PAGE_BROWSE_SPACE
//using namespace PAGE_BROWSE_SPACE;
#endif // end of __PAGE_BROWSE_H_20100502__
