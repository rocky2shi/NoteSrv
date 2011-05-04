#ifndef __TAG_REGISTERITEMS_H_20100523__
#define __TAG_REGISTERITEMS_H_20100523__
#include "Tag.h"
namespace TAG_REGISTERITEMS_SPACE
{





// 注册用户页子项
class Tag_RegisterItems : public Tag
{
protected:
    virtual string Get(Page *page);

public:
    Tag_RegisterItems(const string &page, const string &tag);
    virtual ~Tag_RegisterItems();

protected:
    Tag_RegisterItems();
    virtual int DoInit();
    virtual Tag *DoNew();
};











}// end of TAG_REGISTERITEMS_SPACE
//using namespace TAG_REGISTERITEMS_SPACE;
#endif // end of __TAG_REGISTERITEMS_H_20100523__
