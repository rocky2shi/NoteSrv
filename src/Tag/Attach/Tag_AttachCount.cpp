// Rocky 2010-05-10 19:47:22
#include "Page.h"
#include "UserData.h"
#include "Tag_AttachCount.h"
namespace TAG_ATTACHCOUNT_SPACE
{


// 标明模块
static const string THIS_MODULE = "AttachCount";





Tag_AttachCount::Tag_AttachCount()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_AttachCount::Tag_AttachCount(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_AttachCount::~Tag_AttachCount()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Tag_AttachCount::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Tag::DoInit();
}

// 子类对象创建器
Tag *Tag_AttachCount::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Tag_AttachCount;
}








/******************************** 业务代码 ********************************/





// 取标题 [2010-05]
string Tag_AttachCount::Get(Page *page)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    const Conf *pack = page->GetCurrentPack();
    const string &attach_num = pack->Get("attach_num");

    /*
     * 最新的数据，附件数由此字段记录，直接取；
     */
    if("" != attach_num)
    {
        return attach_num;
    }

    /*
     * 对于旧数据，则遍历附件来取个数；
     */
    const string &username = page->GetRequest()->GetCurrentUser();
    const string &key = page->GetCurrentKey();
    vector<string> attachs;
    UserData(username).GetAttachList(key, attachs);   // 取附件列表

    return IntToString( attachs.size() );
}

















// 设置为全局标记；
static Tag_AttachCount tmp("attach", THIS_MODULE);

}// end of TAG_ATTACHCOUNT_SPACE
