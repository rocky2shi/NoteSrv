// Rocky 2010-05-19 19:09:18
#include "DataPack.h"
#include "Page_DataImport.h"
namespace PAGE_DATAIMPORT_SPACE
{








// ����ģ��
static const string THIS_MODULE = "DataImport";



Page_DataImport::Page_DataImport()
{
    FUNCTION_TRACK(); // �����켣����
}

Page_DataImport::~Page_DataImport()
{
    FUNCTION_TRACK(); // �����켣����
}


int Page_DataImport::DoInit()
{
    FUNCTION_TRACK(); // �����켣����
    return Page::DoInit();
}

// ������󴴽���
Page *Page_DataImport::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Page_DataImport;
}








/******************************** ҵ����� ********************************/


// ���������
int Page_DataImport::OutBody()
{
    FUNCTION_TRACK(); // �����켣����

    assert(NULL != m_request);

    const string &backup_import = m_request->GetField("backup_import");
    if("" != backup_import)
    {
        /*
         * ȡ�ϴ��Ĵ���ļ�
         */
        FileObj *attach = m_request->GetFileObj("pack");
        const string &pack = attach->Fullname();
        LOG_DEBUG("pack=[%s]", pack.c_str());

        /*
         * ���
         */
        const string &username = m_request->GetCurrentUser();
        DataPack unpack(username);
        if( unpack.UnPack(pack) < 0 )
        {
            LOG_ERROR("Unpack error");
            return ERR;
        }

        /*
         * ������ϣ�������ļ���
         */
        LOG_DEBUG("Delete file: [%s]", pack.c_str());
        DeleteFile(pack);

        /*
         * ȡ������
         */
        const string &success1 = unpack.GetResult("success.data");
        const string &success2 = unpack.GetResult("success.attach");
        const string msg = "�ɹ����룺����[" + success1 + "]��������[" + success2 + "]����";
        m_request->GetConnect()->Send( HtmlAlert(msg) );
        return OK;
    }

    return OK;
}







// ���嶯̬�����
DefinitinoDllEnter(Page_DataImport, THIS_MODULE)

}// end of PAGE_DATAIMPORT_SPACE
