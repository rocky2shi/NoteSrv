#ifndef __TAG_CURRENTPASSWORD_H_20100513__
#define __TAG_CURRENTPASSWORD_H_20100513__
#include "Tag.h"
namespace TAG_CURRENTPASSWORD_SPACE
{







// ��ǰ�����м������õ����루���ģ���ע�⣬�����û����룩
class Tag_CurrentPassword : public Tag
{
protected:
    virtual string Get(Page *page);

public:
    Tag_CurrentPassword(const string &page, const string &tag);
    virtual ~Tag_CurrentPassword();

protected:
    Tag_CurrentPassword();
    virtual int DoInit();
    virtual Tag *DoNew();
};






}// end of TAG_CURRENTPASSWORD_SPACE
//using namespace TAG_CURRENTPASSWORD_SPACE;
#endif // end of __TAG_CURRENTPASSWORD_H_20100513__
