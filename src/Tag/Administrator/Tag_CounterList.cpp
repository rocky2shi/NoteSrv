// Rocky 2010-05-30 01:06:31
#include "Counter.h"
#include "Tag_CounterList.h"
namespace TAG_COUNTERLIST_SPACE
{

// ����ģ��
static const string THIS_MODULE = "CounterList";





Tag_CounterList::Tag_CounterList()
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_CounterList::Tag_CounterList(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_CounterList::~Tag_CounterList()
{
    FUNCTION_TRACK(); // �����켣����
}




int Tag_CounterList::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Tag::DoInit();
}

// ������󴴽���
Tag *Tag_CounterList::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Tag_CounterList;
}








/******************************** ҵ����� ********************************/





//
string Tag_CounterList::Get(Page *page)
{
    FUNCTION_TRACK(); // �����켣����

    /*
     * �������б�
     */
    string html;
    Counter::iterator it;

    // �г����м�����
    while( it.next() )
    {
        html += "  <tr>\n"
                "    <td>" + it->GetName() + "</td>\n"
                "    <td>" + IntToString( it->GetCount() ) + "</td>\n"
                "  </tr>\n"
                "";
    }

    return html;
}








static Tag_CounterList tmp1("administrator", THIS_MODULE );

}// end of TAG_COUNTERLIST_SPACE

