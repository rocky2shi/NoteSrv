#ifndef __TAG_TYPEDROPITEMLIST_H_20100524__
#define __TAG_TYPEDROPITEMLIST_H_20100524__
#include "Tag.h"
namespace TAG_TYPEDROPITEMLIST_SPACE
{





// ����ѡ��������˵����б�
class Tag_TypeDropItemList : public Tag
{
protected:
    virtual string Get(Page *page);


public:
    Tag_TypeDropItemList(const string &page, const string &tag);
    virtual ~Tag_TypeDropItemList();

protected:
    Tag_TypeDropItemList();
    virtual int DoInit();
    virtual Tag *DoNew();
};






}// end of TAG_TYPEDROPITEMLIST_SPACE
//using namespace TAG_TYPEDROPITEMLIST_SPACE;
#endif // end of __TAG_TYPEDROPITEMLIST_H_20100524__
