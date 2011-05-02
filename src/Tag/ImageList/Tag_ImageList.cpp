// Rocky 2010-05-09 23:37:20
#include "Page.h"
#include "User.h"
#include "GetFileList.h"
#include "Tag_ImageList.h"
namespace TAG_IMAGELIST_SPACE
{








// ����ģ��
static const string THIS_MODULE = "ImageList";





Tag_ImageList::Tag_ImageList()
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_ImageList::Tag_ImageList(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_ImageList::~Tag_ImageList()
{
    FUNCTION_TRACK(); // �����켣����
}




int Tag_ImageList::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Tag::DoInit();
}

// ������󴴽���
Tag *Tag_ImageList::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Tag_ImageList;
}








/******************************** ҵ����� ********************************/





// ȡ�б� [2010-05]
string Tag_ImageList::Get(Page *page)
{
    FUNCTION_TRACK(); // �����켣����

    // ȡ��ǰ�û�
    const string &username = page->GetRequest()->GetCurrentUser();
    User *user = User::Get(username);
    // ��ǰ������Ŀ
    const string &key = page->GetCurrentKey();
    // ͼƬ���ڸ���Ŀ¼
    const string &path = user->AttachDir();
    // ��Ŀ¼��ȡ�ļ��б�
    GetFileList image( path );
    GetFileList::file_iterator it( image );
    string html = "";

    int nImageCount = 0;
    while( it.next() )
    {
        const char *basename = GetBaseName( it.name() );

        // ���������ڱ���Ŀ���ļ�
        if(strncmp(key.c_str(), basename, key.length()) != 0)
        {
            //LOG_DEBUG("Ignore: key=[%s], basename=[%s]", key.c_str(), basename);
            continue;
        }

        // ����ǰ׺(key + '.')
        basename += key.length() + 1;

        // �ļ���תΪurl����
        const string sFileOrg = FilenameDecode(basename);  // �ļ���������Ϊ����ʾ��ʽ��
        const string sFileUrl = UrlCode(basename);         // �ļ�����url��ʽ��

        // ������ͼƬ�ļ�
        const char *type = GetFileType( sFileOrg.c_str() );
        if( ! isImage(type) )
        {
            //LOG_DEBUG("Ignore: basename=[%s] type=[%s]", basename, type);
            continue;
        }

        html += ""
                "<li>"
                "  <input name='image' type='radio' id='image_" + IntToString(nImageCount) + "' "
                "     OnClick=\"PreviewImg('" + sFileUrl + "', '" + sFileOrg + "');\">"
                "  <label for='image_" + IntToString(nImageCount) + "'>" + sFileOrg + "</label>"
                "</li>"
                "\n";
        nImageCount++;
    }

    if(0 == nImageCount)
    {
        html =  ""
                "<h1>�ޱ�ѡͼƬ</h1><br>"
                "�������ڱ༭ҳ�����ϴ�ͼƬ����ˢ�±�ҳ�棻��"
                "\n";
    }

    LOG_DEBUG("nImageCount=[%d]", nImageCount);

    // �����Ƿ��Ա�־
    page->SetResult("ImageListCount", IntToString(nImageCount));

    return html;
}

















// ����Ϊȫ�ֱ�ǣ�
static Tag_ImageList tmp("ImageList", THIS_MODULE);

}// end of TAG_IMAGELIST_SPACE

