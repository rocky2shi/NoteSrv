#ifndef __TAG_KEY_H_20100504__
#define __TAG_KEY_H_20100504__
#include "Tag.h"
namespace TAG_KEY_SPACE
{





// ½âÎökey×Ö¶Î
class Tag_Key : public Tag
{
protected:
    virtual string Get(Page *page);


public:
    Tag_Key(const string &page, const string &tag);
    virtual ~Tag_Key();

protected:
    Tag_Key();
    virtual int DoInit();
    virtual Tag *DoNew();
};







}// end of TAG_KEY_SPACE
using namespace TAG_KEY_SPACE;
#endif // end of __TAG_KEY_H_20100504__
