#ifndef __PAGE_TEXT_H_20100423__
#define __PAGE_TEXT_H_20100423__
#include "Page.h"
namespace PAGE_TEXT_SPACE
{






// 文本处理类：html、js、css、txt等
class Page_Text : public Page
{
protected:
    virtual int Deal();


public:
    Page_Text();
    virtual ~Page_Text();

protected:
    virtual int DoInit();
    virtual Page *DoNew();
};









}// end of PAGE_TEXT_SPACE
//using namespace PAGE_TEXT_SPACE;
#endif // end of __PAGE_TEXT_H_20100423__
