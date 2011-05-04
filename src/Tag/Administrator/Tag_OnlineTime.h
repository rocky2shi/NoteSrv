#ifndef __TAG_ONLINETIME_H_20100530__
#define __TAG_ONLINETIME_H_20100530__
#include "Tag.h"
namespace TAG_ONLINETIME_SPACE
{






// 显示系统在线时长等
class Tag_OnlineTime : public Tag
{
protected:
    virtual string Get(Page *page);

public:
    Tag_OnlineTime(const string &page, const string &tag);
    virtual ~Tag_OnlineTime();

protected:
    Tag_OnlineTime();
    virtual int DoInit();
    virtual Tag *DoNew();
};






}// end of TAG_ONLINETIME_SPACE
//using namespace TAG_ONLINETIME_SPACE;
#endif // end of __TAG_ONLINETIME_H_20100530__
