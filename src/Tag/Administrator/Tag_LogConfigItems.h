#ifndef __TAG_LOGCONFIGITEMS_H_20100528__
#define __TAG_LOGCONFIGITEMS_H_20100528__
#include "Tag.h"
namespace TAG_LOGCONFIGITEMS_SPACE
{





// ��־��������
class Tag_LogConfigItems : public Tag
{
protected:
    virtual string Get(Page *page);

public:
    Tag_LogConfigItems(const string &page, const string &tag);
    virtual ~Tag_LogConfigItems();

protected:
    Tag_LogConfigItems();
    virtual int DoInit();
    virtual Tag *DoNew();
};





}// end of TAG_LOGCONFIGITEMS_SPACE
//using namespace TAG_LOGCONFIGITEMS_SPACE;
#endif // end of __TAG_LOGCONFIGITEMS_H_20100528__
