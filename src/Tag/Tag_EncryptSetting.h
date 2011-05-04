#ifndef __TAG_ENCRYPTSETTING_H_20100506__
#define __TAG_ENCRYPTSETTING_H_20100506__
#include "Tag.h"
namespace TAG_ENCRYPTSETTING_SPACE
{








// 加密设置窗口
class Tag_EncryptSetting : public Tag
{
protected:
    virtual string Get(Page *page);

public:
    Tag_EncryptSetting(const string &page, const string &tag);
    virtual ~Tag_EncryptSetting();

protected:
    Tag_EncryptSetting();
    virtual int DoInit();
    virtual Tag *DoNew();
};













}// end of TAG_ENCRYPTSETTING_SPACE
//using namespace TAG_ENCRYPTSETTING_SPACE;
#endif // end of __TAG_ENCRYPTSETTING_H_20100506__
