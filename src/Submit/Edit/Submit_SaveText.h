#ifndef __SUBMIT_SAVETEXT_H_20100515__
#define __SUBMIT_SAVETEXT_H_20100515__
namespace SUBMIT_SAVETEXT_SPACE
{







// 处理正文内容提交事件
class Submit_SaveText : public Submit
{
protected:
    virtual int Deal(Page *page);

    int Save(Page *page);
    int AdjustType(Page *page, const string &types);

public:
    Submit_SaveText();
    Submit_SaveText(const string &page, const string &element);
    virtual ~Submit_SaveText();

protected:
    virtual int DoInit();
    virtual Submit *DoNew();
};










}// end of SUBMIT_SAVETEXT_SPACE
//using namespace SUBMIT_SAVETEXT_SPACE;
#endif // end of __SUBMIT_SAVETEXT_H_20100515__

