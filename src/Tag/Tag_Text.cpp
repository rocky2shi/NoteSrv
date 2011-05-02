// Rocky 2010-05-03 17:36:28
#include "Tag_Text.h"
#include "Encrypt.h"
#include "Page.h"
#include "UserData.h"
namespace TAG_TEXT_SPACE
{




// ����ģ��
static const string THIS_MODULE = "Text";





Tag_Text::Tag_Text()
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_Text::Tag_Text(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_Text::~Tag_Text()
{
    FUNCTION_TRACK(); // �����켣����
}




int Tag_Text::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Tag::DoInit();
}

// ������󴴽���
Tag *Tag_Text::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Tag_Text;
}








/******************************** ҵ����� ********************************/



// ת������
const string Tag_Text::ThisTextToWeb(const string &txt) const
{
    return ::TextToWebHtml(txt);
}


string Tag_Text::Get(Page *page)
{
    FUNCTION_TRACK(); // �����켣����

    const Conf *pack = page->GetCurrentPack();   // ��ǰkey��Ӧ���ݼ�
    const string &text = pack->Get("text");     // ȡ����������
    const string &password = pack->Get("password");
    const string &decryption = page->GetResult("decryption");

    LOG_DEBUG("[%s] [%s]", password.c_str(), decryption.c_str());

    /* ���������Ƿ�������ʾ��һ������£����ֶ�Ϊ�գ�ֻ�е���
     * ���Ǽ��ܵġ��ҽ��سɹ�ʱ��decryption�ű���Ϊ"YES"��
     * ��ע����ֵ��Submit_Decryption::Deal()���д���
     */
    if( "" != password && "YES" != decryption )
    {
        LOG_DEBUG("Data have been encrypted, forbidding them to display.");
        return "";
    }

    // �Ǽ������ݣ����ܺ������
    if("" != password)
    {
        // ��key��Ӧ���ݵ����루ע�⣬�ǿͻ��˴������������룩
        const string &paswd = page->GetRequest()->GetField("password");
        const string &str = Encrypt(paswd).decrypt( text );
        LOG_DEBUG("Decrypt...");
        return ThisTextToWeb( str );
    }

    LOG_DEBUG("Not need to decrypt...");

    // δ��������
    return ThisTextToWeb( text );
}

















// ����Ϊȫ�ֱ�ǣ�
static Tag_Text tmp(TAG_GLOBAL, THIS_MODULE);

}// end of TAG_TEXT_SPACE

