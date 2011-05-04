#ifndef __TAG_BROWSEFILTERITEMS_H_20100518__
#define __TAG_BROWSEFILTERITEMS_H_20100518__
#include "Tag.h"
namespace TAG_BROWSEFILTERITEMS_SPACE
{






// 显示过滤选项
class Tag_BrowseFilterItems : public Tag
{
protected:
    virtual string Get(Page *page);

public:
    Tag_BrowseFilterItems(const string &page, const string &tag);
    virtual ~Tag_BrowseFilterItems();

protected:
    Tag_BrowseFilterItems();
    virtual int DoInit();
    virtual Tag *DoNew();
};







}// end of TAG_BROWSEFILTERITEMS_SPACE
//using namespace TAG_BROWSEFILTERITEMS_SPACE;
#endif // end of __TAG_BROWSEFILTERITEMS_H_20100518__
