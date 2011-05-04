#ifndef __TAG_ESC_H_20100506__
#define __TAG_ESC_H_20100506__
namespace TAG_ESC_SPACE
{







// 当前页面中Esc键对应函数
class Tag_Esc : public Tag
{
protected:
    virtual string Get(Page *page);

public:
    Tag_Esc(const string &page, const string &tag);
    virtual ~Tag_Esc();

protected:
    Tag_Esc();
    virtual int DoInit();
    virtual Tag *DoNew();
};











}// end of TAG_ESC_SPACE
//using namespace TAG_ESC_SPACE;
#endif // end of __TAG_ESC_H_20100506__

