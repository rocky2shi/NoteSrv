#ifndef __TAG_SERVERTIME_H_20100425__
#define __TAG_SERVERTIME_H_20100425__
#include "Tag.h"
namespace TAG_SERVERTIME_SPACE
{




// 取服务器时间（秒数）
class Tag_ServerTime : public Tag
{
protected:
    virtual string Get(Page *page);

public:
    Tag_ServerTime(const string &page, const string &tag);
    virtual ~Tag_ServerTime();

protected:
    Tag_ServerTime();
    virtual int DoInit();
    virtual Tag *DoNew();
};








}// end of TAG_SERVERTIME_SPACE
using namespace TAG_SERVERTIME_SPACE;
#endif // end of __TAG_SERVERTIME_H_20100425__
