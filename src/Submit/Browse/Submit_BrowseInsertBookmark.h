#ifndef __SUBMIT_BROWSEINSERTBOOKMARK_H_20100517__
#define __SUBMIT_BROWSEINSERTBOOKMARK_H_20100517__
#include "Submit.h"
namespace SUBMIT_BROWSEINSERTBOOKMARK_SPACE
{







// ����browseҳ�������ǩ�ύ�¼�
class Submit_BrowseInsertBookmark : public Submit
{
protected:
    virtual int Deal(Page *page);


public:
    Submit_BrowseInsertBookmark();
    Submit_BrowseInsertBookmark(const string &page, const string &element);
    virtual ~Submit_BrowseInsertBookmark();

protected:
    virtual int DoInit();
    virtual Submit *DoNew();
};










}// end of SUBMIT_BROWSEINSERTBOOKMARK_SPACE
//using namespace SUBMIT_BROWSEINSERTBOOKMARK_SPACE;
#endif // end of __SUBMIT_BROWSEINSERTBOOKMARK_H_20100517__
