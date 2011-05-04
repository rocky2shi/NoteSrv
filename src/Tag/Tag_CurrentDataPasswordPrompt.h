#ifndef __TAG_CURRENTDATAPASSWORDPROMPT_H_20100513__
#define __TAG_CURRENTDATAPASSWORDPROMPT_H_20100513__
#include "Tag.h"
namespace TAG_CURRENTDATAPASSWORDPROMPT_SPACE
{




// 当前数据密码提示
class Tag_CurrentDataPasswordPrompt : public Tag
{
protected:
    virtual string Get(Page *page);

public:
    Tag_CurrentDataPasswordPrompt(const string &page, const string &tag);
    virtual ~Tag_CurrentDataPasswordPrompt();

protected:
    Tag_CurrentDataPasswordPrompt();
    virtual int DoInit();
    virtual Tag *DoNew();
};







}// end of TAG_CURRENTDATAPASSWORDPROMPT_SPACE
//using namespace TAG_CURRENTDATAPASSWORDPROMPT_SPACE;
#endif // end of __TAG_CURRENTDATAPASSWORDPROMPT_H_20100513__
