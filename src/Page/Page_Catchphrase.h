#ifndef __PAGE_CATCHPHRASE_H_20100428__
#define __PAGE_CATCHPHRASE_H_20100428__
#include "Page.h"
namespace PAGE_CATCHPHRASE_SPACE
{




// ��ҳͷ�����ʾ���Ծ���
class Page_Catchphrase : public Page
{
protected:
    virtual int OutBody();



public:
    Page_Catchphrase();
    virtual ~Page_Catchphrase();

protected:
    virtual int DoInit();
    virtual Page *DoNew();

private:
    static vector<string> m_sentence; // ��ž���
};







}// end of PAGE_CATCHPHRASE_SPACE
//using namespace PAGE_CATCHPHRASE_SPACE;
#endif // end of __PAGE_CATCHPHRASE_H_20100428__
