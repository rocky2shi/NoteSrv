#ifndef __PAGE_IMAGE_H_20100422__
#define __PAGE_IMAGE_H_20100422__
#include "Page.h"
namespace PAGE_IMAGE_SPACE
{






// 图片处理类：png、jpg、gif等
class Page_Image : public Page
{
protected:
    virtual int OutHead();
    virtual int OutBody();


public:
    Page_Image();
    virtual ~Page_Image();

protected:
    virtual int DoInit();
    virtual Page *DoNew();

private:
    FileObj m_file;
};






}// end of PAGE_IMAGE_SPACE
//using namespace PAGE_IMAGE_SPACE;
#endif // end of __PAGE_IMAGE_H_20100422__
