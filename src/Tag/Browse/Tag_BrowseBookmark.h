#ifndef __TAG_BROWSEBOOKMARK_H_20100517__
#define __TAG_BROWSEBOOKMARK_H_20100517__
#include "Tag.h"
namespace TAG_BROWSEBOOKMARK_SPACE
{






// browse页面的书签数据
class Tag_BrowseBookmark : public Tag
{
protected:
    virtual string Get(Page *page);

public:
    Tag_BrowseBookmark(const string &page, const string &tag);
    virtual ~Tag_BrowseBookmark();

protected:
    Tag_BrowseBookmark();
    virtual int DoInit();
    virtual Tag *DoNew();
};






}// end of TAG_BROWSEBOOKMARK_SPACE
//using namespace TAG_BROWSEBOOKMARK_SPACE;
#endif // end of __TAG_BROWSEBOOKMARK_H_20100517__
