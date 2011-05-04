#ifndef __PAGE_READING_H_20100503__
#define __PAGE_READING_H_20100503__
#include "Page.h"
namespace PAGE_READING_SPACE
{






// 阅读页面
class Page_Reading : public Page
{
protected:
    virtual int OutBody();



public:
    Page_Reading();
    virtual ~Page_Reading();

protected:
    virtual int DoInit();
    virtual Page *DoNew();
};








}// end of PAGE_READING_SPACE
//using namespace PAGE_READING_SPACE;
#endif // end of __PAGE_READING_H_20100503__
