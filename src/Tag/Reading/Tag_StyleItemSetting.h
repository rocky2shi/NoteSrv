#ifndef __TAG_STYLEITEMSETTING_H_20100514__
#define __TAG_STYLEITEMSETTING_H_20100514__
#include "Tag.h"
namespace TAG_STYLEITEMSETTING_SPACE
{




// 正文的风格显示设置子项
class Tag_StyleItemSetting : public Tag
{
protected:
    virtual string Get(Page *page);

public:
    Tag_StyleItemSetting(const string &page, const string &tag);
    virtual ~Tag_StyleItemSetting();

protected:
    Tag_StyleItemSetting();
    virtual int DoInit();
    virtual Tag *DoNew();
};








}// end of TAG_STYLEITEMSETTING_SPACE
//using namespace TAG_STYLEITEMSETTING_SPACE;
#endif // end of __TAG_STYLEITEMSETTING_H_20100514__

