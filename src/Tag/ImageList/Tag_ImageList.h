#ifndef __TAG_IMAGELIST_H_20100509__
#define __TAG_IMAGELIST_H_20100509__
#include "Tag.h"
namespace TAG_IMAGELIST_SPACE
{






// Õº∆¨œ‘ æ¡–±Ì
class Tag_ImageList : public Tag
{
protected:
    virtual string Get(Page *page);

public:
    Tag_ImageList(const string &page, const string &tag);
    virtual ~Tag_ImageList();

protected:
    Tag_ImageList();
    virtual int DoInit();
    virtual Tag *DoNew();
};








}// end of TAG_IMAGELIST_SPACE
using namespace TAG_IMAGELIST_SPACE;
#endif // end of __TAG_IMAGELIST_H_20100509__
