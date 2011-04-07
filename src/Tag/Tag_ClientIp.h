#ifndef __TAG_CLIENTIP_H_20100502__
#define __TAG_CLIENTIP_H_20100502__
#include "Tag.h"
namespace TAG_CLIENTIP_SPACE
{




// ¿Í»§¶Ëip
class Tag_ClientIp : public Tag
{
protected:
    virtual string Get(Page *page);

public:
    Tag_ClientIp(const string &page, const string &tag);
    virtual ~Tag_ClientIp();

protected:
    Tag_ClientIp();
    virtual int DoInit();
    virtual Tag *DoNew();
};







}// end of TAG_CLIENTIP_SPACE
using namespace TAG_CLIENTIP_SPACE;
#endif // end of __TAG_CLIENTIP_H_20100502__
