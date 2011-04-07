#ifndef __SUBMIT_DECRYPTION_H_20100512__
#define __SUBMIT_DECRYPTION_H_20100512__
#include "Submit.h"
namespace SUBMIT_DECRYPTION_SPACE
{






// 验证“查看加密数据输入窗口”提交的数据
class Submit_Decryption : public Submit
{
protected:
    virtual int Deal(Page *page);


public:
    Submit_Decryption();
    Submit_Decryption(const string &page, const string &element);
    virtual ~Submit_Decryption();

protected:
    virtual int DoInit();
    virtual Submit *DoNew();
};









}// end of SUBMIT_DECRYPTION_SPACE
//using namespace SUBMIT_DECRYPTION_SPACE;
#endif // end of __SUBMIT_DECRYPTION_H_20100512__

