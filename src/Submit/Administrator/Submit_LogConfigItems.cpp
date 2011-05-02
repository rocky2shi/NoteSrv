// Rocky 2010-05-28 14:34:39
#include "Page.h"
#include "Submit_LogConfigItems.h"
namespace SUBMIT_LOGCONFIGITEMS_SPACE
{



// ����ģ�飨��Ӧ��ҳ���е��ύ�ֶΣ�
static const string THIS_MODULE = "LogConfigItems";





Submit_LogConfigItems::Submit_LogConfigItems()
{
    FUNCTION_TRACK(); // �����켣����
}

Submit_LogConfigItems::Submit_LogConfigItems(const string &page, const string &element)
                        : Submit(page, element)
{
    FUNCTION_TRACK(); // �����켣����
}

Submit_LogConfigItems::~Submit_LogConfigItems()
{
    FUNCTION_TRACK(); // �����켣����
}




int Submit_LogConfigItems::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Submit::DoInit();
}

// ������󴴽���
Submit *Submit_LogConfigItems::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Submit_LogConfigItems;
}





/******************************** ҵ����� ********************************/



// [Rocky 2010-05-28 15:04:36]
int Submit_LogConfigItems::Deal(Page *page)
{
    // ������־
    const string &level_debug = page->GetRequest()->GetField("level_debug");
    Log::instance()->SetLevel( Log::DEBUG, ("open" == level_debug ? true : false) );
    LOG_DEBUG("level_debug=[%s]", level_debug.c_str());

    // һ����־
    const string &level_info = page->GetRequest()->GetField("level_info");
    Log::instance()->SetLevel( Log::INFO, ("open" == level_info ? true : false) );
    LOG_DEBUG("level_info=[%s]", level_info.c_str());

    // ������־
    const string &level_error = page->GetRequest()->GetField("level_error");
    Log::instance()->SetLevel( Log::ERROR, ("open" == level_error ? true : false) );
    LOG_DEBUG("level_error=[%s]", level_error.c_str());

    return OK;
}













static Submit_LogConfigItems tmp("administrator", THIS_MODULE);

}// end of SUBMIT_LOGCONFIGITEMS_SPACE
