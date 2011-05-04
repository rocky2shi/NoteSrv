#ifndef __TAG_BROWSELIST_H_20100502__
#define __TAG_BROWSELIST_H_20100502__
#include "MultiStringMatch.h"
#include "Tag.h"
namespace TAG_BROWSELIST_SPACE
{



// 数据预览列表
class Tag_BrowseList : public Tag
{
protected:
    virtual string Get(Page *page);

    int Filter(const string &key, const Conf *pack, Page *page, map<string, MultiStringMatch*> &matchs);

public:
    Tag_BrowseList(const string &page, const string &tag);
    virtual ~Tag_BrowseList();

protected:
    Tag_BrowseList();
    virtual int DoInit();
    virtual Tag *DoNew();
};









}// end of TAG_BROWSELIST_SPACE
//using namespace TAG_BROWSELIST_SPACE;
#endif // end of __TAG_BROWSELIST_H_20100502__
