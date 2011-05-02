// Rocky 2010-05-14 18:59:27
#include "UserData.h"
#include "Page.h"
#include "Encrypt.h"
#include "ChineseCoding.h"
#include "Submit_TextInsertImage.h"
namespace SUBMIT_TEXTINSERTIMAGE_SPACE
{




// ����ģ�飨��Ӧ��ҳ���е��ύ�ֶΣ�
static const string THIS_MODULE = "TextInsertImage";





Submit_TextInsertImage::Submit_TextInsertImage()
{
    FUNCTION_TRACK(); // �����켣����
}

Submit_TextInsertImage::Submit_TextInsertImage(const string &page, const string &element)
                        : Submit(page, element)
{
    FUNCTION_TRACK(); // �����켣����
}

Submit_TextInsertImage::~Submit_TextInsertImage()
{
    FUNCTION_TRACK(); // �����켣����
}




int Submit_TextInsertImage::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Submit::DoInit();
}

// ������󴴽���
Submit *Submit_TextInsertImage::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Submit_TextInsertImage;
}





/******************************** ҵ����� ********************************/




/*
 * ������readingҳ���ύ���¼���Ŀǰ����
 *   1. ����ͼƬ��operate=InsertObj��type=""����ע��ֻ�޸������ֶΣ�
 *   2. ������ǩ��operate=InsertObj��type=bookmark����ע���޸����ġ�����ǩ�ֶΣ�
 */
int Submit_TextInsertImage::Deal(Page *page)
{
    const string &key = page->GetCurrentKey();
    const string &password = page->GetRequest()->GetField("password");
    const Conf *pack = page->GetCurrentPack();          // ��ǰkey��Ӧ���ݼ�

    /*
     * �Ⱥ˶�����
     */
    if( "" != pack->Get("password")
        && Crypt(password, CRYPT_VERSION) != pack->Get("password")
      )
    {
        LOG_DEBUG("password=[%s]", password.c_str());
        LOG_ERROR("Password error, text is't saved.");
        return ERR;
    }

    /*
     * �������в���ͼƬ������Ϊ������ܣ��ȳ����ڲ���㴦�޸ģ�Ϊ��ֹ
     * �ظ���������Ҫ��Σ�����ƥ�䴮�����ң��ٴ����ظ�ʱ����ִ��ȫ��
     * ���棬��˺Ϳͻ���֮����ж�ν�����
     */

    // ȡ������
    string text = page->GetRequest()->GetField("text");
    string find = page->GetRequest()->GetField("find");        // �滻ǰ�Ĵ�
    string replace = page->GetRequest()->GetField("replace");  // �滻��Ĵ�

    // ����ת����utf8 => gbk
    ChineseCoding code("utf-8", "gb18030");
    int ret;

    if( code.Converter(text, text) < 0
        || code.Converter(find, find) < 0
        || code.Converter(replace, replace) < 0
      )
    {
        LOG_ERROR("Converter error");
        return ERR;
    }

    // text�գ�˵���ͻ��˷������ǲ��ִ��������޸Ĳ��֣���ִ���������棻
    if("" == text)
    {
        // ��ȡ�������ݣ������Ǻ�̨���ݣ������ܵ����Ƚ��ܣ�
        text = pack->Get("text");
        if("" != password)
        {
            LOG_DEBUG("Decrypt...");
            text = Encrypt(password).decrypt(text);
        }

        // ת����web��ʾ��ʽ���Ա�ƥ�䣻
        text = TextToWeb( text );

        /*
         * �������в��ң������ζ��ҵ���˵�����ظ���"$find"�������
         */
        const unsigned int pos = text.find(find);
        if(pos == text.npos)
        {
            LOG_DEBUG("Can't find: [%s]", find.c_str());
            return ERR;
        }
        // �ٴβ���
        const unsigned int pos2 = text.find(find, pos + find.length());
        if(pos2 != text.npos)
        {
            LOG_DEBUG("String repeat: find=[%s], pos=[%u], pos2=[%u]",
                            find.c_str(), pos, pos2);
            return ERR;
        }

        /*
         * ������������replace�滻find��
         *  ��������װ���Σ�
         */
        const string &before = text.substr(0, pos);             // ԭ���У��滻��֮ǰ����
        const string &after = text.substr(pos + find.length()); // ԭ���У��滻��֮�󲿷�

        text = before + replace + after;
    }
    else
    {
        LOG_DEBUG("whole text save...");
    }

    text = WebHtmlToText( text );   // [XXX]

    // ԭ�Ǽ������ݣ���ִ�м��ܣ�
    if("" != password)
    {
        text = Encrypt(password).encrypt( text );
    }

    /*
     * ���ݴ��
     */
    Ini data;
    data.Set(key, "text", text);                      // ����
    data.Set(key, "modify", NowTime("%Y%m%d%H%M%S")); // ����޸�ʱ��

    /*
     * �ɴ���Ĳ���
     */
    const string &type = page->GetRequest()->GetField("type");
    // ������ǩ
    if("bookmark" == type)
    {
        string bookmark = page->GetRequest()->GetField("bookmark_data");
        if(code.Converter(bookmark, bookmark) < 0)
        {
            LOG_ERROR("Converter error");
            return ERR;
        }
        data.Set(key, "bookmark", bookmark);    // ��ǩ����
    }

    // ����
    return page->Save(data);
}














static Submit_TextInsertImage tmp("reading", THIS_MODULE);

}// end of SUBMIT_TEXTINSERTIMAGE_SPACE

