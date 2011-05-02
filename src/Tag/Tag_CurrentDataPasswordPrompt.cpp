// Rocky 2010-05-13 16:22:07
#include "UserData.h"
#include "Page.h"
#include "Tag_CurrentDataPasswordPrompt.h"
namespace TAG_CURRENTDATAPASSWORDPROMPT_SPACE
{




// ����ģ��
static const string THIS_MODULE = "CurrentDataPasswordPrompt";





Tag_CurrentDataPasswordPrompt::Tag_CurrentDataPasswordPrompt()
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_CurrentDataPasswordPrompt::Tag_CurrentDataPasswordPrompt(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_CurrentDataPasswordPrompt::~Tag_CurrentDataPasswordPrompt()
{
    FUNCTION_TRACK(); // �����켣����
}




int Tag_CurrentDataPasswordPrompt::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Tag::DoInit();
}

// ������󴴽���
Tag *Tag_CurrentDataPasswordPrompt::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Tag_CurrentDataPasswordPrompt;
}








/******************************** ҵ����� ********************************/





// ȡ���� [2010-05]
string Tag_CurrentDataPasswordPrompt::Get(Page *page)
{
    FUNCTION_TRACK(); // �����켣����

    const Conf *pack = page->GetCurrentPack();          // ��ǰkey��Ӧ���ݼ�
    return pack->Get("prompt");
}

















// ����Ϊȫ�ֱ�ǣ�
static Tag_CurrentDataPasswordPrompt tmp(TAG_GLOBAL, THIS_MODULE);

}// end of TAG_CURRENTDATAPASSWORDPROMPT_SPACE
