#ifndef __PAGE_IMAGELIST_H_20100509__
#define __PAGE_IMAGELIST_H_20100509__
#include "Page.h"
namespace PAGE_IMAGELIST_SPACE
{






// ��ʾ�Ķ�ҳ��ı�ѡͼƬ�б�
class Page_ImageList : public Page
{
protected:
    virtual int Deal();


public:
    Page_ImageList();
    virtual ~Page_ImageList();

protected:
    virtual int DoInit();
    virtual Page *DoNew();
};










}// end of PAGE_IMAGELIST_SPACE
//using namespace PAGE_IMAGELIST_SPACE;
#endif // end of __PAGE_IMAGELIST_H_20100509__
