#ifndef __TAG_ATTACHCOUNT_H_20100510__
#define __TAG_ATTACHCOUNT_H_20100510__
#include "Tag.h"
namespace TAG_ATTACHCOUNT_SPACE
{





// ��ʾ��������
class Tag_AttachCount : public Tag
{
protected:
    virtual string Get(Page *page);

public:
    Tag_AttachCount(const string &page, const string &tag);
    virtual ~Tag_AttachCount();

protected:
    Tag_AttachCount();
    virtual int DoInit();
    virtual Tag *DoNew();
};







}// end of TAG_ATTACHCOUNT_SPACE
//using namespace TAG_ATTACHCOUNT_SPACE;
#endif // end of __TAG_ATTACHCOUNT_H_20100510__
