// Rocky 2010-05-30 11:28:06
#include "User.h"
#include "GetFileList.h"
#include "Tag_UserList.h"
namespace TAG_USERLIST_SPACE
{

// 标明模块
static const string THIS_MODULE = "UserList";





Tag_UserList::Tag_UserList()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_UserList::Tag_UserList(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_UserList::~Tag_UserList()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Tag_UserList::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Tag::DoInit();
}

// 子类对象创建器
Tag *Tag_UserList::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Tag_UserList;
}








/******************************** 业务代码 ********************************/





// 取标题 [2010-05]
string Tag_UserList::Get(Page *page)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    // 取用户存放目录
    const string &users = GlobalConfig::instance()->UserRootDir();
    // 遍历
    GetFileList dir( users );
    GetFileList::dir_iterator it(dir);
    string html = "";

    while( it.next() )
    {
        const string username = GetBaseName(it.name());
        User *user = User::Get( username );

        /*
         *   <tr>
         *   <td>用户名</td>
         *   <td>注册时间</td>
         *   <td>最后登录时间</td>
         *   <td>登录次数</td>
         *   <td>操作</td>
         *   </tr>
         *
         *  注意，有些信息可能位于不同段中：如regtime位于[login]，而
         *  lasttime位于[userinfo]中；
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
                "  <td><input type=button value='删除' onclick='DeleteUser(\"" + username + "\", this)'></td>\n"
                "</tr>\n"
                "";
    }

    return html;
}








static Tag_UserList tmp1("administrator", THIS_MODULE );

}// end of TAG_USERLIST_SPACE

