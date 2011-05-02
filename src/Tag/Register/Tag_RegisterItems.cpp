// Rocky 2010-05-23 20:16:43
#include "User.h"
#include "Request.h"
#include "Page.h"
#include "Tag_RegisterItems.h"
namespace TAG_REGISTERITEMS_SPACE
{


// ����ģ��
//static const string THIS_MODULE = "BrowseFilterItems";




Tag_RegisterItems::Tag_RegisterItems()
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_RegisterItems::Tag_RegisterItems(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_RegisterItems::~Tag_RegisterItems()
{
    FUNCTION_TRACK(); // �����켣����
}




int Tag_RegisterItems::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Tag::DoInit();
}

// ������󴴽���
Tag *Tag_RegisterItems::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Tag_RegisterItems;
}








/******************************** ҵ����� ********************************/





// ȡ���� [2010-05]
string Tag_RegisterItems::Get(Page *page)
{
    FUNCTION_TRACK(); // �����켣����

    const Request *request = page->GetRequest();
    const string &username = request->GetField("username");

    // �û���
    if("username" == m_id)
    {
        return username;
    }
    // �һ����� Step1�������û���
    else if("stet1_display" == m_id)
    {
        return "" == request->GetField("next_step") ? "" : "none";
    }
    // �һ����� Step2���һ�����
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
    // ���ñ�ǩҳ��ʾ
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








// ��������
static Tag_RegisterItems tmp1("register", "stet1_display");
static Tag_RegisterItems tmp2("register", "stet2_display");
static Tag_RegisterItems tmp3("register", "question");
static Tag_RegisterItems tmp5("register", "tag_register");
static Tag_RegisterItems tmp6("register", "tag_retrieve");
static Tag_RegisterItems tmp7("register", "tag_modify");
static Tag_RegisterItems tmp8("register", "username");

}// end of TAG_REGISTERITEMS_SPACE

