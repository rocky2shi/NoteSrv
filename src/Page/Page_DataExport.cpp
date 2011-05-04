// Rocky 2010-05-19 17:36:42
#include "DataPack.h"
#include "Page_DataExport.h"
namespace PAGE_DATAEXPORT_SPACE
{









// 标明模块
static const string THIS_MODULE = "DataExport";



Page_DataExport::Page_DataExport()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Page_DataExport::~Page_DataExport()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}


int Page_DataExport::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return Page::DoInit();
}

// 子类对象创建器
Page *Page_DataExport::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Page_DataExport;
}








/******************************** 业务代码 ********************************/


// 输出http协议头部
int Page_DataExport::OutHead()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    Connect * const connect = m_request->GetConnect();
    const string &username = m_request->GetCurrentUser();
    const string &backup_range = m_request->GetField("backup_range"); // 导出范围
    const string &include_attach = m_request->GetField("include_attach");
    DataPack pack(username);
    string fullpath;
    string filename; // 传到客户端的文件名

    // 是否需是导出附件
    if("yes" != include_attach)
    {
        // 排除附件
        pack.SetAttrib(DataPack::A_EXCLUDE_ATTACH);
    }

    if("all" == backup_range)
    {
        /*
         * 导出所有数据
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
         * 导出选定的数据
         */
        int i;
        vector<string> keys;
        vector<string>::iterator it;
        UserData data( username );
        Split(m_request->GetField("select_key"), " ", keys); // 分割串为数组

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

            // 以50个为一组
            for(i=0; i<50 && keys.end() != it; it++)
            {
                const string &key = *it;
                if("" == key)
                {
                    continue;
                }
                group.Set(key, Conf()); // 先填充一空的Conf()
                LOG_DEBUG("key=[%s]", key.c_str());
            }

            // 取key组对应数据
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

    filename = username + "." + GetCurrentKey() + ".pkg";   // 发送到客户端的文件名
    fullpath = pack.PackFinish(); // 打包完成，取包名；
    LOG_DEBUG("fullpath=[%s] filename=[%s]", fullpath.c_str(), filename.c_str());

    // 再次打开打包文件
    if( !m_file.Open(fullpath) )
    {
        Page::OutHead();
        LOG_ERROR("Export error, Can't open file: [%s]", fullpath.c_str());
        return OK;
    }

    const string &size = IntToString(m_file.Size());

    // 文件下载头部格式
    const string html = ""
                        "HTTP/1.1 200 OK\n"
                        "Accept-Ranges: bytes\n"
                        "Content-Disposition: attachment; filename=\"" + filename + "\"\n"
                        "Content-length: " + size + "\n"
                        "Connection: Keep-Alive\n"
                        "Content-Type: application/ms-excel\n"
                        "\n";

    // 发送
    return connect->Send(html) == html.length() ? OK : ERR;
}

// 输出数据体
int Page_DataExport::OutBody()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    assert(NULL != m_request);

    Connect * const connect = m_request->GetConnect();
    int ret = SendFile(m_file, connect);

    // 处理完毕，清除包文件；
    DeleteFile(m_file.Fullname());

    return OK;
}







// 定义动态库入口
DefinitinoDllEnter(Page_DataExport, THIS_MODULE)

}// end of PAGE_DATAEXPORT_SPACE
