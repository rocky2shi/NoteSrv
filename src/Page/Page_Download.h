#ifndef __PAGE_DOWNLOAD_H_20100423__
#define __PAGE_DOWNLOAD_H_20100423__
#include "Page.h"
namespace PAGE_DOWNLOAD_SPACE
{






// 文件下载处理类
class Page_Download : public Page
{
protected:
    virtual int OutHead();
    virtual int OutBody();


public:
    Page_Download();
    virtual ~Page_Download();

protected:
    virtual int DoInit();
    virtual Page *DoNew();

private:
    FileObj m_file;
};







}// end of PAGE_DOWNLOAD_SPACE
//using namespace PAGE_DOWNLOAD_SPACE;
#endif // end of __PAGE_DOWNLOAD_H_20100423__
