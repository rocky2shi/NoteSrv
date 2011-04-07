#ifndef __PAGE_DATAIMPORT_H_20100519__
#define __PAGE_DATAIMPORT_H_20100519__
#include "Page.h"
namespace PAGE_DATAIMPORT_SPACE
{





// 处理导入数据
class Page_DataImport : public Page
{
protected:
    virtual int OutBody();


public:
    Page_DataImport();
    virtual ~Page_DataImport();

protected:
    virtual int DoInit();
    virtual Page *DoNew();

private:
    FileObj m_file;
};






}// end of PAGE_DATAIMPORT_SPACE
//using namespace PAGE_DATAIMPORT_SPACE;
#endif // end of __PAGE_DATAIMPORT_H_20100519__
