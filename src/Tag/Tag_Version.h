#ifndef __TAG_VERSION_H_20100603__
#define __TAG_VERSION_H_20100603__
#include "Tag.h"
namespace TAG_VERSION_SPACE
{






// 显示版本号
class Tag_Version : public Tag
{
protected:
    virtual string Get(Page *page);


public:
    Tag_Version(const string &page, const string &tag);
    virtual ~Tag_Version();

protected:
    Tag_Version();
    virtual int DoInit();
    virtual Tag *DoNew();
};






}// end of TAG_VERSION_SPACE
//using namespace TAG_VERSION_SPACE;
#endif // end of __TAG_VERSION_H_20100603__
