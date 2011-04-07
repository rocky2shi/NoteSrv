#ifndef __PAGE_DATAEXPORT_H_20100519__
#define __PAGE_DATAEXPORT_H_20100519__
#include "Page.h"
namespace PAGE_DATAEXPORT_SPACE
{







// 处理导出数据
class Page_DataExport : public Page
{
protected:
    virtual int OutHead();
    virtual int OutBody();


public:
    Page_DataExport();
    virtual ~Page_DataExport();

protected:
    virtual int DoInit();
    virtual Page *DoNew();

private:
    FileObj m_file;
};







}// end of PAGE_DATAEXPORT_SPACE
//using namespace PAGE_DATAEXPORT_SPACE;
#endif // end of __PAGE_DATAEXPORT_H_20100519__
