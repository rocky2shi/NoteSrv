#ifndef __TAG_ATTACHLIST_H_20100510__
#define __TAG_ATTACHLIST_H_20100510__
#include "Tag.h"
namespace TAG_ATTACHLIST_SPACE
{






// Í¼Æ¬¸½¼þÁÐ±í
class Tag_AttachList : public Tag
{
protected:
    virtual string Get(Page *page);

public:
    Tag_AttachList(const string &page, const string &tag);
    virtual ~Tag_AttachList();

protected:
    Tag_AttachList();
    virtual int DoInit();
    virtual Tag *DoNew();
};






}// end of TAG_ATTACHLIST_SPACE
//using namespace TAG_ATTACHLIST_SPACE;
#endif // end of __TAG_ATTACHLIST_H_20100510__
