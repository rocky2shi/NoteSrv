#ifndef __TAG_AUTHENTICATIONPROMPT_H_20100511__
#define __TAG_AUTHENTICATIONPROMPT_H_20100511__
#include "Tag.h"
namespace TAG_AUTHENTICATIONPROMPT_SPACE
{








// 查看加密数据输入窗口
class Tag_AuthenticationPrompt : public Tag
{
protected:
    virtual string Get(Page *page);

public:
    Tag_AuthenticationPrompt(const string &page, const string &tag);
    virtual ~Tag_AuthenticationPrompt();

protected:
    Tag_AuthenticationPrompt();
    virtual int DoInit();
    virtual Tag *DoNew();
};






}// end of TAG_AUTHENTICATIONPROMPT_SPACE
//using namespace TAG_AUTHENTICATIONPROMPT_SPACE;
#endif // end of __TAG_AUTHENTICATIONPROMPT_H_20100511__
