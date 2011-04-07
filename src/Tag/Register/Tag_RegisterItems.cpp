// Rocky 2010-05-23 20:16:43
#include "User.h"
#include "Request.h"
#include "Page.h"
#include "Tag_RegisterItems.h"
namespace TAG_REGISTERITEMS_SPACE
{


// 标明模块
//static const string THIS_MODULE = "BrowseFilterItems";




Tag_RegisterItems::Tag_RegisterItems()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_RegisterItems::Tag_RegisterItems(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_RegisterItems::~Tag_RegisterItems()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Tag_RegisterItems::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Tag::DoInit();
}

// 子类对象创建器
Tag *Tag_RegisterItems::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Tag_RegisterItems;
}








/******************************** 业务代码 ********************************/





// 取标题 [2010-05]
string Tag_RegisterItems::Get(Page *page)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    const Request *request = page->GetRequest();
    const string &username = request->GetField("username");

    // 用户名
    if("username" == m_id)
    {
        return username;
    }
    // 找回密码 Step1：输入用户名
    else if("stet1_display" == m_id)
    {
        return "" == request->GetField("next_step") ? "" : "none";
    }
    // 找回密码 Step2：找回密码
    else if("stet2_display" == m_id)
    {
        return "" == request->GetField("next_step") ? "none" : "";
    }
    else if("question" == m_id)
    {
        if("" != username)
        {
            const User *user = User::Get(username);
            return user->GetInfo(m_id);
        }
    }
    // 设置标签页显示
    else if("tag_register" == m_id)
    {
        if("register" == request->GetField("tag")
           || "register" == request->GetField("tag_page")
          )
        {
            return "true";
        }
        else
        {
            return "false";
        }
    }
    else if("tag_retrieve" == m_id)
    {
        if("retrieve" == request->GetField("tag")
           || "retrieve" == request->GetField("tag_page")
           || "" != request->GetField("next_step")
           || "" != request->GetField("prev_step")
          )
        {
            return "true";
        }
        else
        {
            return "false";
        }
    }
    else if("tag_modify" == m_id)
    {
        if("modify" == request->GetField("tag")
           || "modify" == request->GetField("tag_page")
          )
        {
            return "true";
        }
        else
        {
            return "false";
        }
    }

    return "";
}








// 处理多个项
static Tag_RegisterItems tmp1("register", "stet1_display");
static Tag_RegisterItems tmp2("register", "stet2_display");
static Tag_RegisterItems tmp3("register", "question");
static Tag_RegisterItems tmp5("register", "tag_register");
static Tag_RegisterItems tmp6("register", "tag_retrieve");
static Tag_RegisterItems tmp7("register", "tag_modify");
static Tag_RegisterItems tmp8("register", "username");

}// end of TAG_REGISTERITEMS_SPACE

