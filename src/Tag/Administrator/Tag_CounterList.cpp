// Rocky 2010-05-30 01:06:31
#include "Counter.h"
#include "Tag_CounterList.h"
namespace TAG_COUNTERLIST_SPACE
{

// 标明模块
static const string THIS_MODULE = "CounterList";





Tag_CounterList::Tag_CounterList()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_CounterList::Tag_CounterList(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_CounterList::~Tag_CounterList()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Tag_CounterList::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Tag::DoInit();
}

// 子类对象创建器
Tag *Tag_CounterList::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Tag_CounterList;
}








/******************************** 业务代码 ********************************/





//
string Tag_CounterList::Get(Page *page)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    /*
     * 计数器列表
     */
    string html;
    Counter::iterator it;

    // 列出所有计数器
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

