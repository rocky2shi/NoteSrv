#ifndef __SUBMIT_TEXTINSERTIMAGE_H_20100514__
#define __SUBMIT_TEXTINSERTIMAGE_H_20100514__
#include "Submit.h"
namespace SUBMIT_TEXTINSERTIMAGE_SPACE
{






// ���������в���ͼƬ����ǩ���ύ�¼�
class Submit_TextInsertImage : public Submit
{
protected:
    virtual int Deal(Page *page);


public:
    Submit_TextInsertImage();
    Submit_TextInsertImage(const string &page, const string &element);
    virtual ~Submit_TextInsertImage();

protected:
    virtual int DoInit();
    virtual Submit *DoNew();
};






}// end of SUBMIT_TEXTINSERTIMAGE_SPACE
//using namespace SUBMIT_TEXTINSERTIMAGE_SPACE;
#endif // end of __SUBMIT_TEXTINSERTIMAGE_H_20100514__
