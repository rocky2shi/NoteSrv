#ifndef __TAG_TITLE_H_20100504__
#define __TAG_TITLE_H_20100504__
#include "Tag.h"
namespace TAG_TITLE_SPACE
{






// 编辑页面：标题
class Tag_Title : public Tag
{
protected:
    virtual string Get(Page *page);

public:
    Tag_Title(const string &page, const string &tag);
    virtual ~Tag_Title();

protected:
    Tag_Title();
    virtual int DoInit();
    virtual Tag *DoNew();
};











}// end of TAG_TITLE_SPACE
using namespace TAG_TITLE_SPACE;
#endif // end of __TAG_TITLE_H_20100504__
