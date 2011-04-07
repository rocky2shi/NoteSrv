#ifndef __TAG_COUNTERLIST_H_20100530__
#define __TAG_COUNTERLIST_H_20100530__
#include "Tag.h"
namespace TAG_COUNTERLIST_SPACE
{






// 计数据列表
class Tag_CounterList : public Tag
{
protected:
    virtual string Get(Page *page);

public:
    Tag_CounterList(const string &page, const string &tag);
    virtual ~Tag_CounterList();

protected:
    Tag_CounterList();
    virtual int DoInit();
    virtual Tag *DoNew();
};






}// end of TAG_COUNTERLIST_SPACE
//using namespace TAG_COUNTERLIST_SPACE;
#endif // end of __TAG_COUNTERLIST_H_20100530__
