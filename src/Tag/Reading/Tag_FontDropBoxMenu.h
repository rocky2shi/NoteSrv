#ifndef __TAG_FONTDROPBOXMENU_H_20100514__
#define __TAG_FONTDROPBOXMENU_H_20100514__
#include "Tag.h"
namespace TAG_FONTDROPBOXMENU_SPACE
{






// 字体下拉框菜单
class Tag_FontDropBoxMenu : public Tag
{
protected:
    virtual string Get(Page *page);

public:
    Tag_FontDropBoxMenu(const string &page, const string &tag);
    virtual ~Tag_FontDropBoxMenu();

protected:
    Tag_FontDropBoxMenu();
    virtual int DoInit();
    virtual Tag *DoNew();
};






}// end of TAG_FONTDROPBOXMENU_SPACE
//using namespace TAG_FONTDROPBOXMENU_SPACE;
#endif // end of __TAG_FONTDROPBOXMENU_H_20100514__
