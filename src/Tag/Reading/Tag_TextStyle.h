#ifndef __TAG_TEXTSTYLE_H_20100514__
#define __TAG_TEXTSTYLE_H_20100514__
#include "Tag.h"
namespace TAG_TEXTSTYLE_SPACE
{






// 阅读正文的风格显示设置
class Tag_TextStyle : public Tag
{
protected:
    virtual string Get(Page *page);

public:
    Tag_TextStyle(const string &page, const string &tag);
    virtual ~Tag_TextStyle();

protected:
    Tag_TextStyle();
    virtual int DoInit();
    virtual Tag *DoNew();
};





}// end of TAG_TEXTSTYLE_SPACE
using namespace TAG_TEXTSTYLE_SPACE;
#endif // end of __TAG_TEXTSTYLE_H_20100514__
