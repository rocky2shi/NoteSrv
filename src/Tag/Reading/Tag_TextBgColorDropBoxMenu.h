#ifndef __TAG_TEXTBGCOLORDROPBOXMENU_H_20100514__
#define __TAG_TEXTBGCOLORDROPBOXMENU_H_20100514__
#include "Tag.h"
namespace TAG_TEXTBGCOLORDROPBOXMENU_SPACE
{







// 正文背景颜色下接选框菜单
class Tag_TextBgColorDropBoxMenu : public Tag
{
protected:
    virtual string Get(Page *page);

public:
    Tag_TextBgColorDropBoxMenu(const string &page, const string &tag);
    virtual ~Tag_TextBgColorDropBoxMenu();

protected:
    Tag_TextBgColorDropBoxMenu();
    virtual int DoInit();
    virtual Tag *DoNew();
};











}// end of TAG_TEXTBGCOLORDROPBOXMENU_SPACE
//using namespace TAG_TEXTBGCOLORDROPBOXMENU_SPACE;
#endif // end of __TAG_TEXTBGCOLORDROPBOXMENU_H_20100514__
