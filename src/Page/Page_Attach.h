#ifndef __PAGE_ATTACH_H_20100510__
#define __PAGE_ATTACH_H_20100510__
#include "Page.h"
namespace PAGE_ATTACH_SPACE
{





// 附件列表
class Page_Attach : public Page
{
protected:
    virtual int Deal();
    virtual int OutBody();

    // 上传附件
    int Upload();

public:
    Page_Attach();
    virtual ~Page_Attach();

protected:
    virtual int DoInit();
    virtual Page *DoNew();
};








}// end of PAGE_ATTACH_SPACE
//using namespace PAGE_ATTACH_SPACE;
#endif // end of __PAGE_ATTACH_H_20100510__
