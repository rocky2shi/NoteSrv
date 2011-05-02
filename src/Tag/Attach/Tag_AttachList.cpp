// Rocky 2010-05-10 19:31:08
#include "Page.h"
#include "UserData.h"
#include "Tag_AttachList.h"
namespace TAG_ATTACHLIST_SPACE
{






// ����ģ��
static const string THIS_MODULE = "AttachList";





Tag_AttachList::Tag_AttachList()
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_AttachList::Tag_AttachList(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_AttachList::~Tag_AttachList()
{
    FUNCTION_TRACK(); // �����켣����
}




int Tag_AttachList::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Tag::DoInit();
}

// ������󴴽���
Tag *Tag_AttachList::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Tag_AttachList;
}








/******************************** ҵ����� ********************************/





// ȡ�б� [Rocky 2010-05-20]
string Tag_AttachList::Get(Page *page)
{
    FUNCTION_TRACK(); // �����켣����

    const Conf *pack = page->GetCurrentPack();  // ��ǰkey��Ӧ���ݼ�
    const string &password = pack->Get("password");
    const string &decryption = page->GetResult("decryption");

    // ����ǰ�û����������ݣ�����ʾ�����б�
    if( "" != password && "YES" != decryption )
    {
        LOG_DEBUG("User data have been encrypted.");
        return "";
    }

    // ȡ��ǰ�û�
    const string &username = page->GetRequest()->GetCurrentUser();
    // ��ǰ������Ŀ
    const string &key = page->GetCurrentKey();
    const string &pagename = page->GetRequest()->GetPageName();
    vector<string> attachs;
    vector<string>::iterator it;
    UserData(username).GetAttachList(key, attachs);   // ȡ�����б�
    string html = "";

    for(it = attachs.begin(); attachs.end() != it; it++)
    {
        const char *basename = GetBaseName( it->c_str() );

        // ���������ڱ���Ŀ���ļ�
        if(strncmp(key.c_str(), basename, key.length()) != 0)
        {
            //LOG_DEBUG("Ignore: key=[%s], basename=[%s]", key.c_str(), basename);
            continue;
        }

        // ȡ�ļ���С
        FileObj file( *it );
        const string &sFileSize = ByteSizeFormat( file.Size() );

        // ����ǰ׺(key + '.')
        basename += key.length() + 1;

        // �ļ���תΪurl����
        const string sFileOrg = FilenameDecode(basename);  // �ļ���������Ϊ����ʾ��ʽ��
        const string sFileUrl = UrlCode(basename);         // �ļ�����url��ʽ��

        /*
         * ��ͬ��ҳ�������ͬ�Ĵ�����ʽ
         */
        if("reading" == pagename)
        {
            // readingҳ�棨���ϵĸ����б�
            // <a href='download?key=20100521163715&file=logo%2Egif%2Epng' title='2.29KB' target='tmp_iframe'>logo.gif.png </a>
            html += " <a href='download?key=" + key + "&file=" + sFileUrl + "' title='" + sFileSize + "' target='tmp_iframe'>" + sFileOrg + " </a>\n";
        }
        else
        {
            // ������������ҳ������
            html += "  <li>\n"
                    "    <a href='download?key=" + key + "&file=" + sFileUrl + "' title='" + sFileSize + "' target='tmp_iframe'>" + sFileOrg + " </a>\n"
                    "    <a href='#' title='ɾ���ļ�' OnClick=\"Delete(this, '" + key + "', '" + sFileUrl + "', '" + sFileOrg + "')\">��</a>\n"
                    "  </li>\n"
                    "\n";
        }
    }// end of for(it = attachs.begin()...

    return html;
}













// ����Ϊȫ�ֱ�ǣ�
static Tag_AttachList tmp(TAG_GLOBAL, THIS_MODULE);

}// end of TAG_ATTACHLIST_SPACE
