// Rocky 2010-05-09 01:47:16
#include "User.h"
#include "GetFileList.h"
#include "Page_ImageList.h"
namespace PAGE_IMAGELIST_SPACE
{









// ����ģ��
static const string THIS_MODULE = "ImageList";



Page_ImageList::Page_ImageList()
{
    FUNCTION_TRACK(); // �����켣����
}

Page_ImageList::~Page_ImageList()
{
    FUNCTION_TRACK(); // �����켣����
}


int Page_ImageList::DoInit()
{
    FUNCTION_TRACK(); // �����켣����
    m_html = GlobalConfig::instance()->TemplateDir() + "ImageList.html";
    return Page::DoInit();
}

// ������󴴽���
Page *Page_ImageList::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Page_ImageList;
}








/******************************** ҵ����� ********************************/


// ���������
int Page_ImageList::Deal()
{
    FUNCTION_TRACK(); // �����켣����

    assert(NULL != m_request);

    return Page::Deal();
}








// ���嶯̬�����
DefinitinoDllEnter(Page_ImageList, THIS_MODULE)

}// end of PAGE_IMAGELIST_SPACE

