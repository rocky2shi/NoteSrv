// Rocky 2010-05-03 02:36:14
#include "UserData.h"
#include "Encrypt.h"
#include "Page_Reading.h"
namespace PAGE_READING_SPACE
{











// ����ģ��
static const string THIS_MODULE = "reading";



Page_Reading::Page_Reading()
{
    FUNCTION_TRACK(); // �����켣����
}

Page_Reading::~Page_Reading()
{
    FUNCTION_TRACK(); // �����켣����
}


int Page_Reading::DoInit()
{
    FUNCTION_TRACK(); // �����켣����
    m_html = GlobalConfig::instance()->TemplateDir() + "Reading.html";
    return Page::DoInit();
}

// ������󴴽���
Page *Page_Reading::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Page_Reading;
}








/******************************** ҵ����� ********************************/

int Page_Reading::OutBody()
{
    FUNCTION_TRACK(); // �����켣����

    assert(NULL != m_request);


    /*
     * ����������ʾ��������ύ�¼�
     */
    const string &set_text_style = m_request->GetField("set_text_style");
    if("" != set_text_style)
    {
        Submit::Ptr submit( Submit::New("reading", "TextStyleSetting") );
        submit->Deal(this);
        return OK;
    }
    LOG_DEBUG("set_text_style=[%s]", set_text_style.c_str());

    /*
     * ���������в���ͼƬ����ǩ���ύ�¼�
     */
    const string &operate = m_request->GetField("operate");
    if("InsertObj" == operate)
    {
        Submit::Ptr submit( Submit::New("reading", "TextInsertImage") );
        if(submit->Deal(this) < 0)
        {
            m_request->GetConnect()->Send("[ERROR]");
            return OK;
        }
        m_request->GetConnect()->Send("[OK]");
        return OK;
    }
    LOG_DEBUG("operate=[%s]", operate.c_str());

    /*
     * �Ƿ�Ϊ�����ύ�¼�
     */
    const string &decryption = m_request->GetField("decryption");
    if("" != decryption)
    {
        Submit::Ptr submit( Submit::New(SUBMIT_GLOBAL, "decryption") );
        /*
         * ��֤δͨ������submit->Deal()�����ڲ�����encryptionΪ"YES"��
         * ������ʾ�ȴ�����ݴ�ֵ�����Ƿ���ʾ���ݡ�
         */
        submit->Deal(this);
    }

    /*
     * ��������ת�����¡������ݼ䣩�ύ�¼�
     */
    const string &conversion = m_request->GetField("DataConversion");
    if("" != conversion)
    {
        Submit::Ptr submit( Submit::New("reading", "DataConversion") );
        if(submit->Deal(this) < 0)
        {
            m_request->GetConnect()->Send("ERROR");
        }
        else
        {
            m_request->GetConnect()->Send("OK");
        }
        return OK;
    }

    return Page::OutBody();
}







// ���嶯̬�����
DefinitinoDllEnter(Page_Reading, THIS_MODULE)

}// end of PAGE_READING_SPACE

