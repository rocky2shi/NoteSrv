#ifndef __TAG_TYPEDROPBOXMENUSELECEDITEMS_H_20100513__
#define __TAG_TYPEDROPBOXMENUSELECEDITEMS_H_20100513__
#include "Tag.h"
namespace TAG_TYPEDROPBOXMENUSELECEDITEMS_SPACE
{





// ��ǰ����ѡ�е�����ѡ��������˵������б�
class Tag_TypeDropBoxMenuSelecedItems : public Tag
{
protected:
    virtual string Get(Page *page);

public:
    Tag_TypeDropBoxMenuSelecedItems(const string &page, const string &tag);
    virtual ~Tag_TypeDropBoxMenuSelecedItems();

protected:
    Tag_TypeDropBoxMenuSelecedItems();
    virtual int DoInit();
    virtual Tag *DoNew();
};







}// end of TAG_TYPEDROPBOXMENUSELECEDITEMS_SPACE
//using namespace TAG_TYPEDROPBOXMENUSELECEDITEMS_SPACE;
#endif // end of __TAG_TYPEDROPBOXMENUSELECEDITEMS_H_20100513__
