#ifndef __PAGE_PING_H_20100529__
#define __PAGE_PING_H_20100529__
#include "Page.h"
namespace PAGE_PING_SPACE
{








class Page_Ping : public Page
{
protected:
    virtual int OutBody();


public:
    Page_Ping();
    virtual ~Page_Ping();

protected:
    virtual int DoInit();
    virtual Page *DoNew();
};








}// end of PAGE_PING_SPACE
//using namespace PAGE_PING_SPACE;
#endif // end of __PAGE_PING_H_20100529__
