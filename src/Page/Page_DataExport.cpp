// Rocky 2010-05-19 17:36:42
#include "DataPack.h"
#include "Page_DataExport.h"
namespace PAGE_DATAEXPORT_SPACE
{









// ����ģ��
static const string THIS_MODULE = "DataExport";



Page_DataExport::Page_DataExport()
{
    FUNCTION_TRACK(); // �����켣����
}

Page_DataExport::~Page_DataExport()
{
    FUNCTION_TRACK(); // �����켣����
}


int Page_DataExport::DoInit()
{
    FUNCTION_TRACK(); // �����켣����
    return Page::DoInit();
}

// ������󴴽���
Page *Page_DataExport::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Page_DataExport;
}








/******************************** ҵ����� ********************************/


// ���httpЭ��ͷ��
int Page_DataExport::OutHead()
{
    FUNCTION_TRACK(); // �����켣����

    Connect * const connect = m_request->GetConnect();
    const string &username = m_request->GetCurrentUser();
    const string &backup_range = m_request->GetField("backup_range"); // ������Χ
    const string &include_attach = m_request->GetField("include_attach");
    DataPack pack(username);
    string fullpath;
    string filename; // �����ͻ��˵��ļ���

    // �Ƿ����ǵ�������
    if("yes" != include_attach)
    {
        // �ų�����
        pack.SetAttrib(DataPack::A_EXCLUDE_ATTACH);
    }

    if("all" == backup_range)
    {
        /*
         * ������������
         */
        if( pack.MakePack() == ERR
            || pack.AddAll() == ERR
          )
        {
            LOG_ERROR("Export data error");
            return ERR;
        }
    }
    else if("select" == backup_range)
    {
        /*
         * ����ѡ��������
         */
        int i;
        vector<string> keys;
        vector<string>::iterator it;
        UserData data( username );
        Split(m_request->GetField("select_key"), " ", keys); // �ָΪ����

        LOG_DEBUG("keys.size=[%d]", keys.size());
        LOG_DEBUG("keys=[%s]", m_request->GetField("select_key").c_str());

        if( pack.MakePack() == ERR )
        {
            LOG_ERROR("Export data error");
            return ERR;
        }

        it = keys.begin();
        while(1)
        {
            if(keys.end() == it)
            {
                break;
            }

            Ini group;

            // ��50��Ϊһ��
            for(i=0; i<50 && keys.end() != it; it++)
            {
                const string &key = *it;
                if("" == key)
                {
                    continue;
                }
                group.Set(key, Conf()); // �����һ�յ�Conf()
                LOG_DEBUG("key=[%s]", key.c_str());
            }

            // ȡkey���Ӧ����
            if( data.Get( group ) < 0 )
            {
                LOG_ERROR("Get data error, username=[%s]", username.c_str());
                return ERR;
            }

            if( pack.Add( group ) == ERR )
            {
                LOG_ERROR("Pack Add error");
                return ERR;
            }
        }// end of while(1...
    }// end of else if("select" ==...
    else
    {
        LOG_ERROR("Range error: [%s]", backup_range.c_str());
        return OK;
    }

    filename = username + "." + GetCurrentKey() + ".pkg";   // ���͵��ͻ��˵��ļ���
    fullpath = pack.PackFinish(); // �����ɣ�ȡ������
    LOG_DEBUG("fullpath=[%s] filename=[%s]", fullpath.c_str(), filename.c_str());

    // �ٴδ򿪴���ļ�
    if( !m_file.Open(fullpath) )
    {
        Page::OutHead();
        LOG_ERROR("Export error, Can't open file: [%s]", fullpath.c_str());
        return OK;
    }

    const string &size = IntToString(m_file.Size());

    // �ļ�����ͷ����ʽ
    const string html = ""
                        "HTTP/1.1 200 OK\n"
                        "Accept-Ranges: bytes\n"
                        "Content-Disposition: attachment; filename=\"" + filename + "\"\n"
                        "Content-length: " + size + "\n"
                        "Connection: Keep-Alive\n"
                        "Content-Type: application/ms-excel\n"
                        "\n";

    // ����
    return connect->Send(html) == html.length() ? OK : ERR;
}

// ���������
int Page_DataExport::OutBody()
{
    FUNCTION_TRACK(); // �����켣����

    assert(NULL != m_request);

    Connect * const connect = m_request->GetConnect();
    int ret = SendFile(m_file, connect);

    // ������ϣ�������ļ���
    DeleteFile(m_file.Fullname());

    return OK;
}







// ���嶯̬�����
DefinitinoDllEnter(Page_DataExport, THIS_MODULE)

}// end of PAGE_DATAEXPORT_SPACE
