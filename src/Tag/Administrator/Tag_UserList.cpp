// Rocky 2010-05-30 11:28:06
#include "User.h"
#include "GetFileList.h"
#include "Tag_UserList.h"
namespace TAG_USERLIST_SPACE
{

// ����ģ��
static const string THIS_MODULE = "UserList";





Tag_UserList::Tag_UserList()
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_UserList::Tag_UserList(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_UserList::~Tag_UserList()
{
    FUNCTION_TRACK(); // �����켣����
}




int Tag_UserList::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Tag::DoInit();
}

// ������󴴽���
Tag *Tag_UserList::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Tag_UserList;
}








/******************************** ҵ����� ********************************/





// ȡ���� [2010-05]
string Tag_UserList::Get(Page *page)
{
    FUNCTION_TRACK(); // �����켣����

    // ȡ�û����Ŀ¼
    const string &users = GlobalConfig::instance()->UserRootDir();
    // ����
    GetFileList dir( users );
    GetFileList::dir_iterator it(dir);
    string html = "";

    while( it.next() )
    {
        const string username = GetBaseName(it.name());
        User *user = User::Get( username );

        /*
         *   <tr>
         *   <td>�û���</td>
         *   <td>ע��ʱ��</td>
         *   <td>����¼ʱ��</td>
         *   <td>��¼����</td>
         *   <td>����</td>
         *   </tr>
         *
         *  ע�⣬��Щ��Ϣ����λ�ڲ�ͬ���У���regtimeλ��[login]����
         *  lasttimeλ��[userinfo]�У�
         */
        const string &regtime = TimeTo( KeyToSecond(user->GetInfo("regtime")) );
        const string &lasttime = TimeTo( KeyToSecond(user->GetInfo("login", "lasttime")) );
        const string &counter = EmptyStringToDefault(user->GetInfo("login", "counter"), "0");
        html += ""
                "<tr>\n"
                "  <td id='ID_USER_" + username + "'>" + username + "</td>\n"
                "  <td>" + regtime + "</td>\n"
                "  <td>" + lasttime + "</td>\n"
                "  <td>" + counter + "</td>\n"
                "  <td><input type=button value='ɾ��' onclick='DeleteUser(\"" + username + "\", this)'></td>\n"
                "</tr>\n"
                "";
    }

    return html;
}








static Tag_UserList tmp1("administrator", THIS_MODULE );

}// end of TAG_USERLIST_SPACE

