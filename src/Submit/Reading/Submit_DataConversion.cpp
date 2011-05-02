// Rocky 2010-05-25 16:57:22
#include "Conf.h"
#include "Page.h"
#include "Submit_DataConversion.h"
namespace SUBMIT_DATACONVERSION_SPACE
{




// ����ģ��
static const string THIS_MODULE = "DataConversion";





Submit_DataConversion::Submit_DataConversion()
{
    FUNCTION_TRACK(); // �����켣����
}

Submit_DataConversion::Submit_DataConversion(const string &page, const string &element)
                        : Submit(page, element)
{
    FUNCTION_TRACK(); // �����켣����
}

Submit_DataConversion::~Submit_DataConversion()
{
    FUNCTION_TRACK(); // �����켣����
}




int Submit_DataConversion::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Submit::DoInit();
}

// ������󴴽���
Submit *Submit_DataConversion::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Submit_DataConversion;
}





/******************************** ҵ����� ********************************/





int Submit_DataConversion::Deal(Page *page)
{
    const string &key = page->GetCurrentKey();
    Conf *pack = page->GetCurrentPack();
    string text = TextToWeb( pack->Get("text") );
    int ret;

    if("" == text)
    {
        return OK;
    }

    const string tmpfile = StringToTempFile(text);

    // ���ⲿ�������ļ�
    const string cmd = GlobalConfig::instance()->ToolDir() + "DataConversion.pl " + tmpfile;
    ret = system(cmd.c_str());
    LOG_DEBUG("ret=[%d] cmd=[%s]", ret, cmd.c_str());
    if(0 != ret)
    {
        LOG_ERROR("Command error: [%s]", cmd.c_str());
        return ERR;
    }

    // �ų�ת���������
    text = FileToStr(tmpfile);

    // ������ϣ�ɾ����ʱ�ļ���
    DeleteFile(tmpfile);

    // ���
    Ini item;
    item.Set(key, "text", WebToText(text));
    item.Set(key, "modify", NowTime("%Y%m%d%H%M%S"));

    // ����
    return page->Save( item );
}














static Submit_DataConversion tmp("reading", THIS_MODULE);

}// end of SUBMIT_DATACONVERSION_SPACE
