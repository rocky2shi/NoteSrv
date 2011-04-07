#ifndef __TAG_TEXT_H_20100503__
#define __TAG_TEXT_H_20100503__
#include "Tag.h"
namespace TAG_TEXT_SPACE
{





// 编辑：正文
class Tag_Text : public Tag
{
protected:
    virtual string Get(Page *page);

    // 转换函数
    virtual const string ThisTextToWeb(const string &txt) const;


public:
    Tag_Text(const string &page, const string &tag);
    virtual ~Tag_Text();

protected:
    Tag_Text();
    virtual int DoInit();
    virtual Tag *DoNew();
};










}// end of TAG_TEXT_SPACE
using namespace TAG_TEXT_SPACE;
#endif // end of __TAG_TEXT_H_20100503__
