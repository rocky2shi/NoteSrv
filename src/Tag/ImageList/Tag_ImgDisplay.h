#ifndef __TAG_IMGDISPLAY_H_20100509__
#define __TAG_IMGDISPLAY_H_20100509__
#include "Tag.h"
namespace TAG_IMGDISPLAY_SPACE
{





// 图片显示列表为空时的处理
class Tag_ImgDisplay : public Tag
{
protected:
    virtual string Get(Page *page);

public:
    Tag_ImgDisplay(const string &page, const string &tag);
    virtual ~Tag_ImgDisplay();

protected:
    Tag_ImgDisplay();
    virtual int DoInit();
    virtual Tag *DoNew();
};










}// end of TAG_IMGDISPLAY_SPACE
using namespace TAG_IMGDISPLAY_SPACE;
#endif // end of __TAG_IMGDISPLAY_H_20100509__

