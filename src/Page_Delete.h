#ifndef __PAGE_DELETE_H_20100510__
#define __PAGE_DELETE_H_20100510__
#include "Page.h"
namespace PAGE_DELETE_SPACE
{








// ɾ���ļ�
class Page_Delete : public Page
{
protected:
    virtual int Deal();
    virtual int OutBody();


public:
    Page_Delete();
    virtual ~Page_Delete();

protected:
    virtual int DoInit();
    virtual Page *DoNew();
};




}// end of PAGE_DELETE_SPACE
using namespace PAGE_DELETE_SPACE;
#endif // end of __PAGE_DELETE_H_20100510__
