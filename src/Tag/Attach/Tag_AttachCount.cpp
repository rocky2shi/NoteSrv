// Rocky 2010-05-10 19:47:22
#include "Page.h"
#include "UserData.h"
#include "Tag_AttachCount.h"
namespace TAG_ATTACHCOUNT_SPACE
{


// ����ģ��
static const string THIS_MODULE = "AttachCount";





Tag_AttachCount::Tag_AttachCount()
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_AttachCount::Tag_AttachCount(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_AttachCount::~Tag_AttachCount()
{
    FUNCTION_TRACK(); // �����켣����
}




int Tag_AttachCount::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Tag::DoInit();
}

// ������󴴽���
Tag *Tag_AttachCount::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Tag_AttachCount;
}








/******************************** ҵ����� ********************************/





// ȡ���� [2010-05]
string Tag_AttachCount::Get(Page *page)
{
    FUNCTION_TRACK(); // �����켣����

    const Conf *pack = page->GetCurrentPack();
    const string &attach_num = pack->Get("attach_num");

    /*
     * ���µ����ݣ��������ɴ��ֶμ�¼��ֱ��ȡ��
     */
    if("" != attach_num)
    {
        return attach_num;
    }

    /*
     * ���ھ����ݣ������������ȡ������
     */
    const string &username = page->GetRequest()->GetCurrentUser();
    const string &key = page->GetCurrentKey();
    vector<string> attachs;
    UserData(username).GetAttachList(key, attachs);   // ȡ�����б�

    return IntToString( attachs.size() );
}

















// ����Ϊȫ�ֱ�ǣ�
static Tag_AttachCount tmp("attach", THIS_MODULE);

}// end of TAG_ATTACHCOUNT_SPACE
