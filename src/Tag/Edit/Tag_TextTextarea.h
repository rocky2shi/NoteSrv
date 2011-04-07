#ifndef __TAG_TEXTTEXTAREA_H_20100611__
#define __TAG_TEXTTEXTAREA_H_20100611__
#include "Tag/Tag_Text.h"
namespace TAG_TEXTTEXTAREA_SPACE
{






class Tag_TextTextarea : public Tag_Text
{
protected:
    virtual string Get(Page *page);

    // ×ª»»º¯Êý
    virtual const string ThisTextToWeb(const string &txt) const;


public:
    Tag_TextTextarea(const string &page, const string &tag);
    virtual ~Tag_TextTextarea();

protected:
    Tag_TextTextarea();
    virtual int DoInit();
    virtual Tag *DoNew();
};







}// end of TAG_TEXTTEXTAREA_SPACE
//using namespace TAG_TEXTTEXTAREA_SPACE;
#endif // end of __TAG_TEXTTEXTAREA_H_20100611__
