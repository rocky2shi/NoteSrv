// Rocky 2010-05-19 19:09:18
#include "DataPack.h"
#include "Page_DataImport.h"
namespace PAGE_DATAIMPORT_SPACE
{








// 标明模块
static const string THIS_MODULE = "DataImport";



Page_DataImport::Page_DataImport()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Page_DataImport::~Page_DataImport()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}


int Page_DataImport::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return Page::DoInit();
}

// 子类对象创建器
Page *Page_DataImport::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Page_DataImport;
}








/******************************** 业务代码 ********************************/


// 输出数据体
int Page_DataImport::OutBody()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    assert(NULL != m_request);

    const string &backup_import = m_request->GetField("backup_import");
    if("" != backup_import)
    {
        /*
         * 取上传的打包文件
         */
        FileObj *attach = m_request->GetFileObj("pack");
        const string &pack = attach->Fullname();
        LOG_DEBUG("pack=[%s]", pack.c_str());

        /*
         * 解包
         */
        const string &username = m_request->GetCurrentUser();
        DataPack unpack(username);
        if( unpack.UnPack(pack) < 0 )
        {
            LOG_ERROR("Unpack error");
            return ERR;
        }

        /*
         * 处理完毕，清理包文件；
         */
        LOG_DEBUG("Delete file: [%s]", pack.c_str());
        DeleteFile(pack);

        /*
         * 取导入结果
         */
        const string &success1 = unpack.GetResult("success.data");
        const string &success2 = unpack.GetResult("success.attach");
        const string msg = "成功导入：数据[" + success1 + "]条，附件[" + success2 + "]个。";
        m_request->GetConnect()->Send( HtmlAlert(msg) );
        return OK;
    }

    return OK;
}







// 定义动态库入口
DefinitinoDllEnter(Page_DataImport, THIS_MODULE)

}// end of PAGE_DATAIMPORT_SPACE
