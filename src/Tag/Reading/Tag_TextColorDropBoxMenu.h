#ifndef __TAG_TEXTCOLORDROPBOXMENU_H_20100514__
#define __TAG_TEXTCOLORDROPBOXMENU_H_20100514__
#include "Tag.h"
namespace TAG_TEXTCOLORDROPBOXMENU_SPACE
{





// 正文字体颜色下拉框菜单
class Tag_TextColorDropBoxMenu : public Tag
{
protected:
    virtual string Get(Page *page);

public:
    Tag_TextColorDropBoxMenu(const string &page, const string &tag);
    virtual ~Tag_TextColorDropBoxMenu();

protected:
    Tag_TextColorDropBoxMenu();
    virtual int DoInit();
    virtual Tag *DoNew();
};







}// end of TAG_TEXTCOLORDROPBOXMENU_SPACE
//using namespace TAG_TEXTCOLORDROPBOXMENU_SPACE;
#endif // end of __TAG_TEXTCOLORDROPBOXMENU_H_20100514__
