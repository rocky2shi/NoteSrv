#ifndef __TAG_CURRENTUSER_H_20100502__
#define __TAG_CURRENTUSER_H_20100502__
#include "Tag.h"
namespace TAG_CURRENTUSER_SPACE
{



// 当前登录的用户名
class Tag_CurrentUser : public Tag
{
protected:
    virtual string Get(Page *page);

public:
    Tag_CurrentUser(const string &page, const string &tag);
    virtual ~Tag_CurrentUser();

protected:
    Tag_CurrentUser();
    virtual int DoInit();
    virtual Tag *DoNew();
};







}// end of TAG_CURRENTUSER_SPACE
using namespace TAG_CURRENTUSER_SPACE;
#endif // end of __TAG_CURRENTUSER_H_20100502__
