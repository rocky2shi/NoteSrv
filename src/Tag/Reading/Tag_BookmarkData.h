#ifndef __TAG_BOOKMARKDATA_H_20100514__
#define __TAG_BOOKMARKDATA_H_20100514__
#include "Tag.h"
namespace TAG_BOOKMARKDATA_SPACE
{





// 书签数据
class Tag_BookmarkData : public Tag
{
protected:
    virtual string Get(Page *page);

public:
    Tag_BookmarkData(const string &page, const string &tag);
    virtual ~Tag_BookmarkData();

protected:
    Tag_BookmarkData();
    virtual int DoInit();
    virtual Tag *DoNew();
};







}// end of TAG_BOOKMARKDATA_SPACE
//using namespace TAG_BOOKMARKDATA_SPACE;
#endif // end of __TAG_BOOKMARKDATA_H_20100514__
