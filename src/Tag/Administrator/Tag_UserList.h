#ifndef __TAG_USERLIST_H_20100530__
#define __TAG_USERLIST_H_20100530__
#include "Tag.h"
namespace TAG_USERLIST_SPACE
{






// 显示用户信息
class Tag_UserList : public Tag
{
protected:
    virtual string Get(Page *page);

public:
    Tag_UserList(const string &page, const string &tag);
    virtual ~Tag_UserList();

protected:
    Tag_UserList();
    virtual int DoInit();
    virtual Tag *DoNew();
};








}// end of TAG_USERLIST_SPACE
//using namespace TAG_USERLIST_SPACE;
#endif // end of __TAG_USERLIST_H_20100530__
