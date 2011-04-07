#ifndef __PAGE_SYSTEMINFO_H_20100604__
#define __PAGE_SYSTEMINFO_H_20100604__
#include "Page.h"
namespace PAGE_SYSTEMINFO_SPACE
{






// 取系统信息：cpu使用率等
class Page_SystemInfo : public Page
{
protected:
    virtual int Deal();


public:
    Page_SystemInfo();
    virtual ~Page_SystemInfo();

protected:
    virtual int DoInit();
    virtual Page *DoNew();
};





}// end of PAGE_SYSTEMINFO_SPACE
//using namespace PAGE_SYSTEMINFO_SPACE;
#endif // end of __PAGE_SYSTEMINFO_H_20100604__
