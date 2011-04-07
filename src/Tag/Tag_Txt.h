#ifndef __TAG_TXT_H_20100422__
#define __TAG_TXT_H_20100422__
#include "Tag.h"
namespace TAG_TXT_SPACE
{




// 页面中一般的文本（即普通的html代码）处理类
class Tag_Txt : public Tag
{
protected:
    virtual string Get(Page *page);


public:
    Tag_Txt(const string &page, const string &tag);
    virtual ~Tag_Txt();

protected:
    Tag_Txt();
    virtual int DoInit();
    virtual Tag *DoNew();
};







}// end of TAG_TXT_SPACE
using namespace TAG_TXT_SPACE;
#endif // end of __TAG_TXT_H_20100422__
