#ifndef __TAG_TYPEDROPBOXMENU_H_20100506__
#define __TAG_TYPEDROPBOXMENU_H_20100506__
#include "Tag.h"
namespace TAG_TYPEDROPBOXMENU_SPACE
{





// 类型选择下拉框菜单
class Tag_TypeDropBoxMenu : public Tag
{
protected:
    virtual string Get(Page *page);

    virtual const string GetSelectedItems(Page *page) const;

public:
    Tag_TypeDropBoxMenu(const string &page, const string &tag);
    virtual ~Tag_TypeDropBoxMenu();

protected:
    Tag_TypeDropBoxMenu();
    virtual int DoInit();
    virtual Tag *DoNew();
};







}// end of TAG_TYPEDROPBOXMENU_SPACE
using namespace TAG_TYPEDROPBOXMENU_SPACE;
#endif // end of __TAG_TYPEDROPBOXMENU_H_20100506__
